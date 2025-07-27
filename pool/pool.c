#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../detail/mutex.h"
#include "../include/muda/allocator.h"
#include "../include/muda/pool.h"

void
muda_pool_set_mutex (pool_ctx_t *ctx, void *mutex)
{
  ctx->_mutex = mutex;
}

void
muda_pool_set_allocator (pool_ctx_t *ctx, allocator_malloc_fn _f_malloc,
                         allocator_free_fn _f_free)
{
  if (!ctx)
    return;

  muda_detail_mutex_lock (ctx->_mutex);

  ctx->_f_malloc = _f_malloc;
  ctx->_f_free   = _f_free;

  muda_detail_mutex_unlock (ctx->_mutex);
}

int
muda_pool_init (pool_ctx_t *ctx, size_t blk_sz, size_t cap)
{
  if (!ctx->_f_malloc || !ctx->_f_free)
    muda_pool_set_allocator (ctx, malloc, free);

  muda_detail_mutex_lock (ctx->_mutex);

  if (!ctx->_f_malloc)
    ctx->_f_malloc = malloc;
  if (!ctx->_f_free)
    ctx->_f_free = free;

  ctx->blk_sz = (blk_sz > sizeof (void *)) ? blk_sz : sizeof (void *);
  ctx->cap    = cap;
  ctx->data   = ctx->_f_malloc (ctx->blk_sz * cap);

  if (!ctx->data)
  {
    ctx->free_list = NULL;
    ctx->cap       = 0;
    ctx->blk_sz    = 0;
    muda_detail_mutex_unlock (ctx->_mutex);
    return -1;
  }

  ctx->free_list = NULL;

  for (size_t i = 0; i < cap; ++i)
  {
    void *block     = (char *)ctx->data + i * ctx->blk_sz;
    *(void **)block = ctx->free_list;
    ctx->free_list  = block;
  }

  muda_detail_mutex_unlock (ctx->_mutex);
  return 0;
}

void *
muda_pool_alloc (pool_ctx_t *ctx)
{
  muda_detail_mutex_lock (ctx->_mutex);
  void *block = ctx->free_list;
  if (block)
    ctx->free_list = *(void **)block;
  muda_detail_mutex_unlock (ctx->_mutex);
  return block;
}

void
muda_pool_free (pool_ctx_t *ctx, void *ptr)
{
  if (!ptr)
    return;

  muda_detail_mutex_lock (ctx->_mutex);

  *(void **)ptr  = ctx->free_list;
  ctx->free_list = ptr;

  muda_detail_mutex_unlock (ctx->_mutex);
}

void
muda_pool_destroy (pool_ctx_t *ctx)
{
  muda_detail_mutex_lock (ctx->_mutex);

  ctx->_f_free (ctx->data);
  ctx->data      = NULL;
  ctx->free_list = NULL;

  muda_detail_mutex_unlock (ctx->_mutex);
}

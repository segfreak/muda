#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../detail/align.h"
#include "../detail/mutex.h"
#include "../include/muda/allocator/arena.h"

void
muda_arena_set_mutex (arena_ctx_t *ctx, void *mutex)
{
  ctx->_mutex = mutex;
}

void
muda_arena_set_allocator (arena_ctx_t *ctx, allocator_malloc_fn _f_malloc,
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
muda_arena_init (arena_ctx_t *ctx, size_t size)
{
  if (!ctx->_f_malloc || !ctx->_f_free)
    muda_arena_set_allocator (ctx, malloc, free);

  muda_detail_mutex_lock (ctx->_mutex);

  if (!ctx->_f_malloc)
    ctx->_f_malloc = malloc;
  if (!ctx->_f_free)
    ctx->_f_free = free;

  ctx->data = ctx->_f_malloc (size);
  if (!ctx->data)
  {
    ctx->sz  = 0;
    ctx->off = 0;
    muda_detail_mutex_unlock (ctx->_mutex);
    return -1;
  }

  ctx->sz  = size;
  ctx->off = 0;

  muda_detail_mutex_unlock (ctx->_mutex);
  return 0;
}

void *
muda_arena_alloc (arena_ctx_t *ctx, size_t size, size_t alignment)
{
  muda_detail_mutex_lock (ctx->_mutex);

  uintptr_t base = (uintptr_t)ctx->data;
  uintptr_t offset
      = muda_detail_align_up (base + ctx->off, alignment) - base;

  if (offset + size > ctx->sz)
  {
    muda_detail_mutex_unlock (ctx->_mutex);
    return NULL;
  }

  void *ptr = (char *)ctx->data + offset;
  ctx->off  = offset + size;

  muda_detail_mutex_unlock (ctx->_mutex);
  return ptr;
}

void
muda_arena_reset (arena_ctx_t *ctx)
{
  muda_detail_mutex_lock (ctx->_mutex);
  ctx->off = 0;
  muda_detail_mutex_unlock (ctx->_mutex);
}

void
muda_arena_destroy (arena_ctx_t *ctx)
{
  muda_detail_mutex_lock (ctx->_mutex);
  ctx->_f_free (ctx->data);
  ctx->data = NULL;
  ctx->sz   = 0;
  ctx->off  = 0;
  muda_detail_mutex_unlock (ctx->_mutex);
}

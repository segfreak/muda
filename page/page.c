#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../detail/mutex.h"
#include "../include/muda/allocator/page.h"

void
muda_page_set_mutex (page_ctx_t *ctx, void *mutex)
{
  ctx->_mutex = mutex;
}

void
muda_page_set_allocator (page_ctx_t *ctx, allocator_malloc_fn _f_malloc,
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
muda_page_init (page_ctx_t *ctx, size_t page_sz, size_t cap)
{
  if (!ctx->_f_malloc || !ctx->_f_free)
    muda_page_set_allocator (ctx, malloc, free);

  muda_detail_mutex_lock (ctx->_mutex);

  if (!ctx->_f_malloc)
    ctx->_f_malloc = malloc;
  if (!ctx->_f_free)
    ctx->_f_free = free;

  ctx->page_sz = page_sz;
  ctx->cap     = cap;
  ctx->used    = 0;

  ctx->pages   = ctx->_f_malloc (sizeof (void *) * cap);
  if (!ctx->pages)
  {
    muda_detail_mutex_unlock (ctx->_mutex);
    return -1;
  }

  memset (ctx->pages, 0, sizeof (void *) * cap);

  muda_detail_mutex_unlock (ctx->_mutex);
  return 0;
}

void *
muda_page_alloc (page_ctx_t *ctx)
{
  muda_detail_mutex_lock (ctx->_mutex);

  if (ctx->used >= ctx->cap)
  {
    muda_detail_mutex_unlock (ctx->_mutex);
    return NULL;
  }

  void *page = ctx->_f_malloc (ctx->page_sz);
  if (!page)
  {
    muda_detail_mutex_unlock (ctx->_mutex);
    return NULL;
  }

  ctx->pages[ctx->used++] = page;

  muda_detail_mutex_unlock (ctx->_mutex);
  return page;
}

void
muda_page_reset (page_ctx_t *ctx)
{
  muda_detail_mutex_lock (ctx->_mutex);

  for (size_t i = 0; i < ctx->used; ++i)
  {
    ctx->_f_free (ctx->pages[i]);
    ctx->pages[i] = NULL;
  }

  ctx->used = 0;

  muda_detail_mutex_unlock (ctx->_mutex);
}

void
muda_page_destroy (page_ctx_t *ctx)
{
  muda_detail_mutex_lock (ctx->_mutex);

  for (size_t i = 0; i < ctx->used; ++i)
    ctx->_f_free (ctx->pages[i]);

  ctx->_f_free (ctx->pages);

  ctx->pages   = NULL;
  ctx->used    = 0;
  ctx->cap     = 0;
  ctx->page_sz = 0;

  muda_detail_mutex_unlock (ctx->_mutex);
}

#ifndef POOL_H
#define POOL_H

#include "../allocator/allocator.h"
#include <stddef.h>

typedef struct pool_ctx
{
  void               *data;
  size_t              blk_sz;
  size_t              cap;
  void               *free_list;

  void               *_mutex;
  allocator_malloc_fn _f_malloc;
  allocator_free_fn   _f_free;
} pool_ctx_t;

void  muda_pool_set_mutex (pool_ctx_t *ctx, void *mutex);
void  muda_pool_set_allocator (pool_ctx_t         *ctx,
                               allocator_malloc_fn _f_malloc,
                               allocator_free_fn   _f_free);
/*
 * @brief returns nonzero status if badalloc
 */
int   muda_pool_init (pool_ctx_t *ctx, size_t blk_sz, size_t cap);
void *muda_pool_alloc (pool_ctx_t *ctx);
void  muda_pool_free (pool_ctx_t *ctx, void *ptr);
void  muda_pool_destroy (pool_ctx_t *ctx);

#endif
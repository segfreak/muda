#ifndef ARENA_H
#define ARENA_H

#include "./allocator.h"
#include <stddef.h>

typedef struct arena_ctx
{
  void               *data;
  size_t              sz;
  size_t              off;

  void               *_mutex;
  allocator_malloc_fn _f_malloc;
  allocator_free_fn   _f_free;
} arena_ctx_t;

void  muda_arena_set_mutex (arena_ctx_t *ctx, void *mutex);
void  muda_arena_set_allocator (arena_ctx_t        *ctx,
                                allocator_malloc_fn _f_malloc,
                                allocator_free_fn   _f_free);
/*
 * @brief returns nonzero status if badalloc
 */
int   muda_arena_init (arena_ctx_t *ctx, size_t size);
void *muda_arena_alloc (arena_ctx_t *ctx, size_t size, size_t alignment);
void  muda_arena_reset (arena_ctx_t *ctx);
void  muda_arena_destroy (arena_ctx_t *ctx);

#endif

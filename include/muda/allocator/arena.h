#ifndef MUDA_ARENA_H
#define MUDA_ARENA_H

/**
 * @file arena.h
 * @brief Arena-style memory allocator.
 */

#include "./allocator.h"
#include <stddef.h>

/**
 * @struct arena_ctx
 * @brief Arena allocator context for linear allocations.
 */
typedef struct arena_ctx
{
  void               *data;   /**< Pointer to allocated memory region. */
  size_t              sz;     /**< Total size of the arena. */
  size_t              off;    /**< Current offset from the beginning. */

  void               *_mutex; /**< Optional mutex for thread safety. */
  allocator_malloc_fn _f_malloc; /**< Custom malloc function. */
  allocator_free_fn   _f_free;   /**< Custom free function. */
} arena_ctx_t;

/**
 * @brief Set the mutex used for arena synchronization.
 *
 * @param ctx Pointer to arena context.
 * @param mutex Pointer to the mutex object.
 */
void  muda_arena_set_mutex (arena_ctx_t *ctx, void *mutex);

/**
 * @brief Set custom memory allocation and deallocation functions.
 *
 * @param ctx Pointer to arena context.
 * @param _f_malloc Custom malloc function.
 * @param _f_free Custom free function.
 */
void  muda_arena_set_allocator (arena_ctx_t        *ctx,
                                allocator_malloc_fn _f_malloc,
                                allocator_free_fn   _f_free);

/**
 * @brief Initialize the arena allocator with a given size.
 *
 * If allocation fails, returns non-zero status.
 *
 * @param ctx Pointer to arena context.
 * @param size Total size of the arena in bytes.
 * @return 0 on success, non-zero on failure.
 */
int   muda_arena_init (arena_ctx_t *ctx, size_t size);

/**
 * @brief Allocate memory from the arena with alignment.
 *
 * @param ctx Pointer to arena context.
 * @param size Size of the allocation in bytes.
 * @param alignment Alignment constraint (must be power of two).
 * @return Pointer to allocated memory, or NULL on failure.
 */
void *muda_arena_alloc (arena_ctx_t *ctx, size_t size, size_t alignment);

/**
 * @brief Reset the arena offset, reclaiming all memory.
 *
 * Does not deallocate the arena memory.
 *
 * @param ctx Pointer to arena context.
 */
void  muda_arena_reset (arena_ctx_t *ctx);

/**
 * @brief Destroy the arena and free all associated memory.
 *
 * @param ctx Pointer to arena context.
 */
void  muda_arena_destroy (arena_ctx_t *ctx);

#endif /* MUDA_ARENA_H */

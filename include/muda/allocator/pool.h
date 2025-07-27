#ifndef MUDA_POOL_H
#define MUDA_POOL_H

/**
 * @file pool.h
 * @brief Fixed-size block memory pool allocator.
 */

#include "./allocator.h"
#include <stddef.h>

/**
 * @struct pool_ctx
 * @brief Memory pool context for fixed-size block allocation.
 */
typedef struct pool_ctx
{
  void               *data;      /**< Pointer to allocated memory block. */
  size_t              blk_sz;    /**< Size of each block in bytes. */
  size_t              cap;       /**< Total number of blocks. */
  void               *free_list; /**< Linked list of free blocks. */

  void               *_mutex;    /**< Optional mutex for thread safety. */
  allocator_malloc_fn _f_malloc; /**< Custom malloc function. */
  allocator_free_fn   _f_free;   /**< Custom free function. */
} pool_ctx_t;

/**
 * @brief Set the mutex used for pool synchronization.
 *
 * @param ctx Pointer to the pool context.
 * @param mutex Pointer to the mutex object.
 */
void  muda_pool_set_mutex (pool_ctx_t *ctx, void *mutex);

/**
 * @brief Set custom memory allocation and deallocation functions.
 *
 * @param ctx Pointer to the pool context.
 * @param _f_malloc Custom malloc function.
 * @param _f_free Custom free function.
 */
void  muda_pool_set_allocator (pool_ctx_t         *ctx,
                               allocator_malloc_fn _f_malloc,
                               allocator_free_fn   _f_free);

/**
 * @brief Initialize the memory pool.
 *
 * Allocates a fixed number of blocks of a specified size.
 * Returns non-zero if allocation fails.
 *
 * @param ctx Pointer to the pool context.
 * @param blk_sz Size of each block.
 * @param cap Number of blocks.
 * @return 0 on success, non-zero on allocation failure.
 */
int   muda_pool_init (pool_ctx_t *ctx, size_t blk_sz, size_t cap);

/**
 * @brief Allocate a block from the memory pool.
 *
 * @param ctx Pointer to the pool context.
 * @return Pointer to the allocated block, or NULL if exhausted.
 */
void *muda_pool_alloc (pool_ctx_t *ctx);

/**
 * @brief Return a block back to the memory pool.
 *
 * @param ctx Pointer to the pool context.
 * @param ptr Pointer to the block to free.
 */
void  muda_pool_free (pool_ctx_t *ctx, void *ptr);

/**
 * @brief Destroy the pool and free all associated memory.
 *
 * @param ctx Pointer to the pool context.
 */
void  muda_pool_destroy (pool_ctx_t *ctx);

#endif /* MUDA_POOL_H */

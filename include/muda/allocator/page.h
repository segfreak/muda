#ifndef MUDA_PAGE_H
#define MUDA_PAGE_H

/**
 * @file page.h
 * @brief Page-based memory allocator with fixed-size pages.
 */

#include "./allocator.h"
#include <stddef.h>

/**
 * @struct page_ctx
 * @brief Memory context for managing fixed-size page allocations.
 */
typedef struct page_ctx
{
  void              **pages;     /**< Array of allocated page pointers. */
  size_t              page_sz;   /**< Size of each page in bytes. */
  size_t              cap;       /**< Maximum number of pages. */
  size_t              used;      /**< Number of allocated pages. */

  void               *_mutex;    /**< Optional mutex for thread safety. */
  allocator_malloc_fn _f_malloc; /**< Custom malloc function. */
  allocator_free_fn   _f_free;   /**< Custom free function. */
} page_ctx_t;

/**
 * @brief Set the mutex used for page allocator synchronization.
 *
 * @param ctx Pointer to the page context.
 * @param mutex Pointer to the mutex object.
 */
void  muda_page_set_mutex (page_ctx_t *ctx, void *mutex);

/**
 * @brief Set custom memory allocation and deallocation functions.
 *
 * @param ctx Pointer to the page context.
 * @param _f_malloc Custom malloc function.
 * @param _f_free Custom free function.
 */
void  muda_page_set_allocator (page_ctx_t         *ctx,
                               allocator_malloc_fn _f_malloc,
                               allocator_free_fn   _f_free);

/**
 * @brief Initialize the page allocator.
 *
 * Allocates space to track a fixed number of pages of the given size.
 * Actual pages are allocated on demand.
 *
 * @param ctx Pointer to the page context.
 * @param page_sz Size of each page in bytes.
 * @param cap Maximum number of pages.
 * @return 0 on success, non-zero on allocation failure.
 */
int   muda_page_init (page_ctx_t *ctx, size_t page_sz, size_t cap);

/**
 * @brief Allocate a new page.
 *
 * Allocates a page of `page_sz` bytes and tracks it internally.
 *
 * @param ctx Pointer to the page context.
 * @return Pointer to the allocated page, or NULL if exhausted.
 */
void *muda_page_alloc (page_ctx_t *ctx);

/**
 * @brief Reset all allocated pages.
 *
 * Frees all pages but keeps the internal structure and capacity intact.
 *
 * @param ctx Pointer to the page context.
 */
void  muda_page_reset (page_ctx_t *ctx);

/**
 * @brief Destroy the page allocator and free all associated memory.
 *
 * Frees all pages and tracking structures.
 *
 * @param ctx Pointer to the page context.
 */
void  muda_page_destroy (page_ctx_t *ctx);

#endif /* MUDA_PAGE_H */

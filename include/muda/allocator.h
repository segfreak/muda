#ifndef MUDA_ALLOCATOR_H
#define MUDA_ALLOCATOR_H

/**
 * @file allocator.h
 * @brief Custom allocator function type definitions for MUDA.
 */

#include <stddef.h>

/**
 * @typedef allocator_malloc_fn
 * @brief Function pointer type for custom memory allocation.
 *
 * A function that allocates a block of memory of the specified size.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to the allocated memory block, or NULL on failure.
 */
typedef void *(*allocator_malloc_fn) (size_t);

/**
 * @typedef allocator_free_fn
 * @brief Function pointer type for custom memory deallocation.
 *
 * A function that frees a block of memory previously allocated.
 *
 * @param ptr Pointer to the memory block to free.
 */
typedef void (*allocator_free_fn) (void *);

#endif /* MUDA_ALLOCATOR_H */

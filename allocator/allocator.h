#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

typedef void *(*allocator_malloc_fn) (size_t);
typedef void (*allocator_free_fn) (void *);

#endif
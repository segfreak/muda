#ifndef CUST_ALLOC_H
#define CUST_ALLOC_H

#include <stddef.h>

void  *muda_detail_nmalloc (size_t sz);
void   muda_detail_nfree (void *p);

size_t muda_detail_mallocs_count (void);
size_t muda_detail_frees_count (void);

void  *muda_detail_malloc_null (size_t sz);
void   muda_detail_free_null (void *p);

#endif
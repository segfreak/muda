#include "cust_alloc.h"
#include <stdlib.h>

static _Atomic size_t _n_malloc_called = 0;
static _Atomic size_t _n_free_called   = 0;

void *
muda_detail_nmalloc (size_t sz)
{
  _n_malloc_called++;
  return malloc (sz);
}

void
muda_detail_nfree (void *p)
{
  _n_free_called++;
  free (p);
}

size_t
muda_detail_mallocs_count (void)
{
  return _n_malloc_called;
}

size_t
muda_detail_frees_count (void)
{
  return _n_free_called;
}

void *
muda_detail_malloc_null (size_t sz)
{
  (void)sz;
  return NULL;
}

void
muda_detail_free_null (void *p)
{
  (void)p;
}
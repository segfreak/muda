#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../unit/unit.h"
#include "../pool.h"

#define POOL_CAPACITY 10
#define BLOCK_SIZE    32

void
test_basic_allocation ()
{
  pool_ctx_t      pool = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_pool_set_mutex (&pool, &mutex);

  muda_pool_init (&pool, BLOCK_SIZE, POOL_CAPACITY);

  void *blocks[POOL_CAPACITY];
  for (int i = 0; i < POOL_CAPACITY; ++i)
  {
    blocks[i] = muda_pool_alloc (&pool);
    ut_assert (blocks[i] != NULL, "expected non-null block on allocation");
    memset (blocks[i], 0xAB, BLOCK_SIZE);
  }

  void *fail_block = muda_pool_alloc (&pool);
  ut_assert (fail_block == NULL,
             "expected NULL on allocation after pool exhausted");

  muda_pool_free (&pool, blocks[5]);
  void *reused = muda_pool_alloc (&pool);
  ut_assert (reused != NULL, "expected block to be reused after free");

  muda_pool_destroy (&pool);
  pthread_mutex_destroy (&mutex);
}

void
test_zero_block_size ()
{
  pool_ctx_t      pool = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_pool_set_mutex (&pool, &mutex);

  muda_pool_init (&pool, 0, 1);
  void *ptr = muda_pool_alloc (&pool);
  ut_assert (ptr != NULL,
             "expected non-null pointer even for 0-sized blocks");

  muda_pool_free (&pool, ptr);
  muda_pool_destroy (&pool);
  pthread_mutex_destroy (&mutex);
}

static _Atomic size_t _n_malloc_called = 0;
static _Atomic size_t _n_free_called   = 0;

void *
nmalloc (size_t sz)
{
  _n_malloc_called++;
  return malloc (sz);
}

void
nfree (void *p)
{
  _n_free_called++;
  free (p);
}

void *
malloc_null (size_t sz)
{
  (void)sz;
  return NULL;
}

void
free_null (void *p)
{
  (void)p;
}

void
test_allocator_override_0 ()
{
  pool_ctx_t      pool = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_pool_set_mutex (&pool, &mutex);
  muda_pool_set_allocator (&pool, nmalloc, nfree);
  muda_pool_init (&pool, BLOCK_SIZE, POOL_CAPACITY);

  void *ptr = muda_pool_alloc (&pool);
  ut_assert (ptr != NULL,
             "expected non-null allocation using custom allocator");
  muda_pool_free (&pool, ptr);
  muda_pool_destroy (&pool);
  pthread_mutex_destroy (&mutex);

  ut_assert (_n_malloc_called > 0, "expected custom malloc to be called");
  ut_assert (_n_free_called > 0, "expected custom free to be called");
}

void
test_allocator_override_1 ()
{
  pool_ctx_t      pool = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_pool_set_mutex (&pool, &mutex);
  muda_pool_set_allocator (&pool, malloc_null, free_null);
  muda_pool_init (&pool, BLOCK_SIZE, POOL_CAPACITY);

  void *ptr = muda_pool_alloc (&pool);
  ut_assert (ptr == NULL,
             "expected non-null allocation using custom allocator");
  muda_pool_free (&pool, ptr);
  muda_pool_destroy (&pool);
  pthread_mutex_destroy (&mutex);
}

void
test_full_alloc_and_free_cycle ()
{
  pool_ctx_t      pool = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_pool_set_mutex (&pool, &mutex);
  muda_pool_init (&pool, BLOCK_SIZE, POOL_CAPACITY);

  void *blocks[POOL_CAPACITY];
  for (int i = 0; i < POOL_CAPACITY; ++i)
  {
    blocks[i] = muda_pool_alloc (&pool);
    ut_assert (blocks[i] != NULL,
               "expected successful allocation in full cycle");
  }

  for (int i = 0; i < POOL_CAPACITY; ++i)
    muda_pool_free (&pool, blocks[i]);

  for (int i = 0; i < POOL_CAPACITY; ++i)
  {
    void *ptr = muda_pool_alloc (&pool);
    ut_assert (ptr != NULL,
               "expected successful re-allocation after free");
  }

  muda_pool_destroy (&pool);
  pthread_mutex_destroy (&mutex);
}

static void
_unit (void)
{
  test_basic_allocation ();
  test_zero_block_size ();
  test_allocator_override_0 ();
  test_allocator_override_1 ();
  test_full_alloc_and_free_cycle ();
}

__attribute__ ((constructor (150))) static void
_unit_registrar (void)
{
  ut_register (_unit, "pool/unit");
}

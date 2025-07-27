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
    ut_assert (blocks[i] != NULL, "ptr must be nonnull");
    memset (blocks[i], 0xAB, BLOCK_SIZE);
  }

  void *fail_block = muda_pool_alloc (&pool);
  ut_assert (fail_block == NULL, "");

  muda_pool_free (&pool, blocks[5]);
  void *reused = muda_pool_alloc (&pool);
  ut_assert (reused != NULL, "ptr must be nonnull");

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
  ut_assert (ptr != NULL, "ptr must be nonnull");

  muda_pool_free (&pool, ptr);
  muda_pool_destroy (&pool);
  pthread_mutex_destroy (&mutex);
}

static _Atomic int malloc_called = 0;
static _Atomic int free_called   = 0;

void *
my_malloc (size_t sz)
{
  malloc_called++;
  return malloc (sz);
}

void
my_free (void *p)
{
  free_called++;
  free (p);
}

void
test_allocator_override ()
{
  pool_ctx_t      pool = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_pool_set_mutex (&pool, &mutex);
  muda_pool_set_allocator (&pool, my_malloc, my_free);
  muda_pool_init (&pool, BLOCK_SIZE, POOL_CAPACITY);

  void *ptr = muda_pool_alloc (&pool);
  ut_assert (ptr != NULL, "ptr must be nonnull");
  muda_pool_free (&pool, ptr);
  muda_pool_destroy (&pool);
  pthread_mutex_destroy (&mutex);

  ut_assert (malloc_called > 0,
             "malloc_called counter must be greater than zero");
  ut_assert (free_called > 0,
             "free_called counter must be greater than zero");
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
    ut_assert (blocks[i] != NULL, "ptr must be nonnull");
  }

  for (int i = 0; i < POOL_CAPACITY; ++i)
    muda_pool_free (&pool, blocks[i]);

  for (int i = 0; i < POOL_CAPACITY; ++i)
  {
    void *ptr = muda_pool_alloc (&pool);
    ut_assert (ptr != NULL, "ptr must be nonnull");
  }

  muda_pool_destroy (&pool);
  pthread_mutex_destroy (&mutex);
}

static void
_unit (void)
{
  test_basic_allocation ();
  test_zero_block_size ();
  test_allocator_override ();
  test_full_alloc_and_free_cycle ();
}

__attribute__ ((constructor (150))) static void
_unit_registrar (void)
{
  ut_register (_unit, "pool/unit");
}

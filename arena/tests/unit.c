#include <pthread.h>
#include <stdint.h>
#include <string.h>

#include "../../detail/cust_alloc.h"

#include "../../include/muda/arena.h"
#include "../../unit/unit.h"

#ifndef ARENA_SIZE
#define ARENA_SIZE 1024
#endif

#ifndef ALLOC_SIZE
#define ALLOC_SIZE 32
#endif

#ifndef ALLOC_ALIGN
#define ALLOC_ALIGN 16
#endif

#ifndef NUM_ALLOCS
#define NUM_ALLOCS (ARENA_SIZE / ALLOC_SIZE)
#endif

void
test_basic_arena_allocation ()
{
  arena_ctx_t     arena = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_arena_set_mutex (&arena, &mutex);

  muda_arena_init (&arena, ARENA_SIZE);

  for (int i = 0; i < NUM_ALLOCS; ++i)
  {
    void *ptr = muda_arena_alloc (&arena, ALLOC_SIZE, ALLOC_ALIGN);
    ut_assert (ptr != NULL, "expected non-null arena allocation");
    ut_assert (((uintptr_t)ptr % ALLOC_ALIGN) == 0,
               "expected pointer alignment");
    memset (ptr, 0xCD, ALLOC_SIZE);
  }

  void *fail_ptr = muda_arena_alloc (&arena, ALLOC_SIZE, ALLOC_ALIGN);
  ut_assert (fail_ptr == NULL, "expected NULL after arena exhausted");

  muda_arena_destroy (&arena);
  pthread_mutex_destroy (&mutex);
}

void
test_arena_reset ()
{
  arena_ctx_t     arena = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_arena_set_mutex (&arena, &mutex);
  muda_arena_init (&arena, ARENA_SIZE);

  void *first = muda_arena_alloc (&arena, ALLOC_SIZE, ALLOC_ALIGN);
  ut_assert (first != NULL, "expected allocation before reset");

  muda_arena_reset (&arena);

  void *second = muda_arena_alloc (&arena, ALLOC_SIZE, ALLOC_ALIGN);
  ut_assert (second != NULL, "expected allocation after reset");

  muda_arena_destroy (&arena);
  pthread_mutex_destroy (&mutex);
}

void
test_allocator_override_success ()
{
  arena_ctx_t     arena = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_arena_set_mutex (&arena, &mutex);
  muda_arena_set_allocator (&arena, muda_detail_nmalloc,
                            muda_detail_nfree);
  muda_arena_init (&arena, ARENA_SIZE);

  void *ptr = muda_arena_alloc (&arena, ALLOC_SIZE, ALLOC_ALIGN);
  ut_assert (ptr != NULL,
             "expected non-null allocation using custom allocator");

  muda_arena_destroy (&arena);
  pthread_mutex_destroy (&mutex);

  ut_assert (muda_detail_mallocs_count () > 0,
             "expected custom malloc to be called");
  ut_assert (muda_detail_frees_count () > 0,
             "expected custom free to be called");
}

void
test_allocator_override_failure ()
{
  arena_ctx_t     arena = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_arena_set_mutex (&arena, &mutex);
  muda_arena_set_allocator (&arena, muda_detail_malloc_null,
                            muda_detail_free_null);

  int res = muda_arena_init (&arena, ARENA_SIZE);
  ut_assert (res != 0, "expected arena init to fail with NULL allocator");

  muda_arena_destroy (&arena);
  pthread_mutex_destroy (&mutex);
}

static void
_unit (void)
{
  test_basic_arena_allocation ();
  test_arena_reset ();
  test_allocator_override_success ();
  test_allocator_override_failure ();
}

__attribute__ ((constructor (151))) static void
_unit_registrar (void)
{
  ut_register (_unit, "arena/unit");
}

#include <pthread.h>
#include <string.h>

#include "../../detail/cust_alloc.h"

#include "../../include/muda/allocator/page.h"
#include "../../unit/unit.h"

#ifndef PAGE_CAPACITY
#define PAGE_CAPACITY 10
#endif

#ifndef PAGE_SIZE
#define PAGE_SIZE 128
#endif

void
test_page_init_and_alloc ()
{
  page_ctx_t      page = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_page_set_mutex (&page, &mutex);

  int result = muda_page_init (&page, PAGE_SIZE, PAGE_CAPACITY);
  ut_assert (result == 0, "expected successful page init");

  void *p = muda_page_alloc (&page);
  ut_assert (p != NULL, "expected successful page allocation");
  memset (p, 0xCD, PAGE_SIZE);

  muda_page_destroy (&page);
  pthread_mutex_destroy (&mutex);
}

void
test_page_allocation_limit ()
{
  page_ctx_t      page = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_page_set_mutex (&page, &mutex);

  muda_page_init (&page, PAGE_SIZE, PAGE_CAPACITY);

  void *pages[PAGE_CAPACITY];
  for (int i = 0; i < PAGE_CAPACITY; ++i)
  {
    pages[i] = muda_page_alloc (&page);
    ut_assert (pages[i] != NULL, "expected page allocation to succeed");
  }

  void *fail = muda_page_alloc (&page);
  ut_assert (fail == NULL, "expected NULL after exceeding capacity");

  muda_page_destroy (&page);
  pthread_mutex_destroy (&mutex);
}

void
test_page_reset ()
{
  page_ctx_t      page = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_page_set_mutex (&page, &mutex);

  muda_page_init (&page, PAGE_SIZE, PAGE_CAPACITY);

  for (int i = 0; i < PAGE_CAPACITY; ++i)
    muda_page_alloc (&page);

  muda_page_reset (&page);

  for (int i = 0; i < PAGE_CAPACITY; ++i)
  {
    void *ptr = muda_page_alloc (&page);
    ut_assert (ptr != NULL, "expected allocation after reset");
  }

  muda_page_destroy (&page);
  pthread_mutex_destroy (&mutex);
}

void
test_page_custom_allocator ()
{
  page_ctx_t      page = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_page_set_mutex (&page, &mutex);
  muda_page_set_allocator (&page, muda_detail_nmalloc, muda_detail_nfree);
  muda_page_init (&page, PAGE_SIZE, PAGE_CAPACITY);

  void *p = muda_page_alloc (&page);
  ut_assert (p != NULL, "expected page alloc with custom allocator");

  muda_page_destroy (&page);
  pthread_mutex_destroy (&mutex);

  ut_assert (muda_detail_mallocs_count () > 0,
             "expected custom malloc to be called");
  ut_assert (muda_detail_frees_count () > 0,
             "expected custom free to be called");
}

void
test_page_custom_allocator_null ()
{
  page_ctx_t      page = { 0 };
  pthread_mutex_t mutex;
  pthread_mutex_init (&mutex, NULL);
  muda_page_set_mutex (&page, &mutex);
  muda_page_set_allocator (&page, muda_detail_malloc_null,
                           muda_detail_free_null);

  muda_page_init (&page, PAGE_SIZE, PAGE_CAPACITY);
  void *ptr = muda_page_alloc (&page);
  ut_assert (ptr == NULL, "expected NULL from null allocator");

  muda_page_destroy (&page);
  pthread_mutex_destroy (&mutex);
}

static void
_unit (void)
{
  test_page_init_and_alloc ();
  test_page_allocation_limit ();
  test_page_reset ();
  test_page_custom_allocator ();
  test_page_custom_allocator_null ();
}

__attribute__ ((constructor (151))) static void
_unit_registrar (void)
{
  ut_register (_unit, "page/unit");
}

#include <stdio.h>
#include <stdlib.h>

#include "../include/muda/aqueue.h"
#include "unit.h"

typedef struct unit_test
{
  unit_test_fn fn;
  const char  *name;
} unit_test;

static muda_aq_t m_tst_queue;

__attribute__ ((constructor (103))) static void
unit_ctor ()
{
  muda_aq_init (&m_tst_queue);
}

void
ut_register (unit_test_fn fn, const char *name)
{
  unit_test *t = malloc (sizeof (unit_test));
  if (!t)
  {
    fprintf (stderr, "Failed to allocate memory for test registration\n");
    exit (1);
  }
  t->fn   = fn;
  t->name = name;

  muda_aq_enqueue (&m_tst_queue, t);
}

int
ut_run_all (void)
{
  if (muda_aq_empty (&m_tst_queue))
  {
    printf ("No tests registered.\n");
    return 1;
  }

  int passed = 0;
  int failed = 0;

  printf ("Running unit tests...\n\n");

  while (!muda_aq_empty (&m_tst_queue))
  {
    unit_test *t = (unit_test *)muda_aq_dequeue (&m_tst_queue);
    if (!t)
      break;

    printf ("[ RUN      ] %s\n", t->name);
    t->fn ();
    printf ("[     OK   ] %s\n\n", t->name);
    free (t);
    passed++;
  }

  muda_aq_destroy (&m_tst_queue);

  printf ("Tests done: %d passed, %d failed\n", passed, failed);
  return failed == 0 ? 0 : 1;
}

void
ut_basic_assert (int cond, const char *cond_str, const char *msg)
{
  if (!cond)
  {
    fprintf (stderr, "Assertion (\"%s\") failed: %s\n", cond_str, msg);
    exit (1);
  }
}

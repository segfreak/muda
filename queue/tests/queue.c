#include "../queue.h"
#include "../../unit/unit.h"

#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>

static void
test_init_and_empty (void)
{
  muda_q_t q;
  muda_q_init (&q);
  ut_assert (muda_q_empty (&q), "muda_q_t must be empty after init");
  muda_q_destroy (&q);
}

static void
test_enqueue_dequeue_one (void)
{
  muda_q_t q;
  muda_q_init (&q);

  int val = 42;
  muda_q_enqueue (&q, &val);
  ut_assert (!muda_q_empty (&q),
             "muda_q_t must not be empty after enqueue");

  int *p = (int *)muda_q_dequeue (&q);
  ut_assert (p != NULL && *p == 42, "Dequeued value must be correct");
  ut_assert (muda_q_empty (&q), "muda_q_t must be empty after dequeue");

  muda_q_destroy (&q);
}

static void
test_fifo_order (void)
{
  muda_q_t q;
  muda_q_init (&q);

  int vals[] = { 1, 2, 3 };
  for (int i = 0; i < 3; i++)
    muda_q_enqueue (&q, &vals[i]);

  for (int i = 0; i < 3; i++)
  {
    int *p = (int *)muda_q_dequeue (&q);
    ut_assert (p != NULL && *p == vals[i], "FIFO order must be preserved");
  }

  ut_assert (muda_q_empty (&q),
             "muda_q_t must be empty after dequeuing all");

  muda_q_destroy (&q);
}

static void
test_peek (void)
{
  muda_q_t q;
  muda_q_init (&q);

  ut_assert (muda_q_peek (&q) == NULL,
             "Peek on empty muda_q_t should return NULL");

  int val = 99;
  muda_q_enqueue (&q, &val);
  int *p = (int *)muda_q_peek (&q);
  ut_assert (p != NULL && *p == 99,
             "Peek must return first element without dequeue");

  ut_assert (!muda_q_empty (&q), "muda_q_t must not be empty after peek");

  p = (int *)muda_q_dequeue (&q);
  ut_assert (p != NULL && *p == 99,
             "Dequeue after peek must return correct element");
  ut_assert (muda_q_empty (&q), "muda_q_t must be empty after dequeue");

  muda_q_destroy (&q);
}

static void
test_destroy (void)
{
  muda_q_t q;
  muda_q_init (&q);

  int vals[] = { 1, 2, 3, 4, 5 };
  for (int i = 0; i < 5; i++)
    muda_q_enqueue (&q, &vals[i]);
  muda_q_destroy (&q);

  ut_assert (muda_q_empty (&q), "muda_q_t must be empty after destroy");
}

static void
_unit (void)
{
  test_init_and_empty ();
  test_enqueue_dequeue_one ();
  test_fifo_order ();
  test_peek ();
  test_destroy ();
}

__attribute__ ((constructor (150))) static void
_unit_registrar (void)
{
  ut_register (_unit, "queue/unit");
}

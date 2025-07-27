#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "aqueue.h"
#include "qnode.h"

#include "../detail/mutex.h"

void
muda_aq_init (muda_aqueue_t *q)
{
  q->front = q->rear = NULL;
  pthread_mutex_init (&q->lock, NULL);
}

int
muda_aq_empty (muda_aqueue_t *q)
{
  muda_detail_mutex_lock (&q->lock);

  int result = (q->front == NULL);

  muda_detail_mutex_unlock (&q->lock);

  return result;
}

void
muda_aq_enqueue (muda_aqueue_t *q, void *data)
{
  muda_qnode_t *newNode = malloc (sizeof (muda_qnode_t));
  if (!newNode)
  {
    fprintf (stderr, "Memory allocation failed\n");
    exit (EXIT_FAILURE);
  }
  newNode->data = data;
  newNode->next = NULL;

  muda_detail_mutex_lock (&q->lock);
  if (q->rear == NULL)
  {
    q->front = q->rear = newNode;
  }
  else
  {
    q->rear->next = newNode;
    q->rear       = newNode;
  }
  muda_detail_mutex_unlock (&q->lock);
}

void *
muda_aq_dequeue (muda_aqueue_t *q)
{
  muda_detail_mutex_lock (&q->lock);

  if (q->front == NULL)
  {
    muda_detail_mutex_unlock (&q->lock);
    return NULL;
  }

  muda_qnode_t *temp = q->front;
  void         *data = temp->data;
  q->front           = q->front->next;

  if (q->front == NULL)
    q->rear = NULL;

  muda_detail_mutex_unlock (&q->lock);
  free (temp);
  return data;
}

void *
muda_aq_peek (muda_aqueue_t *q)
{
  muda_detail_mutex_lock (&q->lock);
  void *data = (q->front != NULL) ? q->front->data : NULL;
  muda_detail_mutex_unlock (&q->lock);
  return data;
}

void
muda_aq_destroy (muda_aqueue_t *q)
{
  muda_detail_mutex_lock (&q->lock);
  muda_qnode_t *node = q->front;
  while (node != NULL)
  {
    muda_qnode_t *next = node->next;
    free (node);
    node = next;
  }
  q->front = q->rear = NULL;
  muda_detail_mutex_unlock (&q->lock);
  pthread_mutex_destroy (&q->lock);
}

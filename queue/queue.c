#include <stdio.h>
#include <stdlib.h>

#include "../include/muda/queue.h"
#include "qnode.h"

void
muda_q_init (muda_queue_t *q)
{
  q->front = q->rear = NULL;
}

int
muda_q_empty (muda_queue_t *q)
{
  return q->front == NULL;
}

void
muda_q_enqueue (muda_queue_t *q, void *data)
{
  muda_qnode_t *newNode = malloc (sizeof (muda_qnode_t));
  if (!newNode)
  {
    fprintf (stderr, "Memory allocation failed\n");
    exit (EXIT_FAILURE);
  }
  newNode->data = data;
  newNode->next = NULL;

  if (q->rear == NULL)
  {
    q->front = q->rear = newNode;
  }
  else
  {
    q->rear->next = newNode;
    q->rear       = newNode;
  }
}

void *
muda_q_dequeue (muda_queue_t *q)
{
  if (muda_q_empty (q))
    return NULL;

  muda_qnode_t *temp = q->front;
  void         *data = temp->data;
  q->front           = q->front->next;

  if (q->front == NULL)
    q->rear = NULL;

  free (temp);
  return data;
}

void *
muda_q_peek (muda_queue_t *q)
{
  if (muda_q_empty (q))
    return NULL;
  return q->front->data;
}

void
muda_q_destroy (muda_queue_t *q)
{
  while (!muda_q_empty (q))
    muda_q_dequeue (q);
}
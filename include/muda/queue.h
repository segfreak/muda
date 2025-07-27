#ifndef __Q_IMPL_H
#define __Q_IMPL_H

typedef struct muda_queue
{
  struct muda_qnode *front;
  struct muda_qnode *rear;
} muda_q_t, muda_queue_t;

void *muda_q_peek (muda_queue_t *q);
void  muda_q_init (muda_queue_t *q);
void  muda_q_destroy (muda_queue_t *q);
int   muda_q_empty (muda_queue_t *q);
void  muda_q_enqueue (muda_queue_t *q, void *data);
void *muda_q_dequeue (muda_queue_t *q);

#endif
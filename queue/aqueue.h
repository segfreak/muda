#ifndef MUDA_AQ_H
#define MUDA_AQ_H

#include <pthread.h>

typedef struct muda_aq
{
  struct muda_qnode *front;
  struct muda_qnode *rear;
  pthread_mutex_t    lock;
} muda_aq_t, muda_aqueue_t;

void *muda_aq_peek (muda_aqueue_t *q);
void  muda_aq_init (muda_aqueue_t *q);
void  muda_aq_destroy (muda_aqueue_t *q);
int   muda_aq_empty (muda_aqueue_t *q);
void  muda_aq_enqueue (muda_aqueue_t *q, void *data);
void *muda_aq_dequeue (muda_aqueue_t *q);

#endif /* MUDA_AQ_H */

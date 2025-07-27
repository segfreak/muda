#ifndef MUDA_AQUEUE_QUEUE_H
#define MUDA_AQUEUE_QUEUE_H

#include <pthread.h>

/**
 * @file aqueue.h
 * @brief Thread-Safe Queue.
 */

/**
 * @struct muda_aq
 * @brief Thread-Safe queue structure.
 */
typedef struct muda_aq
{
  struct muda_qnode *front; /**< Pointer to the front node. */
  struct muda_qnode *rear;  /**< Pointer to the rear node. */
  pthread_mutex_t    lock;  /**< Mutex to ensure thread safety. */
} muda_aq_t, muda_aqueue_t;

/**
 * @brief Peek at the front element of the queue without removing it.
 *
 * @param q Pointer to the queue.
 * @return Pointer to the front data, or NULL if queue is empty.
 */
void *muda_aq_peek (muda_aqueue_t *q);

/**
 * @brief Initialize a thread-safe queue.
 *
 * @param q Pointer to the queue to initialize.
 */
void  muda_aq_init (muda_aqueue_t *q);

/**
 * @brief Destroy the queue and free internal resources.
 *
 * @param q Pointer to the queue to destroy.
 */
void  muda_aq_destroy (muda_aqueue_t *q);

/**
 * @brief Check if the queue is empty.
 *
 * @param q Pointer to the queue.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int   muda_aq_empty (muda_aqueue_t *q);

/**
 * @brief Enqueue an element to the queue.
 *
 * @param q    Pointer to the queue.
 * @param data Pointer to the data to enqueue.
 */
void  muda_aq_enqueue (muda_aqueue_t *q, void *data);

/**
 * @brief Dequeue an element from the queue.
 *
 * @param q Pointer to the queue.
 * @return Pointer to the data removed from the front, or NULL if empty.
 */
void *muda_aq_dequeue (muda_aqueue_t *q);

#endif /* MUDA_AQUEUE_QUEUE_H */

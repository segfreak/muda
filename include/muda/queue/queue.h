#ifndef MUDA_QUEUE_QUEUE_H
#define MUDA_QUEUE_QUEUE_H

/**
 * @file queue.h
 * @brief Queue.
 */

/**
 * @struct muda_q
 * @brief Queue structure.
 */
typedef struct muda_q
{
  struct muda_qnode *front; /**< Pointer to the front node. */
  struct muda_qnode *rear;  /**< Pointer to the rear node. */
} muda_q_t, muda_queue_t;

/**
 * @brief Get the front element of the queue without removing it.
 *
 * @param q Pointer to the queue.
 * @return Pointer to the front element, or NULL if the queue is empty.
 */
void *muda_q_peek (muda_queue_t *q);

/**
 * @brief Initialize the queue.
 *
 * @param q Pointer to the queue.
 */
void  muda_q_init (muda_queue_t *q);

/**
 * @brief Destroy the queue and free all associated memory.
 *
 * @param q Pointer to the queue.
 */
void  muda_q_destroy (muda_queue_t *q);

/**
 * @brief Check if the queue is empty.
 *
 * @param q Pointer to the queue.
 * @return Non-zero if empty, 0 otherwise.
 */
int   muda_q_empty (muda_queue_t *q);

/**
 * @brief Enqueue a new element to the rear of the queue.
 *
 * @param q Pointer to the queue.
 * @param data Pointer to the data to enqueue.
 */
void  muda_q_enqueue (muda_queue_t *q, void *data);

/**
 * @brief Dequeue the front element of the queue.
 *
 * @param q Pointer to the queue.
 * @return Pointer to the dequeued element, or NULL if the queue is empty.
 */
void *muda_q_dequeue (muda_queue_t *q);

#endif /* MUDA_MUDA_QUEUE_H */

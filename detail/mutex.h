#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>

int muda_detail_mutex_lock (pthread_mutex_t *mutex);
int muda_detail_mutex_unlock (pthread_mutex_t *mutex);

#endif
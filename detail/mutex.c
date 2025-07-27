#include "mutex.h"

int
muda_detail_mutex_lock (pthread_mutex_t *mutex)
{
  if (mutex)
    return pthread_mutex_trylock (mutex);
  return 0;
}

int
muda_detail_mutex_unlock (pthread_mutex_t *mutex)
{
  if (mutex)
    return pthread_mutex_unlock (mutex);
  return 0;
}
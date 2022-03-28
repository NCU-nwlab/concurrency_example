#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 20000
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

void *func(void *cnt) {
  pthread_mutex_lock(&mu);
  (*(long *)cnt)++;
  pthread_mutex_unlock(&mu);
  pthread_exit(NULL);
}

int main() {
  long count = 0;
  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; ++i)
    pthread_create(&threads[i], NULL, func, (void *)&count);

  for (int i = 0; i < NUM_THREADS; ++i)
    pthread_join(threads[i], NULL);

  printf("count = %ld\n", count);
  pthread_mutex_destroy(&mu);
  pthread_exit(NULL);
}
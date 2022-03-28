#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 20000

void *func(void *cnt) {
  (*(long *)cnt)++;
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
  pthread_exit(NULL);
}

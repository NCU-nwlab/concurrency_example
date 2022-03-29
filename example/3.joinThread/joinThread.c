#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5

void *dosomething(void *thread_id) {
  long tid = (long)thread_id;
  sleep(tid);
  printf("Thread %ld is finish\n", tid);
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[NUM_THREADS];

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for (long i = 0; i < NUM_THREADS; ++i) {
    pthread_create(&threads[i], &attr, dosomething, (void *)i);
  }
  pthread_attr_destroy(&attr);

  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_join(threads[i], NULL);
  }
}
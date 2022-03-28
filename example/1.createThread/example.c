#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

void *print_thread_msg(void *thread_id) {
  long tid = (long)thread_id;
  printf("Hello thread %ld\n", tid);
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  pthread_t threads[NUM_THREADS];
  for (long i = 0; i < NUM_THREADS; ++i) {
    printf("In main, creating a thread %ld\n", i);
    pthread_create(&threads[i], NULL, print_thread_msg, (void *)i);
  }

  pthread_exit(NULL);
}
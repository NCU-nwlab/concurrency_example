#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5000

void *print_thread_msg() {
  char buff[1024] = {};
  printf("Work done, than resources release!\n");
  pthread_detach(pthread_self());
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_create(&threads[i], NULL, print_thread_msg, NULL);
  }

  pthread_exit(NULL);
}
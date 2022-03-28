#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5000

void *print_thread_msg() {
  char buff[1024] = {};
  printf("Work done, than resources release!\n");
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  pthread_t threads[NUM_THREADS];

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_create(&threads[i], &attr, print_thread_msg, NULL);
  }
  pthread_attr_destroy(&attr);
  pthread_exit(NULL);
}
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5000

void *print_thread_msg() {
  char buff[1024] = {};
  printf("Memory leaking... Your computer will crash soon!\n");
  pthread_exit(NULL);
}

int main(int argc, char **argv) {
  pthread_t threads[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; ++i) {
    pthread_create(&threads[i], NULL, print_thread_msg, NULL);
    sleep(1);
  }

  pthread_exit(NULL);
}
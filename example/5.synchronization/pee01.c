#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_STUDENTS 120
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

void pee() {
  srand(time(NULL));
  sleep(rand() % 300);
}

void *nature_calling() {
  pthread_mutex_lock(&mu);
  pee();
  pthread_mutex_unlock(&mu);
  pthread_exit(NULL);
}

int main() {
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  pthread_t students[NUM_STUDENTS];
  for (int i = 0; i < NUM_STUDENTS; i++) {
    pthread_create(&students[i], &attr, nature_calling, NULL);
  }

  pthread_mutex_destroy(&mu);
  pthread_exit(NULL);
}

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_STUDENTS 120
pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;

typedef struct bathroom {
  int count;
  int capacity;
  bool *urinals;
  pthread_mutex_t mu;
  pthread_cond_t full;
} bathroom;

bathroom *bathroom_create(const int capacity) {
  bathroom *b = malloc(sizeof(bathroom));
  b->count = 0;
  b->capacity = capacity;
  b->urinals = malloc(sizeof(bool) * capacity);
  pthread_mutex_init(&b->mu, NULL);
  pthread_cond_init(&b->full, NULL);
  return b;
}

void bathroom_destroy(bathroom *b) {
  free(b->urinals);
  pthread_mutex_destroy(&b->mu);
  free(b);
}

bool isempty(bathroom *b) { return b->count == 0; }

void *someone_need_to_pee(void *br) {
  bathroom *b = (bathroom *)br;
  for (;;) {
    sleep(rand() % 3);
    pthread_mutex_lock(&b->mu);
    while (b->count == b->capacity)
      pthread_cond_wait(&b->full, &b->mu);
    if (!b->urinals[b->count])
      b->urinals[b->count++] = true;
    printf("Peeing count = %d\n", b->count);
    pthread_mutex_unlock(&b->mu);
  }
  pthread_exit(NULL);
}

void *finish_pee(void *br) {
  bathroom *b = (bathroom *)br;
  for (;;) {
    sleep(rand() % 7);
    pthread_mutex_lock(&b->mu);
    if (!isempty(b) && b->urinals[--b->count] == true) {
      b->urinals[b->count] = false;
      printf("A people finish, count = %d\n", b->count);
    }
    pthread_mutex_unlock(&b->mu);
    pthread_cond_signal(&b->full);
  }
  pthread_exit(NULL);
}

int main() {
  bathroom *b = bathroom_create(5);
  pthread_t threads[4];

  pthread_create(&threads[0], NULL, finish_pee, (void *)b);
  pthread_create(&threads[1], NULL, someone_need_to_pee, (void *)b);
  pthread_create(&threads[2], NULL, someone_need_to_pee, (void *)b);
  pthread_create(&threads[3], NULL, someone_need_to_pee, (void *)b);

  pthread_exit(NULL);
  bathroom_destroy(b);
}

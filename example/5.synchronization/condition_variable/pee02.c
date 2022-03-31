#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct bathroom {
  int count;
  int capacity;
  bool *urinals;
  pthread_mutex_t mu;
  pthread_cond_t full;
} bathroom_t;

bathroom_t *bathroom_create(const int capacity) {
  bathroom_t *b = malloc(sizeof(bathroom_t));
  b->count = 0;
  b->capacity = capacity;
  b->urinals = malloc(sizeof(bool) * capacity);
  pthread_mutex_init(&b->mu, NULL);
  pthread_cond_init(&b->full, NULL);
  return b;
}

void bathroom_t_destroy(bathroom_t *b) {
  free(b->urinals);
  pthread_mutex_destroy(&b->mu);
  pthread_cond_destroy(&b->full);
  free(b);
}

bool bathroom_full(bathroom_t *b) { return b->count == b->capacity; }
bool bathroom_empty(bathroom_t *b) { return b->count == 0; }

void *someone_need_to_pee(void *br) {
  bathroom_t *b = (bathroom_t *)br;
  for (;;) {
    sleep(rand() % 3);
    pthread_mutex_lock(&b->mu);
    while (bathroom_full(b))
      pthread_cond_wait(&b->full, &b->mu);
    if (!b->urinals[b->count])
      b->urinals[b->count++] = true;
    printf("Peeing count = %d\n", b->count);
    pthread_mutex_unlock(&b->mu);
  }
  pthread_exit(NULL);
}

void *someone_finish_pee(void *br) {
  bathroom_t *b = (bathroom_t *)br;
  for (;;) {
    sleep(rand() % 2);
    pthread_mutex_lock(&b->mu);
    if (!bathroom_empty(b) && b->urinals[--b->count]) {
      b->urinals[b->count] = false;
      printf("A people finish, count = %d\n", b->count);
    }
    pthread_cond_signal(&b->full);
    pthread_mutex_unlock(&b->mu);
  }
  pthread_exit(NULL);
}

int main() {
  bathroom_t *b = bathroom_create(5);
  pthread_t threads[4];

  pthread_create(&threads[0], NULL, someone_finish_pee, (void *)b);
  pthread_create(&threads[1], NULL, someone_need_to_pee, (void *)b);
  pthread_create(&threads[2], NULL, someone_need_to_pee, (void *)b);
  pthread_create(&threads[3], NULL, someone_need_to_pee, (void *)b);

  pthread_exit(NULL);
  bathroom_t_destroy(b);
}

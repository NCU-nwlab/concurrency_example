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
} bathroom_t;

bathroom_t *bathroom_t_create(const int capacity) {
  bathroom_t *b = malloc(sizeof(bathroom_t));
  b->count = 0;
  b->capacity = capacity;
  b->urinals = malloc(sizeof(bool) * capacity);
  pthread_mutex_init(&b->mu, NULL);
  return b;
}

void bathroom_t_destroy(bathroom_t *b) {
  free(b->urinals);
  pthread_mutex_destroy(&b->mu);
  free(b);
}

bool isempty(bathroom_t *b) { return b->count == 0; }

void *someone_need_to_pee(void *br) {
  bathroom_t *b = (bathroom_t *)br;
  for (;;) {
    sleep(rand() % 3);
    pthread_mutex_lock(&b->mu);
    if (b->count == b->capacity) {
        pthread_mutex_unlock(&b->mu);
        continue;
    }
    if (!b->urinals[b->count])
      b->urinals[b->count++] = true;
    printf("Peeing count = %d\n", b->count);
    pthread_mutex_unlock(&b->mu);
  }
  pthread_exit(NULL);
}

void *finish_pee(void *br) {
  bathroom_t *b = (bathroom_t *)br;
  for (;;) {
    sleep(rand() % 3);
    pthread_mutex_lock(&b->mu);
    if (!isempty(b) && b->urinals[--b->count]) {
      b->urinals[b->count] = false;
      printf("A people finish, count = %d\n", b->count);
    }
    pthread_mutex_unlock(&b->mu);
  }
  pthread_exit(NULL);
}

int main() {
  bathroom_t *b = bathroom_t_create(5);
  pthread_t threads[4];

  pthread_create(&threads[0], NULL, finish_pee, (void *)b);
  pthread_create(&threads[1], NULL, someone_need_to_pee, (void *)b);
  pthread_create(&threads[2], NULL, someone_need_to_pee, (void *)b);
  pthread_create(&threads[3], NULL, someone_need_to_pee, (void *)b);

  pthread_exit(NULL);
  bathroom_t_destroy(b);
}

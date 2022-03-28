#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>
#include <unistd.h>

#define BufferSize 5

sem_t empty, full;
int buff[BufferSize];

void *producer() {
    int idx = -1;
    for (;;) {
        sem_wait(&empty);
        idx = (idx + 1) % BufferSize;
        printf("Producer %d\n", idx);
        sem_post(&full);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumer() {
    int idx = -1;
    for (;;) {
        sem_wait(&full);
        idx = (idx + 1) % BufferSize;
        printf("Consumer %d\n", idx);
        sem_post(&empty);
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[2];
    sem_init(&empty, 0, BufferSize);
    sem_init(&full, 0, 0);

    pthread_create(&threads[0], NULL, producer, NULL);
    pthread_create(&threads[1], NULL, consumer, NULL);

    pthread_exit(NULL);
    sem_destroy(&empty);
    sem_destroy(&full);
}
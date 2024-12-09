#include <stdio.h>
#include "queue.h"
#include "consumer.h"
#include "producer.h"
#include "observer.h"

#define CONSUMER_THREAD_COUNT 1
#define PRODUCER_THREAD_COUNT 1

pthread_t producer[PRODUCER_THREAD_COUNT] = {};
pthread_t consumer[CONSUMER_THREAD_COUNT] = {};
pthread_t observer;
pthread_t queue;

packet_t empty_packet = {
        -1,
        PTHREAD_MUTEX_DEFAULT
};

queue_element_t queue_head  = {
        &empty_packet,
        NULL,
        NULL,
};

void create_threads() {
    // create producer thread
    for (int i = 0; i < PRODUCER_THREAD_COUNT; i++) {
        pthread_create(producer+i, NULL, run_producer, NULL);
    }

    // create consumer thread
    for (int i = 0; i < CONSUMER_THREAD_COUNT; i++) {
        pthread_create(consumer+i, NULL, run_consumer, NULL);
    }

    // create observer thread
    pthread_create(&observer, NULL, run_observer, NULL);

    // create queue thread
    pthread_create(&queue, NULL, run_queue, &queue_head);
}

void init() {
  create_threads();
}

void wait_for_termination() {
    for (int i = 0; i < PRODUCER_THREAD_COUNT; i++) {
        pthread_join(producer[i], NULL);
    }

    for (int i = 0; i < CONSUMER_THREAD_COUNT; i++) {
        pthread_join(consumer[i], NULL);
    }

    pthread_join(observer, NULL);
    pthread_join(queue, NULL);
}

int main() {
  init();
  wait_for_termination();

  return 0;
}

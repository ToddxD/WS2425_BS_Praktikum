#include <stdio.h>
#include "queue.h"

#define CONSUMER_THREAD_COUNT = 1
#define PRODUCER_THREAD_COUNT = 1


void create_queue(int* queue_head) {

}

void create_threads() {

}

void init() {
  create_queue(0);
  create_threads();
}

void wait_for_termination() {

}

int main() {
  init();
  wait_for_termination();

  return 0;
}

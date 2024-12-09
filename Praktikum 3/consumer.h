//
// Created by Hendrik on 04.12.2024.
//

#ifndef PRAKTIKUM_3_CONSUMER_H
#define PRAKTIKUM_3_CONSUMER_H

#include <stdio.h>
#include <pthread.h>
#include "queue.h"
#include <unistd.h>

/**
 * Runs this thread.
 */
void* run_consumer(void* p);

/**
 * 1. Consumes queue element in 500ms - 2000ms and frees memory.<br>
 * 2. Blocks if queue is empty.<br>
 * 3. Terminates after all 1000 packets have been consumed.
 */
void consume_queue_element();

#endif //PRAKTIKUM_3_CONSUMER_H

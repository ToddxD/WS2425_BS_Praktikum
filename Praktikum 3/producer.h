//
// Created by Hendrik on 04.12.2024.
//

#ifndef PRAKTIKUM_3_PRODUCER_H
#define PRAKTIKUM_3_PRODUCER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "queue.h"
#include <unistd.h>
#include <errno.h>
#include "p3errno.h"

/**
 * Runs this thread.
 */
void* run_producer(void* p);

/**
 * 1. Creates and adds a random number paket (0-99) and sleeps for 200ms.<br>
 * 2. Blocks for 1s if Queue is full.<br>
 * 3. Terminates after 1000 total packets
 */
void create_queue_element();

#endif //PRAKTIKUM_3_PRODUCER_H

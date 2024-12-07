//
// Created by Hendrik on 04.12.2024.
//

#ifndef PRAKTIKUM_3_QUEUE_H
#define PRAKTIKUM_3_QUEUE_H

#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include "p3errno.h"
#include <stdlib.h>

#define QMAX 25
#define MAX_PACKETS 100

typedef struct packet_t {
    int num;
    pthread_mutex_t mutex;
} packet_t;

typedef struct queue_element_t {
    packet_t packet;
    struct queue_element_t* next;
    struct queue_element_t* prev;
} queue_element_t;

/**
 * Runs the queue
 */
void* run_queue(void* p);

/**
 * Adds a packet to the queue in the correct order
 *
 * @param paket number
 */
int add_paket(packet_t* paket);

/**
 * Returns and removes packet with smallest number.
 *
 * @return packet with smallest number
 */
packet_t* get_paket();

/**
 * Sorts queue.
 */
void sort_queue();

#endif //PRAKTIKUM_3_QUEUE_H

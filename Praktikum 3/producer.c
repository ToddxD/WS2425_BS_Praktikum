//
// Created by Hendrik on 04.12.2024.
//

#include "producer.h"

#define MS *100

static int p_counter = 0;
static pthread_mutex_t mp = PTHREAD_MUTEX_INITIALIZER;


void* run_producer(void* p) {
    srand(time(NULL) * pthread_self());

    pthread_mutex_lock(&mp);
    while (p_counter < MAX_PACKETS) {   // Critical Section
        p_counter++;                    // |
        pthread_mutex_unlock(&mp);

        create_queue_element();

        pthread_mutex_lock(&mp);
    }

    pthread_mutex_unlock(&mp);
    return NULL;
}

void create_queue_element() {
    packet_t* packet_ptr = malloc(sizeof(packet_t));

    //packet_t packet = {
    //        rand() % 100,
    //        PTHREAD_MUTEX_INITIALIZER
    //};

    packet_ptr->num = rand() % 100;
    packet_ptr->mutex = PTHREAD_MUTEX_INITIALIZER;

    while (add_paket(packet_ptr) < 0) {
        if (errno == QUEUE_FULL) {
            usleep(1000 MS); // ist usleep blocked oder busy?
        }
    }
    usleep(200 MS);
}

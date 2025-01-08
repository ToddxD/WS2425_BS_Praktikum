//
// Created by Hendrik on 04.12.2024.
//

#include "consumer.h"

int c_counter = 0;
static pthread_mutex_t mc = PTHREAD_MUTEX_INITIALIZER;

void* run_consumer(void* p) {

    srand(time(NULL) * pthread_self());

    pthread_mutex_lock(&mc);

    while (c_counter < MAX_PACKETS) {   // Critical Section
        c_counter++;                    // |
        pthread_mutex_unlock(&mc);

        consume_queue_element();

        pthread_mutex_lock(&mc);
    }


    pthread_mutex_unlock(&mc);

    return NULL;
}

void consume_queue_element() {
    int time = (rand() % 1500) + 500;

    packet_t* packet = NULL;
    if (get_paket(&packet) == 0) {
        printf("   consumed packet: %d\n", packet->num);
        usleep(time MS);
        //pthread_mutex_destroy(&packet->mutex);
        free(packet);
    }
    // TODO waiting for not empty


}

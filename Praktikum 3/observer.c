//
// Created by Hendrik on 04.12.2024.
//

#include "observer.h"

extern queue_element_t queue_head;
extern int c_counter;

void* run_observer(void* p) {
    while(c_counter < MAX_PACKETS) {
        print_queue_status();
        usleep(1000 MS);
    }

    print_if_done();
    return NULL;
}

void print_if_done() {
    printf("Ende\n");
}

void print_queue_status() {
    pthread_mutex_lock(&queue_head.packet->mutex);
    queue_element_t* current = &queue_head;

    int queue_index = 0;

    printf("Queue:\n");

    while (current != NULL) {
        printf("| %d. packet: %d\n", queue_index, current->packet->num);
        queue_index++;
        if (current->next == NULL) {
            pthread_mutex_unlock(&current->packet->mutex);
            break;
        }

        current = current->next;

        pthread_mutex_unlock(&current->prev->packet->mutex);

        if (current->next != NULL) {
            pthread_mutex_lock(&current->next->packet->mutex);
        }
    }
    printf("______\n");
}

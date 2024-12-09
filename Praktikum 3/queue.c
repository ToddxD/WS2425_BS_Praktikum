//
// Created by Hendrik on 04.12.2024.
//

#include "queue.h"


extern queue_element_t queue_head;

void* run_queue(void* p) {
    return NULL;
}

int add_paket(packet_t* paket) {
    queue_element_t* current = &queue_head;
    int queue_index = 0;

    while (current != NULL) {
        if (queue_index >= QMAX) {

            errno = QUEUE_FULL;
            return -1;
        }

        if (paket->num == current->packet->num) {
            free(paket);
            errno = ALREADY_IN_QUEUE;
            return -1;
        }

        if (paket->num < current->packet->num) {

            pthread_mutex_lock(&current->prev->packet->mutex);
            pthread_mutex_lock(&current->packet->mutex);
            // new packet
            queue_element_t* new = malloc(sizeof(queue_element_t));
            new->packet = paket;
            new->next = current;
            new->prev = current->prev;

            // previous packet points to new
            current->prev->next = new;

            // current packet points to new
            current->prev = new;

            pthread_mutex_unlock(&current->packet->mutex);
            pthread_mutex_unlock(&new->prev->packet->mutex);
            break;
        }

        if (current->next != NULL) {
            queue_index++;
            current = current->next;
        } else { // end of queue
            // new packet
            pthread_mutex_lock(&current->packet->mutex);
            queue_element_t* new = malloc(sizeof(queue_element_t));
            new->packet = paket;
            new->prev = current;
            new->next = NULL;

            // current packet points to new
            current->next = new;

            pthread_mutex_unlock(&current->packet->mutex);
            break;
        }
    }


  return 0;
}

int get_paket(packet_t** r_packet) {
    pthread_mutex_lock(&queue_head.packet->mutex);
    if (queue_head.next != NULL) {
        pthread_mutex_lock(&queue_head.next->packet->mutex);
    }
    if (queue_head.next->next != NULL) {
        pthread_mutex_lock(&queue_head.next->next->packet->mutex);
    }

    queue_element_t* first = queue_head.next;

    if (first == NULL) {
        if (queue_head.next->next != NULL) {
            pthread_mutex_unlock(&queue_head.next->next->packet->mutex);
        }
        if (queue_head.next != NULL) {
            pthread_mutex_unlock(&queue_head.next->packet->mutex);
        }
        pthread_mutex_unlock(&queue_head.packet->mutex);
        errno = QUEUE_EMPTY;
        return -1;
    }

    // head points to new next
    queue_head.next = first->next;

    // new next points to head
    if (first->next != NULL) {
        first->next->prev = &queue_head;
    }

    *r_packet = first->packet;

    if (queue_head.next != NULL && queue_head.next->next != NULL) {
        pthread_mutex_unlock(&queue_head.next->next->packet->mutex);
    }

    if (queue_head.next != NULL) {
        pthread_mutex_unlock(&queue_head.next->packet->mutex);
    }
    pthread_mutex_unlock(&queue_head.packet->mutex);

    free(first);

    return 0;
}

//
// Created by Hendrik on 04.12.2024.
//

#include "queue.h"


extern queue_element_t queue_head;
sem_t sem;

static int queue_index = 0;

void* run_queue(void* p) {
    return NULL;
}

void init_queue() {
    sem_init(&sem, 0, QMAX);
}

int add_paket(packet_t* packet) {
    /*if (sem_trywait(&sem) != 0) {
        errno = QUEUE_FULL;
        return -1;
    }*/

    //sem_wait(&sem);

    queue_element_t* current = &queue_head;

    pthread_mutex_lock(&current->packet->mutex); //a
    if (queue_index >= QMAX) {

        pthread_mutex_unlock(&current->packet->mutex); //a
        errno = QUEUE_FULL;
        return -1;
    }

    while (current != NULL) {
        //printf("loop, packet: %d\n", packet->num);
        if (packet->num == current->packet->num) {
            if (current->prev != NULL) {
                pthread_mutex_unlock(&current->prev->packet->mutex); //a
            }
            pthread_mutex_t dele = packet->mutex;
            pthread_mutex_unlock(&dele); //b
            pthread_mutex_destroy(&dele);
            free(packet);
            //sem_post(sem);
            errno = ALREADY_IN_QUEUE;
            return -1;
        }

        if (packet->num < current->packet->num) {
            // new packet
            queue_element_t* new = malloc(sizeof(queue_element_t));
            new->packet = packet;
            new->next = current;
            new->prev = current->prev;

            // previous packet points to new
            current->prev->next = new;

            // current packet points to new
            current->prev = new;

            pthread_mutex_unlock(&new->prev->packet->mutex);//a
            pthread_mutex_unlock(&current->packet->mutex);//b
            break;
        }

        if (current->prev != NULL) {
            pthread_mutex_unlock(&current->prev->packet->mutex); //a
        }
        if (current->next != NULL) {

            pthread_mutex_lock(&current->next->packet->mutex); //b
            current = current->next;

        } else { // end of queue
            // new packet
            queue_element_t* new = malloc(sizeof(queue_element_t));
            new->packet = packet;
            new->prev = current;
            new->next = NULL;

            // current packet points to new
            current->next = new;

            if (current->prev != NULL) {
                pthread_mutex_unlock(&current->prev->packet->mutex); //a
            }
            pthread_mutex_unlock(&current->packet->mutex); //b
            break;
        }
    }

    //sem_post(&sem);

    queue_index++;
    return 0;
}

int get_paket(packet_t** r_packet) {
    pthread_mutex_lock(&queue_head.packet->mutex); //a
    if (queue_head.next != NULL) {
        pthread_mutex_lock(&queue_head.next->packet->mutex); //b
    }
    if (queue_head.next != NULL && queue_head.next->next != NULL) {
        pthread_mutex_lock(&queue_head.next->next->packet->mutex); //c
    }

    queue_element_t* first = queue_head.next;

    if (first == NULL) {
        pthread_mutex_unlock(&queue_head.packet->mutex); //a
        errno = QUEUE_EMPTY;
        return -1;
    }

    //printf("getting %d", first->packet->num);

    // head points to new next
    queue_head.next = first->next;

    // new next points to head
    if (first->next != NULL) {
        first->next->prev = &queue_head;
        pthread_mutex_unlock(&queue_head.next->packet->mutex); //c
    }

    *r_packet = first->packet;
    queue_index--;
    pthread_mutex_t del = first->packet->mutex;
    first->packet = NULL;
    free(first);
    pthread_mutex_unlock(&del); //b
    pthread_mutex_unlock(&queue_head.packet->mutex); //a

    return 0;
}

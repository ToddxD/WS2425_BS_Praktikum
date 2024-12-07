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

        if (paket->num == current->packet.num) {
            free(paket);
            errno = ALREADY_IN_QUEUE;
            return -1;
        }

        if (paket->num < current->packet.num) {

            // new packet
            queue_element_t* new = malloc(sizeof(queue_element_t));
            new->packet = *paket;
            new->next = current;
            new->prev = current->prev;

            // previous packet points to new
            current->prev->next = new;

            // current packet points to new
            current->prev = new;
            break;
        }

        if (current->next != NULL) {
            queue_index++;
            current = current->next;
        } else { // end of queue
            // new packet
            queue_element_t* new = malloc(sizeof(queue_element_t));
            new->packet = *paket;
            new->prev = current;

            // current packet points to new
            current->next = new;
            break;
        }
    }


  return 0;
}

packet_t* get_paket() {
  return 0;
}

void sort_queue() {

}

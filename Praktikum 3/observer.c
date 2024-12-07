//
// Created by Hendrik on 04.12.2024.
//

#include "observer.h"

#define MS *1000

extern queue_element_t queue_head;

void* run_observer(void* p) {
    while(1) {
        print_queue_status();
        usleep(1000 MS);
    }

    return NULL;
}

void print_if_done() {

}

void print_queue_status() {
    queue_element_t* current = &queue_head;
    int queue_index = 0;

    printf("Queue:\n");
    while (current != NULL) {
        printf("%d. packet: %d\n", queue_index, current->packet.num);
        queue_index++;
        current = current->next;
    }
}

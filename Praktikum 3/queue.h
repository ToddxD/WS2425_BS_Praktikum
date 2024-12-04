//
// Created by Hendrik on 04.12.2024.
//

#ifndef PRAKTIKUM_3_QUEUE_H
#define PRAKTIKUM_3_QUEUE_H

/**
 * Adds a packet to the queue in the correct order
 *
 * @param paket number
 */
int add_paket(int paket);

/**
 * Returns and removes packet with smallest number.
 *
 * @return packet with smallest number
 */
int get_paket();

/**
 * Sorts queue.
 */
void sort_queue();

#endif //PRAKTIKUM_3_QUEUE_H

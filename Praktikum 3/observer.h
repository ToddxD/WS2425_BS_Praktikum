//
// Created by Hendrik on 04.12.2024.
//

#ifndef PRAKTIKUM_3_OBSERVER_H
#define PRAKTIKUM_3_OBSERVER_H

#include <stdio.h>
#include "queue.h"
#include <unistd.h>

/**
 * Runs this thread.
 */
void* run_observer(void* p);

/**
 * Prints "Ende" if all 1000 packets have been consumed.
 * @return
 */
void print_if_done();

/**
 * Prints queue elements every second.
 * @return
 */
void print_queue_status();

#endif //PRAKTIKUM_3_OBSERVER_H

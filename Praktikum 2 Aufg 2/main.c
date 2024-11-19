#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>

#define RUNDEN 10
#define ANZAHL_RACER 5

typedef struct Pod_Racer {
    char name[256];
    int rennzeit;
    pthread_t thread_id;
} Pod_Racer;

volatile bool crashed = false;

void* fliegen(void* r) {
  Pod_Racer* racer = (Pod_Racer*) r;
  srand(time(NULL) * racer->thread_id);

  if (rand() % 2 == 0) {
    printf("Racer %s CRASHED", racer->name);
    crashed = true;
  }

  if (crashed) {
    return NULL;
  }

  for (int i = 0; i < RUNDEN; i++) {
    int zeit = (rand() % 100);
    racer->rennzeit += zeit;
    usleep(zeit * 10000);
  }

  printf("Racer %s ist mit %d angekommen\n", racer->name, racer->rennzeit);
  return NULL;
}

void createPodProcess(Pod_Racer* racer) {
  pthread_create(&racer->thread_id, NULL, fliegen, (void*) racer);
}

void waitForPods(Pod_Racer racer[]) {
  for (int i = 0; i < ANZAHL_RACER; i++) {
    pthread_join(racer->thread_id, NULL);
  }
}

void swap(Pod_Racer* xp, Pod_Racer* yp){
  Pod_Racer temp = *xp;
  *xp = *yp;
  *yp = temp;
}


void sort(Pod_Racer arr[], int n){
  int i, j;
  bool swapped;
  for (i = 0; i < n - 1; i++) {
    swapped = false;
    for (j = 0; j < n - i - 1; j++) {
      if (arr[j].rennzeit > arr[j + 1].rennzeit) {
        swap(&arr[j], &arr[j + 1]);
        swapped = true;
      }
    }

    if (swapped == false)
      break;
  }
}

int main() {
  Pod_Racer racer[ANZAHL_RACER] = {
          {"Erster", 0},
          {"Zweiter", 0},
          {"Dritter", 0},
          {"Vierter", 0},
          {"Fuenfter", 0}
  };

  for (int i = 0; i < ANZAHL_RACER; i++) {
    createPodProcess(&racer[i]);
  }

  waitForPods(racer);

  if (crashed) {
    return 0;
  }

  sort(racer, ANZAHL_RACER);

  for (int i = 0; i < ANZAHL_RACER; i++) {
    printf("%d. Platz: Pod %s, Zeit %d ms\n", i + 1, racer[i].name, racer[i].rennzeit);
  }

  return 0;
}

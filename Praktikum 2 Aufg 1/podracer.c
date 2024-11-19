#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
//#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>

#define RUNDEN 10
#define ANZAHL_RACER 5

typedef struct Pod_Racer {
    char name[256];
    int rennzeit;
} Pod_Racer;

void* fliegen(void* r) {
  Pod_Racer* racer = (Pod_Racer*) r;
  for (int i = 0; i <= RUNDEN; i++) {
    racer->rennzeit += (rand() % 100);
    sleep(100);
  }
  return NULL;
}

void fliegen2(Pod_Racer* racer) {
  for (int i = 0; i <= RUNDEN; i++) {
    racer->rennzeit += (rand() % 100);
    sleep(100);
  }
  printf("Racer %s ist mit %d angekommen", racer->name, racer->rennzeit);
}

int createPodProcess() {
  //pthread_t thread_id;
  //pthread_create(&thread_id, NULL, fliegen, NULL);

}

int main() {
  Pod_Racer racer[ANZAHL_RACER] = {
          {"Erster", 0},
          {"Zweiter", 0},
          {"Dritter", 0},
          {"Vierter", 0},
          {"Fünfter", 0}
  };

  fliegen2(&racer[0]);

  return 0;
}

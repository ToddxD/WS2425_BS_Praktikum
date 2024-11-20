#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

#define RUNDEN 5
#define ANZAHL_RACER 5
#define CRASH_FACTOR 20

typedef struct Pod_Racer {
    char name[256];
    int rennzeit;
    pthread_t thread_id;
} Pod_Racer;

static volatile bool crashed = false;

void* fliegen(void* r) {
  Pod_Racer* racer = (Pod_Racer*) r;

  // Seed für Random wählen. Abhängig der aktuellen Zeit und der Thread-ID (damit Thread-Safe)
  srand(time(NULL) * racer->thread_id);

  for (int i = 0; i < RUNDEN; i++) {
    int zeit = (rand() % 1000);

    // Prüfe Crash
    if (zeit <= CRASH_FACTOR && !crashed) {
      crashed = true;
      printf("Racer %s CRASHED", racer->name);
    }

    // Rundenzeit addieren
    racer->rennzeit += zeit;
    usleep(zeit * 10000);

    // Beenden, wenn irgendein Pod gecrashed ist
    if (crashed) {
      return NULL;
    }

    printf("[debug] Racer %s ist %d Runde/n gefahren. Rundenzeit: %d, Gesamtzeit: %d\n", racer->name, i+1, zeit, racer->rennzeit);
  }

  printf("[debug] Racer %s ist mit %d angekommen\n", racer->name, racer->rennzeit);
  return NULL;
}

void createPodProcesses(Pod_Racer racer[]) {
  for (int i = 0; i < ANZAHL_RACER; i++) {
    pthread_create(&(racer+i)->thread_id, NULL, fliegen, (void*) (racer+i));
  }
}

void waitForPods(Pod_Racer racer[]) {
  for (int i = 0; i < ANZAHL_RACER; i++) {
    pthread_join((racer+i)->thread_id, NULL);
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

void printErgebnis(Pod_Racer racer[]) {
  printf("*** Ergebnis ***\n");
  for (int i = 0; i < ANZAHL_RACER; i++) {
    printf("%d. Platz: Pod %s, Zeit %d ms\n", i + 1, racer[i].name, racer[i].rennzeit);
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

  createPodProcesses(racer);
  waitForPods(racer);

  if (crashed) {
    return 0;
  }

  sort(racer, ANZAHL_RACER);
  printErgebnis(racer);

  return 0;
}

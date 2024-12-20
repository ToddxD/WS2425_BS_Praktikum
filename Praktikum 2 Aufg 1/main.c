#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>


void prompt() {
  char buffer[256];
  getcwd(buffer, 256);
  printf("%s>>>", buffer);
}

void read_command(char* command) {
  fgets(command, 256, stdin);
}

void trimString(char* str) {
  for (int i = strlen(str) - 1; i >= 0; i--) {
    if (str[i] != '\0' && str[i] != ' ' && str[i] != '\n') {
      return;
    }
    str[i] = '\0';
  }
}

int startProcess(char* command) {
  pid_t pid = fork(); /* Kind erzeugen */
  if (pid < 0) {
    printf("Unable to fork"); /* Fehlerbedingung */
    errno = 10;
    return -1; /* Schleife wiederholen */
  }

  if (pid > 0) {
    int status;
    if (command[strlen(command) - 1] != '&') {
      printf("waiting\n");
      waitpid(pid, &status, 0); /* Elternprozess wartet auf Kind */
    }
    printf("resumed, status:%d\n", status);
  } else {
    if (command[strlen(command) - 1] == '&') {
      command[strlen(command) - 1] = '\0';
    }
    execlp(command, command, 0); /* Das Kind-Programm ausführen */
    return -1;
  }
  return 0;
}

int hawsh() {
  printf("Willkommen in der besten Shell der Welt!\n");
  while (true) { /* Endlosschleife */
    prompt(); /* Prompt ausgeben */
    char command[256] = {};

    read_command(command); /* Eingabezeile von Tastatur lesen */
    trimString(command);
    if (strcmp(command, "quit") == 0) {
      printf("Ciao Kakao\n");
      return 0;
    } else if(strcmp(command, "version") == 0) {
      printf("HAW Shell Version 1.0 Autor: Hendrik Pilz \n");
    } else if (strcmp(command, "help") == 0) {
      printf("-- Die selbst programmierte HAW Shell --\n\n"
             "Befehle:\n\tversion \t Anzeige des Autors und der Versionsnummer der HAW-Shell\n\t/[Pfadname] \t Wechsel des aktuellen Arbeitsverzeichnisses. Es muss immer ein kompletter Pfadname eingegeben werden.\n\thelp \t\t Diese Hilfe-Seite\n\tquit \t\t Beendet die Shell\n----------------------------------------\n");
    } else if (command[0] == '/') {
      chdir(command);
    } else if (strcmp(command, "") != 0) {
      if (startProcess(command) == -1) {
        printf("ERROR: %s\n", strerror(errno));
        errno = 0;
        return 0;
      }
    } else {
      printf("Kein gültiger Command\n");
    }
  }
  return 0;
}

int main() {
  hawsh();
  return 0;
}


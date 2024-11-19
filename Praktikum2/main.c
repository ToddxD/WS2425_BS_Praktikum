#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void prompt() {
  char buffer[256];
  getcwd(buffer, sizeof(buffer));
  printf("%s?", buffer);
}

void read_command(char* command) {
  fgets(command, sizeof(command),stdin);
}

int hawsh() {
  while (true) { /* Endlosschleife */
    prompt(); /* Prompt ausgeben */
    char command[256];
    char* params[0];

    read_command(command/*, &params*/); /* Eingabezeile von Tastatur lesen */

    if (strcmp(command, "quit")) {
      return 0;
    }


    pid_t pid = fork(); /* Kind erzeugen */
    if (pid < 0) {
      printf("Unable to fork"); /* Fehlerbedingung */
      continue; /* Schleife wiederholen */
    }

    if (pid > 0) {
      int status;
      waitpid(pid, &status, 0); /* Elternprozess wartet auf Kind */
    } else {
      execve(command, params, 0); /* Das Kind-Programm ausführen */
    }
  }

  "-- Die selsbt programmierte HAW Shell --\n\nBefehle:\n\tversion \tAnzeige des Autors und der Versionsnummer der HAW-Shell\n\t/[Pfadname] \t Wechsel des aktuellen Arbeitsverzeichnisses. Es muss immer ein kompletter Pfadname eingegeben werden.\n\thelp \t\t Diese Hilfe-Seite\n\tquit \t\t Beendet die Shell\n----------------------------------------\n"

  return
}

int main() {
  hawsh();
  return 0;
}


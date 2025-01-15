#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>



int main() {
  //int fd = open("/dev/decrypt", O_RDWR);
  //int fd2 = open("/dev/encrypt", O_RDWR);
//
  //printf("open...");
  //printf("%s\n", strerror(errno));
//
  //char* buf = "CBA//cDb a";
  //char* buf2 = "zyx// AZXY";
  //write(fd, buf, 10);
  //write(fd2, buf2, 10);
//
  //printf("write...");
  //printf("%s\n", strerror(errno));
//
  //char* out = malloc(sizeof(char) * 10);
  //char* out2 = malloc(sizeof(char) * 10);
  //read(fd, out, 10);
  //read(fd2, out2, 10);
  //printf("expectedDec: zyx// AZXY\n");
  //printf("verschluesselt: %s\n", out);
  //printf("expectedEnc: CBA//cDb a\n");
  //printf("entschluesselt: %s\n", out2);
  //printf("%s\n", strerror(errno));
//
  //close(fd);
  //close(fd2);
  //free(out);
  //free(out2);

  int dec_dev = 0;
  int enc_dev = 0;

  while (1) {
    printf("Moin! Encrypt oder Decrypt Device öffnen\n");
    char* input = malloc(sizeof(char) * 40);
    scanf("%s", input);
    if(strcmp("help", input)){
      printf("Liste der möglichen Befehle:\n "
             "help: Ausgabe der möglichen Befehle\n"
             "oe: Öffnen des Encrypt Devices\n"
             "od: Öffnen des Decrypt Devices\n"
             "we: Benötigt einene Text, welcher verschlüsselt werden soll\n"
             "wd: Benötigt einene Text, welcher entschlüsselt werden soll\n"
             "re: Liest den aktuell im Encrypt Buffer gespeicherten Text aus\n"
             "rd: Liest den aktuell im Decrypt Buffer gespeicherten Text aus\n"
             "ce: Schließt das Encrypt Device\n"
             "cd: Schließt das Decrypt Device\n");
    }
    if (strcmp("oe", input)) {
      dec_dev = open("/dev/decrypt", O_RDWR);
      if (dec_dev < 0) {
        printf("Device konnte nicht geöffnet werden, Error: %s\n", strerror(errno));
      }
    } else if (strcmp("od", input)) {
      enc_dev = open("/dev/encrypt", O_RDWR);
      if (enc_dev < 0) {
        printf("Device konnte nicht geöffnet werden, Error: %s\n", strerror(errno));
      }
    } else if (strcmp("wd", input)) {
      printf("Gib den Text zum entschluesseln an:\n");
      scanf("%s", input);
      int err;
      err = write(dec_dev, input, 40);
      if (err < 0) {
        printf("%s\n", strerror(errno));
      }
    } else if (strcmp("we", input)) {
      printf("Gib den Text zum verschluesseln an:\n");
      scanf("%s", input);
      int err;
      err = write(enc_dev, input, 40);
      if (err < 0) {
        printf("%s\n", strerror(errno));
      }
    } else if (strcmp("rd", input)) {
      int err;
      err = read(dec_dev, input, 40);
      printf("Entschluesselter Text: %s\n", input);
      if (err < 0) {
        printf("%s\n", strerror(errno));
      }
    } else if (strcmp("re", input)) {
      int err;
      err = read(enc_dev, input, 40);
      printf("Verschluesselter Text: %s\n", input);
      if (err < 0) {
        printf("%s\n", strerror(errno));
      }
    } else if (strcmp("ce", input)) {
      int err = close(dec_dev);
      if (err < 0) {
        printf("%s\n", strerror(errno));
      }
    } else if (strcmp("cd", input)) {
      int err = close(enc_dev);
      if (err < 0) {
        printf("%s\n", strerror(errno));
      }
    } else {
        printf("Befehl nicht erkannt, bitte korrekten Befehl eingeben, 'help' für Hilfe\n");
    }
    free(input);

  }
  return 0;
}
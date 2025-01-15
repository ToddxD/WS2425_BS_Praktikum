#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>

int main() {
  int fd = open("/dev/decrypt", O_RDWR);
  int fd2 = open("/dev/encrypt", O_RDWR);

  printf("open...");
  printf("%s\n", strerror(errno));

  char* buf = "CBA//cDb a";
  char* buf2 = "zyx// AZXY";
  write(fd, buf, 10);
  write(fd2, buf2, 10);

  printf("write...");
  printf("%s\n", strerror(errno));

  char* out = malloc(sizeof(char) * 10);
  char* out2 = malloc(sizeof(char) * 10);
  read(fd, out, 10);
  read(fd2, out2, 10);
  printf("expectedDec: zyx// AZXY\n");
  printf("verschluesselt: %s\n", out);
  printf("expectedEnc: CBA//cDb a\n");
  printf("entschluesselt: %s\n", out2);
  printf("%s\n", strerror(errno));

  close(fd);
  close(fd2);
  free(out);
  free(out2);
  return 0;
}
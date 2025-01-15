#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>

int main() {
  int fd = open("/dev/decrypt", O_RDWR);
  int fd2 = open("/dev/decrypt", O_RDWR);
  //int fd = open("/dev/encrypt", O_RDWR);

  printf("open...");
  printf("%s\n", strerror(errno));

  char* buf = "CBA//cDb a";
  write(fd, buf, 10);

  printf("write...");
  printf("%s\n", strerror(errno));

  char* out = malloc(sizeof(char) * 10);
  read(fd, out, 10);
  printf("expected: zyx// AZXY\n");
  printf("verschluesselt: %s\n", out);
  printf("%s\n", strerror(errno));

  close(fd);
  return 0;
}
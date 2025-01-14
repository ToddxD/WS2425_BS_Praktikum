#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>

int main() {
  printf("before...");
  printf("%s\n", strerror(errno));
  int fd = open("/dev/encrypt", 0_RDWR);

  printf("open...");
  printf("%s\n", strerror(errno));

  char* buf = malloc(sizeof(char));
  write(fd, buf, 10);

  printf("write...");
  printf("%s\n", strerror(errno));

  close(fd);
  free(buf);
  return 0;
}
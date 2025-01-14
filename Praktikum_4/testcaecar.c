#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <string.h>

int main() {
  int fd = open("/dev/encrypt", O_RDWR);

  printf("open...");
  printf("%s\n", strerror(errno));

  char* buf = malloc(10*sizeof(char));
  buf = "abcdefghij";
  write(fd, buf, 10);

  printf("write...");
  printf("%s\n", strerror(errno));

  close(fd);
  free(buf);
  return 0;
}
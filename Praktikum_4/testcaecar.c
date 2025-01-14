#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>

int main() {
  int fd = open("/dev/encrypt", O_NONBLOCK);

  printf("opened: %s\n", strerror(errno));

  char* buffer = malloc(sizeof(char));
  write(fd, buffer, 10);

  printf("written: %s\n", strerror(errno));

  close(fd);
  free(buffer);
  return 0;
}
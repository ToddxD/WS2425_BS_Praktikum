#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  int fd = open("/dev/encrypt", O_NONBLOCK);

  char buffer[0] = {};
  write(fd, buffer, 10);
  printf("written");

  close(fd);

  return 0;
}
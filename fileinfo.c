#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *args[]) {
  struct stat buf;
  stat(args[1], &buf);

  printf("%dB\n", (int)buf.st_size);
  return 0;
}

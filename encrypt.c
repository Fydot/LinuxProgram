#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* args[]) {
  char *input, *output;
  input = (char*)calloc(20000, 1);
  scanf("%s", input);
  output = crypt(input, "hddhasasalt");
  printf("%s\n", output);
  free(input);
  return 0;
}

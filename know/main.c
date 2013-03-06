#include "calculate.h"
#include <stdio.h>


int
main(int argc, char* args[]) {
  int a, b;
  while(~scanf("%d%d", &a, &b)) {
    add(a, b);
    mult(a, b);
  }
  return 0;
}

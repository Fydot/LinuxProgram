#include <stdio.h>


typedef char (*T3)(void);
typedef T3 T2[3];
typedef T2 *T1;
T1 x(void);

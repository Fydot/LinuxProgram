#include <stdio.h>

typedef int (*F)(int a, int b);

char a[4][3][2] = {{{'a', 'b'}, {'c', 'd'}, {'e', 'f'}},
                  {{'g', 'h'}, {'i', 'j'}, {'k', 'l'}},
                  {{'m', 'n'}, {'o', 'p'}, {'q', 'r'}},
                  {{'s', 't'}, {'u', 'v'}, {'w', 'x'}}};



int add(int a, int b)
{
        return a + b;
}

int sub(int a, int b)
{
        return a - b;
}

F funcs[] = {add, sub};
enum FS {F_ADD, F_SUB};

int main(void)
{
        char (*pa)[2] = &a[1][0];
        char (*ppa)[3][2] = &a[1];
        char (*pppa)[4][3][2] = &a;

        printf("%c %c %c\n", *((*(pa+5)) + 1), *((*((*(ppa+1))+2))+1), *(*(*(*(pppa) + 2) + 2) + 1));

        printf("%d %d\n", (funcs[F_ADD])(1, 2), (funcs[F_SUB])(2, 1));

        return 0;
}

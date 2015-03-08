#include <stdio.h>

int main()
{
    char* names = "hhhhhhhh";
    FILE *out;
    int i;

    out = fopen("data", "w");
    if (!out) {
        perror("fopen");
        return 1;
    }
    for(i = 0; names[i]; i++) {
        if(fputc(names[i], out) == EOF) {
            perror("fputc");
            return 1;
        }
        long pos = ftell(out);
        printf("pos: %ld\n", pos);

        if (fseek(out, 2, SEEK_CUR) == -1) {
            perror("fseek");
            return 1;
        }
    }
    fclose(out);
}

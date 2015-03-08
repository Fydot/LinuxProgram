#include <stdio.h>


typedef struct {
    double x, y;
} point;


int main(void)
{
    point p1 = {.x=1.0, .y=2.0};
    point p2;

    FILE* out = fopen("data", "w");
    if(!out) {
        perror("fopen");
        return 1;
    }
    if(!fwrite(&p1, sizeof(point), 1, out)){
        perror("fwrite");
        return 1;
    }
    if(fclose(out)){
        perror("fclose");
        return 1;
    }

    FILE* in = fopen("data", "r");
    if(!in){
        perror("fopen");
        return 1;
    }
    if(!fread(&p2, sizeof(point), 1, in)){
        perror("fread");
        return 1;
    }
    if(fclose(in)){
        perror("fclose");
        return 1;
    }

    printf("%lf %lf\n", p2.x, p2.y);
    
    return 0;
}
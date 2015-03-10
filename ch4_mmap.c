#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char* argv[])
{
    int fd, index;
    struct stat sta;
    char *ptr;

    if ((fd = open(argv[1], O_RDWR)) == -1) {
        perror("open argv");
        return 1;
    }
    if (fstat(fd, &sta) == -1) {
        perror("fstat fd");
        return 1;
    }

    if ((ptr = mmap(NULL, sta.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
 
    memset(ptr, 'h', sta.st_size);

    for (index = 0; index < sta.st_size; index ++) {
        printf("%c", *(ptr+index));
    }

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    if(munmap(ptr, sta.st_size) == -1) {
        perror("munmap");
        return 1;
    }

    return 0;
}

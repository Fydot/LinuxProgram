#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>

#define BUFLEN 3

typedef struct iovec vec;

int main()
{
    vec vecs[BUFLEN];
    int fd, i;

    char *buf[] = {"huangdiandian.\n", "liuyuan.\n", "huangjianjian.\n"};

    fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

    for (i = 0; i < BUFLEN; i++) {
        vecs[i].iov_base = buf[i];
        vecs[i].iov_len = strlen(buf[i]);
    }

    if(writev(fd, vecs, BUFLEN) == -1) {
        perror("writev");
        return 1;
    }

    close(fd);

    return 0;
}

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

    fd = open("data.txt", O_RDONLY);

    char me[40], wife[40], bro[40];
    vecs[0].iov_base = me;
    vecs[0].iov_len = strlen(me);
    vecs[1].iov_base = wife;
    vecs[1].iov_len = strlen(wife);
    vecs[2].iov_base = bro;
    vecs[2].iov_len = strlen(bro);

    if(readv(fd, vecs, BUFLEN) == -1) {
        perror("readv");
        return 1;
    }

    for (i = 0; i < BUFLEN; i++) {
        printf("%s\n", (char *)vecs[i].iov_base);
    }

    close(fd);

    return 0;
}

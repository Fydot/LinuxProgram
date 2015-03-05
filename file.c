#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
    int fd;

	if((fd = open("/Users/hdd/main.txt", O_RDONLY)) == -1) {
	    perror("open");
		return 1;
	}

    char *buf;
    char buf1[] = "abcd\0\ncdjcn";
    ssize_t ret;

    printf("length %ld %ld\n", sizeof(buf1) / sizeof(char), strlen(buf1));

    while ((ret = read(fd, buf, 1)) != 0) {
        if (ret == -1) {
            if (errno == EINTR) continue;

            perror("read");
            break;
        }


        printf("%s", buf);
    }

    return 0;
}

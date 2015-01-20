#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int main()
{
	int fd = open("~/main.c", O_RDONLY);
    perror("open");
	printf("%d\n", fd);
	return 0;
}

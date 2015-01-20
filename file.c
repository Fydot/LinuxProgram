#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int fd = open("~/main.c", O_RDONLY);
	printf("%d\n", fd);
	return 0;
}

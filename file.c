#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = open("~/main.c", O_RDONLY);
	printf("%d\n", fd);
	return 0;
}
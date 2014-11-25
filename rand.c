#include <stdio.h>
#include <stdlib.h>
#define N 10
#define M 1000000

int number[N];

int main(int argc, char *args[])
{
	for (int i = 0; i < M; ++i) {
		number[rand() % 10]++;
	}

	for (int i = 0; i < N; i++) {
		printf("%d: %d\n", i, number[i]);
	}
	return 0;
}

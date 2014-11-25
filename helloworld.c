#include <stdio.h>
#define N 10

const int number = N + N;


struct rational {
	int deno, melo;
};


int abs(int a)
{
	return a > 0 ? a : -a;
}

int gcd(int a, int b)
{
	return a % b == 0 ? b : gcd(b, a % b);
}

struct rational make(int a, int b)
{
	struct rational rat = { a, b };
	return rat;
}

int deno(struct rational a)
{
	return a.deno;
}

int melo(struct rational a)
{
	return a.melo;
}


struct rational simple(struct rational a)
{
	struct rational b = { deno(a) / gcd(deno(a), melo(a)),
		melo(a) / gcd(deno(a), melo(a))
	};
	return b;
}

struct rational add(struct rational a, struct rational b)
{
	struct rational res = { deno(a) * melo(b) + deno(b) * melo(a),
		melo(a) * melo(b)
	};
	return simple(res);
}

struct rational sub(struct rational a, struct rational b)
{
	struct rational res = { deno(a) * melo(b) - deno(b) * melo(a),
		melo(a) * melo(b)
	};
	return simple(res);
}

struct rational mul(struct rational a, struct rational b)
{
	struct rational res = { deno(a) * deno(b), melo(a) * melo(b) };
	return simple(res);
}

struct rational div(struct rational a, struct rational b)
{
	struct rational res = { deno(a) * melo(b), melo(a) * deno(b) };
	return simple(res);
}

void print(struct rational a)
{
	int d = deno(a);
	int m = melo(a);

	if (d % m == 0) {
		printf("%d\n", d / m);
		return;
	}

	if (d == 0) {
		printf("0\n");
		return;
	}

	if (d * m < 0) {
		printf("-%d/%d\n", abs(d), abs(m));
		return;
	}
	printf("%d/%d\n", d, m);
}

int main(void)
{
	int a, b, c, d;
	while (~scanf("%d %d %d %d", &a, &b, &c, &d)) {
		struct rational left = make(a, b);
		struct rational right = make(c, d);
		print(add(left, right));
		print(sub(left, right));
		print(mul(left, right));
		print(div(left, right));
	}
	return 0;
}

main: main.o calculate.o output.o
	cc -o main main.o calculate.o output.o

main.o: main.c calculate.h
	cc -c main.c

calculate.o: calculate.c output.h
	cc -c calculate.c

output.o: output.c
	cc -c output.c

clean:
	rm *.o main

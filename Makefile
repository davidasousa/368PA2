test: pa2.c
	gcc -O3 -std=c99 -Wall -Wvla -pedantic *.c -o pa2

valgrind: pa2
	valgrind ./pa2

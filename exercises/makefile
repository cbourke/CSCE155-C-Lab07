# Makefile for array lab

CC = gcc
CFLAGS = -g -Wall -std=gnu99

TARGETS := array_demo array_test

all: $(TARGETS)

array_demo: array_utils.o array_demo.c
	$(CC) $(CFLAGS) array_utils.o array_demo.c -o array_demo

array_test: array_utils.o array_cmocka_test.c
	$(CC) $(CFLAGS) array_utils.o array_cmocka_test.c -o array_test -lcmocka

array_utils.o: array_utils.c array_utils.h
	$(CC) $(CFLAGS) -c array_utils.c -o array_utils.o

clean:
	rm -f *.o

.DEFAULT_GOAL := array_demo

.PHONY: all clean

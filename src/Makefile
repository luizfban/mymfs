CC=g++
CFLAGS=-c -Wall -pedantic

all: mymfs

mymfs: mymfs.cpp mymfsKernel.o zip.c
	$(CC) mymfsKernel.o mymfs.cpp zip.c -o mymfs

mymfsKernel.o: mymfsKernel.cpp
	$(CC) mymfsKernel.cpp $(CFLAGS)

clean:
	rm *.o

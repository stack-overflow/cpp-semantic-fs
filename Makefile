CC=g++

CFLAGS=--std=c++11 -O3 -Wall -Werror -pipe -Wpointer-arith -Winit-self

all:
	$(CC) $(CFLAGS) main.cpp linear_regression.h linear_regression.cpp -o semantic-fs

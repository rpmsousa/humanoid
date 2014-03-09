all: skeleton
#test

CFLAGS=-O2 -Wall
LDFLAGS=-lglut -lm -lGLU
CC=gcc

SRC = skeleton.c model.c model_bulky.c model_sticky.c model_simple.c movement.c linear.c physics.c
HDR = model.h movement.h linear.h physics.h

test: linear.c linear.h
	$(CC) $(CFLAGS) -o test linear.c $(LDFLAGS)


skeleton: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o skeleton $(SRC) $(LDFLAGS)

clean:
	rm skeleton

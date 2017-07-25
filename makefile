all: humanoid test

CFLAGS=-O2 -Wall
LDFLAGS=-lglut -lm -lGLU
CC=gcc

SRC = main.c model.c model_bulky.c model_sticky.c model_simple.c movement.c linear.c physics.c
HDR = model.h movement.h linear.h physics.h

test: linear.c test.c linear.h
	$(CC) $(CFLAGS) -o $@ test.c linear.c $(LDFLAGS)


humanoid: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

clean:
	rm -f humanoid test

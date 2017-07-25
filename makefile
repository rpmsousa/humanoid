all: humanoid humanoid2 test

CFLAGS=-O2 -Wall
#LDFLAGS=-DUSE_GLUT -lglut -lm -lGLU
LDFLAGS=-lX11 -lm -lGL -lGLU
CC=gcc

SRC = main.c model.c model_bulky.c model_sticky.c model_simple.c movement.c linear.c physics.c draw.c
HDR = model.h movement.h linear.h physics.h draw.h

SRC2 = main2.c model2.c linear.c physics2.c draw.c
HDR2 = model2.h linear.h list.h draw.h

test: linear.c test.c linear.h
	$(CC) $(CFLAGS) -o $@ test.c linear.c $(LDFLAGS)


humanoid: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

humanoid2: $(SRC2) $(HDR2)
	$(CC) $(CFLAGS) -o $@ $(SRC2) $(LDFLAGS)

clean:
	rm -f humanoid humanoid2 test *.o

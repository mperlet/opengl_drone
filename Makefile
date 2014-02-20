CC = gcc
CFLAGS = -g -Wall -m64
LIBS = -lGL -lGLU -lglut
TUNE= -O2

all: drone

drone:
	$(CC) $(TUNE) $(CFLAGS) -o bin/drone $(LIBS) src/drone.c


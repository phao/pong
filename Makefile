CC=gcc
DEBUG_OPTS=-g
OPTIMIZATION_OPTS=-O3
CC_DEFAULT_OPTS=-Wall -Wextra -std=c99 -pedantic $(DEBUG_OPTS) \
	$(OPTIMIZATION_OPTS)

# Other options for SDL extension libraries include -lSDL2_ttf -lSDL2_mixer.
LINKER_OPTS=-lSDL2 -lSDL2_image -lm

OBJS=main.o

.c.o:
	$(CC) $(CC_DEFAULT_OPTS) -c $<

all: $(OBJS)
	$(CC) *.o -o main $(LINKER_OPTS)

run: all
	./main

clear:
	rm -f *.o main

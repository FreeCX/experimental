PROG=font
SOURCE=font.c token.c
OS=$(shell uname -s)
CFLAGS=-Wall $(shell sdl2-config --cflags --libs) -lSDL2_image

all:
	$(CC) $(SOURCE) -o $(PROG) $(CFLAGS)
clean:
	$(RM) $(PROG)
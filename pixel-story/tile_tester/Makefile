PROG=tile-tester
SOURCE=main.c ../code/graphic.c
OS=$(shell uname -s)
ifneq ($(OS),Linux)
	CFLAGS+=-lmingw32 -Dmain=SDL_main -lSDL2main
	PROG=tile-tester.exe
endif
CFLAGS+=-Wall -lSDL2 -lSDL2_image

all:
	$(CC) $(SOURCE) -o $(PROG) $(CFLAGS)
clean:
	$(RM) $(PROG)

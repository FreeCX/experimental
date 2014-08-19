PROG=pixel-story
SOURCE=main.c graphic.c
OS=$(shell uname -s)
ifneq ($(OS),Linux)
	CFLAGS+=-lmingw32 -Dmain=SDL_main -lSDL2main
	PROG=pixel-story.exe
endif
CFLAGS+=-Wall -lSDL2 -lSDL2_image

all:
	$(CC) $(SOURCE) -o $(PROG) $(CFLAGS)
	$(CC) scroll.c graphic.c -o scroll $(CFLAGS)
clean:
	$(RM) $(PROG) scroll
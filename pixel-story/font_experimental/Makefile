PROG=font
SOURCE=font.c token.c
OS=$(shell uname -s)
ifneq ($(OS),Linux)
	CFLAGS+=-lmingw32 -Dmain=SDL_main -lSDL2main
	PROG=font.exe
endif
CFLAGS+=-Wall -lSDL2 -lSDL2_image

all:
	$(CC) $(SOURCE) -o $(PROG) $(CFLAGS)
clean:
	$(RM) $(PROG)
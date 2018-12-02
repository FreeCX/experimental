CC=gcc
SOURCE=test.c
OS=$(shell uname -s)

ifeq ($(OS),Linux)
	PROG=test
	CFLAGS=-L . ../libimage.so -lGL -lglut -lGLU
else
	PROG=test.exe
	CFLAGS=-L . libimage.dll -lfreeglut -lopengl32 -lglu32
endif

all:
	$(MAKE) -C ..
	$(CC) $(SOURCE) $(CFLAGS) -o $(PROG) 
	
clean:
	$(RM) $(PROG)
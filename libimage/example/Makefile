PROG=test
OS=$(shell uname -s)
ARCH=$(shell uname -m)
ifeq ($(OS),Linux)
	CFLAGS=-L . ../libimage.so -lGL -lglut
else
	CFLAGS=-L . libimage.dll -lfreeglut -lopengl32 -lglu32
endif

all:
	$(CC) $(PROG).c -o $(PROG) $(CFLAGS)
clean:
	$(RM) $(PROG)
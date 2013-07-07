PROG=test
OS=$(shell uname -s)
ARCH=$(shell uname -m)
ifeq ($(OS),Linux)
	CFLAGS=-L . ../libimage.so
else
	CFLAGS=-L . libimage.dll
endif

all:
	$(CC) $(CFLAGS) $(PROG).c -o $(PROG)
clean:
	$(RM) $(PROG)
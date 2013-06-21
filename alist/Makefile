CC=clang
PROG=alist

all:
	$(CC) logic.c main.c -o $(PROG)
alist:
	$(CC) alist.c -o alist `pkg-config --cflags --libs gtk+-2.0`
clean:
	$(RM) $(PROG)
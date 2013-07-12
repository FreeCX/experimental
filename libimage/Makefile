PROG=libimage
DIR_P=src
SOURCES=$(DIR_P)/data.c $(DIR_P)/pcx.c $(DIR_P)/tga.c $(DIR_P)/bmp.c
CFLAGS=-O3 -W -Wall
LDFLAGS=
OS=$(shell uname -s)
ARCH=$(shell uname -m)

ifeq ($(OS),Linux)
	LDFLAGS+=-fPIC
	LIB=$(PROG).so
	CFLAGS+=--def export.def -s -fPIC -shared -g
	T_CFLAGS=-L . ../libimage.so -lGL -lglut
else
	LIB=$(PROG).dll
	CFLAGS+=-shared -Wl,-add-stdcall-alias export.def
	T_CFLAGS=-L . libimage.dll -lfreeglut -lopengl32 -lglu32
endif

OBJS := $(patsubst %.c, %.o, $(SOURCES))
DEPS := $(patsubst %.o, %.d, $(OBJS))

$(LIB): $(OBJS) $(DEPS)
	$(CC) $(OBJS) -o $@ $(CFLAGS)
	strip -s $(LIB)
	chmod -x $(LIB)
 
%.o: %.c %.d
	$(CC) $(LDFLAGS) -c $< -o $@
 
%.d: %.c
	@set -e; $(CC) -M $< | \
		sed -e 's%\($*\)\.o[ :]*%\1.o $@ : %g' > $@; \
		[ -s $@ ] || rm -f $@
	@echo create $@

test:
	$(CC) test.c -o test $(T_CFLAGS)
	
clean:
	$(RM) $(LIB) $(OBJS) $(DEPS)
	$(RM) *~
	$(RM) test.exe test
 
-include $(DEPS)

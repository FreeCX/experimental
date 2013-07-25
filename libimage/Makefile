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
else
	LIB=$(PROG).dll
	CFLAGS+=-shared -Wl,-add-stdcall-alias export.def
endif

OBJS := $(patsubst %.c, %.o, $(SOURCES))
DEPS := $(patsubst %.o, %.d, $(OBJS))

$(LIB): $(OBJS) $(DEPS)
	$(CC) $(OBJS) -o $@ $(CFLAGS)
	strip -s $(LIB)
	chmod -x $(LIB)
	cp $(LIB) test/
 
%.o: %.c %.d
	$(CC) $(LDFLAGS) -c $< -o $@
 
%.d: %.c
	@set -e; $(CC) -M $< | \
		sed -e 's%\($*\)\.o[ :]*%\1.o $@ : %g' > $@; \
		[ -s $@ ] || rm -f $@
	@echo create $@
	
clean:
	$(RM) $(LIB) $(OBJS) $(DEPS) *~ test/$(LIB)
 
-include $(DEPS)

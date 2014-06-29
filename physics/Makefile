PROG=physics
SOURCES=vector.cpp simulator.cpp physics.cpp
CFLAGS=-O3 -W -Wall
LDFLAGS=
OS=$(shell uname -s)
ARCH=$(shell uname -m)

ifeq ($(OS),Linux)
	CFLAGS+=-lGL -lglut
else
	CFLAGS+=-lfreeglut -lopengl32
endif

OBJS := $(patsubst %.cpp, %.o, $(SOURCES))
DEPS := $(patsubst %.o, %.d, $(OBJS))

$(PROG): $(OBJS) $(DEPS)
	$(CXX) $(OBJS) -o $@ $(CFLAGS)
 
%.o: %.cpp %.d
	$(CXX) $(LDFLAGS) -c $< -o $@
 
%.d: %.cpp
	@set -e; $(CC) -M $< | \
		sed -e 's%\($*\)\.o[ :]*%\1.o $@ : %g' > $@; \
		[ -s $@ ] || rm -f $@
	@echo create $@
	
clean:
	$(RM) $(PROG) $(OBJS) $(DEPS) *~
 
-include $(DEPS)

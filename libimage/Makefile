CC 			 := gcc
CFLAGS 		 := -W -Wall -O3
LFLAGS 		 := -lm
OS 			 := $(shell uname -s)
ARCH 		 := $(shell uname -m)
target_file  := loader
prog_name    := libimage
dest_dir     := ./src/
object_files := $(patsubst %.c, %.o, $(wildcard $(dest_dir)*.c))

ifeq ($(OS),Linux)
	lib_name := $(prog_name).so
	LFLAGS += --def export.def -s -fPIC -shared
else
	lib_name := $(prog_name).dll
	LFLAGS += -shared -Wl,-add-stdcall-alias export.def
endif

all: $(dest_dir)$(target_file)

$(dest_dir)$(target_file): $(object_files)
	$(CC) $(CLFAGS) $(object_files) -o $(lib_name) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-$(RM) $(lib_name) $(object_files)

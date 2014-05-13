CFLAGS := -fPIC -std=c99 -pedantic -O3 -march=native -g \
	-Wall -Werror -Wextra -Winline  -Wno-unused-variable -Wno-unused-parameter
CC := clang
NAME := entee

SRCDIR = src
EXAMPLE_SOURCES = $(SRCDIR)/example.c
EXAMPLE_OBJECTS = $(EXAMPLE_SOURCES:%.c=%.o)

LIB_PARSERS := $(wildcard $(SRCDIR)/*.c.rl)
LIB_SOURCES := $(filter-out $(EXAMPLE_SOURCES), $(wildcard $(SRCDIR)/*.c) $(LIB_PARSERS:%.c.rl=%.c))
LIB_OBJECTS := $(LIB_SOURCES:%.c=%.o)

all: lib

lib: lib$(NAME).so

lib$(NAME).so: $(LIB_OBJECTS)
	$(CC) -shared -Wl,-soname,lib$(NAME).so $^ -o $@

%.c: %.c.rl
	ragel -C -G2 -o $@ $<

example: $(EXAMPLE_OBJECTS)
	$(CC) $^ -L. -l$(NAME) -lz -lbz2 -Wl,-rpath,. -o $@

.PHONEY: clean
clean:
	@$(RM) $(EXAMPLE_OBJECTS) example $(LIB_OBJECTS) lib$(NAME).so

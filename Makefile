BUILDDIR := build
INCDIR := inc

CC := gcc
CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -I$(INCDIR) -lm

HDRS := $(wildcard inc/*.h)
SRCS := $(wildcard *.c)
OBJS := $(SRCS:%.c=$(BUILDDIR)/%.o)
EXEC := launch

all: $(EXEC)

$(BUILDDIR):
	mkdir -p $@

$(BUILDDIR)/%.o: %.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS) | 
	$(CC) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -rf $(BUILDDIR) $(EXEC)

.PHONY: all clean

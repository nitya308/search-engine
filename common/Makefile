# Makefile for common directory
# Builds and compiles the pagedir module
#
# Nitya Agarwala, 2022

L = ../libcs50

# object files, and the target library
OBJS = pagedir.o
LIB = common.a

FLAGS =

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L
CC = gcc
MAKE = make

.PHONY: all clean

# Build $(LIB) by archiving object files
all: $(LIB)

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
# index.o: index.h $L/file.h $L/counters.h $L/hashtable.h
pagedir.o: pagedir.h $L/webpage.h $L/file.h
# word.o: word.h

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
# Makefile for 'crawler' module
#
# Nitya Agarwala, Jan 2022

C = ../common
L = ../libcs50
LIBS =
LLIBS = $C/common.a $L/libcs50.a

# add -DAPPEST for functional tracking report
# add -DMEMTEST for memory tracking report
# (and run `make clean; make` whenever you change this)
FLAGS = # -DAPPTEST # -DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$L -I$C
CC = gcc
MAKE = make

.PHONY: all test valgrind clean

########### default ##################

all: crawler

########### crawler ##################

crawler: crawler.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# crawler source dependencies
crawler.o: $L/webpage.h $C/pagedir.h
crawler.o: $L/mem.h $L/bag.h $L/hashtable.h

########### test ##################
test: crawler testing.sh
	bash -v testing.sh

########### valgrind ##################
valgrind: crawler valgrind.sh
	bash -v valgrind.sh

clean:
	rm -f core
	rm -rf *~ *.o *.dSYM
	rm -f crawler
	rm -rf ../testDir
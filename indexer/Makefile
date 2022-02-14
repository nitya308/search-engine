# Makefile for 'indexer' module
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

all: indexer indextest

########### indexer ##################

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# indexer source dependencies
indexer.o: $L/webpage.h $C/pagedir.h $C/index.h $C/word.h
indexer.o: $L/mem.h $L/hashtable.h $L/file.h

########### indextest ##################
indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@
# indextest source dependencies
indextest.o: $C/index.h
indextest.o: $L/hashtable.h

########### test ##################
test: indexer testing.sh
	bash -v testing.sh

########### valgrind ##################
valgrind: indexer valgrind.sh
	bash -v valgrind.sh

clean:
	rm -f core
	rm -rf *~ *.o *.dSYM
	rm -f indexer
	rm -f indextest
	rm -rf testing.out
	
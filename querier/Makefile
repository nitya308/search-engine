# Makefile for 'querier' module
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

all: querier fuzzquery

########### querier ##################

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

########### fuzzquery ##################

fuzzquery: fuzzquery.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# querier source dependencies
querier.o: $L/webpage.h $C/pagedir.h $C/index.h $C/word.h
querier.o: $L/mem.h $L/hashtable.h $L/counters.h $L/file.h
fuzzquery.o: $L/mem.h $L/file.h

########### test ##################
test: querier testing.sh
	bash -v testing.sh

########### valgrind ##################
valgrind: querier valgrind.sh
	bash -v valgrind.sh

clean:
	rm -f core
	rm -rf *~ *.o *.dSYM
	rm -f querier
	rm -rf testing.out
	
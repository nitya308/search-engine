# CS50 TSE
## Nitya Agarwala (nitya308)

```
****************
* EXTENSION USED
****************
```

This is the **Tiny Search Engine** project for CS 50: Software Design and Implementation

It contains (will contain) 3 main parts: _crawler_, _indexer_ and _querier_. For assumptions and implementation of each, see individual directories.

Files contained in this repository so far:
```
|-- .gitignore
|-- Makefile
|-- README.md
|-- common
|   |-- Makefile
|   |-- index.c
|   |-- index.h
|   |-- pagedir.c
|   |-- pagedir.h
|   |-- word.c
|   `-- word.h
|-- crawler
|   |-- .gitignore
|   |-- DESIGN.md
|   |-- IMPLEMENTATION.md
|   |-- Makefile
|   |-- README.md
|   |-- REQUIREMENTS.md
|   |-- crawler.c
|   |-- testing.out
|   |-- testing.sh
|   `-- valgrind.sh
|-- indexer
|   |-- .gitignore
|   |-- IMPLEMENTATION.md
|   |-- Makefile
|   |-- README.md
|   |-- indexer.c
|   |-- indextest.c
|   |-- testing.out
|   |-- testing.sh
|   `-- valgrind.sh
|-- querier
|   |-- .gitignore
|   |-- IMPLEMENTATION.md
|   |-- DESIGN.md
|   |-- Makefile
|   |-- README.md
|   |-- querier.c
|   |-- fuzzquery.c
|   |-- testing.out
|   |-- testing.sh
|   |-- badqueries.txt
|   |-- letterstest.txt
|   |-- scrapetest.txt
|   |-- wikitest.txt
```
To compile, simply `make` in the top-level directory or current directory.

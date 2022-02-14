# Tiny Search Engine CS50
## CS50 Winter 2022

### Indexer

The `indexer` reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the `indextest`, loads an index file produced by the indexer and saves it to another file.

### Usage

./indexer pageDirectory indexFileName

where:
- pageDirectory is the pathname of a directory produced by the Crawler, and
- indexFilename is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).

### Implementation

The indexer has 4 functions:
```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* currPage, int docID);
```
The `main` function simply calls `parseArgs` and `indexBuild` to make the index. Then it calls `index_save` to write the index to a file and `index_delete`, then exits zero.


The `parseArgs` function takes arguments from the command line, extracts them into the function parameters. It also: 
- validates whether `pageDirectory` is an existing, crawler produced directory
- creates `indexFileName` it if it doesn't exist and verifies it is a writeable file
If any errors are found, it prints an error to stderr and exits non-zero.

The `indexBuild` function does the real work of building the index from the crawler produced output in `pageDirectory`.

This `indexPage` function, given a `webpage`, scans the given page to extract any words, ignoring words with length under 3. It add the words and their count along to the `index`.

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `indexer.c` - the program to create the index
* `indextest.c` - program to test the index by loading index and putting it into new file
* `testing.sh` - script for testing indexer.c
* `valgrind.sh` - script for testing indexer.c for leaks using valgrind
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make` in the top-level directory or current directory.

### Testing

The `testing.sh` script tests indexer.c using the crawler output in ~/cs50-dev/shared/tse/output
It tests error and edge cases extensively.
- All erroneous arguments are tested.
- It runs indexer over crawler output from letters at depths 2, 3, 10, toscrape at depths 1, 2, wikipedia at depths 1, 2, 3
- It then runs indextest over all of them, and uses indexcmp to compare old and new indexes
- It runs indexcmp over index files produced by this idnexer and sample index files in the cs50-dev/shared/tse/output directory to verify correct behaviour.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

To test with valgrind, `make valgrind`.

# Tiny Search Engine CS50
## CS50 Winter 2022

### Querier

The `Querier` is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

### Usage

./querier pageDirectory indexFilename

where:
- pageDirectory is the pathname of a directory produced by the Crawler, and
- indexFilename is the pathname of a file produced by the Indexer.

### Functionality

The querier meets the full specs outlined in REQUIREMENTS.md.
It:
* Prints the set of documents that contain all the words in the query
* It also supports 'and' and 'or' operators.
* It also supports 'and' precedence over 'or'.
* And it prints the document set in decreasing order by score, thus meeting the full specs.

### Implementation

The querier has 7 main functions with associated helpers:
```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName);
static int parseQuery(char* line, char** words);
static bool validateQuery(char** words, int wordCount);
static counters_t* matchQuery(char** words, const int wordCount, index_t* index);
static void findBestScore(void* arg, const int key, const int count);
static void printMatch(void* arg, const int key, const int count);
```
The `main` function calls `parseArgs` loads the index from the file by calling index_load, reads queries from stdin one line at a time and calls all other functions to handle the query, then exits zero.


The `parseArgs` function takes arguments from the command line, extracts them into the function parameters. It also: 
- validates whether `pageDirectory` is an existing, crawler produced directory
- makes sure `indexFileName` is a readable file
If any errors are found, it prints an error to stderr and exits non-zero.

The `parseQuery` function normalizes and separates the query into words

`validateQuery` makes sure it has no bad characters or errors like consecutive "and", "or".

The `matchQuery` function, does the real work of handling the query and finding documents that match.

### Assumptions

These are the assumptions clear from the spec:
* pageDirectory has files named 1, 2, 3, ..., without gaps.
* The content of files in pageDirectory follow the format as defined in the specs; thus your code (to read the files) need not have extensive error checking.
* The content of the file named by indexFilename follows our index file format.
* The provided index file corresponds to the provided pageDirectory, that is, was built by indexer from the files in that directory.
In addition:
* Assuming that the max length of a query is 150 characters as the function `fgets` requires a maximum parameter

### Files

* `Makefile` - compilation procedure
* `querier.c` - the program that runs the querier 
* `fuzzquery.c` - program provided to test query by producing random queries based on a seed
* `testing.sh` - script for testing querier.c
* `testing.out` - result of `make test &> testing.out`
* `letterstest.txt`, `scrapetest.txt` and `wikitest.txt`- files with queries for regression testing
* `badqueries.txt`- file with bad queries to test edge cases

### Compilation

To compile, simply `make` in the top-level directory or current directory.

### Testing

The `testing.sh` script tests querier.c using the crawler output and index files in ~/cs50-dev/shared/tse/output
It tests error and edge cases extensively.
- All erroneous arguments are tested.
- It tests bad queries from the file badqueries.txt. These produce appropriate error messages which are compared manually against the sample output from REQUIREMENTS.md. 
- Regression testing: we run querier over crawler and indexer output from letters at depths 2, 3 toscrape at depths 1, 2 and wikipedia at depths 2, 3. Queries are read from individual files for each directory.
- It then runs valgrind over all 3 directories to make sure there are no memory leaks
- Output is manually checked. We start small with single word queries that only have a few matches which can easily be checked. Then we incorporate "and" and "or" queries.
- Fuzz testing: The program fuzzquery is used to generate 1000 random queries and prints success or fail depending on the result.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.
`testing.sh` incorporates testing with valgrind.

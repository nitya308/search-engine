# Tiny Search Engine CS50
## CS50 Winter 2022

### Crawler

The `crawler` crawls a website and retrieves webpages starting with a specified _URL_. 
It **parses** the initial webpage, **extracts** any embedded _URLs_ and **retrieves** those pages, and crawls the pages found at those URLs, to the _depth_ provided.
It does not visit any given _URL_ more than once. 
It **saves** the pages, and the _URL_ and depth for each, in files. 
When the `crawler` process is complete, the indexing of the collected documents can begin.

### Usage

./crawler seedURL pageDirectory maxDepth

where:
- seedURL is the starting URL and must be a valid internal URL
- pageDirectory is the directory where crawler writes files and must be an existing directory
- maxDepth is the maximum depth from starting URL to which the crawler crawls. It must be a valid integer in range 0 to 10

### Implementation

The crawler has 4 functions:
```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
```
The `main` function simply calls `parseArgs` and `crawl`, then exits zero.

The `parseArgs` function takes arguments from the command line, extracts them into the function parameters. It also: 
- normalizes the seedURL and validates it is an internal URL
- call `pagedir_init()` to intialize pageDirectory
- checks maxDepth is an integer between 0 and 10
If any errors are found, it prints an error to stderr and exits non-zero.

The `crawl` function crawls from `seedURL` to `maxDepth` and saves a file for each webpage in `pageDirectory` by using the [pagedir](../common/README.md) module.
A `hashtable` from the [libcs50](../libscs50) module is used to keep track of seen webpages and a `bag` from the [libcs50](../libscs50) module is used to store pages to be crawled.

This `pageScan` function scans a given webpage to extract any links URLs and adds internal URLs that have not been seen before to the hashtable `pagesSeen` and to the bag `pagesToCrawl`.

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `crawler.c` - the program to crawl webpages
* `testing.sh` - script for testing crawler.c
* `valgrind.sh` - script for testing crawler.c for leaks using valgrind
* `testing.out` - result of `make test &> testing.out`

### Compilation

To compile, simply `make` in the top-level directory or current directory.

### Testing

The `testing.sh` script tests crawler.c using the CS50 webpages playground.
It tests error and edge cases extensively.
- All erroneous arguments are tested. 
- It runs valgrind over toscrape at smaller depths.
- It then runs over all three CS50 websites (letters at depths 0,1,2,10, toscrape at depths 0,1,2,3, wikipedia at depths 0,1,2)
- Correct behavior is verified by studying the output, and by sampling the files created in the respective pageDirectories.

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

To test with valgrind, `make valgrind`.

### Exit codes
0 - everything successful
1 - invalid arguments to command line
2 - page not fetched
3 - error creating file pathnames or opening files
4 - error writing to files
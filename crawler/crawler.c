/*
 * crawler.c   Nitya Agarwala     Jan 31, 2022
 *
 * This file contains the crawler for the tse module.
 * It crawls the web starting from a seedURL and retrives webpages to a certain depth.
 * It parses the current webpage and extracts embedded URLs
 * It then retrieves each of those pages until depth is received
 *
 * It includes functions:
 * main which runs the program
 * parseArgs which initializes arguments
 * crawl which crawls from seedURL to maxDepth and saves pages in pageDirectory
 * pageScan which scans the given webpage to extract any links
 *
 * usage: ./crawler seedURL pageDirectory maxDepth
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/pagedir.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"

// function prototypes
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/**************** main ****************/
/* Main fucntion for crawler.c
 *
 * Arguments: seedURL pageDirectory maxDepth
 *
 * Arguments are checked and validated by parseArgs which is called by main
 * Crawling is done by crawler which is also called by main
 *
 * We exit:
 *  with 0 if no errors have occured
 */
int main(const int argc, char* argv[])
{
  char* seedURL = NULL;
  char* pageDirectory = NULL;
  int maxDepth = 0;
  parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
  crawl(seedURL, pageDirectory, maxDepth);
  exit(0);
}

/**************** parseArgs ****************/
/* To check and validate arguments
 *
 * Caller provides
 *   arguments to main: argc and argv[]
 *   pointers to seedURL, pageDirectory and maxDepth
 *
 * We print an error and exit non-0 if any of the following conditions are not met
 *    seedURL = argv[1] must be a valid internal URL
 *    pageDirectory = argv[2] must be a directory that can be successfully initialized
 *    maxDepth = argv[3] must be a valid integer in range 0 to 10
 */
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth)
{
  const char* progName = argv[0];  // name of this program

  if (argc != 4) {
    fprintf(stderr, "error: wrong number of arguments\nusage: %s seedURL pageDirectory maxDepth\n", progName);
    exit(1);
  }

  *seedURL = argv[1];
  char* normalizedURL = normalizeURL(*seedURL);
  if (normalizedURL == NULL) {
    fprintf(stderr, "error: invalid URL\n");
    exit(1);
  }
  if (!isInternalURL(normalizedURL)) {
    free(normalizedURL);
    fprintf(stderr, "error: URL is not internal URL\n");
    exit(1);
  }
  free(normalizedURL);

  *pageDirectory = argv[2];
  if (!pagedir_init(*pageDirectory)) {
    fprintf(stderr, "error: page directory does not exist or not writeable\n");
    exit(1);
  }
  const char* maxDepthString = argv[3];
  // try to convert nLines to an int
  *maxDepth = 0;  // initialize calling function's value
  char excess;    // any excess chars after the number
  // if the argument is valid, sscanf should extract exactly one thing.
  if (sscanf(maxDepthString, "%d%c", maxDepth, &excess) != 1) {
    fprintf(stderr, "%s: '%s' invalid integer\n", progName, maxDepthString);
    exit(1);
  }
  if (*maxDepth < 0) {
    fprintf(stderr, "error: maxDepth is too small, it must be >= 0\n");
    exit(1);
  }
  if (*maxDepth > 10) {
    fprintf(stderr, "error: maxDepth is too large, it must be <= 10\n");
    exit(1);
  }
}

/**************** crawl ****************/
/* Crawls from seedURL to maxDepth and saves pages in pageDirectory
 *
 * Caller provides
 *   valid internal seedURL
 *   initialized pageDirectory for writing
 *   valid integer maxDepth between 0 to 10
 *
 * IMPORTANT:
 *   we call free() on all memory initialized in the function
 */
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth)
{
  // Create bag of webpages to crawl
  bag_t* toCrawlBag = mem_assert(bag_new(), "bag_new");

  // Make a copy of the URL string
  char* URLcopy = mem_malloc(strlen(seedURL) + 1);
  strcpy(URLcopy, seedURL);

  // Add seedURL to the bag of webpages to crawl, marked with depth=0
  bag_insert(toCrawlBag, webpage_new(URLcopy, 0, NULL));

  // Create a hashtable of seen URLs
  const int numSlots = 200;
  hashtable_t* seenht = mem_assert(hashtable_new(numSlots), "hastable_new");

  // Add seedURL to the hashtable of URLs seen so far
  if (!hashtable_insert(seenht, seedURL, "")) {
    fprintf(stderr, "error inserting in hashtable");
    exit(1);
  }

  webpage_t* currWebPage = NULL;

  // while there are more webpages in the bag
  // extract a webpage (URL,depth) item from the bag
  int uniqueID = 0;
  while ((currWebPage = (webpage_t*)bag_extract(toCrawlBag)) != NULL) {
    // use pagefetcher to retrieve a webpage for that URL
    if (webpage_fetch(currWebPage)) {
      printf("%d  Fetched: %s\n", webpage_getDepth(currWebPage), webpage_getURL(currWebPage));
      // use pagesaver to write the webpage to the pageDirectory with a unique document ID
      pagedir_save(currWebPage, pageDirectory, ++uniqueID);
      // if the webpage is not at maxDepth
      if (webpage_getDepth(currWebPage) < maxDepth) {
        // pageScan that HTML
        pageScan(currWebPage, toCrawlBag, seenht);
      }
    }
    webpage_delete(currWebPage);  // delete that webpage
  }
  hashtable_delete(seenht, NULL);          // delete the hashtable
  bag_delete(toCrawlBag, webpage_delete);  // delete the bag
}

/**************** pageScan ****************/
/* Scan the given webpage to extract any links
 * Adds any URL not already seen before to the hashtable pagesSeen and bag pagesToCrawl
 *
 * Caller provides
 *   pointer to a webpage_t provided by webpage_new.
 *   pointer to bag of pages to crawl
 *   pointer to hashtable to pages already seen
 *
 * IMPORTANT:
 *   we make a copy of the URL and free() it at the end of the function
 */
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
  const int depth = webpage_getDepth(page);
  printf("%d Scanning: %s\n", depth, webpage_getURL(page));
  char* currURL = NULL;
  // while there is another URL in the page
  int pos = 0;
  while ((currURL = webpage_getNextURL(page, &pos)) != NULL) {
    printf("%d    Found: %s\n", depth, currURL);
    char* normalizedURL = normalizeURL(currURL);
    if (normalizedURL != NULL) {
      if (isInternalURL(normalizedURL)) {
        // if that URL is Internal
        // insert the webpage into the hashtable
        if (hashtable_insert(pagesSeen, normalizedURL, "")) {
          // create a webpage_t for it
          webpage_t* newWebPage = webpage_new(normalizedURL, webpage_getDepth(page) + 1, NULL);
          // insert the webpage into the bag
          bag_insert(pagesToCrawl, newWebPage);
          printf("%d    Added: %s\n", depth, normalizedURL);
        }
        else {
          printf("%d  IgnDupl: %s\n", depth, normalizedURL);
          mem_free(normalizedURL);
        }
      }
      else {
        printf("%d IgnExtrn: %s\n", depth, normalizedURL);
        mem_free(normalizedURL);
      }
    }
    mem_free(currURL);
  }
}
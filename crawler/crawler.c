/*
 * crawler.c   Nitya Agarwala     Jan 31, 2022
 *
 * This file contains the crawler for the tse module. 
 * It crawls the web starting from a seedURL and retrives webpages to a certain depth. 
 * It parses the current webpage and extracts embedded URLs
 * then retrieves each of those pages until depth is received
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../libcs50/bag.h"
#include "../libcs50/mem.h"
#include "../common/pagedir.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"

// prototype declarations
int main(const int argc, char *argv[]);
static void parseArgs(const int argc, char *argv[],
                      char **seedURL, char **pageDirectory, int *maxDepth);
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth);
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen);

int main(const int argc, char *argv[])
{
  char *seedURL = NULL;
  char *pageDirectory = NULL;
  int maxDepth = 0;
  parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
  crawl(seedURL, pageDirectory, maxDepth);
  exit(0);
}

static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth)
{
  const char *progName = argv[0]; // name of this program

  if (argc != 4)
  {
    fprintf(stderr, "X usage: %s seedURL pageDirectory maxDepth\n", progName);
    exit(1);
  }

  // TO  ASK: MALLOC???
  *seedURL = argv[1];
  char *normalizedURL = normalizeURL(*seedURL); // TO ASK: where to free this????
  if (normalizedURL == NULL)
  {
    fprintf(stderr, "error: invalid URL\n");
    exit(2);
  }
  if (!isInternalURL(normalizedURL))
  {
    mem_free(normalizedURL); //??
    fprintf(stderr, "error: URL is not internal URL\n");
    exit(3);
  }

  // TO ASK: malloc
  *pageDirectory = argv[2];
  if (!pagedir_init(*pageDirectory))
  {
    fprintf(stderr, "error: could not open crawler file for writing\n");
    exit(4);
  }
  const char *maxDepthString = argv[3];
  // try to convert nLines to an int
  *maxDepth = 0; // initialize calling function's value
  char excess;   // any excess chars after the number
  // if the argument is valid, sscanf should extract exactly one thing.
  if (sscanf(maxDepthString, "%d%c", maxDepth, &excess) != 1)
  {
    fprintf(stderr, "%s: '%s' invalid integer\n", progName, maxDepthString);
    exit(4);
  }
  if (*maxDepth < 0)
  {
    fprintf(stderr, "error: maxDepth is too small, it must be >= 0\n");
    exit(5);
  }
  if (*maxDepth > 10)
  {
    fprintf(stderr, "error: maxDepth is too large, it must be <= 10\n");
    exit(6);
  }
}

static void crawl(char *seedURL, char *pageDirectory, const int maxDepth)
{
  hashtable_t *seenht = hashtable_new(200);
  if (seenht == NULL)
  {
    fprintf(stderr, "error creating hashtable");
    exit(1);
  }
  bag_t *toCrawlBag = bag_new();
  if (toCrawlBag == NULL)
  {
    fprintf(stderr, "error creating bag");
    exit(1);
  }

  char* URLcopy = mem_malloc(strlen(seedURL)+1);
  strcpy(URLcopy, seedURL);

  // add seedURL to the hashtable of URLs seen so far
  if (!hashtable_insert(seenht, seedURL, ""))
  {
    fprintf(stderr, "error inserting in hashtable");
    exit(1);
  }

  // add seedURL to the bag of webpages to crawl, marked with depth=0
  bag_insert(toCrawlBag, webpage_new(URLcopy, 0, NULL));

  webpage_t *currWebPage = NULL;
  // while there are more webpages in the bag
  // extract a webpage (URL,depth) item from the bag
  int uniqueID = 1;
  while ((currWebPage = (webpage_t *)bag_extract(toCrawlBag)) != NULL)
  {
    sleep(1); //pause for one second
    //use pagefetcher to retrieve a webpage for that URL
    if (webpage_fetch(currWebPage))
    {
      // use pagesaver to write the webpage to the pageDirectory with a unique document ID
      pagedir_save(currWebPage, pageDirectory, ++uniqueID); //UNIQUE ID?
      // if the webpage is not at maxDepth
      if (webpage_getDepth(currWebPage) < maxDepth) //LESS THAN???
      {
        // pageScan that HTML
        pageScan(currWebPage, toCrawlBag, seenht);
      }
    }
    else
    {
      fprintf(stderr, "error: page not fetched properly\n");
      exit(1);
    }
    webpage_delete(currWebPage); //delete that webpage
  }
  hashtable_delete(seenht, NULL); // delete the hashtable NULL?? (TO ASK)
  bag_delete(toCrawlBag, NULL);   // delete the bag
}

static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen)
{
  char *currURL = NULL;
  // while there is another URL in the page
  int pos = 0;
  while ((currURL = webpage_getNextURL(page, &pos)) != NULL)
  {
    currURL = normalizeURL(currURL);
    if (isInternalURL(currURL)) // if that URL is Internal
    {
      //insert the webpage into the hashtable
      if (hashtable_insert(pagesSeen, currURL, ""))
      {
        //create a webpage_t for it
        webpage_t *newWebPage = webpage_new(currURL, webpage_getDepth(page) + 1, NULL);
        // insert the webpage into the bag
        bag_insert(pagesToCrawl, newWebPage);
      }
    }
    mem_free(currURL);
  }
}
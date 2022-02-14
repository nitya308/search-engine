#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/pagedir.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "index.h"

// function prototypes
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName);
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
  char* pageDirectory = NULL;
  char* indexFileName = NULL;
  parseArgs(argc, argv, &pageDirectory, &indexFileName);

  // load the index from indexFilename into an internal data structure.
  FILE* fp = fopen(indexFileName, "r");
  index_t* index = mem_assert(index_load(fp), "loading index failed");

  // read search queries from stdin, one per line, until EOF.
  char* line = NULL;
  while (fscanf(stdin,"%s", line) != 0) {

  }

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
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName)
{
  const char* progName = argv[0];  // name of this program

  if (argc != 3) {
    fprintf(stderr, "error: wrong number of arguments\nusage: %s seedURL pageDirectory maxDepth\n", progName);
    exit(1);
  }

  *pageDirectory = argv[1];
  if (!pagedir_validate(pageDirectory)) {
    fprintf(stderr, "error: page directory is invalid or not a crawler produced directory\n");
    exit(1);
  }

  *indexFileName = argv[2];
  FILE* fp = fopen(indexFileName, "r");
  if (fp == NULL) {
    fprintf(stderr, "error: index file not readable\n");
    exit(1);
  }
}

char* parseQuery (char* line) {

}
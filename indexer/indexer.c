/*
 * indexer.c   Nitya Agarwala     Feb 4, 2022
 *
 * This file contains the indexer for the tse module.
 * It reads the document files produced by the TSE crawler and builds an index
 * It writes that index to a file.
 *
 * It includes functions:
 * main which parses arguments and initializes other modules
 * indexBuild which builds an index from webpage files in the pageDirectory
 * indexPage which scans a webpage document to add its words to the index.
 *
 * usage: ./indexer pageDirectory indexFilename
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "index.h"
#include "pagedir.h"
#include "word.h"
#include "counters.h"
#include "file.h"
#include "hashtable.h"
#include "mem.h"
#include "webpage.h"

// function prototypes
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* currPage, int docID);

/**************** main ****************/
/* Main fucntion for indexer.c
 *
 * Arguments: pageDirectory indexFileName
 *
 * Arguments are checked and validated by parseArgs which is called by main
 * main also calss index vuild to build the index and deletes the index 
 * 
 * We exit:
 *  with 0 if no errors have occured
 */
int main(const int argc, char* argv[])
{
  char* pageDirectory = NULL;  // will store pathname of a directory produced by the Crawler
  char* indexFileName = NULL;  // pathname of a file into which the index should be written
  parseArgs(argc, argv, &pageDirectory, &indexFileName);
  index_t* index = indexBuild(pageDirectory);
  FILE* fp = fopen(indexFileName, "w");
  index_save(index, fp);
  index_delete(index);
  exit(0);
}

/**************** parseArgs ****************/
/* To check and validate arguments
 *
 * Caller provides
 *   arguments to main: argc and argv[]
 *   pointers to pageDirectory and indexFileName
 *
 * We print an error and exit non-0 if any of the following conditions are not met
 *    pageDirectory = argv[1] must be a crawler produced directory
 *    indexFileName = argv[2] must be a writeable file that we create if it doesn't exist
 */
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName)
{
  const char* progName = argv[0];  // name of this program
  if (argc != 3) {
    fprintf(stderr, "error: wrong number of arguments\nusage: %s pageDirectory indexFilename\n", progName);
    exit(1);
  }

  *pageDirectory = argv[1];
  if (!pagedir_validate(*pageDirectory)) {
    fprintf(stderr, "error: pageDirectory does not exist or is not a crawler produced directory");
    exit(1);
  }

  *indexFileName = argv[2];
  FILE* fp = fopen(*indexFileName, "w");
  if (fp != NULL) {
    fclose(fp);
  }
  else {
    fprintf(stderr, "Index file %s is invalid or is not a writeable file\n", *indexFileName);
    exit(1);
  }
}

/**************** indexBuild ****************/
/* Build the index from the given crawler produced directory
 *
 * Caller provides
 *   valid crawler produced directory
 * 
 * We guarantee:
 *   A new index_t with the words, docIDs and count of words in each file
 *   We do this by calling indexPage on each file in the directory
 *
 * IMPORTANT:
 *   Caller must later free the index created.
 */
static index_t* indexBuild(char* pageDirectory)
{
  const int numSlots = 900;
  index_t* index = mem_assert(index_new(numSlots), "index_new failed");
  int id = 0;
  webpage_t* currPage = NULL;
  while ((currPage = pagedir_load(pageDirectory, ++id)) != NULL) { //IS THIS OK?
    indexPage(index, currPage, id);
    webpage_delete(currPage);
  }
  return index;
}

/**************** indexPage ****************/
/* Read the given webpage to add word, docID and count to index
 *
 * Caller provides
 *   valid pointer to a index_t which is not null
 *   valid webpage to read from
 *   valid integer docID >=1
 * 
 * We guarantee:
 *   we add words with their frequency to the index.
 *   print to stderr and exit using mem_assert if null errors occur
 *
 * IMPORTANT:
 *   we malloc memory to store words in the webpage and free() it at the end of the function
 */
static void indexPage(index_t* index, webpage_t* currPage, int docID)
{
  int pos = 0;
  char* word;
  while ((word = webpage_getNextWord(currPage, &pos)) != NULL) {
    if (strlen(word) >= 3) {
      char* normalWord = normalize_word(word);
      counters_t* counter = index_find(index, word);
      if (counter == NULL) {
        counter = mem_assert(counters_new(), "counters_new failed");
        if (!index_insert(index, normalWord, counter)) {
          fprintf(stderr, "insert failed");
        }
      }
      counters_add(counter, docID);
    }
    free(word);
  }
}
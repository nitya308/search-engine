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

#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"

// function prototypes
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* currPage, int docID);

int main(const int argc, char* argv[])
{
  char* pageDirectory = NULL;  // will store pathname of a directory produced by the Crawler
  // FILE pointer ask??
  char* indexFileName = NULL;  // pathname of a file into which the index should be written
  parseArgs(argc, argv, &pageDirectory, &indexFileName);
  index_t* index = indexBuild(pageDirectory);  // ASK: use & here???
  FILE* fp = fopen(indexFileName, "w");
  index_save(index, fp);
  index_delete(index);
  exit(0);
}

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
  // checking if file can be opened to ASK
  if (fp != NULL) {
    fclose(fp);
  }
  else {
    fprintf(stderr, "Index file %s is invalid or is not a writeable file\n", *indexFileName);
    exit(1);
  }
}

static index_t* indexBuild(char* pageDirectory)
{
  /*
    creates a new 'index' object
    loops over document ID numbers, counting from 1
      loads a webpage from the document file 'pageDirectory/id'
      if successful,
        passes the webpage and docID to indexPage */
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

static void indexPage(index_t* index, webpage_t* currPage, int docID)
{
  /* steps through each word of the webpage,
     skips trivial words (less than length 3),
     normalizes the word (converts to lower case),
     looks up the word in the index,
       adding the word to the index if needed
     increments the count of occurrences of this word in this docID */
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
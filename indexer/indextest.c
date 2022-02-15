/*
 * indextest.c   Nitya Agarwala     Feb 4, 2022
 *
 * This file contains the indextest for the tse module.
 * It loads an index file produced by the indexer and saves it to another file.
 * It compares that index with the old index.
 *
 * It includes functions:
 * main which parses arguments and initializes other modules
 * indexBuild which builds an index from webpage files in the pageDirectory
 * indexPage which scans a webpage document to add its words to the index.
 *
 * usage: ./indextest oldIndexFilename newIndexFilename
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "index.h"
#include "pagedir.h"
#include "bag.h"
#include "counters.h"
#include "file.h"
#include "hashtable.h"
#include "mem.h"
#include "webpage.h"

// function prototypes
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** oldIndexFileName, char** newIndexFileName);

int main(const int argc, char* argv[])
{
  char* oldIndexFileName = NULL;
  char* newIndexFileName = NULL;
  parseArgs(argc, argv, &oldIndexFileName, &newIndexFileName);
  FILE* fpOld = fopen(oldIndexFileName, "r");
  index_t* index = index_load(fpOld);
  if (index == NULL) {
    fprintf(stderr, "error loading index");
    exit(1);
  }
  FILE* fpNew = fopen(newIndexFileName, "w");
  if (!index_save(index, fpNew)) {
    fprintf(stderr, "error saving index");
    exit(1);
  }
  index_delete(index);
}

static void parseArgs(const int argc, char* argv[], char** oldIndexFileName, char** newIndexFileName)
{
  const char* progName = argv[0];  // name of this program
  if (argc != 3) {
    fprintf(stderr, "error: wrong number of arguments\nusage: %s pageDirectory indexFilename\n", progName);
    exit(1);
  }

  *oldIndexFileName = argv[1];
  FILE* fp = fopen(*oldIndexFileName, "r");
  if (fp != NULL) {
    fclose(fp);
  }
  else {
    fprintf(stderr, "Old index file %s is not a readable file\n", *oldIndexFileName);
    exit(1);
  }

  *newIndexFileName = argv[2];
  // checking if file can be opened to ASK
  FILE* fpNew = fopen(*newIndexFileName, "w");
  if (fpNew != NULL) {
    fclose(fpNew);
  }
  else {
    fprintf(stderr, "New index file %s is not a writeable file\n", *newIndexFileName);
    exit(1);
  }
}
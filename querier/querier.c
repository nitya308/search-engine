/*
 * querier.c   Nitya Agarwala     Feb 18, 2022
 *
 * This file contains the querier for the tse module.
 * It reads the index file produced by the TSE Indexer
 * It read page files produced by the TSE Crawler to get URLs
 * It then answers search queries submitted via stdin.
 *
 * It includes functions:
 * main which runs the program
 * parseArgs which initializes arguments
 * parseQuery which parses the query
 * validateQuery which makes sure the query is valid
 * matchQuery which searches for matches
 * and associated helper functions
 *
 * usage: ./querier pageDirectory indexFilename
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"
#include "file.h"
#include "index.h"

// function prototypes
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName);
static int parseQuery(char* line, char** words);
static bool validateQuery(char** words, int wordCount);
static counters_t* matchQuery(char** words, const int wordCount, index_t* index);
static void mergeMatches(counters_t** andResult, counters_t** orResult);
static void matchOr(counters_t* all, counters_t* word);
static void matchOrHelper(void* arg, int key, const int count);
static void matchAnd(counters_t* all, counters_t* word);
static void matchAndHelper(void* arg, int key, const int count);
static void findNumMatches(void* arg, const int key, const int count);
static void findBestScore(void* arg, const int key, const int count);
static void printMatch(void* arg, const int key, const int count);
static char* getURL(const char* pageDirectory, int docID);
static int min(const int a, const int b);

// struct declarations
struct twoCounters {
  counters_t* all;
  counters_t* word;
};

struct bestScore {
  int topScore;
  int topDoc;
};

/**************** main ****************/
/* Main fucntion for querier.c
 *
 * Arguments: pageDirectory indexFilename
 *
 * Arguments are checked and validated by parseArgs which is called by main
 * Main calls paresArgs,
 * reads queries from stdin
 * and calls functions to find and print result of query
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
  printf("%s", "Query? ");
  char line[150];  // to store line from stdin
  while (fgets(line, 150, stdin) != NULL) {
    if (line != NULL) {
      int len = strlen(line);
      int size = len / 3;
      char* words[size];
      int wc = parseQuery(line, words);
      if (wc != 0) {
        // if more than 0 words exist in query
        if (validateQuery(words, wc)) {
          counters_t* ctr = matchQuery(words, wc, index);
          int nonZero = 0;
          counters_iterate(ctr, &nonZero, findNumMatches);
          if (nonZero < 0) {
            // defensive check for number of matches
            fprintf(stderr, "error occured while counting matches");
            exit(0);
          }
          // if no matches are found
          if (nonZero == 0) {
            printf("%s\n", "No documents match.");
          }
          // if matches are found, it enters loop and prints them
          for (int i = 0; i < nonZero; i++) {
            struct bestScore* currBest = (struct bestScore*)mem_malloc(sizeof(struct bestScore));
            currBest->topDoc = 0;
            currBest->topScore = -1;
            counters_iterate(ctr, (void*)currBest, findBestScore);
            printMatch(pageDirectory, currBest->topDoc, currBest->topScore);
            counters_set(ctr, currBest->topDoc, 0);
            mem_free(currBest);  // ask: where to free?
          }
          counters_delete(ctr);
        }
      }
    }
    printf("%s", "Query? ");
  }
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
 *    indexFileName = argv[2] must be a valid file that can be opened for reading
 */
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName)
{
  const char* progName = argv[0];  // name of this program

  if (argc != 3) {
    fprintf(stderr, "error: wrong number of arguments\nusage: %s seedURL pageDirectory maxDepth\n", progName);
    exit(1);
  }

  *pageDirectory = argv[1];
  if (!pagedir_validate(*pageDirectory)) {
    fprintf(stderr, "error: page directory is invalid or not a crawler produced directory\n");
    exit(1);
  }

  *indexFileName = argv[2];
  FILE* fp = fopen(*indexFileName, "r");
  if (fp == NULL) {
    fprintf(stderr, "error: index file not readable\n");
    exit(1);
  }
  fclose(fp);
}

/**************** parseQuery ****************/
/* Separates query into an array of words
 *
 * Caller provides
 *   a valid line of query string
 *   pointers to an array of strings (array of char pointers) to store words in
 *
 * We return:
 *   the number of words in the query
 * We guarantee:
 *   We print an error and return 0 if a non letter character found in query
 *   We normalize each word
 *   We separate each word and store in words array
 */
static int parseQuery(char* line, char** words)
{
  int wordCount = 0;
  for (char* p = line; *p != '\0'; p++) {
    if (!isspace(*p) && !isalpha(*p)) {
      fprintf(stderr, "Error: bad character '%c' in query\n", *p);
      return 0;
    }
  }

  for (char* q = line; *q != '\0';) {
    char* word = NULL;
    char* s;
    char* l;
    for (s = q; isspace(*s); s++) {
    }
    word = s;
    for (l = s; isalpha(*l); l++) {
    }
    if (*l != '\0') {
      *l = '\0';
      l++;
    }
    q = l;
    normalize_word(word);
    words[wordCount] = word;
    wordCount++;
  }
  return wordCount;
}

/**************** validateQuery ****************/
/* Checks if query is valid
 *
 * Caller provides
 *   array of words in query
 *   int number of words in the array
 *
 * We return:
 *   false if query is invalid, true if it is valid
 *
 * We guarantee:
 *   and/or cannot be the first or last words
 *   and/or cannot be consecutive
 *   we print an error and return 0 as word count if conditions are not met
 *   we print clean query for user to see if conditions are met
 */
static bool validateQuery(char** words, int wordCount)
{
  mem_assert(words, "words given to matchQuery is null");
  if (wordCount < 0) {
    fprintf(stderr, "word count cannot be negative");
    return false;
  }

  if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0) {
    fprintf(stderr, "Error: '%s' cannot be first\n", words[0]);
    return false;
  }

  if (strcmp(words[wordCount - 1], "and") == 0 || strcmp(words[wordCount - 1], "or") == 0) {
    fprintf(stderr, "Error: '%s' cannot be last\n", words[wordCount - 1]);
    return false;
  }

  for (int i = 0; i < wordCount - 1; i++) {
    if (strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0) {
      if (strcmp(words[i + 1], "and") == 0 || strcmp(words[i + 1], "or") == 0) {
        fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", words[i], words[i + 1]);
        return false;
      }
    }
  }

  // print the clean, normalized query for user to see
  printf("%s", "Query:");
  for (int i = 0; i < wordCount; i++) {
    printf(" %s", words[i]);
  }
  printf("%s", "\n");
  return true;
}

/**************** matchQuery ****************/
/* Creates a counter with all macthes for the query searched by user
 *
 * Caller provides
 *   array of words in query
 *   int number of words in the array
 *   index of words in webpage loaded from document
 *
 * We return:
 *   counter with docIDs and score of all documents for given query
 */
static counters_t* matchQuery(char** words, const int wordCount, index_t* index)
{
  mem_assert(words, "words given to matchQuery is null");
  mem_assert(index, "inddex given to matchQuery is null");

  // allocating counters for results of and and or queries
  counters_t* andResult = NULL;
  counters_t* orResult = NULL;

  // variable to track if no match is found
  bool noMatch = false;

  // loop over all words
  for (int i = 0; i < wordCount; i++) {
    if (strcmp(words[i], "or") == 0) {
      mergeMatches(&andResult, &orResult);
      noMatch = false;
      continue;
    }

    if (noMatch) {
      continue;
    }

    if (strcmp(words[i], "and") == 0) {
      continue;
    }

    // finding macthes for each word
    counters_t* matches = index_find(index, words[i]);
    // if no matches found
    if (matches == NULL) {
      noMatch = true;
      if (andResult != NULL) {
        counters_delete(andResult);
        andResult = NULL;
      }
    }
    // if matches found
    else {
      if (andResult == NULL) {
        andResult = mem_assert(counters_new(), "andResult was null");
        matchOr(andResult, matches);
      }
      else {
        matchAnd(andResult, matches);
      }
    }
  }
  mergeMatches(&andResult, &orResult);
  return orResult;
}

/**************** mergeMatches ****************/
/* combine results of and and or seacrhes for each query
 *
 * Caller provides
 *   counter result of and search (andResult)
 *   counter result of or search  (orResult)
 *
 * We guarantee:
 *   We combine both results into orResult and delete the andResult
 */
static void mergeMatches(counters_t** andResult, counters_t** orResult)
{
  if (*andResult != NULL) {
    if (*orResult == NULL) {
      *orResult = mem_assert(counters_new(), "");
    }
    matchOr(*orResult, *andResult);  // combines counters
    counters_delete(*andResult);
    *andResult = NULL;
  }
}

/**************** matchAnd ****************/
/* Find minimum of two word matches for "and" queries
 *
 * Caller provides
 *   counter with matches of query so far (all)
 *   counter with matches of current word (word)
 *
 * We guarantee:
 *   We get the minimum from both counters for each doc
 *   And modify all to store that
 */
static void matchAnd(counters_t* all, counters_t* word)
{
  mem_assert(all, "all in matchAnd was NULL");
  mem_assert(word, "word in matchAnd was NULL");
  struct twoCounters args = {all, word};
  counters_iterate(all, &args, matchAndHelper);
}

/**************** matchAndHelper ****************/
/* helper function for matchAnd. Sets first counter to minumum of two counters
 *
 * Caller provides
 *   arg with two counters in a twoCounters struct
 *
 * We guarantee:
 *   We calculate the minimum from both counters for each doc
 *   And modify the first counter to store it
 */
static void matchAndHelper(void* arg, int key, const int count)
{
  struct twoCounters* args = (struct twoCounters*)arg;
  counters_set(args->all, key, min(count, counters_get(args->word, key)));
}

/**************** matchOr ****************/
/* Find sum of matches for "or" queries
 *
 * Caller provides
 *   counter with matches of query so far (all)
 *   counter with matches of current word (word)
 *
 * We guarantee:
 *   We get the sum of both counters for each doc
 *   And store it in the all counter
 */
static void matchOr(counters_t* all, counters_t* word)
{
  mem_assert(all, "all was NULL in matchOr");
  mem_assert(word, "word was NULL in matchOr");
  counters_iterate(word, all, matchOrHelper);
}

/**************** matchOrHelper ****************/
/* helper function for matchOr. Adds matches for curr word to counter
 *
 * Caller provides
 *   arg with counter
 *
 * We guarantee:
 *   We calculate the sum from both counters for each doc
 *   And modify the first counter to store it
 */
static void matchOrHelper(void* arg, int key, const int count)
{
  counters_t* all = arg;
  int wordFreq = counters_get(all, key);
  counters_set(all, key, count + wordFreq);
}

/**************** findNumMatches ****************/
/* calculate number of matches for a query (how many scores are > 0)
 *
 * Caller provides
 *   arg with pointer to int to store number of matches
 *   docID as key
 *   score as count for that docID
 *
 * We guarantee:
 *   If the score is > 0, we increment number of non zero matches for the query
 */
static void findNumMatches(void* arg, const int key, const int count)
{
  int* nonzero = arg;
  if (count > 0) {
    (*nonzero)++;
  }
}

/**************** findBestScore ****************/
/* Finds best score out of all current scores for query
 *
 * Caller provides
 *   arg with pointer to a bestScore struct that has current best score
 *   docID as key
 *   score as count for that docID
 *
 * We guarantee:
 *   If the score is > current bestScore, we update bestScore
 *   We update it with the new docID and score of bestScore
 */
static void findBestScore(void* arg, const int key, const int count)
{
  struct bestScore* top = arg;
  if (count > top->topScore) {
    top->topDoc = key;
    top->topScore = count;
  }
}

/**************** printMatch ****************/
/* print a match in the appropriate format
 *
 * Caller provides
 *   char pointer to pagedirectory produced by crawler
 *   docID as key
 *   score as count for that docID
 *
 * We guarantee:
 *   We print the score and docID along with URL (if not null)
 */
static void printMatch(void* arg, const int key, const int count)
{
  const char* pageDirectory = arg;
  const int docID = key;
  char* URL = getURL(pageDirectory, docID);
  if (URL == NULL) {
    printf("score %d  doc %d: NULL\n", count, docID);
  }
  else {
    printf("score %d  doc %d: %s\n", count, docID, URL);
  }
  mem_free(URL);
}

/**************** min ****************/
/* returns the minimum of two integer variables
 *
 * Caller provides
 *   two integer variables
 *
 * We return
 *   the smaller integer variable
 */
static int min(const int a, const int b)
{
  return (a > b) ? b : a;
}

/**************** getURL ****************/
/* get a URL from given pageDirectory and docID
 *
 * Caller provides
 *   char pointer to pagedirectory produced by crawler
 *   docID as key
 *   score as count for that docID
 *
 * We guarantee:
 *   We print the score and docID along with URL (if not null)
 */
static char* getURL(const char* pageDirectory, int docID)
{
  if (pageDirectory == NULL) {
    fprintf(stderr, "page directory cannot be NULL");
    return NULL;
  }
  char* URL = NULL;
  char* strID = malloc(11);  // max length of integer in c is 10 + 1 for '\0'
  sprintf(strID, "%d", docID);
  if (strID == NULL) {
    fprintf(stderr, "error: could not access ID\n");
    return NULL;
  }
  // create pathname for crawler produced file
  char* pathName = mem_malloc_assert((strlen(pageDirectory) + 11), "error making filename");
  strcpy(pathName, pageDirectory);
  strcat(pathName, "/");
  strcat(pathName, strID);

  FILE* fp = fopen(pathName, "r");
  if (fp == NULL) {
    fprintf(stderr, "error: file does not exist or cannot be read");
    return NULL;
  }
  URL = file_readLine(fp);  // read URL from file (first line of file)
  fclose(fp);
  free(strID);
  free(pathName);
  return URL;
}

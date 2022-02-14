/*
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Nitya Agarwala, Jan 2022
 */

#include "index.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counters.h"
#include "file.h"
#include "hashtable.h"
#include "mem.h"

/**************** file-local global variables ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
index_t* index_new(const int index_slots);
bool index_insert(index_t* index, char* word, counters_t* ctr);
counters_t* index_find(index_t* index, char* word);
bool index_save(index_t* index, FILE* fp);
void index_delete(index_t* index);
index_t* index_load(FILE* fp);

/**************** local functions ****************/
static void print_word(void* fp, const char* word, void* counter);
static void print_count(void* fp, const int docID, const int count);
static void delete_count(void* item);

/**************** local types ****************/
typedef hashtable_t index_t;

/**************** index_new() ****************/
/* see index.h for description */
index_t* index_new(const int index_slots)
{
  // ERROR CHECKING HERE ASK
  return (index_t*)hashtable_new(index_slots);
}

/**************** index_insert() ****************/
/* see index.h for description */
bool index_insert(index_t* index, char* word, counters_t* ctr)
{
  return hashtable_insert(index, word, ctr);
}

/**************** index_find() ****************/
/* see index.h for description */
counters_t* index_find(index_t* index, char* word)
{
  return (counters_t*)(hashtable_find(index, word));
}

/**************** index_save() ****************/
/* see index.h for description */
bool index_save(index_t* index, FILE* fp)
{
  if (index == NULL || fp == NULL) {
    return false;
  }
  hashtable_iterate(index, fp, print_word);
  fclose(fp);
  return true;
}

/**************** print_word() ****************/
/* Helper function to print each word when saving index */
static void print_word(void* fp, const char* word, void* counter)
{
  fprintf((FILE*)fp, "%s ", word);
  counters_iterate((counters_t*)counter, fp, print_count);
  fprintf((FILE*)fp, "\n");
}

/**************** print_count() ****************/
/* Helper function to print each docID and count when saving index */
static void print_count(void* fp, const int docID, const int count)
{
  fprintf((FILE*)fp, "%d %d ", docID, count);
}

/**************** index_delete() ****************/
/* see index.h for description */
void index_delete(index_t* index)
{
  hashtable_delete(index, delete_count);
}

/**************** delete_count() ****************/
/* Helper function to call counters_delete to delete counters in the index */
static void delete_count(void* item)
{
  counters_t* ctrs = item;
  counters_delete(ctrs);
}

/**************** index_load() ****************/
/* see index.h for description */
index_t* index_load(FILE* fp)
{
  // ASK: null check + assert?
  if (fp == NULL) {
    fprintf(stderr, "file pointer was NULL");
    return NULL;
  }
  // create new index of appropriate size
  int num_words = file_numLines(fp);
  index_t* index = index_new((num_words / 2) + 1);
  if (index == NULL) {
    fprintf(stderr, "xyz");
    return NULL;
  }
  // get information from file
  char* currWord = NULL;
  while ((currWord = file_readWord(fp)) != NULL) {
    counters_t* ctr = mem_assert(counters_new(), "creating counter failed");
    int count;
    int docID;
    while ((fscanf(fp, " %d %d", &docID, &count) == 2)) {
      if (!counters_set(ctr, docID, count)) {
        fprintf(stderr, "counters set failed");
      }
    }
    if (!hashtable_insert(index, currWord, ctr)) {
      fprintf(stderr, "hashtable insert failed");
    }
    free(currWord);
  }
  // close file and return index
  fclose(fp);
  return index;
}
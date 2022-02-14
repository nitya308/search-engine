/*
 * index.h - header file for CS50 hashtable module
 *
 * A *hashtable* is a set of (key,item) pairs.  It acts just like a set,
 * but is far more efficient for large collections.
 *
 * Nitya Agarwala, Feb 2022
 */

#include <stdbool.h>
#include <stdio.h>

#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index
 *
 * Caller provides:
 *   number of slots to be used for the index (must be > 0).
 * We return:
 *   pointer to the new index; return NULL if error.
 * We guarantee:
 *   index is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
 */
index_t* index_new(const int index_slots);

/**************** index_insert ****************/
/* Insert word into the given index
 *
 * Caller provides:
 *   valid pointer to index, valid string for word, valid int for docID>0
 * We return:
 *   false if any parameter is NULL, or error;
 *   true iff new item was inserted.
 * Notes:
 *   A new counter for that word is inserted if the word is not in the index
 */
bool index_insert(index_t* index, char* word, counters_t* ctr);

/**************** index_find ****************/
/* Return the count associated with the given work in given docID.
 *
 * Caller provides:
 *   valid pointer to index, valid string for word, valid docID > 0
 * We return:
 *   the counter associated with the word in the index
 *   NULL if the counter doesn't exist
 * Notes:
 *   the index is unchanged by this operation.
 */
counters_t* index_find(index_t* index, char* word);

/**************** index_delete ****************/
/* Delete index
 *
 * Caller provides:
 *   valid index pointer
 * We do:
 *   if index==NULL, do nothing.
 *   Delete the index
 *   Delete each item in the index by deleting the counters
 */
void index_delete(index_t* index);

/**************** index_save ****************/
/* Save the index to a file
 *
 * Caller provides:
 *   valid index pointer and valid file pointer open for writing
 * We do:
 *   if index==NULL, return flase
 *   write the index to the file
 *   index is unchanged by this operation
 *   return true if index is successfully saved
 */
bool index_save(index_t* index, FILE* fp);

/**************** index_load ****************/
/* Load an index from a file
 *
 * Caller provides:
 *   valid file pointer open for reading with index in it
 * We do:
 *   if new index can't be created, return NULL
 *   read file to create new index
 *   return the index
 */
index_t* index_load(FILE* fp);
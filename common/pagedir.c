/*
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Nitya Agarwala, Jan 2022
 */

#include "pagedir.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libcs50/file.h"
#include "../libcs50/mem.h"
#include "../libcs50/webpage.h"

/**************** file-local global variables ****************/
/* none */

/**************** global functions ****************/
/* that is, visible outside this file */
bool pagedir_init(const char *pageDirectory);
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID);
bool pagedir_validate(const char *dir);
webpage_t *pagedir_load(const char *pageDirectory, const int docID);
static char *make_filename(const char *pageDirectory, const int docID);

/**************** pagedir_init() ****************/
/* see pagedir.h for description */
bool pagedir_init(const char *pageDirectory)
{
  // construct the pathname for the .crawler file in that directory
  char *pathName = mem_malloc_assert((strlen(pageDirectory) + strlen("/.crawler") + 1), "error making pathname");
  if (pathName == NULL) {
    fprintf(stderr, "error: could not create pathname\n");
    exit(3);
  }
  strcpy(pathName, pageDirectory);
  strcat(pathName, "/.crawler");

  // open the file for writing
  FILE *fp = NULL;
  fp = fopen(pathName, "w");
  if (fp == NULL) {
    // on error, returns false
    free(pathName);
    return false;
  }
  // close the file and return true
  fclose(fp);
  free(pathName);
  return true;
}

/**************** pagedir_save() ****************/
/* see pagedir.h for description */
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID)
{
  // construct the pathname for the page file in pageDirectory
  char *pathName = make_filename(pageDirectory, docID);

  // open the file for writing
  FILE *fp = NULL;
  fp = fopen(pathName, "w");
  if (fp == NULL) {
    free(pathName);
    fprintf(stderr, "error: could not open file %d for writing\n", docID);
    exit(4);
  }

  free(pathName);

  // print the URL
  char *url = webpage_getURL(page);
  if (url == NULL) {
    fprintf(stderr, "error while writing the URL of file %d\n", docID);
    fclose(fp);
    exit(4);
  }
  fprintf(fp, "%s\n", url);

  // print the depth
  int depth = webpage_getDepth(page);
  if (depth < 0) {
    fprintf(stderr, "error while writing the depth of file %d\n", docID);
    fclose(fp);
    exit(4);
  }
  fprintf(fp, "%d\n", depth);

  // print the contents of the webpage
  char *html = webpage_getHTML(page);
  if (html == NULL) {
    fprintf(stderr, "error while writing the html content of file %d\n", docID);
    fclose(fp);
    exit(4);
  }
  fprintf(fp, "%s", html);

  // close the file
  if (fclose(fp) != 0) {
    fprintf(stderr, "error while closing the file %d\n", docID);
    exit(3);
  }
}

/**************** pagedir_validate() ****************/
/* see pagedir.h for description */
bool pagedir_validate(const char *dir)
{
  char *fileName = mem_malloc_assert((strlen(dir) + strlen("/.crawler") + 1), "error creating filename");
  strcpy(fileName,dir);
  strcat(fileName, "/.crawler");
  FILE *fp;
  if ((fp = fopen(fileName, "r")) != NULL) {
    // File exists thus its a crawler directory
    fclose(fp);
    mem_free(fileName);
    return true;
  }
  mem_free(fileName);
  return false;
}

/**************** pagedir_load() ****************/
/* see pagedir.h for description */
webpage_t *pagedir_load(const char *pageDirectory, const int docID)
{
  char *pathName = make_filename(pageDirectory, docID);
  FILE *fp = fopen(pathName, "r");
  if (fp == NULL) {
    // fill does not exist
    // fclose(fp);
    mem_free(pathName);
    return NULL;
  }
  char *url = file_readLine(fp);
  char *depthString = file_readLine(fp);
  int depth;
  sscanf(depthString, "%d", &depth);
  free(depthString);
  char *html = file_readFile(fp);
  fclose(fp);
  mem_free(pathName);
  webpage_t *result = mem_assert(webpage_new(url, depth, html), "creating webpage failed");
  return result;
}

/**************** make_filename() ****************/
/* Make a filename from a page directory and integer id of file */
static char *make_filename(const char *pageDirectory, const int docID)
{
  char *strID = malloc(11);  // max length of integer in c is 10 + 1 for '\0'
  sprintf(strID, "%d", docID);
  if (strID == NULL) {
    fprintf(stderr, "error: could not access ID\n");
    exit(3);
  }
  char *pathName = mem_malloc_assert((strlen(pageDirectory) + 11), "error making filename");
  strcpy(pathName, pageDirectory);
  strcat(pathName, "/");
  strcat(pathName, strID);
  free(strID);
  return pathName;
}
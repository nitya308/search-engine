/* 
 * bag.c - CS50 'bag' module
 *
 * see bag.h for more information.
 *
 * David Kotz, April 2016, 2017, 2019, 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../libcs50/webpage.h"
#include "pagedir.h"

bool pagedir_init(const char *pageDirectory);
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID);

char *make_filename(const char *directorName, const char *file);

bool pagedir_init(const char *pageDirectory)
{
  //construct the pathname for the .crawler file in that directory
  // TOASK: HOW TO???
  char *pathName = make_filename(pageDirectory, ".crawler");

  // open the file for writing
  FILE *fp = NULL;
  fp = fopen(pathName, "w");
  if (fp == NULL)
  {
    // on error, returns false
    free(pathName);
    return false;
  }
  // close the file and return true
  fclose(fp);
  free(pathName);
  return true;
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID)
{
  // construct the pathname for the page file in pageDirectory
  // TO ASK HOW TO MAKE PATHNAME AND FREE????

  char *pathName = make_filename(pageDirectory, docID); //HOW TO CONVERT TO INT?

  // open the file for writing
  FILE *fp = NULL;
  fp = fopen(pathName, "w");
  if (fp == NULL)
  {
    free(pathName);
    fprintf(stderr, "error: could not open file %d for writing\n", docID);
    exit(3);
  }

  free(pathName); //FREE HERE?

  char *url = webpage_getURL(page);
  if (url == NULL)
  {
    fprintf(stderr, "error while writing the URL of file %d\n", docID);
    exit(3); //TOASK: EXIT HERE OK?
  }
  fprintf(fp, "%s\n", url);

  int depth = webpage_getDepth(page);
  if (depth < 0)
  {
    fprintf(stderr, "error while writing the depth of file %d\n", docID);
    exit(4);
  }
  fprintf(fp, "%d\n", depth);

  // print the contents of the webpage TO ASK OK??
  char *html = webpage_getHTML(page);
  if (html == NULL)
  {
    fprintf(stderr, "error while writing the html content of file %d\n", docID);
    exit(5);
  }
  else
  {
    fprintf(fp, "%s", html);
  }

  // close the file
  if (fclose(fp) != 0)
  {
    fprintf(stderr, "error while closing the file %d\n", docID);
    exit(6);
  }
}

char *make_filename(const char *directoryName, const char *file)
{
  char *fileName = malloc(strlen(directoryName) + strlen(file) + 2);
  if (fileName == NULL)
  {
    fprintf(stderr, "Error in allocating memory for filename");
    exit(5);
  }
  strcat(fileName, directoryName);
  strcat(fileName, "/");
  strcat(fileName, file);
  return fileName;
}

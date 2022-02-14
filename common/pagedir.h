/*
 * pagedir.h - header file for CS50 'pagedir' module
 *
 *
 * This module is respinsible for:
 * Initializing and validating a pageDirectory
 * Handles writing a page to the pageDirectory
 *
 * Nitya Agarwala, Jan 2022
 */

#include <stdbool.h>
#include "../libcs50/webpage.h"

/**************** pagedir_init ****************/
/* Initializes and validates a pageDirectory
 *
 * Caller provides:
 *   a valid pageDirectory name of an existing directory
 * We guarantee:
 *   We create a .crawler file in the directory
 *   If file can not be created or opened for writing, we return false
 *   If file can be opened, we return true and close the file
 *   We free the memory allocated for path name of the file
 */
bool pagedir_init(const char *pageDirectory);

/**************** pagedir_init ****************/
/* Saves the webpage given
 *
 * Caller provides:
 *   a valid webpage_t to save
 *   a unique ID
 *   an initialized pageDirectory with a .crawler file in it
 * We guarantee:
 *   We create a file with the pathname pageDirectory/docID
 *   We print the URL, depth and HTML contents of the webpage to the file
 *   We close the file and free memory allocated for path name of file
 * We print an error to stderr and exit non-0 if:
 *   If we cannot create the pathname for the file
 *   If file cannot be created or opened for writing
 *   If there is an error while retrieving content to print to file
 */
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID);

/**************** pagedir_validate ****************/
/* Validates if the given directory is a crawler produced directory
 * by checking if .crawler file exists in it
 *
 * Caller provides:
 *   a valid char pointer to a directory name
 * We guarantee:
 *   We return true if the directory contains a .crawler file
 *   We return false if the directory does not exist or does not contain a .crawler file
 */
bool pagedir_validate(const char *dir);

/**************** pagedir_load ****************/
/* loads a webpage from file of given docID
 *
 * Caller provides:
 *   a valid webpage_t pagedirectory
 *   a unique ID
 * We guarantee:
 *   We create the pathname for the file pageDirectory/docID
 *   We create a webpage with the contents by reading the file
 *   We close the file and free memory allocated for path name of file
 * We print an error to stderr and exit non-0 if:
 *   If we cannot create the pathname for the file
 *   If file cannot be opened for reading
 *   If there is an error while retrieving content from the file for the webpage
 */
webpage_t *pagedir_load(const char *pageDirectory, const int docID);
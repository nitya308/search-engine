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


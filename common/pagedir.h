/* 
 * bag.h - header file for CS50 'bag' module
 * 
 * A 'bag' is a collection of indistinguishable items; it starts empty, 
 * grows as the caller adds one item at a time, and shrinks as the caller 
 * extracts one item at a time. It could be empty, or could contain hundreds
 * of items. Since items are indistinguishable, the module is free to return
 * any item from the bag. 
 *
 * David Kotz, April 2016, 2017, 2019, 2021
 */

#include <stdbool.h>
#include "../libcs50/webpage.h"

bool pagedir_init(const char *pageDirectory);
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID);

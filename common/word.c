/*
 * word.c - CS50 'word' module
 *
 * see word.h for more information.
 *
 * Nitya Agarwala, Jan 2022
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pagedir.h"

/**************** normalize_word() ****************/
/* see word.h for description */
char* normalize_word(char* word)
{
  if(word!=NULL) {
    for (char* c = word; *c!='\0'; c++)
    {
      *c = tolower(*c);
    }
  }
  return word;
}
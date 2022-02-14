/* 
 * word.h - header file for CS50 'word' module
 * 
 * 
 * This module is respinsible for: 
 * converting a word to lowercase
 *
 * Nitya Agarwala, Jan 2022
 */

/**************** normalize_word() ****************/
/* Converts word to lowercase
 *
 * Caller provides:
 *   a valid character pointer to a word
 * We guarantee:
 *   a NULL word is ignored
 *   the lowercase version of the word is returned
 * Caller is responsible for:
 *   not free-ing the item as long as it remains in the bag.
 */
char* normalize_word(char *word);

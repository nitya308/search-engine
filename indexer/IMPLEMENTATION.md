# CS50 TSE Indexer
## Implementation Spec

Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

The indexer uses a new data structure index which is defined as index_t in the index module (see index.h).
The index stores a 'hashtable' where the _key_ is the word and the _item_ is a 'counter' with the _key_ being the docID and the _count_ being the number of times that word appears in the document.
The index starts empty
The size of the index is impossible to determine in advance, so we use 900.

## Control flow

The indexer is implemented in one file `indexer.c`, with four functions.

### main

The `main` function simply calls `parseArgs` and `indexBuild` to make the index. Then it calls `index_save` to write the index to a file and `index_delete`, then exits zero.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, validate it is an existing, crawler produced directory
* for `indexFileName`, create it if it doesn't exist and verify it is a writeable file

### indexBuild

Do the real work of building the index from the crawler produced output in `pageDirectory`.
Pseudocode:
  create a new 'index' object
  loop over ids from 1
  while a file with pathname 'pageDirectory/id' exists
    loads a webpage from the document file 'pageDirectory/id'
    if successful, 
      passes the webpage and docID to indexPage

### indexPage

This function implements the *indexPage* mentioned in the design.
Given a `webpage`, scan the given page to extract any words, ignoring words with length under 3. Add its words and their count along to the `index` by passing the word as the _key_, and adding to counter with the _docID_.
Pseudocode:

while there is another word in the page,
  if that word is greater than length 3
    normalizes the word (converts to lower case),
    if word does not alreayd exist in the index
      add word with a new counter to the index
    increments the count of occurrences of this word in this docID

## Other modules

### index

We create a re-usable module `index.c` to handles the *index_t* type mentioned in the design. It has functions to create a new index, insert a word into the index, save the index to a file, delete it and load an index from a new file.
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about index type.

It has the following functions 
Pseudocode for `index_new`: 
  creates a new index_t, by calling hashtable_new with specified number of slots.

Pseudocode for `index_insert`: 
  calls hashtable_insert on the index to insert key as the word and the provided ctr as the item

Pseudocode for `index_find`:
  call hashtable_find to find the item for corresponding key (word) if it exists
  return the item after typecasting to counter

Pseudocode for `index_save`:
  if index and file pointer are not null
    iterate over hashtable
      print word in each iteration by calling helper print_word
        iterate over the counter
          print docID and word count in each iteration by calling helper print_count
  close the file
  return true

Pseudocode for `index_delete`:
  Delete the hashtable by calling hashtable_delete
    In each slot of hashtable, delete the counter by calling counters_delete

Pseudocode for `index_load`:
  extract number of words in the open file
  create new index with (num_words / 2) + 1 slots
  while there is another word in the file
    extract the word
    create a new counter
    extract the docID and count from the file
    set docID and count in counter
    insert counter into hashtable with word as key
    free the word
  close the file
  return the index

We also add new functions `pagedir_validate` and `pagedir_load` to the pagedir module to verify whether a pageDirectory is a crawler produced directory.


Pseudocode for `pagedir_validate`:
	construct the pathname for the .crawler file in that directory
	open the file for reading; on error return false
	close the file and return true


Pseudocode for `pagedir_load`:
	construct the pathname for the page file in pageDirectory
	open that file for reading
	extract the URL
	extract the depth
	extract the html contents of the webpage
	create a webpage with the URL, depth and html; on error exit
  return the webpage


### libcs50

We leverage the modules of libcs50, most notably `hashtable`, `counter`, and `webpage`.
See that directory for module interfaces.

## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `indexer.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[], char** pageDirectory, char** indexFileName);
static index_t* indexBuild(char* pageDirectory);
static void indexPage(index_t* index, webpage_t* currPage, int docID);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_validate(const char *dir);
webpage_t *pagedir_load(const char *pageDirectory, const int docID);
```

### index

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `index.h` and is not repeated here.

```c
index_t* index_new(const int index_slots);
bool index_insert(index_t* index, char* word, counters_t* ctr);
counters_t* index_find(index_t* index, char* word, int docID);
bool index_save(index_t* index, FILE* fp);
void index_delete(index_t* index);
index_t* index_load(FILE* fp);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `pagedir_init` returns false if there is any trouble creating the `.crawler` file, allowing the Crawler to decide what to do; the `webpage` module returns false when URLs are not retrievable, and the Crawler does not treat that as a fatal error.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are only two units (indexer and index).
The indexer represents the whole system and is covered below.
A program indextest serves as a unit test for the index module; it reads an index file into the internal index data structure, then writes the index out to a new index file. We use indexcmp to compare that the two indexes in the old and new file are the same. 

### Integration/system testing

We write a script `testing.sh` that invokes the inexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
* no arguments
* one argument
* three or more arguments 
* invalid pageDirectory (non-existent path) 
* invalid pageDirectory (not a crawler directory) 
* invalid indexFile (non-existent path) 
* invalid indexFile (read-only directory) 
* invalid indexFile (existing, read-only file)
Second, we run indexer on directories from the cs50-dev/shared/tse/output.
We use indextest as one means of validating the resulting index

We run valgrind on both indexer and indextest to ensure no memory leaks or errors.

We run that script with `bash -v testing.sh` so the output of crawler is intermixed with the commands used to invoke the crawler.

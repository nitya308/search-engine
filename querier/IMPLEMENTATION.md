# CS50 TSE Querier
## Implementation Spec

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

The main data structure we use is the 'index', loaded from the index file. The index contains 'counters' inside it. Inside that we use counter methods such as counters_iterate to work with the number of words in each document. 

We create two structs to help us:
`struct twoCounters` allows us to store two counter and pass them as an argument.
`struct bestScore` stores integers docID and the score of the current best score document for a particular query.

## Control flow

The Querier is implemented in one file `querier.c`, with four functions.

### main

The `main` function simply calls `parseArgs` and loads the index from a file.
It then reads from stdin in a loop, callig functions to find and print each query searched
It exits zero.
Psudeocode:

  call parseArgs
  load index from index file by calling index_load
  while the stdin input is not null
    store input in line
    call parseQuery on line
    if query has> 0 words 
      call validateQuery on line
      if query is valid 
        call matchQuery and store resulting counter
        count number of matches found
        if > 0 matches are found
          loop number of matches times
            find bestScore from counter
            print bestScore from counter
            set that score to 0
        delete counter
  delete index
  exit 0

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, call `pagedir_validate()`
* for `indexFileName` ensure it is an existing, readable file
* if any trouble is found, print an error to stderr and exit non-zero.

### parseQuery

Given a char pointer to a query and an empty array of words, separates query into an array of words.

Pseudocode:

	loop over word
		if non letter character found, print error and return false
	loop over line
		create a string variable word (initialized to NULL)
		skip empty spaces
		store pointer to first letter in word
		skip letters
		set the last character after the letter to '\0'
		normalize word
		add word to words array
		increment word count
	return word count

### validateQuery

Checks if query is valid as per requirement specs. 
Given a array of words in the query and the number of words in the query, returns true if it is valid and false if not.

Pseudocode:

	if the first word is "and" / "or"
		print error "and/"or" cannot be first
		return false

	loop over all words except last word
		if word is "and" / "or"
			if next word is "and" / "or"
				print error "and/"or" cannot be consecutive
				return false

	if the last word is "and" / "or"
		print error "and/"or" cannot be last
		return false
  
	print clean query for user to see
	return true

### matchQuery
Creates a counter with all matches for the query searched by user
Given a array of words in the query, the number of words and an index, return a counter with the docIDs and score of the documents in the index for the query

Psuedocode:

	initialize a counter for currentMatches to NULL
	initialize a counter for finalMatches to NULL
	initalize bool variable to track noMatch to false
		loop over all words
			if word is "or"
				combine all current matches into final matches
			if no match was found for previous word
				move to next word
			if word is "and"
				move to next word
			find counter with matches for current word
			if no match was found 
				set noMatch tracker to true
				delete current matches (since no words with "and" will match)
			if matches were found
				if no current matches exist
					allocate a new counter for current matches
					call OR matching function to save matches for new word in current matches
				if current matches already exist
					call AND matching function on current matches and new matches
					store the result in current matches
	combine all current matches into final matches
	return final matches

### matchAnd
Two find matches for two sequences when "and" is between them. Given a counter with all matching documents, and a counter for matching documents for the current word, it returns a counter with the mininum score of each document.

Psuedocode:

	put both counters in a structure called twoCounters
	calls counters_iterate on the counter with all matches and passes matchAndHelper

### matchAndHelper
The helper function for matchAnd above. Given an argument with two counters, a key and a count, it sets the count of each docID in the first counter to the minumum of counts from the two counters.

Psuedocode:

	extracts two counters from argument
	find the minimum count for the given docID key from the two counters
	sets the count of the docID in the first counter to the minimum

### matchOr
Two find matches for two sequences when "or" is between them. Given a counter with all matching documents, and a counter for matching documents for the current word, it returns a counter with the score = sum of hits in each document.

Psuedocode:

	verifies that arguments are not null
	calls counters_iterate on the counter with current word matches and passes matchOrHelper

### matchOrHelper
The helper function for matchOr above. Given an argument with a counters, a key and a count, it sets the count of each docID in the first counter to the sum of hits in each document.

Psuedocode:

	extracts the counter with all matches from argument
	adds the count from counter for current word and count from all 
	sets the count of all to this sum

### findNumMatches
Given an integer pointer as an argument, a docID as key and a count, calculates number of documents with matches for a query (how many scores are > 0).

Psuedocode:

	extracts int pointer from argument
	if the count is greater than 0, increment the int pointer

### findBestScore
Given a bestScore struct that holds the docID and score for the best score, updates it if the given docID has a better score.

Psuedocode:

	if the count> current best score
		set bestScore id to docID
		set bestScore score to new best score

### printMatch
Prints a match for the query in the appropriate format as specified in the requirements spec.

### getURL
Given a pageDirectory and docID, extracts the URL from that file.

Psuedocode:

	construct a pathname = pageDirectory/docID
	open a file pointer for reading
	if file cannot be read
		print error and return NULL
	read the first line of the file (URL)
	close file
	return the URL


## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: for example, `getURL` returns NULL if there is any trouble extracting the URL from the `.crawler` file. `printMatch` does not treat this as a fatal error but rather prints NULL for that match.

## Testing plan

Here is an implementation-specific testing plan.

### Regression testing

The bash script `testing.sh` tests querier.c using the crawler output and index files in ~/cs50-dev/shared/tse/output

We test `querier` with various invalid arguments.

We test bad queries from the file `badqueries.txt`. These produce appropriate error messages which are compared manually against the sample output from REQUIREMENTS.md. 

We run querier over crawler and indexer output from `letters` at depths 2, 3, `toscrape` at depths 1, 2, and `wikipedia` at depths 2, 3. Queries are read from individual files for each directory.

We then runs *valgrind* on querier results on over all 3 directories to make sure there are no memory leaks or errors

Verify correct behavior by studying the output manually. We start small with single word queries that only have a few matches which can easily be checked. Then we incorporate "and" and "or" queries and check the results against the index.

### Fuzz testing

The program `fuzzquery` is used to generate 1000 random queries and prints success or fail depending on the result. It runs on toscrape-2. We also run this with *valgrind* to make sure there are no errors.

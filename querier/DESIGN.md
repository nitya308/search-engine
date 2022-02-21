# CS50 TSE Querier
## Design Spec

According to the [Querier Requirements Spec](REQUIREMENTS.md), TSE *Querier* is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

### User interface

The querier's only interface with the user is on the command-line; it must always have two arguments.

```
querier pageDirectory indexFilename
```

For example, if `letters` is a directory produced by the crawler and `../letters.index` is thepathname of a file produced by the indexer.

``` bash
$ querier ../data/letters ../data/letters.index
```

### Inputs and outputs

**Input**: the querier reads search queries from stdin, one per line, until the end of file or end of input EOF (as described in the Requirements).

**Output**: We print to stdout:
- No documents match if no documents satisfy the query.
- Set of documents in decreasing rank order in format score, document ID and URL.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which runs the program and reads lines from stdin;
 2. *parseArgs*, which parses and validates the argument
 3. *parseQuery* which parses the query into an array of normalized words
 4. *validateQuery* which validates that the query meets the requirements
 5. *matchQuery*, which returns a counter of all matching docIDs and scores
 6. Associated helper functions for matchQuery
 7. *findNumMatches* which counts the number of successful matching documents
 8. *findBestScore* which calculates the top scoring document from all the documents in the counter of matches
 8. *printMatches* which prints matching documents with scores and URL as specified by querier

Additionally we use functions from some of helper modules that we built ealier:

 1. *index*, we use `index_load` to load the index from the file
 1. *counters*, we store the matches in a counter and call on functions from this module to find the top matches
 2. *pagedir*, a module providing functions to check if the pageDirectory is a crawler produced directory
 4. *word*, a module providing a function to normalize a word.

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

	parse the command line, validate parameters
	call parseQuery
	call validateQuery (checks if it's valid per requirements spec)
	if query is valid
		call matchQuery to find matches
		calculate number of successful matches
		for each match (print from best to worst per req. spec)
			call findBestScore to get top score
			print that best score
			set that score to 0

where *parseQuery:*

	loops over wordCount counting from 0
	for each new word found in the query string
		normalizes the word
		adds it to an array of words at index wordCount
		increments wordCount

where *matchQuery:*

	stores two counters for current and final matches
	loops over all words
	if word is "or"
		call OR matching helper function to find union of current and new matches
    call merge helper function to save this union into final matches
	finds counter with matches for current word
		if no matches are found 
			delete current matches (since no words with "and" will match)
		if matches are found
			if no current matches exist
				save matches for new word in current matches
			if current matches already exist
				call AND matching helper function to find intersection of current and new matches
				store the result in current matches
	combine union of all current matches with final matches
  store union in final matches
	return final matches


### Major data structures

The key data structure is the *index*, mapping from *word* to *(docID, #occurrences)* pairs.
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* is keyed by *docID* and stores a count of the number of occurrences of that word in the document with that ID. 

### Testing plan

*Regression testing*.  The bash script `testing.sh` tests querier.c using the crawler output and index files in ~/cs50-dev/shared/tse/output

1. Test `querier` with various invalid arguments.
	2. no arguments
	3. one argument
	4. three or more arguments
	5. invalid `pageDirectory` (non-existent path)
	5. invalid `pageDirectory` (not a crawler directory)
	6. invalid `indexFileName` (non-existent path)
2. Test bad queries from the file badqueries.txt. These produce appropriate error messages which are compared manually against the sample output from REQUIREMENTS.md. 
3. We run querier over crawler and indexer output from `letters` at depths 2, 3, `toscrape` at depths 1, 2, and `wikipedia` at depths 2, 3. Queries are read from individual files for each directory.
4. It then runs *valgrind* on querier results on over all 3 directories to make sure there are no memory leaks or errors
5. All output is manually checked. We start small with single word queries that only have a few matches which can easily be checked. Then we incorporate "and" and "or" queries and check the results against the index.

*Fuzz testing*: The program fuzzquery is used to generate 1000 random queries and prints success or fail depending on the result. It runs on toscrape-2. We also run this with *valgrind* to make sure there are no errors.



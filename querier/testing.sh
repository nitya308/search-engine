# testing.sh - performs indexer testing
#
# usage: testing.sh
#
# input: none
# output: writes file with results from tests

# Checking with wrong arguments
# zero arguments                  
./querier

# one argument                                         
./querier ~/cs50-dev/shared/tse/output

# three arguments
./querier ~/cs50-dev/shared/tse/output ../indexOutput extraArgument

# invalid pageDirectory (non-existent path) 
./querier ../doesNotExist ../indexOutput/1

# invalid pageDirectory (not a crawler directory)
mkdir ../notcrawler
./querier ../notcrawler ../indexOutput/1

# invalid indexFile (non-existent path) 
./querier ~/cs50-dev/shared/tse/output/letters-3 ../doesNotExist/file

# testing bad queries
./querier ~/cs50-dev/shared/tse/output/letters-3 ~/cs50-dev/shared/tse/output/letters-3.index < badqueries.txt


# **** Regression Testing ****
# Testing letters depth 2
./querier ~/cs50-dev/shared/tse/output/letters-2/ ~/cs50-dev/shared/tse/output/letters-2.index < letterstest.txt

# Testing letters depth 3 with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./querier ~/cs50-dev/shared/tse/output/letters-3/ ~/cs50-dev/shared/tse/output/letters-3.index < letterstest.txt

# Testing toscrape depth 2
./querier ~/cs50-dev/shared/tse/output/toscrape-2/ ~/cs50-dev/shared/tse/output/toscrape-2.index < scrapetest.txt

# Testing toscrape depth 1 with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./querier ~/cs50-dev/shared/tse/output/toscrape-1/ ~/cs50-dev/shared/tse/output/toscrape-1.index < scrapetest.txt

# Testing wikipedia depth 2
./querier ~/cs50-dev/shared/tse/output/wikipedia-2/ ~/cs50-dev/shared/tse/output/wikipedia-2.index < wikitest.txt

# Testing wikipedia depth 3 with valgrind
valgrind --leak-check=full --show-leak-kinds=all ./querier ~/cs50-dev/shared/tse/output/wikipedia-3/ ~/cs50-dev/shared/tse/output/wikipedia-3.index < wikitest.txt

# **** Fuzz Testing ****
# Generating 1000 random queries
./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 1000 0 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index > /dev/null && echo success || echo failed

# Running valgrind on fuzzquery
valgrind --leak-check=full --show-leak-kinds=all ./fuzzquery ~/cs50-dev/shared/tse/output/toscrape-2.index 1000 0 | ./querier ~/cs50-dev/shared/tse/output/toscrape-2 ~/cs50-dev/shared/tse/output/toscrape-2.index > /dev/null && echo success || echo failed
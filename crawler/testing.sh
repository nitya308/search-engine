# testing.sh - performs crawler testing
#
# usage: testing.sh
#
# input: none
# output: writes file with results from tests

# Checking with wrong arguments
# zero arguments                                                                    
./crawler

# one argument (seedURL)                                            
./crawler x

# two arguments (seedURL pageDirectory)
./crawler x y

# three arguments but URL is invalid                            
./crawler XYZ testDir 0

# URL is valid but not internal                          
./crawler https://canvas.dartmouth.edu/ testDir 0

# Directory does not exist                        
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ testDir 0

# creating testDir/test0 now
mkdir testDir
mkdir testDir/test0

# three arguments but maxDepth less than 0 (seedURL pageDirectory maxDepth)                                
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html testDir/test0 -10

# three arguments but maxDepth greater than 10 (seedURL pageDirectory maxDepth)   
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html testDir/test0 15

# Running valgrind over moderate-sized test case 
mkdir testDir/valgrind
make valgrind

# Running over all 3 CS50 websites
# Run over letters at depth 0
mkdir testDir/letters-depth0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testDir/letters-depth0 0

# Run over letters at depth 1
mkdir testDir/letters-depth1
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testDir/letters-depth1 1

# Run over letters at depth 2
mkdir testDir/letters-depth2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testDir/letters-depth2 2

# Run over letters at depth 10: this makes sure it can ignore intrnal and duplicate URLs
mkdir testDir/letters-depth10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html testDir/letters-depth10 10

# Run over toscrape at depth 0
mkdir testDir/toscrape-depth0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html testDir/toscrape-depth0 0

# Run over toscrape at depth 1
mkdir testDir/toscrape-depth1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html testDir/toscrape-depth1 1

# Run over toscrape at depth 2
mkdir testDir/toscrape-depth2
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html testDir/toscrape-depth2 2

# Run over toscrape at depth 3
mkdir testDir/toscrape-depth3
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html testDir/toscrape-depth3 3

# Run over wikipedia at depth 0
mkdir testDir/wikipedia-depth0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html testDir/wikipedia-depth0 0

# Run over wikipedia at depth 1
mkdir testDir/wikipedia-depth1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html testDir/wikipedia-depth1 1

# Run over wikipedia at depth 2
mkdir testDir/wikipedia-depth2
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html testDir/wikipedia-depth2 2

# end of tests
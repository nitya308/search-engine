#!/bin/bash
# 
# test.sh - perform crawler testing
#
# usage: test.sh
#
# input: none
# output: writes files as results from tests
#

# Checking with wrong arguments
# zero arguments                                                                    
./crawler

# one argument (seedURL)                                            
./crawler x

# two arguments (seedURL pageDirectory)
./crawler x y

# three arguments but URL is invalid                            
./crawler XYZ ../testDir 0

# URL is valid but not internal                          
./crawler https://canvas.dartmouth.edu/ ../testDir 0

# Directory does not exist                        
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../testDir 0

# creating testDir/test0 now
mkdir ../testDir
mkdir ../testDir/test0

# three arguments but maxDepth less than 0 (seedURL pageDirectory maxDepth)                                
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html ../testDir/test0 -10

# three arguments but maxDepth greater than 10 (seedURL pageDirectory maxDepth)   
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html ../testDir/test0 15

# Valid URL in valid directory
mkdir ../testDir/letters-3
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../testDir/letters-3 10

# Valid URL in valid directory to depth 0
mkdir ../testDir/letters
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/A.html ../testDir/letters 0

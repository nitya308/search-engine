# testing.sh - performs indexer testing
#
# usage: testing.sh
#
# input: none
# output: writes file with results from tests

# Checking with wrong arguments
# zero arguments                                                                    
./indexer

# one argument                                         
./indexer ~/cs50-dev/shared/tse/output

# three arguments
./indexer ~/cs50-dev/shared/tse/output ../indexOutput extraArgument

# invalid pageDirectory (non-existent path) 
./indexer ../doesNotExist ../indexOutput/1


# invalid pageDirectory (not a crawler directory)
mkdir ../notcrawler
./indexer ../notcrawler ../indexOutput/1


# invalid indexFile (non-existent path) 
./indexer ~/cs50-dev/shared/tse/output/letters-3 ../doesNotExist/file

# invalid indexFile (read-only directory) 
mkdir ../readOnly
chmod 0444 ../readOnly
./indexer ~/cs50-dev/shared/tse/output/letters-3 ../readOnly

# invalid indexFile (existing, read-only file)
touch readOnlyFile.txt
chmod 0444 readOnlyFile.txt
./indexer ~/cs50-dev/shared/tse/output/letters-3 readOnlyFile.txt

# we run indexer on directories from the cs50-dev/shared/tse/output
mkdir ../indexOutput

# **** running indexer on letters 1 ****
./indexer ~/cs50-dev/shared/tse/output/letters-1 ../indexOutput/letters1index
# running indextest on the letters 1 index
./indextest ../indexOutput/letters1index ../indexOutput/letters1indexnew
# comparing old and new files produced by indextest
~/cs50-dev/shared/tse/indexcmp ../indexOutput/letters1index ../indexOutput/letters1indexnew
# comparing index produced by indexer to index given in shared dir
~/cs50-dev/shared/tse/indexcmp ../indexOutput/letters1index ~/cs50-dev/shared/tse/output/letters-1.index

# **** running indexer on letters 3 ****
./indexer ~/cs50-dev/shared/tse/output/letters-3 ../indexOutput/letters3index
# running indextest on the letters 3 index
./indextest ../indexOutput/letters3index ../indexOutput/letters3indexnew
# comparing old and new files produced by indextest
~/cs50-dev/shared/tse/indexcmp ../indexOutput/letters3index ../indexOutput/letters3indexnew
# comparing index produced by indexer to index given in shared dir
~/cs50-dev/shared/tse/indexcmp ../indexOutput/letters3index ~/cs50-dev/shared/tse/output/letters-3.index

# **** running indexer on letters 10 ****
./indexer ~/cs50-dev/shared/tse/output/letters-10 ../indexOutput/letters10index
# running indextest on the letters 10 index
./indextest ../indexOutput/letters10index ../indexOutput/letters10indexnew
# comparing old and new files produced by indextest
~/cs50-dev/shared/tse/indexcmp ../indexOutput/letters10index ../indexOutput/letters10indexnew
# comparing index produced by indexer to index given in shared dir
~/cs50-dev/shared/tse/indexcmp ../indexOutput/letters10index ~/cs50-dev/shared/tse/output/letters-10.index

# **** running indexer on toscrape 1 ****
./indexer ~/cs50-dev/shared/tse/output/toscrape-1 ../indexOutput/toscrape1index
# running indextest on the toscrape 1 index
./indextest ../indexOutput/toscrape1index ../indexOutput/toscrape1indexnew
# comparing old and new files produced by indextest
~/cs50-dev/shared/tse/indexcmp ../indexOutput/toscrape1index ../indexOutput/toscrape1indexnew
# comparing index produced by indexer to index given in shared dir
~/cs50-dev/shared/tse/indexcmp ../indexOutput/toscrape1index ~/cs50-dev/shared/tse/output/toscrape-1.index

# **** running indexer on toscrape 2 ****
./indexer ~/cs50-dev/shared/tse/output/toscrape-2 ../indexOutput/toscrape2index
# running indextest on the toscrape 2 index
./indextest ../indexOutput/toscrape2index ../indexOutput/toscrape2indexnew
# comparing old and new files produced by indextest
~/cs50-dev/shared/tse/indexcmp ../indexOutput/toscrape2index ../indexOutput/toscrape2indexnew
# comparing index produced by indexer to index given in shared dir
~/cs50-dev/shared/tse/indexcmp ../indexOutput/toscrape2index ~/cs50-dev/shared/tse/output/toscrape-2.index

# **** running indexer on wikipedia 1 ****
./indexer ~/cs50-dev/shared/tse/output/wikipedia-1 ../indexOutput/wikipedia1index
# running indextest on the wikipedia 1 index
./indextest ../indexOutput/wikipedia1index ../indexOutput/wikipedia1indexnew
# comparing old and new files produced by indextest
~/cs50-dev/shared/tse/indexcmp ../indexOutput/wikipedia1index ../indexOutput/wikipedia1indexnew
# comparing index produced by indexer to index given in shared dir
~/cs50-dev/shared/tse/indexcmp ../indexOutput/wikipedia1index ~/cs50-dev/shared/tse/output/wikipedia-1.index

# **** running indexer on wikipedia 2 ****
./indexer ~/cs50-dev/shared/tse/output/wikipedia-2 ../indexOutput/wikipedia2index
# running indextest on the wikipedia 2 index
./indextest ../indexOutput/wikipedia2index ../indexOutput/wikipedia2indexnew
# comparing old and new files produced by indextest
~/cs50-dev/shared/tse/indexcmp ../indexOutput/wikipedia2index ../indexOutput/wikipedia2indexnew
# comparing index produced by indexer to index given in shared dir
~/cs50-dev/shared/tse/indexcmp ../indexOutput/wikipedia2index ~/cs50-dev/shared/tse/output/wikipedia-2.index

# **** running indexer on wikipedia 3  ****
./indexer ~/cs50-dev/shared/tse/output/wikipedia-3 ../indexOutput/wikipedia3index
# running indextest on the wikipedia 3 index
./indextest ../indexOutput/wikipedia3index ../indexOutput/wikipedia3indexnew
# comparing old and new files produced by indextest
~/cs50-dev/shared/tse/indexcmp ../indexOutput/wikipedia3index ../indexOutput/wikipedia3indexnew
# comparing index produced by indexer to index given in shared dir
~/cs50-dev/shared/tse/indexcmp ../indexOutput/wikipedia3index ~/cs50-dev/shared/tse/output/wikipedia-3.index

# running valgrind on both indexer and indextest
valgrind --leak-check=full --show-leak-kinds=all ./indexer ~/cs50-dev/shared/tse/output/wikipedia-1 ../indexOutput/wikipedia1index

valgrind --leak-check=full --show-leak-kinds=all ./indextest ../indexOutput/wikipedia1index ../indexOutput/wikipedia1indexnew

# end of testing


# valgrind.sh - valgrind for indexer file
#
# usage: myvalgrind ./indexer pageDirectory indexFilename
#
# input: none
# output: writes the valgrind results from tests

myvalgrind='valgrind --leak-check=full --show-leak-kinds=all'
DIR=~/cs50-dev/shared/tse/output/wikipedia-3
INDEXFILE=../indexOutput/wikipedia3index

$myvalgrind ./indexer $DIR $INDEXFILE
rm -rf $INDEXFILE

# valgrind.sh - valgrind for crawler file
#
# usage: myvalgrind ./crawler seedURL pageDirectory maxDepth
#
# input: none
# output: writes the valgrind results from tests

myvalgrind='valgrind --leak-check=full --show-leak-kinds=all'
DATA=../testDir/valgrind
SEED=http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html
DEPTH=1

mkdir -p $DATA
$myvalgrind ./crawler $SEED $DATA $DEPTH
rm -rf $DATA

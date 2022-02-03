myvalgrind='valgrind --leak-check=full --show-leak-kinds=all'
DATA=../data/valgrind
SEED=http://cs50tse.cs.dartmouth.edu/tse/letters/index.html
DEPTH=6

mkdir -p $DATA
$myvalgrind ./crawler $SEED $DATA $DEPTH
rm -rf $DATA

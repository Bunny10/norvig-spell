Implementations of [Norvig's spelling corrector](http://norvig.com/spell-correct.html) in various languages.

Author: [Jyotirmoy Bhattacharya](http://www.jyotirmoy.net), `jyotirmoy@jyotirmoy.net`

## Languages
* [Python](https://github.com/jmoy/norvig-spell/blob/master/python2/norvig.py) (minor modification of Norvig's original program)
* [C++14](https://github.com/jmoy/norvig-spell/blob/master/cxx1y/norvig.cc)
* [Haskell](https://github.com/jmoy/norvig-spell/blob/master/haskell/norvig.hs)

## Building
Running `make` at the top level will build all the programs and place them in `bin/`.

Each program can be run as

    norvig_xx [training data set]

where `[training data set]` is a plain text file from which the program will learn word frequencies. The programs expect to be given one word per line on standard input and print

    word, correction

pairs on standard output.

The folder `data/` has a training file `train.txt` and a test file `test.txt`. The first is exactly Norvig's `big.txt`. The second is Norvig's test set but with multiword strings removed.

## Performance

On Linux running `make benchmark` creates a file `benchmarks/all.md` containing performance results.


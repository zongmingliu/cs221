#include "ordered_array.h"
#include <cctype> // provides isalpha() and tolower()
#include <fstream>
#include <iostream>
#include <sstream>
#include <string> // provides string class

using namespace std;

// The following function should be moved into your skiplist.cc file when you
// write that.  It implements the random bit generation.
#include <cstdlib> // for rand(), srand(), and RAND_MAX
#include <ctime>   // for time()

int randBit(void) { // return a "random" bit
  static int bitsUpperBd = 0;
  static int bits;        // store bits returned by rand()
  if (bitsUpperBd == 0) { // refresh store when empty
    bitsUpperBd = RAND_MAX;
    bits = rand();
  }
  int b = bits & 1;
  bits >>= 1;
  bitsUpperBd >>= 1;
  return b;
}

// Remove all characters except letters (A-Z,a-z) from line,
// except keep '-' or '\'' if they are between letters.
void lowercaseWords(string &line) {
  for (string::iterator it = line.begin(); it != line.end(); ++it) {
    if (!isalpha(*it)) {
      if ((*it != '-' && *it != '\'') || it == line.begin() ||
          it + 1 == line.end() || !isalpha(*(it - 1)) || !isalpha(*(it + 1))) {
        *it = ' ';
      }
    } else {
      *it = tolower(*it);
    }
  }
}

void print(ostream &output, string &word) { output << word << endl; }

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " book.txt index.txt" << endl;
    exit(1);
  }

  ifstream fin;
  fin.open(argv[1]);
  if (!fin.is_open()) {
    cerr << "Unable to open " << argv[1] << endl;
    exit(2);
  }

  ofstream fout;
  fout.open(argv[2]);
  if (!fout.is_open()) {
    cerr << "Unable to open " << argv[2] << endl;
    exit(3);
  }

  string line, word;
  int pagen, linen;
  linen = 0;
  Entry entry;
  vector<Entry> result;
  while (!fin.eof()) {
    getline(fin, line);
    linen = linen + 1;
    if ((linen % 40) == 0) {
      pagen = linen / 40;
    } else {
      pagen = (linen / 40) + 1;
    }
    lowercaseWords(line);
    istringstream iss(line, istringstream::in);
    while (iss >> word) {
      insert(word, pagen, result);
    }
  }
  printInOrder(fout, result);
}

#include <iostream>
#include "lex/lex.h"
using namespace std;

int main () {
//  string re = "a(b|c)*";
//  string re = "fee|fie";
  string re = "";
  RETree tree(re);
  tree.print();
  RE2NFA nfa(tree.head);
  nfa.print();
  NFA2DFA dfa(nfa);
  dfa.print();
  minDFA min(dfa);
  min.print();
  return 0;
}
#include "lex/lex.hpp"
#include "utils/utils.hpp"
using namespace std;

int main () {
//  string re = "a(b|c)*";
//  string re = "fee|fie";
//  string re = "[a-c]+";
  printNow();
  string re = "[a-zA-Z_][a-zA-Z_0-9]*";
  RETree tree(re);
  printNow();
  tree.print();
  RE2NFA nfa(tree.head);
  nfa.print();
  NFA2DFA dfa(nfa);
  dfa.print();
  minDFA min(dfa);
  min.print();
  return 0;
}
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <nlohmann/json.hpp>
#include "lex/lex.hpp"
#include "utils/utils.hpp"
using namespace std;
using namespace nlohmann;

int main () {

//  Lex lex("../../spec/01-lex.json", "../../spec/source.01");
  Lex lex("../../spec/pl0-lex.json", "../../spec/source.pl0");
  printNow();
  unsigned long long a = getNow();
  Token* token = lex.nextToken();
  while (token != nullptr) {
    cout << token->typeDesc << ": " << token->lex << endl;
    token = lex.nextToken();
  }
  cout << "耗时:" << getNow() - a << endl;
  printNow();
//  string re = "a(b|c)*";
//  string re = "fee|fie";
//  string re = "[a-c]+";
//  printNow();
//  string re = "[a-zA-Z_][a-zA-Z_0-9]*";
//  RETree tree(re);
//  printNow();
//  tree.print();
//  RE2NFA nfa(tree.head);
//  nfa.print();
//  NFA2DFA dfa(nfa);
//  dfa.print();
//  minDFA min(dfa);
//  min.print();
  return 0;
}
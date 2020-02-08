#include <iostream>
#include "lex/lex.hpp"
#include "utils/utils.hpp"
#include "spec/lex-spec.hpp"
#include "spec/syntax-spec.hpp"
using namespace std;

int main () {
  Lex lex(PL0_LEX_SPEC, "../spec/test1.pl0");
  LR1(PL0_CFG, NonterminalSymbolType::Program, lex);
  printNow();
  unsigned long long a = getNow();
  Token* token = lex.nextToken();
  while (token != nullptr) {
    cout << token->typeDesc << ": " << token->lex << endl;
    token = lex.nextToken();
  }
  cout << "耗时:" << getNow() - a << endl;
  printNow();
  return 0;
}
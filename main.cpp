#include <iostream>
#include "lex/lex.hpp"
#include "utils/utils.hpp"
#include "spec/parentheses.hpp"
#include "spec/syntax-spec.hpp"
using namespace std;

using My_Lex = WrapLex<TerminalSymbolType, Token, Lex>;
using My_LR1 = LR1<My_Lex>;

int main () {
  My_Lex lex(parentheses_lex, "../spec/test1.pl0");
  My_LR1 lr1(parentheses_cfg, NonterminalSymbolType::Goal, lex);
  printNow();
  unsigned long long a = getNow();
  Token* token = lex.nextToken();
  while (token != nullptr) {
    cout << token->getDesc() << ": " << token->lex << endl;
    token = lex.nextToken();
  }
  cout << "耗时:" << getNow() - a << endl;
  printNow();
  return 0;
}
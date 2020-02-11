#include <iostream>
#include "lex/lex.hpp"
#include "utils/utils.hpp"
#include "spec/pl0.hpp"
#include "spec/syntax-spec.hpp"
using namespace std;

using My_Lex = WrapLex<TerminalSymbolType, Symbol, Lex>;
using My_LR1 = LR1<My_Lex>;

int main () {
  My_Lex lex(PL0_LEX, "../spec/test1.pl0");
  My_LR1 lr1(PL0_CFG, NonterminalSymbolType::Program, lex);
  Node ast = lr1.getAST();
  printNow();
//  unsigned long long a = getNow();
//  Symbol token = lex.nextToken();
//  while (!token.isEOF()) {
//    cout << token.getDesc() << ": " << token->lex << endl;
//    token = lex.nextToken();
//  }
//  cout << "耗时:" << getNow() - a << endl;
  printNow();
  return 0;
}
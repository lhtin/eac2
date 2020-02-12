#include <iostream>
#include "utils/utils.hpp"
//#include "spec/().hpp"
#include "spec/pl0.hpp"
#include "lex/lex.hpp"
#include "syntax/LR1.hpp"
using namespace std;

using My_Lex = WrapLex<Symbol>;
using My_LR1 = LR1<Symbol, My_Lex>;

int main () {
  My_Lex lex(PL0_LEX, "../spec/test1.pl0");
  My_LR1 lr1(PL0_CFG, NonterminalSymbolType::Program, lex);
  My_LR1::AST ast = lr1.getAST();
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
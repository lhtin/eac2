#include <cassert>
#include <string>
#include "utils/utils.hpp"
//#include "spec/().hpp"
#include "lex/lex.hpp"
#include "syntax/LR1.hpp"
#include "spec/pl0.hpp"

using namespace std;

using My_Lex = WrapLex<Symbol>;
using My_LR1 = LR1<Symbol, My_Lex>;

int main (int argc, char *argv[]) {
  assert(argc >= 2);

  printNow();
  My_Lex lex(PL0_LEX, string(argv[1]));
  My_LR1 lr1(PL0_CFG, NonterminalSymbolType::Program, lex);
  printNow();

  My_LR1::AST ast = lr1.getAST();
  cout << ast.toString() << endl;
  return 0;
}
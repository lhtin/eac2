#include <iostream>
#include "utils.hpp"
#include "Lex.hpp"
#include "LR1.hpp"

#include "().hpp"
#include "compiler.hpp"

using namespace std;

using My_Lex = Lex<Symbol>; // 生成PL/0词法分析器的class
using My_LR1 = LR1<Symbol>; // 生成PL/0 LR(1)语法分析器的class
using Compiler = compiler<My_Lex, My_LR1>; // 生成编译器class

int main () {
  My_Lex lex(LEX);
  My_LR1 lr1(CFG, START_SYMBOL);
  Compiler c = Compiler(lex, lr1);
  printNow();
  Compiler::Output o = c.parser("test1.()");
  printNow();
  cout << "Tokens:\n" << o.tokens << endl;
  cout << "AST:\n" << o.ast << endl; // 输出抽象语法树
  return 0;
}

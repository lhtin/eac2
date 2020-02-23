#include <cassert>
#include <string>

#include "utils.hpp"
#include "lex.hpp"
#include "LR1.hpp"

#include "pl0.hpp"

using namespace std;

using My_Lex = WrapLex<Symbol>; // 生成PL/0词法分析器的class
using My_LR1 = LR1<Symbol, My_Lex>; // 生成PL/0 LR(1)语法分析器的class

int main (int argc, char *argv[]) {
  string sourceFile = "test1.pl0";
  printNow();
  My_Lex lex(LEX, sourceFile); // 第二个参数为需要编译的源码文件路径
//  cout << lex.getAllToken();
  My_LR1 lr1(CFG, NonterminalSymbolType::Program, lex);
  printNow();

  My_LR1::AST ast = lr1.getAST();
  cout << "AST:\n" << ast.toString() << endl; // 输出抽象语法树
  return 0;
}

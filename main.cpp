#include <cassert>
#include <string>

#include "utils/utils.hpp"
#include "lex/lex.hpp"
#include "syntax/LR1.hpp"

#include "spec/pl0.hpp" // 解析PL/0语言
//#include "spec/().hpp"  // 解析括号语言

using namespace std;

using My_Lex = WrapLex<Symbol>; // 生成PL/0词法分析器的class
using My_LR1 = LR1<Symbol, My_Lex>; // 生成PL/0 LR(1)语法分析器的class

int main (int argc, char *argv[]) {
  assert(argc >= 2);

  printNow();
  My_Lex lex(PL0_LEX, string(argv[1])); // 第二个参数为需要编译的源码文件路径
  My_LR1 lr1(PL0_CFG, NonterminalSymbolType::Program, lex);
  printNow();

  My_LR1::AST ast = lr1.getAST();
  cout << ast.toString() << endl; // 输出抽象语法树
  return 0;
}

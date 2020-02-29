#include <iostream>
#include <string>
#include <filesystem>
#include "Lex.hpp"
#include "LR1.hpp"
#include "parser.hpp"
#include "test/pl0-spec.hpp"
#include "utils.hpp"

using namespace std;

using My_Lex = Lex<Symbol>; // 生成PL/0词法分析器的class
using My_LR1 = LR1<Symbol>; // 生成PL/0 LR(1)语法分析器的class
using Parser = parser<Symbol, My_Lex, My_LR1>; // 生成编译器class

int main (int argc, char* args[]) {
  filesystem::path currentPath = filesystem::current_path();
  currentPath.append(argc >= 2 ? string(args[1]) : "");
  string p = currentPath.lexically_normal().string();

  checkTailPath(p, "/EaC2/");

  My_Lex lex(LEX);
  My_LR1 lr1(CFG, START_SYMBOL);
  Parser c = Parser(lex, lr1);

  Parser::Output o = c.getOutput(p + "test/pl0/code2.pl0");

  cout << "Tokens:" << o.tokens << endl;
  cout << "AST:" << o.ast << endl;

  return 0;
}
#include <iostream>
#include <filesystem>
#include <string>
#include "Lex.hpp"
#include "LR1.hpp"
#include "parser.hpp"
#include "pl0-spec.hpp"
#include "utils.hpp"

using namespace std;

using My_Lex = Lex<Symbol>; // 生成PL/0词法分析器的class
using My_LR1 = LR1<Symbol>; // 生成PL/0 LR(1)语法分析器的class
using Parser = parser<Symbol, My_Lex, My_LR1>; // 生成编译器class

void test_parser (const filesystem::path& path) {
  My_Lex lex(LEX);
  My_LR1 lr1(CFG, START_SYMBOL);
  Parser c = Parser(lex, lr1);

  {
    vector<pair<string, string>> list = {
        {joinPath(path, "pl0/code1.pl0"), joinPath(path, "pl0/tokens1.txt")},
        {joinPath(path, "pl0/code2.pl0"), joinPath(path, "pl0/tokens2.txt")},
    };

    cout << endl;
    int total = 0;
    int success = 0;
    for (int i = 0; i < list.size(); i += 1) {
      total += 1;
      const pair<string, string> &item = list[i];
      Parser::Output o = c.getTokens(item.first);
      ifstream expected = ifstream(item.second);
      assert_with_msg(expected.is_open(), "无法打开源代码文件");
      if (isSame(expected, o.tokens)) {
        success += 1;
        cout << item.first << " => passed" << endl;
      } else {
        cout << item.first << i << " => failed" << endl;
        cout << "expected: " << item.second << endl;
        cout << "got: " << o.tokens << endl;
      }
    }

    cout << "Tokens test summary: total(" << total << ") success(" << success << ") fail(" << total - success << ")"
         << endl;
  }

  {
    vector<pair<string, string>> list = {
        {joinPath(path, "pl0/code1.pl0"), joinPath(path, "pl0/ast1.txt")},
        {joinPath(path, "pl0/code2.pl0"), joinPath(path, "pl0/ast2.txt")},
    };

    cout << endl;
    int total = 0;
    int success = 0;
    for (int i = 0; i < list.size(); i += 1) {
      total += 1;
      const pair<string, string> &item = list[i];
      Parser::Output o = c.getOutput(item.first);
      ifstream expected = ifstream(item.second);
      assert_with_msg(expected.is_open(), "无法打开源代码文件");
      if (isSame(expected, o.ast)) {
        success += 1;
        cout << item.first << " => passed" << endl;
      } else {
        cout << item.first << i << " => failed" << endl;
        cout << "expected: " << item.second << endl;
        cout << "got: " << o.ast << endl;
      }
    }

    cout << "AST test summary: total(" << total << ") success(" << success << ") fail(" << total - success << ")"
         << endl;
  }
}
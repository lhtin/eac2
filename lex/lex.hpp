//
// Created by 丁乐华 on 2020/1/24.
//

#ifndef EAC2_LEX_HPP
#define EAC2_LEX_HPP
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <unordered_map>
#include <fstream>
#include <utility>
#include <cassert>
#include "all.hpp"
#include "../spec/spec.hpp"

using namespace std;

const int BUFFER_MAX = 1024;

template <typename Token>
class WrapLex {
private:
  using TokenType = typename Token::_TerminalSymbolType;
  using Lex = Spec::Lex<TokenType>;
  vector<pair<WrapFA, TokenType>> list;
  ifstream sourceFile;
  string buf;
  int bufAt;

public:
  WrapLex (Lex lexSpec, string sourcePath): bufAt(0), sourceFile(sourcePath) {
    if (sourceFile.is_open()) {
      for (auto it = lexSpec.begin(); it != lexSpec.end(); it++) {
        string re = it->first;
        TokenType type = it->second;
        addRE(re, type);
      }
    } else {
      // 无法打开需要解析的源代码文件
      assert(false);
    }
  }

  ~WrapLex () {
    sourceFile.close();
  }

  void addRE (string& re, TokenType type) {
//    cout << "RE: " << escape(re) << endl;
    RETree tree(re);
//    tree.print();
    NFA nfa(tree);
//    nfa.print();
    DFA dfa(nfa);
//    dfa.print();
    minDFA* min = new minDFA(dfa);
//    min->print();
    list.push_back(pair<WrapFA, TokenType>(WrapFA(min), type));
  }

  char nextChar () {
    if (!buf.empty() && bufAt < buf.length()) {
      char c = buf[bufAt];
      bufAt += 1;
      return c;
    } else {
      char c = sourceFile.get();
      buf.push_back(c);
      bufAt += 1;
      return c;
    }
  }
  /**
   * 回退n个字符
   * @param n
   */
  void goBack (int n) {
    assert(bufAt - n >= 0);
    bufAt -= n;
  }

  Token nextToken () {
    string lex;
    TokenType type = TokenType::none;
    for (auto item : list) {
      WrapFA& wfa = item.first;
      string temp;
      char c = nextChar();
      if (c == char_traits<char>::eof()) {
        type = TokenType::eof;
        break;
      }
      while (wfa.accept(c)) {
        temp += c;
        c = nextChar();
      }
      goBack(1);
      bool isFinish = wfa.isFinish();
      if (!isFinish && wfa.getPrevFinishedDelta() > 0) {
        int backLen = wfa.getPrevFinishedDelta();
        temp = temp.substr(0, temp.size() - backLen);
        goBack(backLen);
        isFinish = true;
      }
      wfa.reset();
      if (isFinish) {
        if (temp.length() <= lex.length()) {
          // 当前找到的token比前面找到的短，所以丢弃
          continue;
        } else {
          // 当前找到的token比前面找到的长，使用新的
          type = item.second;
          lex = temp;
        }
      } else {
        if (temp.length() > lex.length()) {
          // 当前没有找到token且消耗的字符比前一个token长，则回退
          goBack(temp.length() - lex.length());
        }
      }
      bufAt = 0;
    }

    // 为none说明没有找到匹配的token，源码有误
    assert(type != TokenType::none);

    buf = buf.substr(lex.length(), buf.length() - lex.length());
    bufAt = 0;

    if (type == TokenType::space) {
      return nextToken();
    }
    return Token(type, lex);
  }

  string getAllToken () {
    string res;
    Token t = nextToken();
    while (!t.is(TokenType::eof)) {
      res += t.getDesc() + "\n";
      t = nextToken();
    }
    return res;
  }
};

#endif //EAC2_WRAPLEX_HPP
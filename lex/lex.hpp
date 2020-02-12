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
  char buf[BUFFER_MAX * 2];
  int at;
  ifstream sourceFile;

public:
  WrapLex (Lex lexSpec, string sourcePath): at(0), buf(), sourceFile(sourcePath) {
    if (sourceFile.is_open()) {
      for (auto it = lexSpec.begin(); it != lexSpec.end(); it++) {
        string re = it->first;
        TokenType type = it->second;
        addRE(re, type);
      }
      sourceFile.read(buf, 2 * BUFFER_MAX);
    } else {
      cout << "cannot open" << endl;
    }
  }

  ~WrapLex () {
    sourceFile.close();
  }

  void addRE (string& re, TokenType type) {
    RETree tree(re);
    tree.print();
    RE2NFA nfa(tree.head);
    nfa.print();
    NFA2DFA dfa(nfa);
    dfa.print();
    minDFA* min = new minDFA(dfa);
    min->print();
    list.push_back(pair<WrapFA, TokenType>(WrapFA(min), type));
  }

  Token nextToken () {
    for (auto item : list) {
      WrapFA& wfa = item.first;
      string lex;
      int start = at;
      char c = buf[at];
      int len = 0;
      if (c == EMPTY) {
        return Token(TokenType::eof);
      }
      while (c != EMPTY && wfa.accept(c)) {
        lex += c;
        len += 1;
        if (len > BUFFER_MAX) {
          cout << "token 过长" << endl;
          break;
        }
        at = (at + 1) % (2 * BUFFER_MAX);
        c = buf[at];
      }
      bool isFinish = wfa.isFinish();
      wfa.reset();
      if (len > BUFFER_MAX) {
        at = start;
      } else {
        if (isFinish) {
          if (at < BUFFER_MAX && len > at) {
            sourceFile.read(buf + BUFFER_MAX, BUFFER_MAX);
            int count = sourceFile.gcount();
            if (count < BUFFER_MAX) {
              buf[BUFFER_MAX + count] = EMPTY;
            }
          } else if (at >= BUFFER_MAX && at - len < BUFFER_MAX) {
            sourceFile.read(buf, BUFFER_MAX);
            int count = sourceFile.gcount();
            if (count < BUFFER_MAX) {
              buf[count] = EMPTY;
            }
          }
          if (item.second == TokenType::space) {
            return nextToken();
          }
          return Token(item.second, lex);
        } else if (c == EMPTY) {
          cout << "源码结束但是解析未完成" << endl;
          break;
        } else {
          at = start;
        }
      }
    }
    return Token(TokenType::eof);
  }
};

#endif //EAC2_WRAPLEX_HPP
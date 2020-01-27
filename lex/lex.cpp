#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>
#include "lex.hpp"

using namespace std;
using namespace nlohmann;

WrapFA::WrapFA (FA* fa): fa(fa), current(fa->s0) {}
bool WrapFA::accept (char c) {
  auto it = find_if(
      fa->deltas.begin(), fa->deltas.end(),
      [&](FA::Delta* item) {
        return item->start == current && item->accept.count(c) > 0;
      });
  if (it != fa->deltas.end()) {
    current = (*it)->end;
    return true;
  } else {
    return false;
  }
}
bool WrapFA::isFinish () {
  auto it = find_if(
      fa->SA.begin(), fa->SA.end(),
      [&](FA::State* item) {
        return item == current;
      });
  if (it != fa->SA.end()) {
    return true;
  } else {
    return false;
  }
}
void WrapFA::reset () {
  current = fa->s0;
}
Token::Token (TokenType type, string lex): type(type), lex(lex) {
  if (type == VARIABLE) {
    typeDesc = "VARIABLE";
  } else if (type == KEYWORD) {
    typeDesc = "KEYWORD";
  } else if (type == SPACE) {
    typeDesc = "SPACE";
  } else if (type == INTEGER) {
    typeDesc = "INTEGER";
  } else if (type == STRING) {
    typeDesc = "STRING";
  } else {
    typeDesc = "UNKNOW";
  }
}

Lex::Lex (string specPath, string sourcePath): at(0), buf(), sourceFile(sourcePath) {
  ifstream spec(specPath, ios::binary);
  if (spec.is_open() && sourceFile.is_open()) {
    json lexSpec;
    spec >> lexSpec;

    for (auto it = lexSpec.begin(); it != lexSpec.end(); it++) {
      string re = (*it)["re"];
      string category = (*it)["category"];
      addRE(re, category);
    }
    sourceFile.read(buf, 2 * BUFFER_MAX);
    spec.close();
  } else {
    cout << "cannot open" << endl;
  }
}
Lex::~Lex () {
  sourceFile.close();
}
TokenType getToken (string& category) {
  if (category == "VARIABLE") {
    return VARIABLE;
  } else if (category == "KEYWORD") {
    return KEYWORD;
  } else if (category == "SPACE") {
    return SPACE;
  } else if (category == "INTEGER") {
    return INTEGER;
  } else if (category == "STRING") {
    return STRING;
  } else {
    return UNKNOW;
  }
}
void Lex::addRE (string& re, string& category) {
  RETree tree(re);
//  tree.print();
  RE2NFA nfa(tree.head);
//  nfa.print();
  NFA2DFA dfa(nfa);
//  dfa.print();
  minDFA* min = new minDFA(dfa);
//  min->print();
  list.push_back(pair<WrapFA, TokenType>(WrapFA(min), getToken(category)));
}
Token* Lex::nextToken () {
  for (auto item : list) {
    WrapFA& wfa = item.first;
    string lex;
    int start = at;
    char c = buf[at];
    int len = 0;
    if (c == EMPTY) {
      break;
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
        if (item.second == SPACE) {
          return nextToken();
        }
        return new Token(item.second, lex);
      } else if (c == EMPTY) {
        cout << "源码结束但是解析未完成" << endl;
        break;
      } else {
        at = start;
      }
    }
  }
  return nullptr;
}

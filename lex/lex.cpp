#include <string>
#include <utility>
#include <iostream>
#include <algorithm>
#include "lex.hpp"
#include "../spec/lex-spec.hpp"

using namespace std;

WrapFA::WrapFA (FA* fa): fa(fa), current(fa->s0), current2(fa->s0->n), deltas(fa->getDeltas()) {}
bool WrapFA::accept2 (char c) {
  auto it1 = deltas.find(current2);
  if (it1 != deltas.end()) {
    auto it2 = it1->second.find(c);
    if (it2 != it1->second.end()) {
      current2 = it2->second;
      return true;
    }
  }
  return false;
}
bool WrapFA::isFinish2 () {
  auto it = find_if(
      fa->SA.begin(), fa->SA.end(),
      [&](FA::State* item) {
        return item->n == current2;
      });
  if (it != fa->SA.end()) {
    return true;
  } else {
    return false;
  }
}
void WrapFA::reset2 () {
  current2 = fa->s0->n;
}
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

Lex::Lex (LexSpec lexSpec, string sourcePath): at(0), buf(), sourceFile(sourcePath) {
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
Lex::~Lex () {
  sourceFile.close();
}

void Lex::addRE (string& re, TokenType type) {
  RETree tree(re);
//  tree.print();
  RE2NFA nfa(tree.head);
//  nfa.print();
  NFA2DFA dfa(nfa);
//  dfa.print();
  minDFA* min = new minDFA(dfa);
//  min->print();
  list.push_back(pair<WrapFA, TokenType>(WrapFA(min), type));
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
        if (item.second == TokenType::space) {
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

//
// Created by 丁乐华 on 2020/1/24.
//

#ifndef EAC2_LEX_HPP
#define EAC2_LEX_HPP
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
#include "spec.hpp"

using namespace std;

const int BUFFER_MAX = 1024;

template <typename Token>
class Lex {
private:
  using TokenType = typename Token::_TerminalSymbolType;
  using LexSpec = Spec::Lex<TokenType>;

public:
  vector<pair<WrapFA, TokenType>> list;

  Lex (LexSpec lexSpec) {
    for (auto it = lexSpec.begin(); it != lexSpec.end(); it++) {
      string re = it->first;
      TokenType type = it->second;
      addRE(re, type);
    }
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
};

#endif //EAC2_WRAPLEX_HPP
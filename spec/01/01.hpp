/** 括号语法

 */

#ifndef EAC2_01_HPP
#define EAC2_01_HPP

#include <map>
#include <string>
#include "../spec.hpp"

using namespace std;

using SymbolType = Spec::SymbolType;

enum class TerminalSymbolType {
  keyword,
  ident,
  space,
  eof,
  none
};

enum class NonterminalSymbolType {
  Goal,
  None
};

using Symbol = Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>;

template <>
map<TerminalSymbolType, string> Symbol::TerminalDesc = {
    {TerminalSymbolType::keyword, "keyword"},
    {TerminalSymbolType::ident, "ident"},
    {TerminalSymbolType::space, "space"},
    {TerminalSymbolType::eof, "eof"}
};

template <>
map<NonterminalSymbolType, string> Symbol::NonterminalDesc = {
    {NonterminalSymbolType::Goal, "Goal"}
};

template <>
Symbol Symbol::getPureSymbol() {
  if (
      this->type == SymbolType::TERMINAL_SYMBOL &&
      this->t_type == TerminalSymbolType::ident) {
    return Symbol(this->t_type);
  }
  return *this;
}

// lex
const Spec::Lex<Symbol::_TerminalSymbolType> LEX = {
    {
        "ab|abab",
        TerminalSymbolType::keyword
    },
    {
        "[a-z]+",
        TerminalSymbolType::ident
    }
};

// syntax
const Symbol START_SYMBOL = Symbol(NonterminalSymbolType::Goal);
const Spec::CFG<Symbol> CFG{
    {
        Symbol(NonterminalSymbolType::Goal),
        {
            {
                Symbol(TerminalSymbolType::keyword)
            },
            {
                Symbol(TerminalSymbolType::ident)
            }
        }
    }
};

#endif

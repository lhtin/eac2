#ifndef EAC2_PARENTHESES_HPP
#define EAC2_PARENTHESES_HPP

#include <vector>
#include <utility>
#include <string>
#include "spec.hpp"

using namespace std;

using SymbolType = Spec::SymbolType;

enum class TerminalSymbolType {
  keyword,
  space,
  eof,
  none
};

enum class NonterminalSymbolType {
  Goal,
  List,
  Pair,
  None
};

using Symbol = Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>;

template <>
map<TerminalSymbolType, string> Symbol::TerminalDesc = {
    {TerminalSymbolType::keyword, "keyword"},
    {TerminalSymbolType::space, "space"},
    {TerminalSymbolType::eof, "eof"}
};

template <>
map<NonterminalSymbolType, string> Symbol::NonterminalDesc = {
    {NonterminalSymbolType::Goal, "Goal"},
    {NonterminalSymbolType::List, "List"},
    {NonterminalSymbolType::Pair, "Pair"}
};

template <>
Symbol Symbol::getPureSymbol() {
  return *this;
}

// lex
using Lex = Spec::Lex<Symbol::_TerminalSymbolType>;
const Lex PARENTHESES_LEX = {
    {
        R"(\(|\))",
        TerminalSymbolType::keyword
    },
    {
        "[ \n\t]+",
        TerminalSymbolType::space
    }
};

// syntax
using CFG = Spec::CFG<Symbol>;
const CFG PARENTHESES_CFG{
    {
        Symbol(NonterminalSymbolType::Goal),
        {
            {
                Symbol(NonterminalSymbolType::List)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::List),
        {
            {
                Symbol(NonterminalSymbolType::List),
                Symbol(NonterminalSymbolType::Pair)
            },
            {
                Symbol(NonterminalSymbolType::Pair)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Pair),
        {
            {
                Symbol(TerminalSymbolType::keyword, "("),
                Symbol(NonterminalSymbolType::Pair),
                Symbol(TerminalSymbolType::keyword, ")")
            },
            {
                Symbol(TerminalSymbolType::keyword, "("),
                Symbol(TerminalSymbolType::keyword, ")")
            }
        }
    }
};

#endif

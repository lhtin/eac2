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

template <typename NonterminalSymbolType, typename TerminalSymbolType>
map<TerminalSymbolType, string> Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>::TerminalDesc = {
    {TerminalSymbolType::keyword, "keyword"},
    {TerminalSymbolType::space, "space"},
    {TerminalSymbolType::eof, "eof"}
};

template <typename NonterminalSymbolType, typename TerminalSymbolType>
map<NonterminalSymbolType, string> Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>::NonterminalDesc = {
    {NonterminalSymbolType::Goal, "Goal"},
    {NonterminalSymbolType::List, "List"},
    {NonterminalSymbolType::Pair, "Pair"}
};

using Symbol = Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>;

// lex
using Lex = Spec::Lex<TerminalSymbolType>;
const Lex parentheses_lex = {
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
using Production = Spec::Production<Symbol>;
using ProductionList = Spec::ProductionList<Symbol>;
using CFG = Spec::CFG<Symbol>;
const CFG parentheses_cfg{
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

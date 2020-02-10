#ifndef EAC2_PARENTHESES_HPP
#define EAC2_PARENTHESES_HPP

#include <vector>
#include <utility>
#include <string>
#include "spec.hpp"

using namespace std;

using SymbolType = Spec::SymbolType;

//// lex

enum class TerminalSymbolType {
  keyword,
  space,
  none
};

template class Spec::Token<TerminalSymbolType>;
using Token = Spec::Token<TerminalSymbolType>;

template <typename TerminalSymbolType>
map<TerminalSymbolType, string> Spec::Token<TerminalSymbolType>::desc = {
    {TerminalSymbolType::keyword, "keyword"}
};

using Lex = Spec::Lex<TerminalSymbolType>;

const Lex parentheses_lex = {
    {
        R"((|))",
        TerminalSymbolType::keyword
    },
    {
        "[ \n\t]+",
        TerminalSymbolType::space
    }
};

//// syntax

enum class NonterminalSymbolType {
  Goal,
  List,
  Pair,
  None
};
using Symbol = Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>;
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

#ifndef EAC2_SYNTAX_SPEC_H
#define EAC2_SYNTAX_SPEC_H

#include <vector>
#include <utility>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <iterator>
#include "lex-spec.hpp"
#include "spec.hpp"

using namespace std;

enum class NonterminalSymbolType {
  Program,
  Block,
  Assign,
  AssignRest,
  Declare,
  DeclareRest,
  Procedure,
  Statement,
  StatementRest,
  Condition,
  RelationOp,
  Expression,
  ExpressionRest,
  Op1,
  Term,
  TermRest,
  Op2,
  Factor,
  None
};

//template <SymbolType, NonterminalSymbolType, TokenType>
//class Symbol {
//public:
//  SymbolType type;
//  NonterminalSymbolType nt_type;
//  TokenType t_type;
//  string flex;
//
//  explicit Symbol(NonterminalSymbolType nt_type) : type(SymbolType::NON_TERMINAL_SYMBOL), nt_type(nt_type), t_type(TokenType::invalid), flex("") {}
//
//  explicit Symbol(TokenType t_type, string flex = "") : type(SymbolType::TERMINAL_SYMBOL), t_type(t_type), flex(flex), nt_type(NonterminalSymbolType::Invalid) {}
//
//  bool is (NonterminalSymbolType nt_type) {
//    return type == SymbolType::NON_TERMINAL_SYMBOL && this->nt_type == nt_type;
//  }
//  bool is (TokenType t_type, string flex = "") {
//    return type == SymbolType::TERMINAL_SYMBOL && this->t_type == t_type && this->flex == flex;
//  }
//  bool isNonterminalSymbol () const {
//    return type == SymbolType::NON_TERMINAL_SYMBOL;
//  }
//  bool operator< (const Symbol& rhs) const {
//    if (type < rhs.type) {
//      return true;
//    } else if (type == rhs.type) {
//      if (nt_type < rhs.nt_type) {
//        return true;
//      } else if (nt_type == rhs.nt_type) {
//        if (t_type < rhs.t_type) {
//          return true;
//        } else if (t_type == rhs.t_type) {
//          return flex < rhs.flex;
//        }
//      }
//    }
//    return false;
//  }
//};

using Symbol = Spec::Symbol<NonterminalSymbolType, TokenType>;
using Production = Spec::Production<Symbol>;
using ProductionList = Spec::ProductionList<Symbol>;
using CFG = Spec::CFG<Symbol>;

const CFG PL0_CFG{
    {
        Symbol(NonterminalSymbolType::Program),
        {
            {
                Symbol(NonterminalSymbolType::Block),
                Symbol(TokenType::keyword, ".")
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Block),
        {
            {
                Symbol(NonterminalSymbolType::Assign),
                Symbol(NonterminalSymbolType::Declare),
                Symbol(NonterminalSymbolType::Procedure),
                Symbol(NonterminalSymbolType::Statement)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Assign),
        {
            {
                Symbol(TokenType::keyword, "const"),
                Symbol(TokenType::ident),
                Symbol(TokenType::keyword, "="),
                Symbol(TokenType::number),
                Symbol(NonterminalSymbolType::AssignRest)
            },
            {
                Symbol(TokenType::epsilon)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::AssignRest),
        {
            {
                Symbol(TokenType::keyword, ","),
                Symbol(TokenType::ident),
                Symbol(TokenType::keyword, "="),
                Symbol(TokenType::number),
                Symbol(NonterminalSymbolType::AssignRest)
            },
            {
              Symbol(TokenType::epsilon)
            }
        }
    },
    {
      Symbol(NonterminalSymbolType::Declare),
        {
            {
              Symbol(TokenType::keyword, "var"),
              Symbol(TokenType::ident),
              Symbol(NonterminalSymbolType::DeclareRest),
              Symbol(TokenType::keyword, ";")
            },
            {
              Symbol(TokenType::epsilon)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::DeclareRest),
        {
            {
              Symbol(TokenType::keyword, ","),
              Symbol(TokenType::ident),
              Symbol(NonterminalSymbolType::DeclareRest)
            },
            {
              Symbol(TokenType::epsilon)
            }
        }
    },
    {
      Symbol(NonterminalSymbolType::Procedure),
          {
              {
                  Symbol(TokenType::keyword, "procedure"),
                  Symbol(TokenType::ident),
                  Symbol(TokenType::keyword, ";"),
                  Symbol(NonterminalSymbolType::Block),
                  Symbol(TokenType::keyword, ";"),
                  Symbol(NonterminalSymbolType::Procedure)
              },
              {
                  Symbol(TokenType::epsilon)
              }
          }
    },
    {
        Symbol(NonterminalSymbolType::Statement),
        {
            {
              Symbol(TokenType::ident),
                Symbol(TokenType::keyword, ":="),
                Symbol(NonterminalSymbolType::Expression)
            },
            {
                Symbol(TokenType::keyword, "call"),
                Symbol(TokenType::ident)
            },
            {
                Symbol(TokenType::keyword, "?"),
                Symbol(TokenType::ident)
            },
            {
                Symbol(TokenType::keyword, "!"),
                Symbol(NonterminalSymbolType::Expression)
            },
            {
                Symbol(TokenType::keyword, "begin"),
                Symbol(NonterminalSymbolType::Statement),
                Symbol(NonterminalSymbolType::StatementRest),
                Symbol(TokenType::keyword, "end")
            },
            {
                Symbol(TokenType::keyword, "if"),
                Symbol(NonterminalSymbolType::Condition),
                Symbol(TokenType::keyword, "then"),
                Symbol(NonterminalSymbolType::Statement)
            },
            {
                Symbol(TokenType::keyword, "while"),
                Symbol(NonterminalSymbolType::Condition),
                Symbol(TokenType::keyword, "do"),
                Symbol(NonterminalSymbolType::Statement)
            },
            {
                Symbol(TokenType::epsilon)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::StatementRest),
        {
            {
                Symbol(TokenType::keyword, ";"),
                Symbol(NonterminalSymbolType::Statement),
                Symbol(NonterminalSymbolType::StatementRest)
            },
            {
                Symbol(TokenType::epsilon)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Condition),
        {
            {
                Symbol(TokenType::keyword, "odd"),
                Symbol(NonterminalSymbolType::Expression)
            },
            {
                Symbol(NonterminalSymbolType::Expression),
                Symbol(NonterminalSymbolType::RelationOp),
                Symbol(NonterminalSymbolType::Expression),
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::RelationOp),
        {
            {
                Symbol(TokenType::keyword, "=")
            },
            {
                Symbol(TokenType::keyword, "#")
            },
            {
                Symbol(TokenType::keyword, "<")
            },
            {
                Symbol(TokenType::keyword, "<=")
            },
            {
                Symbol(TokenType::keyword, ">")
            },
            {
                Symbol(TokenType::keyword, ">=")
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Expression),
        {
            {
                Symbol(NonterminalSymbolType::Term),
                Symbol(NonterminalSymbolType::ExpressionRest)
            },
            {
                Symbol(NonterminalSymbolType::Op1),
                Symbol(NonterminalSymbolType::Term),
                Symbol(NonterminalSymbolType::ExpressionRest)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::ExpressionRest),
        {
            {
                Symbol(NonterminalSymbolType::Op1),
                Symbol(NonterminalSymbolType::Term),
                Symbol(NonterminalSymbolType::ExpressionRest)
            },
            {
                Symbol(TokenType::epsilon)
            }
        }
    },
    {
      Symbol(NonterminalSymbolType::Op1),
          {
              {
                  Symbol(TokenType::keyword, "+")
              },
              {
                  Symbol(TokenType::keyword, "-")
              }
          }
    },
    {
      Symbol(NonterminalSymbolType::Term),
          {
              {
                  Symbol(NonterminalSymbolType::Factor),
                  Symbol(NonterminalSymbolType::TermRest)
              }
          }
    },
    {
      Symbol(NonterminalSymbolType::TermRest),
          {
              {
                  Symbol(NonterminalSymbolType::Op2),
                  Symbol(NonterminalSymbolType::Factor),
                  Symbol(NonterminalSymbolType::TermRest)
              },
              {
                  Symbol(TokenType::epsilon)
              }
          }
    },
    {
      Symbol(NonterminalSymbolType::Op2),
          ProductionList {
              Production {
                  Symbol(TokenType::keyword, "*")
              },
              Production {
                  Symbol(TokenType::keyword, "/")
              }
          }
    },
    {
      Symbol(NonterminalSymbolType::Factor),
          {
              {
                  Symbol(TokenType::ident)
              },
              {
                  Symbol(TokenType::number)
              },
              {
                  Symbol(TokenType::keyword, "("),
                  Symbol(NonterminalSymbolType::Expression),
                  Symbol(TokenType::keyword, ")")
              }
          }
    }
};

using LR1Item = Spec::LR1Item<Symbol>;
using LR1CC = set<LR1Item>;
using SetSymbol = set<Symbol>;
using First = set<Symbol>;

class LR1 {
private:
  CFG cfg;
  map<Symbol, set<Symbol>> firstAll;
  Symbol epsilon;
public:
  LR1 (CFG cfg, NonterminalSymbolType start, Lex& lex): cfg(cfg), firstAll(), epsilon(TokenType::epsilon) {
    initAllFirst();

    Symbol goal(start);
    ProductionList list = cfg[goal];
    LR1CC cc0;
    SetSymbol s{goal};
    for (const Production& item : list) {
      cc0.insert(LR1Item{goal, item, 0, Symbol(TokenType::eof)});
    }
    closure(cc0, s);
  }
  void initAllFirst () {
    set<Symbol> es;
    es.insert(epsilon);

    for (auto& item : cfg) {
      firstAll[item.first] = set<Symbol>();
    }
    bool isChanged = true;
    while (isChanged) {
      isChanged = false;
      for (auto& item : cfg) {
        set<Symbol>& first = firstAll[item.first];
        for (Production& p : item.second) {
          set<Symbol> rhs;
          bool isAll = true;
          for (const Symbol& s : p) {
            set<Symbol> firstB;
            if (s.isNonterminalSymbol()) {
              firstB = firstAll[s];
            } else {
              firstB.insert(s);
            }
            set_difference(
                firstB.begin(), firstB.end(),
                es.begin(), es.end(),
                inserter(rhs, rhs.begin()));
            if (firstB.count(epsilon) == 0) {
              isAll = false;
              break;
            }
          }
          if (isAll) {
            rhs.insert(epsilon);
          }
          int size0 = first.size();
          first.insert(rhs.begin(), rhs.end());
          if (first.size() != size0) {
            isChanged = true;
          }
        }
      }
    }
  }
  Symbol getNextSymbol (const LR1Item& item) {
    const Production& p = item.p;
    int index = item.pos;
    if (index < p.size() - 1) {
      return p[index];
    } else {
      return Symbol(NonterminalSymbolType::None);
    }
  }
  First getFirst (Production& rest, Symbol& a) {
    First f;
    bool isAll = true;
    for (const Symbol& s : rest) {
      if (s.isNonterminalSymbol()) {
        First t = firstAll[s];
        if (t.find(epsilon) == t.end()) {

        }
      }
    }
  }
  void closure (LR1CC& c, SetSymbol& s) {
    int count;
    do {
      count = s.size();
      for (const LR1Item& item : c) {
        Symbol nextSymbol = getNextSymbol(item);
        if (
            nextSymbol.isNonterminalSymbol() &&
            !nextSymbol.is(NonterminalSymbolType::None) &&
            s.find(nextSymbol) == s.end()) {
          s.insert(nextSymbol);
          ProductionList ps = cfg[nextSymbol];
          LR1CC c2;
          for (const Production& p : ps) {
            c2.insert(LR1Item{nextSymbol, p, 0, })
          }
        }
      }

    } while (s.size() != count);
  }
//  CC goNext (CC& s, Symbol a) {
//
//  }

//  Production first (Production& s) {
//    set<Symbol> a;
//    int count;
//    do {
//      count = a.size();
//      for (Symbol& item : s) {
//        if (item.isNonterminalSymbol()) {
//          ProductionList pl = cfg[item];
//
//        } else {
//          if (item.is(TokenType::epsilon)) {
//            continue;
//          } else {
//            a.insert(item);
//            break;
//          }
//        }
//      }
//    } while (a.size() != count);
//  }
};

/*
A -> A a
  | e
  F(A) = {}
  F(a) = {a}
  F(e) = {e)}
  1
A -> A a
rhs = F(A) - {e} = {}
F(A) = {}
2
A -> e
rhs = F(e) - {e} = {}
rhs = rhs + {e}
F(A) = {e}
3
A -> A a
rhs = F(A) - {e} = {}
rhs = rhs + {a};
F(A) = {e, a}
*/

#endif
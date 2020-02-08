#ifndef EAC2_SPEC_HPP
#define EAC2_SPEC_HPP

#include <string>
#include <map>
#include <vector>

using namespace std;

namespace Spec {

  enum class SymbolType {
    NON_TERMINAL_SYMBOL,
    TERMINAL_SYMBOL
  };

  template<typename NonterminalSymbolType, typename TerminalSymbolType>
  class Symbol {
  public:
    SymbolType type;
    NonterminalSymbolType nt_type;
    TerminalSymbolType t_type;
    string lex;

    explicit Symbol(NonterminalSymbolType nt_type) :
        type(SymbolType::NON_TERMINAL_SYMBOL),
        nt_type(nt_type),
        t_type(),
        lex("") {}

    explicit Symbol(TerminalSymbolType t_type, string lex = "") :
        type(SymbolType::TERMINAL_SYMBOL),
        t_type(t_type),
        lex(lex),
        nt_type() {}

    bool is(NonterminalSymbolType nt_type1) {
      return type == SymbolType::NON_TERMINAL_SYMBOL && this->nt_type == nt_type1;
    }

    bool is(TerminalSymbolType t_type1, string lex1 = "") {
      return type == SymbolType::TERMINAL_SYMBOL && this->t_type == t_type1 && this->lex == lex1;
    }

    bool isNonterminalSymbol() const {
      return type == SymbolType::NON_TERMINAL_SYMBOL;
    }

    bool operator<(const Symbol &rhs) const {
      return type < rhs.type ||
             (type == rhs.type && nt_type < rhs.nt_type) ||
             (type == rhs.type && nt_type == rhs.nt_type && t_type < rhs.t_type) ||
             (type == rhs.type && nt_type == rhs.nt_type && t_type == rhs.t_type && lex < rhs.lex);
    }
  };

  template <typename Symbol>
  using Production = vector<Symbol>;

  template <typename Symbol>
  using ProductionList = vector<Production<Symbol>>;

  template <typename Symbol>
  using CFG = map<Symbol, ProductionList<Symbol>>;

  template<typename Symbol>
  class LR1Item {
  public:
    Symbol nt;
    Production<Symbol> p;
    int pos;
    Symbol t;

    LR1Item(Symbol nt, Production<Symbol> p, int pos, Symbol t) : nt(nt), p(p), pos(pos), t(t) {}

    bool operator<(const LR1Item &rhs) const {
      return nt < rhs.nt ||
             (nt == rhs.nt && p < rhs.p) ||
             (nt == rhs.nt && p == rhs.p && pos < rhs.pos) ||
             (nt == rhs.nt && p == rhs.p && pos == rhs.pos && t < rhs.t);
    }
  };

}

#endif

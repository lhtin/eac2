#ifndef EAC2_SPEC_HPP
#define EAC2_SPEC_HPP

#include <string>
#include <map>
#include <vector>

using namespace std;

namespace Spec {

  template <typename TerminalSymbolType>
  using Lex = vector<pair<string, TerminalSymbolType>>;

  template <typename TerminalSymbolType>
  class Token {
  public:
    static map<TerminalSymbolType, string> desc;
    string lex;
    TerminalSymbolType type;
    explicit Token (TerminalSymbolType type, string lex = ""):
        type(type),
        lex(lex) {}
    string getDesc () {
      return desc.at(type);
    }
  };

  enum class SymbolType {
    NON_TERMINAL_SYMBOL,
    TERMINAL_SYMBOL,
    EPSILON,
    END_OF_FILE,
    NONE
  };

  template<typename NonterminalSymbolType, typename TerminalSymbolType>
  class Symbol {
  public:
    SymbolType type;
    NonterminalSymbolType nt_type;
    TerminalSymbolType t_type;
    string lex;

    explicit Symbol (SymbolType type) :
      type(type),
      nt_type(NonterminalSymbolType::None),
      t_type(TerminalSymbolType::none),
      lex("") {}

    explicit Symbol(NonterminalSymbolType nt_type) :
        type(SymbolType::NON_TERMINAL_SYMBOL),
        nt_type(nt_type),
        t_type(TerminalSymbolType::none),
        lex("") {}

    explicit Symbol(TerminalSymbolType t_type, string lex = "") :
        type(SymbolType::TERMINAL_SYMBOL),
        t_type(t_type),
        lex(lex),
        nt_type(NonterminalSymbolType::None) {}

    bool is(NonterminalSymbolType nt_type1) const {
      return type == SymbolType::NON_TERMINAL_SYMBOL && this->nt_type == nt_type1;
    }

    bool is(TerminalSymbolType t_type1, string lex1 = "") const {
      return type == SymbolType::TERMINAL_SYMBOL && this->t_type == t_type1 && this->lex == lex1;
    }

    bool isValid () const {
      return type != SymbolType::NONE;
    }

    bool isNonterminalSymbol() const {
      return type == SymbolType::NON_TERMINAL_SYMBOL && nt_type != NonterminalSymbolType::None;
    }

    bool isTerminalSymbol () const {
      return type == SymbolType::TERMINAL_SYMBOL && t_type != TerminalSymbolType::none;
    }

    bool operator<(const Symbol &rhs) const {
      return type < rhs.type ||
             (type == rhs.type && nt_type < rhs.nt_type) ||
             (type == rhs.type && nt_type == rhs.nt_type && t_type < rhs.t_type) ||
             (type == rhs.type && nt_type == rhs.nt_type && t_type == rhs.t_type && lex < rhs.lex);
    }

    bool operator==(const Symbol &rhs) const {
      return type == rhs.type && nt_type == rhs.nt_type && t_type == rhs.t_type && lex == rhs.lex;
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

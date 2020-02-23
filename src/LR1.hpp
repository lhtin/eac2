#ifndef EAC2_SYNTAX_H
#define EAC2_SYNTAX_H

#include <vector>
#include <utility>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <deque>
#include <stack>
#include <set>
#include <iterator>
#include "spec.hpp"
#include "utils.hpp"

using namespace std;

template <typename Symbol>
class LR1 {
public:
  using SymbolType = typename Symbol::_SymbolType;
  using TerminalSymbolType = typename Symbol::_TerminalSymbolType;
  using NonterminalSymbolType = typename Symbol::_NonterminalSymbolType;

  using Production = Spec::Production<Symbol>;
  using ProductionList = Spec::ProductionList<Symbol>;
  using CFG = Spec::CFG<Symbol>;
private:
  class LR1Item {
  public:
    Symbol nt;
    Production p;
    int pos;
    Symbol t;

    LR1Item(Symbol nt, Production p, int pos, Symbol t) : nt(nt), p(p), pos(pos), t(t) {}

    bool operator<(const LR1Item &rhs) const {
      return nt < rhs.nt ||
             (nt == rhs.nt && p < rhs.p) ||
             (nt == rhs.nt && p == rhs.p && pos < rhs.pos) ||
             (nt == rhs.nt && p == rhs.p && pos == rhs.pos && t < rhs.t);
    }
  };

  using LR1Item = LR1Item;
  using LR1CC = set<LR1Item>;
  using SetSymbol = set<Symbol>;
  using First = set<Symbol>;

  CFG cfg;
  map<Symbol, set<Symbol>> firstAll;
  Symbol epsilon;
  map<LR1CC, int> CC2S;

  int id;

public:
  using AST = Spec::AST<Symbol>;
  using Node = typename AST::Node;
  using Key = pair<int, Symbol>;

  map<Key, int> Shift;
  map<Key, LR1Item> Reduce;
  map<Key, int> Goto;
  Symbol Goal;

  int s0;

  LR1 (CFG cfg, Symbol start):
      cfg(cfg),
      firstAll(),
      Goal(start),
      epsilon(SymbolType::EPSILON),
      id(0) {
    initAllFirst();

    ProductionList list = cfg[Goal];
    LR1CC cc0;
    for (const Production& item : list) {
      cc0.insert(makeLR1Item(Goal, item, 0, Symbol(TerminalSymbolType::eof)));
    }
    closure(cc0);

    addReduce(cc0);

    set<LR1CC> cc;
    cc.insert(cc0);
    CC2S[cc0] = getNewState();
    s0 = CC2S[cc0];
    spread(cc, cc0);
  }
  int getNewState () {
    int state = id;
    id += 1;
    return state;
  }
  LR1Item makeLR1Item (const Symbol& nt, const Production& p, int pos, const Symbol& t) {
    while (pos < p.size() && p[pos] == epsilon) {
      pos += 1;
    }
    return LR1Item{nt, p, pos, t};
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
    if (index < p.size()) {
      return p[index];
    } else {
      return Symbol(SymbolType::NONE);
    }
  }
  Production getRest (const LR1Item& item, int n = 0) {
    Production r;
    for (int i = item.pos + n; i < item.p.size(); i += 1) {
      r.push_back(item.p[i]);
    }
    return r;
  }
  First getFirst (const Production& rest, const Symbol& a) {
    First f;
    bool isAll = true;
    for (const Symbol& s : rest) {
      if (s.isNonterminalSymbol()) {
        First t = firstAll[s];
        f.insert(t.begin(), t.end());
        if (t.find(epsilon) == t.end()) {
          isAll = false;
          break;
        }
      } else if (s.isTerminalSymbol()) {
        f.insert(s);
        isAll = false;
        break;
      }
    }
    if (isAll) {
      f.erase(epsilon);
      f.insert(a);
    }
    return f;
  }
  void closure (LR1CC& c) {
    deque<LR1Item> workList(c.begin(), c.end());
    auto it = workList.begin();
    while (!workList.empty()) {
      const LR1Item& item = workList.front();
      Symbol nextSymbol = getNextSymbol(item);
      if (nextSymbol.isNonterminalSymbol()) {
        const Production& rest = getRest(item, 1);
        First first = getFirst(rest, item.t);
        ProductionList ps = cfg[nextSymbol];
        for (const Production& p : ps) {
          for (const Symbol& a : first) {
            LR1Item t = makeLR1Item(nextSymbol, p, 0, a);
            if (c.find(t) == c.end()) {
              c.insert(t);
              workList.push_back(t);
            }
          }
        }
      }
      workList.pop_front();
    }
  }
  void addReduce (const LR1CC& s) {
    int state = CC2S[s];
    for (const LR1Item& item : s) {
      if (item.pos == item.p.size()) {
        Key k = Key{state, item.t};
        assert_with_msg(Reduce.find(k) == Reduce.end(), "Reduce与Shift冲突，语法上有二义性");
        Reduce.emplace(k, item);
      }
    }
  }
  void spread (set<LR1CC>& cc, const LR1CC& s) {
    set<Key> ShiftT;
    map<Symbol, set<LR1Item>> ReduceT;
    set<Key> GotoT;
    int state = CC2S[s];

    map<Symbol, LR1CC> mc;
    for (const LR1Item& item : s) {
      const Symbol& x = getNextSymbol(item);
      if (x.isValid()) {
        if (mc.find(x) == mc.end()) {
          mc[x] = LR1CC();
        }
        LR1Item t = LR1Item(item.nt, item.p, item.pos + 1, item.t);
        mc[x].insert(t);
        if (t.p.size() == t.pos) {
          if (ReduceT.find(x) == ReduceT.end()) {
            ReduceT[x] = set<LR1Item>();
          }
          ReduceT[x].insert(t);
        }

        if (x.isTerminalSymbol()) {
          ShiftT.emplace(state, x);
        } else {
          GotoT.emplace(state, x);
        }
      }
    }
    for (auto& item : mc) {
      closure(item.second);
      LR1CC s2 = item.second;
      if (cc.find(s2) == cc.end()) {
        CC2S[s2] = getNewState();
        cc.insert(s2);

        addReduce(s2);
        spread(cc, s2);
      }
      int state2 = CC2S[s2];
      Key k = Key{state, item.first};
      if (ShiftT.find(k) != ShiftT.end()) {
        assert_with_msg(Reduce.find(k) == Reduce.end(), "Reduce与Shift冲突，语法上有二义性");
        Shift.emplace(k, state2);
      } else if (GotoT.find(k) != GotoT.end()) {
        Goto.emplace(k, state2);
      }
    }
  }
};

#endif

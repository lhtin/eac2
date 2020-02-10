#ifndef EAC2_SYNTAX_SPEC_H
#define EAC2_SYNTAX_SPEC_H

#include <vector>
#include <utility>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <queue>
#include <iterator>
#include "parentheses.hpp"

using namespace std;

using LR1Item = Spec::LR1Item<Symbol>;
using LR1CC = set<LR1Item>;
using SetSymbol = set<Symbol>;
using First = set<Symbol>;

template <typename WrapLex>
class LR1 {
private:
  CFG cfg;
  map<Symbol, set<Symbol>> firstAll;
  Symbol epsilon;
public:
  LR1 (CFG cfg, NonterminalSymbolType start, WrapLex& lex): cfg(cfg), firstAll(), epsilon(Spec::SymbolType::EPSILON) {
    initAllFirst();

    Symbol goal(start);
    ProductionList list = cfg[goal];
    LR1CC cc0;
    for (const Production& item : list) {
      cc0.insert(LR1Item{goal, item, 0, Symbol(SymbolType::END_OF_FILE)});
    }
    closure(cc0);

    set<LR1CC> c;
    c.insert(cc0);
//    spread(c, cc0);
    int a = 1;
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
      return Symbol(Spec::SymbolType::NONE);
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
  void closure (LR1CC& c, const LR1Item& item) {

  }
  void closure (LR1CC& c) {
    vector<LR1Item> workList(c.begin(), c.end());
    auto it = workList.begin();
    while (it != workList.end()) {
      const LR1Item& item = *it;
      Symbol nextSymbol = getNextSymbol(item);
      if (nextSymbol.isNonterminalSymbol()) {
        Production rest = getRest(item, 1);
        First first = getFirst(rest, item.t);
        ProductionList ps = cfg[nextSymbol];
        for (const Production& p : ps) {
          for (const Symbol& a : first) {
            LR1Item t = LR1Item{nextSymbol, p, 0, a};
            if (c.find(t) == c.end()) {
              c.insert(t);
              workList.push_back(t);
            }
          }
        }
      }
      it++;
    }
  }
  LR1CC goNext (const LR1CC& s, const Symbol& x) {
    LR1CC moved;
    for (const LR1Item& item : s) {
      Symbol ns = getNextSymbol(item);
      if (ns == x) {
        moved.insert(LR1Item(item.nt, item.p, item.pos + 1, item.t));
      }
    }
    return closure(moved);
  }
//  void spread (set<LR1CC>& c, const LR1CC& s) {
//    map<Symbol, LR1CC> cc;
//    map<Symbol, SetSymbol> ms;
//    for (const LR1Item& item : s) {
//      const Symbol& x = getNextSymbol(item);
//      if (x.isValid()) {
//        if (cc.find(x) == cc.end()) {
//          cc[x] = LR1CC();
//          ms[x] = SetSymbol();
//        }
//        cc[x].insert(LR1Item(item.nt, item.p, item.pos + 1, item.t));
//        closure(item.second);
//        ms[x].insert(item.nt);
//      }
//    }
//    for (auto& item : cc) {
//      closure(item.second);
//      if (c.find(item.second) == c.end()) {
//        c.insert(item.second);
//        spread(c, item.second);
//      }
//    }
//  }
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
//          if (item.is(TerminalSymbolType::epsilon)) {
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
#ifndef EAC2_SYNTAX_SPEC_H
#define EAC2_SYNTAX_SPEC_H

#include <vector>
#include <utility>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <deque>
#include <iterator>
#include "pl0.hpp"

using namespace std;

using LR1Item = Spec::LR1Item<Symbol>;
using LR1CC = set<LR1Item>;
using SetSymbol = set<Symbol>;
using First = set<Symbol>;

using Node = Spec::AST<Symbol>::Node;
using Key = pair<int, Symbol>;

template <typename WrapLex>
class LR1 {
private:
  CFG cfg;
  map<Symbol, set<Symbol>> firstAll;
  Symbol epsilon;
  Symbol Goal;

  map<Key, int> Shift;
  map<Key, LR1Item> Reduce;
  map<Key, int> Goto;
  map<LR1CC, int> CC2S;
  WrapLex& lex;
  int s0;

  int id;

public:
  LR1 (CFG cfg, NonterminalSymbolType start, WrapLex& lex):
    cfg(cfg),
    firstAll(),
    Goal(start),
    epsilon(SymbolType::EPSILON),
    id(0),
    lex(lex) {
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
  Node getAST () {
    Symbol word = lex.nextToken();
    stack<int> states;
    stack<Node> symbols;
    states.push(s0);
    while (true) {
      int state = states.top();
      Symbol pure = getPureSymbol(word);
      if (Shift.find(Key{state, pure}) != Shift.end()) {
        auto it = Shift.find(Key{state, pure});
        states.push(it->second);
        symbols.push(Node(word));
        word = lex.nextToken();
      } else if (Reduce.find(Key{state, pure}) != Reduce.end()) {
        auto it = Reduce.find(Key{state, pure});
        Production p = it->second.p;
        Symbol nt = it->second.nt;
        Node parent(nt);
        for (auto& it1 : p) {
          if (!it1.isEpsilon()) {
            parent.addChild(symbols.top());
            symbols.pop();
            states.pop();
          }
        }
        symbols.push(parent);
        states.push(Goto[Key{states.top(), nt}]);
        if (nt == Goal) {
          // 构建成功
          return parent;
        }
      } else {
        return Node(Symbol(SymbolType::NONE));
      }
    }
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
        Reduce.emplace(Key{state, item.t}, item);
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
      Key k1 = Key{state, item.first};
      if (ShiftT.find(k1) != ShiftT.end()) {
        Shift.emplace(k1, state2);
      } else if (GotoT.find(k1) != GotoT.end()) {
        Goto.emplace(k1, state2);
      }
    }
  }
};

#endif
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "all.hpp"

using namespace std;

vector<FA::State*>* DFA::findQ (vector<State*>* q) {
  if (q->empty()) {
    return nullptr;
  }
  for (auto item : Q) {
    if (includes(item->begin(), item->end(), q->begin(), q->end())) {
      return item;
    }
  }
  return nullptr;
}

void DFA::spread (vector<State*>* q) {
  for (char c : nfa->chars) {
    if (c == EMPTY) {
      // 忽略epsilon
      continue;
    }
    vector<State*>* t = closure(q, c);
    if (t->empty()) {
      continue;
    }
    vector<State*>* old = findQ(t);
    if (old != nullptr) {
      addDelta(M[q], c, M[old]);
      continue;
    }

    epsilonClosure(t);
    State* end = newState();
    S.push_back(end);
    addDelta(M[q], c, end);
    auto it2 = find(t->begin(), t->end(), nfa->end);
    if (it2 != t->end()) {
      SA.push_back(end);
    }
    Q.push_back(t);
    M[t] = end;

    spread(t);
  }
}

DFA::DFA (NFA& nfa): nfa(&nfa), Q(), M() {
  chars = nfa.chars;
  map<vector<State*>*, State*> sMap;
  map<vector<State*>, vector<State*>*> qMap;
  queue<vector<State*>*> WorkList;
  vector<State*>* q0 = new vector<State*>();

  q0->push_back(nfa.s0);
  epsilonClosure(q0);
  s0 = newState();
  S.push_back(s0);
  auto it = find(q0->begin(), q0->end(), nfa.end);
  if (it != q0->end()) {
    SA.push_back(s0);
  }
  Q.push_back(q0);
  M[q0] = s0;
  spread(q0);
}
vector<FA::State*>* DFA::closure (vector<State*>* q1, char accept) {
  vector<State*>* q2 = new vector<State*>();
  for (auto item : nfa->deltas) {
    bool isExistStart = find(q1->begin(), q1->end(), item->start) != q1->end();
    if (isExistStart && item->accept.count(accept) > 0) {
      q2->push_back(item->end);
    }
  }
  return q2;
}
void DFA::epsilonClosure(vector<State*>* q) {
  bool has;
  do {
    has = false;
    for (auto item : nfa->deltas) {
      bool isExistStart = find(q->begin(), q->end(), item->start) != q->end();
      bool isExistEnd = find(q->begin(), q->end(), item->end) != q->end();
      if (isExistStart &&
          !isExistEnd &&
          item->accept.count(EMPTY) > 0) {
        q->push_back(item->end);
        has = true;
      }
    }
  } while (has);
}
FA::State* DFA::getEndState (State* start, char accept) {
  auto it = find_if(
      deltas.begin(), deltas.end(),
      [&](Delta* item) {
        return item->start == start && item->accept.count(accept) > 0;
      });
  if (it != deltas.end()) {
    return (*it)->end;
  } else {
    return nullptr;
  }
}
void DFA::print () {
  cout << "-----NFA2DFA start-----" << endl;
  cout << FA::toString() << endl;
  cout << "-----NFA2DFA end-----" << endl;
}
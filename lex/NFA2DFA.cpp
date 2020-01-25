#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include "lex.hpp"
using namespace std;

NFA2DFA::NFA2DFA (RE2NFA& nfa): nfa(&nfa) {
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
  sMap[q0] = s0;
  qMap[*q0] = q0;
  WorkList.push(q0);
  while (!WorkList.empty()) {
    vector<State*>* q = WorkList.front();
    State* start = sMap[q];
    WorkList.pop();
    for (char c : nfa.chars) {
      vector<State*>* t = closure(q, c);
      if (t->empty()) {
        continue;
      }
      epsilonClosure(t);

      auto it = qMap.find(*t);
      if (it == qMap.end()) {
        State* end = newState();
        S.push_back(end);
        deltas.push_back(new Delta(start, c, end));
        auto it2 = find(t->begin(), t->end(), nfa.end);
        if (it2 != t->end()) {
          SA.push_back(end);
        }
        sMap[t] = end;
        qMap[*t] = t;
        WorkList.push(t);
      } else {
        delete t;
        t = it->second;
        deltas.push_back(new Delta(start, c, sMap[t]));
      }
    }
  }
  for (const auto& q : qMap) {
    delete q.second;
  }
}
vector<FA::State*>* NFA2DFA::closure (vector<State*>* q1, char accept) {
  vector<State*>* q2 = new vector<State*>();
  for (auto item : nfa->deltas) {
    bool isExistStart = find(q1->begin(), q1->end(), item->start) != q1->end();
    if (isExistStart && item->accept.count(accept) > 0) {
      q2->push_back(item->end);
    }
  }
  return q2;
}
void NFA2DFA::epsilonClosure(vector<State*>* q) {
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
FA::State* NFA2DFA::getEndState (State* start, char accept) {
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
set<char> NFA2DFA::getAccept (State* start, State* end) {
  auto it = find_if(
      deltas.begin(), deltas.end(),
      [&](Delta* item) {
        return item->start == start && item->end == end;
      });
  if (it != deltas.end()) {
    return (*it)->accept;
  } else {
    return set<char>();
  }
}
void NFA2DFA::print () {
  cout << "-----NFA2DFA start-----" << endl;
  FA::print();
  cout << "-----NFA2DFA end-----" << endl;
}
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include "lex.hpp"

minDFA::minDFA (NFA2DFA& dfa): T() {
  chars = dfa.chars;
  map<set<State*>, State*> m;
  set<State*> states(dfa.S.begin(), dfa.S.end());
  T.push_back(states);

  set<State*> firstPart(dfa.SA.begin(), dfa.SA.end());
  State* end = newState();
  m[firstPart] = end;
  S.push_back(end);
  SA.push_back(end);
  if (firstPart.count(dfa.s0) > 0) {
    s0 = end;
  }
  split(firstPart);

  bool needSplit;
  do {
    set<State*>& top = T.back();
    if (top.empty()) {
      break;
    }
    auto begin = top.begin();
    State* first = *begin;
    set<State*> splitPart;
    splitPart.insert(first);
    begin++;
    for (auto it = begin; it != top.end(); it++) {
      State* s = *it;
      bool allSame = true;
      for (char c : dfa.chars) {
        if (!isSameQ(dfa.getEndState(first, c), dfa.getEndState(s, c))) {
          allSame = false;
          break;
        }
      }
      if (allSame) {
        splitPart.insert(s);
      }
    }
    needSplit = splitPart.size() < top.size();
    if (needSplit) {
      split(splitPart);
    }
    State* s = newState();
    m[splitPart] = s;
    S.push_back(s);
    if (splitPart.count(dfa.s0) > 0) {
      s0 = s;
    }
  } while (needSplit);

  for (const auto& item : T) {
    State* start = m[item];
    for (State* s1 : item) {
      for (char c : dfa.chars) {
        State* s2 = dfa.getEndState(s1, c);
        if (s2 != nullptr) {
          set<State*>* q = getQ(s2);
          if (q != nullptr) {
            State* end = m[*q];
            addDelta(start, c, end);
          }
        }
      }
    }
  }
}
bool minDFA::isSameQ (State* s1, State* s2) {
  if (s1 == s2) {
    return true;
  }
  auto it = find_if(
      T.begin(), T.end(),
      [&](const set<State*>& q) {
        return q.count(s1) > 0 && q.count(s2) > 0;
      });
  if (it != T.end()) {
    return true;
  } else {
    return false;
  }
}
set<FA::State*>* minDFA::getQ (State* state) {
  auto it = find_if(
      T.begin(), T.end(),
      [&](const set<State*>& q) {
        return q.count(state) > 0;
      });
  if (it != T.end()) {
    return &(*it);
  } else {
    return nullptr;
  }
}
void minDFA::split (set<State*>& part) {
  set<State*>& top = T.back();
  set<State*> rest;
  set_difference(
      top.begin(), top.end(),
      part.begin(), part.end(),
      inserter(rest, rest.begin()));
  T.pop_back();
  T.push_back(part);
  T.push_back(rest);
}
void minDFA::print () {
  cout << "-----minDFA start-----" << endl;
  FA::print();
  cout << "-----minDFA end-----" << endl;
}
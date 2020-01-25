#include <iostream>
#include <iterator>
#include "lex.h"
using namespace std;

FA::State::State (int id) {
  n = id;
}

FA::Delta::Delta (State* start, char accept, State* end): start(start), accept(), end(end) {
  this->accept.insert(accept);
}

template <typename T>
void FA::move (vector<T>& src, vector<T>& dest) {
  dest.insert(
      dest.end(),
      make_move_iterator(src.begin()),
      make_move_iterator(src.end()));
  src.clear();
}
template <typename T>
void FA::move (set<T>& src, set<T>& dest) {
  dest.insert(
      make_move_iterator(src.begin()),
      make_move_iterator(src.end()));
  src.clear();
}
void FA::move (FA& child) {
  move(child.S, S);
  move(child.deltas, deltas);
  move(child.chars, chars);
}
FA::FA (): id(0) {}
FA::~FA () {
  for (Delta* item : deltas) {
    delete item;
  }
  for (State* item : S) {
    delete item;
  }
}
void FA::addDelta (State* start, char accept, State* end) {
  for (auto item : deltas) {
    if (item->start == start && item->end == end) {
      item->accept.insert(accept);
      return;
    }
  }
  deltas.push_back(new Delta(start, accept, end));
}
FA::State* FA::newState () {
  State* s = new State(id);
  id += 1;
  return s;
}
void FA::print () {
  cout << "S:";
  for (auto item : S) {
    cout << " s" << item->n;
  }
  cout << endl;

  cout << "chars:";
  for (char c : chars) {
    cout << " " << c;
  }
  cout << endl;

  cout << "δ: " << endl;
  for (Delta* item : deltas) {
    string accept;
    for (auto it = item->accept.begin(); it != item->accept.end(); it++) {
      char c = *it;
      if (it != item->accept.begin()) {
        accept += ",";
      }
      if (c == EMPTY) {
        accept += "ε";
      } else {
        accept += c;
      }
    }
    cout << string(1, '\t')
         << "s" << item->start->n
         << " -->|" << accept << "| "
         << "s" << item->end->n << endl;
  }

  cout << "s0: s" << s0->n << endl;

  cout << "SA:";
  for (auto item : SA) {
    cout << " s" << item->n;
  }
  cout << endl;
}

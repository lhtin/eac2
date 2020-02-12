#include <iostream>
#include <iterator>
#include "all.hpp"
#include "../utils/utils.hpp"
using namespace std;

FA::State::State (int id) {
  n = id;
}

FA::Delta::Delta (State* start, char accept, State* end): start(start), accept(), end(end) {
  this->accept.insert(accept);
}
FA::Delta::Delta (State* start, const set<char>& accept, State* end): start(start), accept(), end(end) {
  this->accept.insert(accept.begin(), accept.end());
}
FA::FA (): id(0), s0(nullptr) {}
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
void FA::addDelta (State* start, const set<char>& accept, State* end) {
  for (auto item : deltas) {
    if (item->start == start && item->end == end) {
      item->accept.insert(accept.begin(), accept.end());
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
  printNow();
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

  cout << "δ: " << deltas.size() << endl;
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

unordered_map<int, unordered_map<char, int>> FA::getDeltas () {
  unordered_map<int, unordered_map<char, int>> d;
  for (auto item : deltas) {
    int start = item->start->n;
    set<char> cs = item->accept;
    int end = item->end->n;

    if (d.find(start) == d.end()) {
      d[start] = unordered_map<char, int>();
    }
    for (auto c : cs) {
      d[start][c] = end;
    }
  }
  return d;
}

#include <iostream>
#include <string>
#include "lex.hpp"

using namespace std;

RE2NFA::RE2NFA (RENode& node) {
  if (node.op == CONNECT) {
    RE2NFA left = RE2NFA(*node.left);
    RE2NFA right = RE2NFA(*node.right);
    s0 = left.s0;
    end = right.end;
    SA.push_back(end);
    move(left);
    move(right);
    addDelta(left.end, EMPTY, right.s0);
  } else if (node.op == SELECT) {
    s0 = newState();
    end = newState();
    RE2NFA left = RE2NFA(*node.left);
    RE2NFA right = RE2NFA(*node.right);
    S.push_back(s0);
    S.push_back(end);
    SA.push_back(end);
    move(left);
    move(right);
    addDelta(s0, EMPTY, left.s0);
    addDelta(s0, EMPTY, right.s0);
    addDelta(left.end, EMPTY, end);
    addDelta(right.end, EMPTY, end);
  } else if (node.op == CLOSURE) {
    s0 = newState();
    end = newState();
    RE2NFA child = RE2NFA(*node.child);
    S.push_back(s0);
    S.push_back(end);
    SA.push_back(end);
    move(child);
    addDelta(s0, EMPTY, child.s0);
    addDelta(child.end, EMPTY, end);
    addDelta(s0, EMPTY, end);
    addDelta(child.end, EMPTY, child.s0);
  } else if (node.op == GROUP) {
    RE2NFA child = RE2NFA(*node.child);
    s0 = child.s0;
    end = child.end;
    SA.push_back(end);
    move(child);
  } else if (node.op == LEAF) {
    s0 = newState();
    end = newState();
    S.push_back(s0);
    S.push_back(end);
    SA.push_back(end);
    if (node.leaf != EMPTY) {
      chars.insert(node.leaf);
    }
    addDelta(s0, node.leaf, end);
  }
}
void RE2NFA::print () {
  cout << "-----RE2NFA start-----" << endl;
  FA::print();
  cout << "-----RE2NFA end-----" << endl;
}

#include <iostream>
#include "lex.hpp"

using namespace std;

RE2NFA::Node RE2NFA::construct (RENode& node) {
  Node res;
  State* s0;
  State* end;
  if (node.op == CONNECT) {
    Node left = construct(*node.left);
    Node right = construct(*node.right);
    s0 = left.s0;
    end = right.end;
    SA.push_back(end);
    addDelta(left.end, EMPTY, right.s0);
  } else if (node.op == SELECT) {
    s0 = newState();
    end = newState();
    Node left = construct(*node.left);
    Node right = construct(*node.right);
    S.push_back(s0);
    S.push_back(end);
    SA.push_back(end);
    addDelta(s0, EMPTY, left.s0);
    addDelta(s0, EMPTY, right.s0);
    addDelta(left.end, EMPTY, end);
    addDelta(right.end, EMPTY, end);
  } else if (node.op == CLOSURE) {
    s0 = newState();
    end = newState();
    Node child = construct(*node.child);
    S.push_back(s0);
    S.push_back(end);
    SA.push_back(end);
    addDelta(s0, EMPTY, child.s0);
    addDelta(child.end, EMPTY, end);
    addDelta(s0, EMPTY, end);
    addDelta(child.end, EMPTY, child.s0);
  } else if (node.op == GROUP) {
    Node child = construct(*node.child);
    s0 = child.s0;
    end = child.end;
    SA.push_back(end);
  } else if (node.op == LEAF) {
    s0 = newState();
    end = newState();
    S.push_back(s0);
    S.push_back(end);
    SA.push_back(end);
    chars.insert(node.chars.begin(), node.chars.end());
    addDelta(s0, node.chars, end);
  }
  res.s0 = s0;
  res.end = end;
  return res;
}
RE2NFA::RE2NFA (RENode& node) {
  Node root = construct(node);
  s0 = root.s0;
  end = root.end;
}
void RE2NFA::print () {
  cout << "-----RE2NFA start-----" << endl;
  FA::print();
  cout << "-----RE2NFA end-----" << endl;
}

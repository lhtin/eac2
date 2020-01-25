#include <iostream>
#include <string>
#include <cassert>
#include "lex.hpp"

using namespace std;

void RENode::print (int tabs) {
  switch (this->op) {
    case SELECT:
      cout << string(tabs, '\t') << "(Select" << endl;
      this->left->print(tabs + 1);
      this->right->print(tabs + 1);
      cout << string(tabs, '\t') << ")" << endl;
      break;
    case CONNECT:
      cout << string(tabs, '\t') << "(Connect" << endl;
      this->left->print(tabs + 1);
      this->right->print(tabs + 1);
      cout << string(tabs, '\t') << ")" << endl;
      break;
    case CLOSURE:
      cout << string(tabs, '\t') << "(Closure" << endl;
      this->child->print(tabs + 1);
      cout << string(tabs, '\t') << ")" << endl;
      break;
    case GROUP:
      cout << string(tabs, '\t') << "(Group" << endl;
      this->child->print(tabs + 1);
      cout << string(tabs, '\t') << ")" << endl;
      break;
    case LEAF:
      if (this->leaf == EMPTY) {
        cout << string(tabs, '\t') << "(Leaf 'ε')" << endl;
      } else {
        cout << string(tabs, '\t') << "(Leaf '" << this->leaf << "')" << endl;
      }
      break;
  }
}

RENode::RENode (string& re, int start, int end) {
  if (end < start) {
    this->op = LEAF;
    leaf = EMPTY;
    return;
  }
  int at = start;
  int count = 0;
  while (at <= end) {
    char c = re[at];
    if (c == '(') {
      count += 1;
    } else if (c == ')') {
      count -= 1;
    } else if (c == '|' && count == 0) {
      this->op = SELECT;
//      assert(start <= at - 1); 允许｜左右为空，表示δ
      left = new RENode(re, start, at - 1);
//      assert(at + 1 <= end); 允许｜左右为空δ
      right = new RENode(re, at + 1, end);
      return;
    }
    at += 1;
  }
  assert(count == 0);

  at = start;
  count = 0;
  while (at <= end) {
    char c = re[at];
    if (c == '(') {
      count += 1;
    } else if (c == ')') {
      count -= 1;
      if (count == 0) {
        if (at < end) {
          if (re[at + 1] == '*') {
            at += 1;
            continue;
          } else {
            this->op = CONNECT;
            assert(start <= at);
            left = new RENode(re, start, at);
            assert(at + 1 <= end);
            right = new RENode(re, at + 1, end);
            return;
          }
        } else {
          this->op = GROUP;
          assert(start + 1 <= end - 1);
          child = new RENode(re, start + 1, end - 1);
          return;
        }
      }
    } else if (c == '*') {
      if (count == 0) {
        if (at < end) {
          this->op = CONNECT;
          assert(start <= at);
          left = new RENode(re, start, at);
          assert(at + 1 <= end);
          right = new RENode(re, at + 1, end);
          return;
        } else {
          this->op = CLOSURE;
          assert(start <= end - 1);
          child = new RENode(re, start, end - 1);
          return;
        }
      }
    } else {
      if (count == 0) {
        if (at < end) {
          if (re[at + 1] == '*') {
            at += 1;
            continue;
          } else {
            this->op = CONNECT;
            assert(start <= at);
            left = new RENode(re, start, at);
            assert(at + 1 <= end);
            right = new RENode(re, at + 1, end);
            return;
          }
        } else {
          this->op = LEAF;
          leaf = c;
          return;
        }
      }
    }
    at += 1;
  }
}
RENode::~RENode () {
  delete left;
  delete right;
  delete child;
}

RETree::RETree (string& re): head(re, 0, re.size() - 1) {
}
void RETree::print () {
  cout << "------print RE Tree start-----" << endl;
  head.print();
  cout << "------print RE Tree end-----" << endl;
}
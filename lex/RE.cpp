#include <iostream>
#include <string>
#include <cassert>
#include "all.hpp"
#include "../utils/utils.hpp"

using namespace std;

static char space = ' ';

void RENode::expandRange (string& rre, int start, int end) {
  for (int i = start; i <= end; i += 1) {
    char c0 = rre[i];
    assert(c0 != '-');
    chars.insert(c0);
    if (i + 1 <= end && rre[i + 1] == '-') {
      assert(i + 2 <= end);
      char c1 = rre[i + 2];
      for (char j = static_cast<char>(c0 + 1); j <= c1; j += 1) {
        chars.insert(j);
      }
      i += 2;
    }
  }
}

RENode::RENode (string& re, int start, int end): re(re, start, end < start ? 0 : end - start + 1), left(nullptr), right(
    nullptr), child(nullptr), chars() {
  if (end < start) {
    op = Operator::LEAF;
    chars.insert(EMPTY);
    return;
  }
  int at = start;
  int parenthesisCount = 0;
  int bracketCount = 0;
  while (at <= end) {
    char c = re[at];
    if (c == '\\') {
      // 如果是转义字符则跳过
      assert(at < end);
      at += 1;
    } else if (c == '(') {
      assert(bracketCount == 0); // 小括号不能出现在中括号里面
      parenthesisCount += 1;
    } else if (c == ')') {
      parenthesisCount -= 1;
    } else if (c == '[') {
      bracketCount += 1;
    } else if (c == ']') {
      bracketCount -= 1;
    } else if (c == '|' && parenthesisCount == 0 && bracketCount == 0) {
      op = Operator::SELECT;
//      assert(start <= at - 1); 允许｜左右为空，表示δ
      left = new RENode(re, start, at - 1);
//      assert(at + 1 <= end); 允许｜左右为空δ
      right = new RENode(re, at + 1, end);
      return;
    }
    at += 1;
  }
  assert(parenthesisCount == 0);
  assert(bracketCount == 0);

  at = start;
  parenthesisCount = 0;
  bracketCount = 0;
  while (at <= end) {
    char c = re[at];
    if (c == '(') {
      parenthesisCount += 1;
    } else if (c == '[') {
      bracketCount += 1;
    } else if (c == ')') {
      parenthesisCount -= 1;
      if (parenthesisCount == 0) {
        if (at < end) {
          char next = re[at + 1];
          if (next == '*' || next == '+') {
            at += 1;
            continue;
          } else {
            op = Operator::CONNECT;
            assert(start <= at);
            left = new RENode(re, start, at);
            assert(at + 1 <= end);
            right = new RENode(re, at + 1, end);
            return;
          }
        } else {
          op = Operator::GROUP;
          assert(start + 1 <= end - 1);
          child = new RENode(re, start + 1, end - 1);
          return;
        }
      }
    } else if (c == ']') {
      bracketCount -= 1;
      if (bracketCount == 0) {
        if (at < end) {
          char next = re[at + 1];
          if (next == '*' || next == '+') {
            at += 1;
            continue;
          } else {
            op = Operator::CONNECT;
            assert(start <= at);
            left = new RENode(re, start, at);
            assert(at + 1 <= end);
            right = new RENode(re, at + 1, end);
            return;
          }
        } else {
          op = Operator::LEAF;
          assert(start + 1 <= end - 1);
          expandRange(re, start + 1, end - 1);
          return;
        }
      }
    } else if (c == '*' || c == '+') {
      if (parenthesisCount == 0) {
        if (at < end) {
          op = Operator::CONNECT;
          assert(start <= at);
          left = new RENode(re, start, at);
          assert(at + 1 <= end);
          right = new RENode(re, at + 1, end);
          return;
        } else if (c == '*') {
          op = Operator::CLOSURE;
          assert(start <= end - 1);
          child = new RENode(re, start, end - 1);
          return;
        } else if (c == '+') {
          op = Operator::CONNECT;
          assert(start <= end - 1);
          left = new RENode(re, start, end - 1);
          string t(re, start, end - start + 1);
          t[end - start] = '*';
          right = new RENode(t, 0, t.size() - 1);
          return;
        }
      }
    } else {
      if (parenthesisCount == 0 && bracketCount == 0) {
        if (c == '\\') {
          assert(at < end);
          at += 1;
          c = re[at];
        }
        if (at < end) {
          char next = re[at + 1];
          if (next == '*' || next == '+') {
            at += 1;
            continue;
          } else {
            op = Operator::CONNECT;
            assert(start <= at);
            left = new RENode(re, start, at);
            assert(at + 1 <= end);
            right = new RENode(re, at + 1, end);
            return;
          }
        } else {
          op = Operator::LEAF;
          chars.insert(c);
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

string RENode::toString (int tabs) {
  string res = string(tabs, space);
  switch (op) {
    case Operator::SELECT:
      res += "(Select\n" + left->toString(tabs + 2) + "\n" + right->toString(tabs + 2) + ")";
      break;
    case Operator::CONNECT:
      res += "(Connect\n" + left->toString(tabs + 2) + "\n" + right->toString(tabs + 2) + ")";
      break;
    case Operator::CLOSURE:
      res += "(Closure\n" + child->toString(tabs + 2) + ")";
      break;
    case Operator::GROUP:
      res += "(Group\n" + child->toString(tabs + 2) + ")";
      break;
    case Operator::LEAF:
      res +=  "(Leaf \"";
      for (auto it = chars.begin(); it != chars.end(); it++) {
        if (it != chars.begin()) {
          res += "|";
        }
        char c = *it;
        res += escape(c);
      }
      res += "\")";
      break;
  }
  return res;
}

RETree::RETree (string& re): re(re), head(re, 0, re.size() - 1) {
}
void RETree::print () {
  cout << "------print RE Tree start-----" << endl;
  cout << head.toString() << endl;
  cout << "------print RE Tree end-----" << endl;
}
#include <iostream>
#include <string>
#include "all.hpp"
#include "utils.hpp"

using namespace std;

static char space = ' ';

void RENode::expandRange (const string& rre, int start, int end) {
  for (int i = start; i <= end; i += 1) {
    char c0 = rre[i];
    if (c0 == '\\') {
      continue;
    }
    chars.insert(c0);
    if (i + 1 <= end && rre[i + 1] == '-') {
      assert_with_msg(i + 2 <= end, "范围符(-)之后需要字符");
      char c1 = rre[i + 2];
      for (char j = static_cast<char>(c0 + 1); j <= c1; j += 1) {
        chars.insert(j);
      }
      i += 2;
    }
  }
}

RENode::RENode (const string& re, int start, int end): re(re, start, end < start ? 0 : end - start + 1), left(nullptr), right(
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
      // \\ \( \) \[ \] \* \+ \-
      string t = "\\()[]*+-";
      assert_with_msg(at < end, "转义字符(\\)后面需要字符");
      assert_with_msg(t.find(re[at + 1]) != string::npos, "转义字符(\\)后面只能是 " + t + " 中的一个，但是给的是 " + re[at + 1]);
      at += 1;
    } else if (c == '(') {
      assert_with_msg(bracketCount == 0, "小括号不能出现在中括号里面");
      parenthesisCount += 1;
    } else if (c == ')') {
      parenthesisCount -= 1;
    } else if (c == '[') {
      bracketCount += 1;
    } else if (c == ']') {
      bracketCount -= 1;
    } else if (c == '|' && parenthesisCount == 0 && bracketCount == 0) {
      op = Operator::SELECT;
      left = new RENode(re, start, at - 1);
      right = new RENode(re, at + 1, end);
      return;
    }
    at += 1;
  }
  assert_with_msg(parenthesisCount == 0, "小括号没有成双结对");
  assert_with_msg(bracketCount == 0, "中括号没有成双结对");

  at = start;
  parenthesisCount = 0;
  bracketCount = 0;
  bool isEscape = false;
  while (at <= end) {
    char c = re[at];
    if (c == '\\') {
      isEscape = true;
      at += 1;
      continue;
    }

    if (!isEscape && c == '(') {
      parenthesisCount += 1;
    } else if (!isEscape && c == '[') {
      bracketCount += 1;
    } else if (!isEscape && c == ')') {
      parenthesisCount -= 1;
      if (parenthesisCount == 0) {
        if (at < end) {
          char next = re[at + 1];
          if (next == '*' || next == '+') {
            at += 1;
            continue;
          } else {
            op = Operator::CONNECT;
            left = new RENode(re, start, at);
            right = new RENode(re, at + 1, end);
            return;
          }
        } else {
          op = Operator::GROUP;
          assert_with_msg(start + 1 <= end - 1, "分组括号()里面需要含有内容");
          child = new RENode(re, start + 1, end - 1);
          return;
        }
      }
    } else if (!isEscape && c == ']') {
      bracketCount -= 1;
      if (bracketCount == 0) {
        if (at < end) {
          char next = re[at + 1];
          if (next == '*' || next == '+') {
            at += 1;
            continue;
          } else {
            op = Operator::CONNECT;
            left = new RENode(re, start, at);
            right = new RENode(re, at + 1, end);
            return;
          }
        } else {
          op = Operator::LEAF;
          assert_with_msg(start + 1 <= end - 1, "方括号[]里面需要含有内容");
          expandRange(re, start + 1, end - 1);
          return;
        }
      }
    } else if (!isEscape && (c == '*' || c == '+')) {
      if (parenthesisCount == 0) {
        if (at < end) {
          op = Operator::CONNECT;
          left = new RENode(re, start, at);
          right = new RENode(re, at + 1, end);
          return;
        } else if (c == '*') {
          op = Operator::CLOSURE;
          assert_with_msg(start <= end - 1, "*前面需要有内容");
          child = new RENode(re, start, end - 1);
          return;
        } else if (c == '+') {
          op = Operator::CONNECT;
          assert_with_msg(start <= end - 1, "+前面需要有内容");
          left = new RENode(re, start, end - 1);
          string t(re, start, end - start + 1);
          t[end - start] = '*';
          right = new RENode(t, 0, t.size() - 1);
          return;
        }
      }
    } else {
      if (isEscape) {
        isEscape = false;
      }
      if (parenthesisCount == 0 && bracketCount == 0) {
        if (at < end) {
          char next = re[at + 1];
          if (next == '*' || next == '+') {
            at += 1;
            continue;
          } else {
            op = Operator::CONNECT;
            left = new RENode(re, start, at);
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

RETree::RETree (const string& re): re(re), head(re, 0, re.size() - 1) {
}
void RETree::print () {
  cout << "------print RE Tree start-----" << endl;
  cout << head.toString() << endl;
  cout << "------print RE Tree end-----" << endl;
}
//
// Created by 丁乐华 on 2020/1/24.
//

#ifndef EAC2_LEX_HPP
#define EAC2_LEX_HPP
#include <string>
#include <vector>
#include <set>
#include <queue>
using namespace std;

const char EMPTY = '\0';

//// 正则表达式
enum Operator {
  CONNECT,  // 连接
  SELECT,   // 选择
  CLOSURE,  // 闭包
  GROUP,    // 分组
  LEAF      // 字符
};

class RENode {
public:
  Operator op;
  char leaf;
  RENode* left;
  RENode* right;
  RENode* child;

  RENode (string& re, int start, int end);
  ~RENode ();
  void print (int tabs = 0);
};

class RETree {
public:
  RENode head;

  explicit RETree (string& re);
  void print ();
};

//// 有限自动机
class FA {
public:
  int id;
  class State {
  public:
    int n;

    explicit State (int id);
  };
  class Delta {
  public:

    State* start;
    set<char> accept;
    State* end;

    Delta (State* start, char accept, State* end);
  };

  set<char> chars;
  vector<Delta*> deltas;
  vector<State*> S;
  vector<State*> SA;
  State* s0;

  FA ();
  ~FA ();

  virtual void print ();
  void move (FA& fa);
  template <typename T>
  void move (vector<T>& src, vector<T>& dest);
  template <typename T>
  void move (set<T>& src, set<T>& dest);
  void addDelta (State* start, char accept, State* end);
  State* newState ();
};

//// RE2NFA
class RE2NFA: public FA {
public:
  State* end;

  explicit RE2NFA (RENode& node);
  void print () override;
};

//// NFA2DFA
class NFA2DFA: public FA {
private:
  RE2NFA* nfa;

public:
  explicit NFA2DFA (RE2NFA& nfa);
  void epsilonClosure (vector<State*>* q);
  vector<State*>* closure (vector<State*>* q1, char accept);
  State* getEndState (State* start, char accept);
  set<char> getAccept (State* start, State* end);
  void print () override;
};

//// minDFA
class minDFA: public FA {
private:
  vector<set<State*>> T;

public:
  explicit minDFA (NFA2DFA& dfa);
  void split (set<State*>& part);
  set<State*>* getQ (State* state);
  bool isSameQ (State* s1, State* s2);
  void print () override;
};

#endif //EAC2_LEX_HPP
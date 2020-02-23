//
// Created by 丁乐华 on 2020/1/24.
//

#ifndef EAC2_ALL_HPP
#define EAC2_ALL_HPP
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <unordered_map>
#include <fstream>
#include <utility>

using namespace std;

const char EMPTY = '\0';

//// 正则表达式
enum class Operator {
  CONNECT,  // 连接 ab
  SELECT,   // 选择 |
  CLOSURE,  // 闭包 *
  GROUP,    // 分组 (...)
  LEAF      // 字符集合，包括单个字符或者[]包括的字符
};

class RENode {
public:
  Operator op;
  set<char> chars;
  RENode* left;
  RENode* right;
  RENode* child;
  string re;

  RENode (string& re, int start, int end);
  ~RENode ();
  void expandRange (string& rre, int start, int end);
  string toString (int tabs = 0);
};

class RETree {
public:
  RENode head;
  const string& re;

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
    Delta (State* start, const set<char>& accept, State* end);
  };

  set<char> chars;
  vector<Delta*> deltas;
  vector<State*> S;
  vector<State*> SA;
  State* s0;

  FA ();
  ~FA ();

  string toString ();
  void addDelta (State* start, char accept, State* end);
  void addDelta (State* start, const set<char>& accept, State* end);
  State* newState ();
  unordered_map<int, unordered_map<char, int>> getDeltas ();
};

//// RE2NFA
class NFA: public FA {
public:
  class Node {
  public:
    State* s0;
    State* end;
  };

  State* end;

  explicit NFA (RETree& tree);
  NFA::Node construct (RENode& node);
  void print ();
};

//// NFA2DFA
class DFA: public FA {
private:
  vector<vector<State*>*> Q;
  map<vector<State*>*, State*> M;
  NFA* nfa;

public:
  explicit DFA (NFA& nfa);
  void epsilonClosure (vector<State*>* q);
  vector<State*>* closure (vector<State*>* q1, char accept);
  State* getEndState (State* start, char accept);
  void print ();
  vector<State*>* findQ (vector<State*>* q);
  void spread (vector<State*>* q);
};

//// minDFA
using Set = set<FA::State*>;
using SetSet = set<Set>;
using Map = map<Set, FA::State*>;

class minDFA: public FA {
public:
  explicit minDFA (DFA& dfa);
  SetSet split (const SetSet& P, const Set& p, DFA& dfa);
  Set findP (State* s, SetSet& T);
  void print ();
};

class WrapFA {
private:
  FA* fa;
  FA::State* current;
  int at;
  vector<int> finishedLength;
public:
  explicit WrapFA (FA* fa);
  bool accept (char c);
  int getPrevFinishedDelta ();
  bool isFinish ();
  void reset ();
};

#endif //EAC2_WRAPLEX_HPP
//
// Created by 丁乐华 on 2020/1/24.
//

#ifndef EAC2_LEX_HPP
#define EAC2_LEX_HPP
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <unordered_map>
#include <fstream>
#include <utility>
#include "../spec/lex-spec.hpp"

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
  void print (int tabs = 0);
  void expandRange (string& rre, int start, int end);
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
    Delta (State* start, const set<char>& accept, State* end);
  };

  set<char> chars;
  vector<Delta*> deltas;
  vector<State*> S;
  vector<State*> SA;
  State* s0;

  FA ();
  ~FA ();

  virtual void print ();
  void addDelta (State* start, char accept, State* end);
  void addDelta (State* start, const set<char>& accept, State* end);
  State* newState ();
  unordered_map<int, unordered_map<char, int>> getDeltas ();
};

//// RE2NFA
class RE2NFA: public FA {
public:
  class Node {
  public:
    State* s0;
    State* end;
  };

  State* end;

  explicit RE2NFA (RENode& node);
  RE2NFA::Node construct (RENode& node);
  void print () override;
};

//// NFA2DFA
class NFA2DFA: public FA {
private:
  vector<vector<State*>*> Q;
  map<vector<State*>*, State*> M;
  RE2NFA* nfa;

public:
  explicit NFA2DFA (RE2NFA& nfa);
  void epsilonClosure (vector<State*>* q);
  vector<State*>* closure (vector<State*>* q1, char accept);
  State* getEndState (State* start, char accept);
  void print () override;
  vector<State*>* findQ (vector<State*>* q);
  void spread (vector<State*>* q);
};

//// minDFA
using Set = set<FA::State*>;
using SetSet = set<Set>;
using Map = map<Set, FA::State*>;

class minDFA: public FA {
public:
  explicit minDFA (NFA2DFA& dfa);
  SetSet split (const SetSet& P, const Set& p, NFA2DFA& dfa);
  Set findP (State* s, SetSet& T);
  void print () override;
};

class WrapFA {
private:
  FA* fa;
  FA::State* current;
  unordered_map<int, unordered_map<char, int>> deltas;
  int current2;
public:
  explicit WrapFA (FA* fa);
  bool accept (char c);
  bool isFinish ();
  void reset ();
  bool accept2 (char c);
  bool isFinish2 ();
  void reset2 ();
};

const int BUFFER_MAX = 1024;

class Token {
public:
  string lex;
  TokenType type;
  string typeDesc;
  Token (TokenType type, string lex);
};

class Lex {
private:
  vector<pair<WrapFA, TokenType>> list;
  char buf[BUFFER_MAX * 2];
  int at;
  ifstream sourceFile;

public:
  Lex (LexSpec specPath, string sourcePath);
  ~Lex ();
  void addRE (string& re, TokenType type);
  Token* nextToken ();
};

#endif //EAC2_LEX_HPP
#ifndef EAC2_PARSER_HPP
#define EAC2_PARSER_HPP

template <typename Symbol, typename Lex, typename LR1>
class parser {
private:
  Lex& lex;
  LR1& lr1;
  ifstream sourceFile;
  string buf;
  int bufAt;
public:
  using Node = typename LR1::Node;
  using Key = typename LR1::Key;
  using Production = typename LR1::Production;
  using AST = typename LR1::AST;
  using SymbolType = typename Symbol::_SymbolType;
  using TerminalSymbolType = typename Symbol::_TerminalSymbolType;

  class Output {
  public:
    string tokens;
    string ast;
    Output () {}
  };
  Output o;

  parser (Lex& lex, LR1& lr1): lex(lex), lr1(lr1) {}

  Output getOutput (string source) {
    sourceFile = ifstream(source);
    if (sourceFile.is_open()) {
      buf = "";
      bufAt = 0;
      o = Output();
      AST ast = getAST();
      o.ast = ast.toString();
      sourceFile.close();
      return o;
    } else {
      // 无法打开需要解析的源代码文件
      sourceFile.close();
      assert(false);
    }
  }

  char nextChar () {
    if (!buf.empty() && bufAt < buf.length()) {
      char c = buf[bufAt];
      bufAt += 1;
      return c;
    } else {
      char c = sourceFile.get();
      buf.push_back(c);
      bufAt += 1;
      return c;
    }
  }
  /**
   * 回退n个字符
   * @param n
   */
  void goBack (int n) {
    assert(bufAt - n >= 0);
    bufAt -= n;
  }

  Symbol nextToken () {
    string text;
    TerminalSymbolType type = TerminalSymbolType::none;
    for (auto item : lex.list) {
      WrapFA& wfa = item.first;
      string temp;
      char c = nextChar();
      if (c == char_traits<char>::eof()) {
        type = TerminalSymbolType::eof;
        break;
      }
      while (wfa.accept(c)) {
        temp += c;
        c = nextChar();
      }
      goBack(1);
      bool isFinish = wfa.isFinish();
      if (!isFinish && wfa.getPrevFinishedDelta() > 0) {
        int backLen = wfa.getPrevFinishedDelta();
        temp = temp.substr(0, temp.size() - backLen);
        goBack(backLen);
        isFinish = true;
      }
      wfa.reset();
      if (isFinish) {
        if (temp.length() <= text.length()) {
          // 当前找到的token比前面找到的短，所以丢弃
          continue;
        } else {
          // 当前找到的token比前面找到的长，使用新的
          type = item.second;
          text = temp;
        }
      } else {
        if (temp.length() > text.length()) {
          // 当前没有找到token且消耗的字符比前一个token长，则回退
          goBack(temp.length() - text.length());
        }
      }
      bufAt = 0;
    }

    // 为none说明没有找到匹配的token，源码有误
    assert(type != TerminalSymbolType::none);

    buf = buf.substr(text.length(), buf.length() - text.length());
    bufAt = 0;

    Symbol s = Symbol(type, text);
    o.tokens += s.getDesc() + "\n";
    return s;
  }
  AST getAST () {
    Symbol word = nextToken();
    stack<int> states;
    stack<Node> symbols;
    states.push(lr1.s0);
    while (true) {
      int state = states.top();
      Symbol pure = word.getPureSymbol();
      if (pure.is(TerminalSymbolType::space)) {
        word = nextToken();
        continue;
      } else if (lr1.Shift.find(Key{state, pure}) != lr1.Shift.end()) {
        auto it = lr1.Shift.find(Key{state, pure});
        states.push(it->second);
        symbols.push(Node(word));
        word = nextToken();
      } else if (lr1.Reduce.find(Key{state, pure}) != lr1.Reduce.end()) {
        auto it = lr1.Reduce.find(Key{state, pure});
        Production p = it->second.p;
        Symbol nt = it->second.nt;
        Node parent(nt);
        for (auto& it1 : p) {
          if (!it1.isEpsilon()) {
            parent.addChild(symbols.top());
            symbols.pop();
            states.pop();
          }
        }
        symbols.push(parent);
        states.push(lr1.Goto[Key{states.top(), nt}]);
        if (nt == lr1.Goal) {
          // 构建成功
          return AST(parent);
        }
      } else {
        // 到达此处说明构建不成功
        return AST(Node(Symbol(SymbolType::NONE)));
      }
    }
  }
};

#endif

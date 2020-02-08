#ifndef EAC2_LEX_SPEC_H
#define EAC2_LEX_SPEC_H

#include <vector>
#include <utility>
#include <string>
using namespace std;

enum class TokenType {
  ident,
  keyword,
  space,
  number,
  invalid,
  eof,
  epsilon
};

using LexSpec = vector<pair<string, TokenType>>;

const LexSpec PL0_LEX_SPEC = {
    {
        "const|var|procedure|call|begin|end|while|do|odd|if|=|,|;|:=|?|!|#|<|<=|>|>=|\\+|-|\\*|/|\\(|\\)|.",
        TokenType::keyword
    },
    {
        "[a-zA-Z_][a-zA-Z_0-9]*",
        TokenType::ident
    },
    {
        "[0-9]+",
        TokenType::number
    },
    {
        "[ \n\t]+",
        TokenType::space
    }
};

class Token {
public:
  string lex;
  TokenType type;
  string typeDesc;
  Token (TokenType type, string lex): type(type), lex(lex) {
    initTypeDesc();
  }
  Token (TokenType type): type(type) {
    initTypeDesc();
  }
  void initTypeDesc () {
    if (type == TokenType::ident) {
      typeDesc = "ident";
    } else if (type == TokenType::keyword) {
      typeDesc = "keyword";
    } else if (type == TokenType::space) {
      typeDesc = "space";
    } else if (type == TokenType::number) {
      typeDesc = "number";
    } else if (type == TokenType::eof) {
      typeDesc = "eof";
    } else {
      typeDesc = "invalid";
    }
  }
};

#endif

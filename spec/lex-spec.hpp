#ifndef EAC2_LEX_SPEC_H
#define EAC2_LEX_SPEC_H

#include <vector>
#include <utility>
#include <string>

enum class TokenType {
  VARIABLE,
  KEYWORD,
  SPACE,
  INTEGER,
  STRING,
  UNKNOW
};

using LexSpec = std::vector<std::pair<std::string, TokenType>>;

const LexSpec PL0_LEX_SPEC = {
    {
        "const|var|procedure|call|begin|end|while|do|odd|if|=|,|;|:=|?|!|#|<|<=|>|>=|\\+|-|\\*|/|\\(|\\)|.",
        TokenType::KEYWORD
    },
    {
        "[a-zA-Z_][a-zA-Z_0-9]*",
        TokenType::VARIABLE
    },
    {
        "[0-9]+",
        TokenType::INTEGER
    },
    {
        "[ \n\t]+",
        TokenType::SPACE
    }
};

#endif

#ifndef EAC2_PARSER_H
#define EAC2_PARSER_H

#include "Lex.hpp"
#include "LR1.hpp"
#include "parser.hpp"
#include "pl0.hpp"

using My_Lex = Lex<Symbol>; // 生成PL/0词法分析器的class
using My_LR1 = LR1<Symbol>; // 生成PL/0 LR(1)语法分析器的class
using Parser = parser<Symbol, My_Lex, My_LR1>; // 生成编译器class

#endif

/** PL/0语言定义
Program -> Block "."
Block -> Assign Declare Procedure Statement
Assign -> "const" ident "=" number AssignRest ";"
       | ε
AssignRest -> "," ident "=" number AssignRest
           | ε
Declare -> "var" ident DeclareRest ";"
        | ε
DeclareRest -> "," ident DeclareRest
            | ε
Procedure -> "procedure" ident ";" Block ";" Procedure
          | ε
Statement -> ident ":=" Expression
          | "call" ident
          | "?" ident
          | "!" Expression
          | "begin" Statement StatementRest "end"
          | "if" Condition "then" Statement
          | "while" Condition "do" Statement
          | ε
StatementRest -> ";" Statement StatementRest
              | ε
Condition -> "odd" Expression
          | Expression RelationOp Expression
RelationOp -> "=" | "#" | "<" | "<=" | ">" | ">="
Expression -> Term ExpressionRest
            | Op1 Term ExpressionRest
ExpressionRest -> Op1 Term ExpressionRest
                | ε
Op1 -> "+" | "-"
Term -> Factor TermRest
TermRest -> Op2 Factor TermRest
          | ε
Op2 -> "*" | "/"
Factor -> ident | number | "(" Expression ")"
 */

#ifndef EAC2_PL0_HPP
#define EAC2_PL0_HPP

#include <map>
#include <string>
#include "spec.hpp"

using namespace std;

using SymbolType = Spec::SymbolType;

enum class TerminalSymbolType {
  ignore,
  none,
  eof,
  ident,
  keyword,
  number
};

enum class NonterminalSymbolType {
  Program,
  Block,
  Assign,
  AssignRest,
  Declare,
  DeclareRest,
  Procedure,
  Statement,
  StatementRest,
  Condition,
  RelationOp,
  Expression,
  ExpressionRest,
  Op1,
  Term,
  TermRest,
  Op2,
  Factor,
  None
};

using Symbol = Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>;

template <>
map<TerminalSymbolType, string> Symbol::TerminalDesc = {
    {TerminalSymbolType::ident,   "ident"},
    {TerminalSymbolType::keyword, "keyword"},
    {TerminalSymbolType::ignore,  "ignore"},
    {TerminalSymbolType::number,  "number"},
    {TerminalSymbolType::eof,     "eof"},
    {TerminalSymbolType::none,    "none"}
};

template <>
map<NonterminalSymbolType, string> Symbol::NonterminalDesc = {
    {NonterminalSymbolType::Program, "Program"},
    {NonterminalSymbolType::Block, "Block"},
    {NonterminalSymbolType::Assign, "Assign"},
    {NonterminalSymbolType::AssignRest, "AssignRest"},
    {NonterminalSymbolType::Declare, "Declare"},
    {NonterminalSymbolType::DeclareRest, "DeclareRest"},
    {NonterminalSymbolType::Procedure, "Procedure"},
    {NonterminalSymbolType::Statement, "Statement"},
    {NonterminalSymbolType::StatementRest, "StatementRest"},
    {NonterminalSymbolType::Condition, "Condition"},
    {NonterminalSymbolType::RelationOp, "RelationOp"},
    {NonterminalSymbolType::Expression, "Expression"},
    {NonterminalSymbolType::ExpressionRest, "ExpressionRest"},
    {NonterminalSymbolType::Op1, "Op1"},
    {NonterminalSymbolType::Term, "Term"},
    {NonterminalSymbolType::TermRest, "TermRest"},
    {NonterminalSymbolType::Op2, "Op2"},
    {NonterminalSymbolType::Factor, "Factor"},
    {NonterminalSymbolType::None, "None"}
};

template <>
Symbol Symbol::getPureSymbol() {
  if (
      this->type == SymbolType::TERMINAL_SYMBOL &&
      (this->t_type == TerminalSymbolType::ident ||
       this->t_type == TerminalSymbolType::number ||
       this->t_type == TerminalSymbolType::ignore)) {
    return Symbol(this->t_type);
  }
  return *this;
}

const Spec::Lex<Symbol::_TerminalSymbolType> LEX{
    {
        R"(const|var|procedure|call|begin|end|while|do|odd|if|then|=|,|;|:=|?|!|#|<|<=|>|>=|\+|-|\*|/|\(|\)|.)",
        TerminalSymbolType::keyword
    },
    {
        "[a-zA-Z_][a-zA-Z_0-9]*",
        TerminalSymbolType::ident
    },
    {
        "[0-9]+",
        TerminalSymbolType::number
    },
    {
        "[ \n\t]+",
        TerminalSymbolType::ignore
    }
};

const Symbol START_SYMBOL = Symbol(NonterminalSymbolType::Program);

const Spec::CFG<Symbol> CFG{
    {
        Symbol(NonterminalSymbolType::Program),
        {
            {
                Symbol(NonterminalSymbolType::Block),
                Symbol(TerminalSymbolType::keyword, ".")
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Block),
        {
            {
                Symbol(NonterminalSymbolType::Assign),
                Symbol(NonterminalSymbolType::Declare),
                Symbol(NonterminalSymbolType::Procedure),
                Symbol(NonterminalSymbolType::Statement)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Assign),
        {
            {
                Symbol(TerminalSymbolType::keyword, "const"),
                Symbol(TerminalSymbolType::ident),
                Symbol(TerminalSymbolType::keyword, "="),
                Symbol(TerminalSymbolType::number),
                Symbol(NonterminalSymbolType::AssignRest),
                Symbol(TerminalSymbolType::keyword, ";")
            },
            {
                Symbol(SymbolType::EPSILON)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::AssignRest),
        {
            {
                Symbol(TerminalSymbolType::keyword, ","),
                Symbol(TerminalSymbolType::ident),
                Symbol(TerminalSymbolType::keyword, "="),
                Symbol(TerminalSymbolType::number),
                Symbol(NonterminalSymbolType::AssignRest)
            },
            {
                Symbol(SymbolType::EPSILON)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Declare),
        {
            {
                Symbol(TerminalSymbolType::keyword, "var"),
                Symbol(TerminalSymbolType::ident),
                Symbol(NonterminalSymbolType::DeclareRest),
                Symbol(TerminalSymbolType::keyword, ";")
            },
            {
                Symbol(SymbolType::EPSILON)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::DeclareRest),
        {
            {
                Symbol(TerminalSymbolType::keyword, ","),
                Symbol(TerminalSymbolType::ident),
                Symbol(NonterminalSymbolType::DeclareRest)
            },
            {
                Symbol(SymbolType::EPSILON)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Procedure),
        {
            {
                Symbol(TerminalSymbolType::keyword, "procedure"),
                Symbol(TerminalSymbolType::ident),
                Symbol(TerminalSymbolType::keyword, ";"),
                Symbol(NonterminalSymbolType::Block),
                Symbol(TerminalSymbolType::keyword, ";"),
                Symbol(NonterminalSymbolType::Procedure)
            },
            {
                Symbol(SymbolType::EPSILON)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Statement),
        {
            {
                Symbol(TerminalSymbolType::ident),
                Symbol(TerminalSymbolType::keyword, ":="),
                Symbol(NonterminalSymbolType::Expression)
            },
            {
                Symbol(TerminalSymbolType::keyword, "call"),
                Symbol(TerminalSymbolType::ident)
            },
            {
                Symbol(TerminalSymbolType::keyword, "?"),
                Symbol(TerminalSymbolType::ident)
            },
            {
                Symbol(TerminalSymbolType::keyword, "!"),
                Symbol(NonterminalSymbolType::Expression)
            },
            {
                Symbol(TerminalSymbolType::keyword, "begin"),
                Symbol(NonterminalSymbolType::Statement),
                Symbol(NonterminalSymbolType::StatementRest),
                Symbol(TerminalSymbolType::keyword, "end")
            },
            {
                Symbol(TerminalSymbolType::keyword, "if"),
                Symbol(NonterminalSymbolType::Condition),
                Symbol(TerminalSymbolType::keyword, "then"),
                Symbol(NonterminalSymbolType::Statement)
            },
            {
                Symbol(TerminalSymbolType::keyword, "while"),
                Symbol(NonterminalSymbolType::Condition),
                Symbol(TerminalSymbolType::keyword, "do"),
                Symbol(NonterminalSymbolType::Statement)
            },
            {
                Symbol(SymbolType::EPSILON)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::StatementRest),
        {
            {
                Symbol(TerminalSymbolType::keyword, ";"),
                Symbol(NonterminalSymbolType::Statement),
                Symbol(NonterminalSymbolType::StatementRest)
            },
            {
                Symbol(SymbolType::EPSILON)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Condition),
        {
            {
                Symbol(TerminalSymbolType::keyword, "odd"),
                Symbol(NonterminalSymbolType::Expression)
            },
            {
                Symbol(NonterminalSymbolType::Expression),
                Symbol(NonterminalSymbolType::RelationOp),
                Symbol(NonterminalSymbolType::Expression),
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::RelationOp),
        {
            {
                Symbol(TerminalSymbolType::keyword, "=")
            },
            {
                Symbol(TerminalSymbolType::keyword, "#")
            },
            {
                Symbol(TerminalSymbolType::keyword, "<")
            },
            {
                Symbol(TerminalSymbolType::keyword, "<=")
            },
            {
                Symbol(TerminalSymbolType::keyword, ">")
            },
            {
                Symbol(TerminalSymbolType::keyword, ">=")
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Expression),
        {
            {
                Symbol(NonterminalSymbolType::Term),
                Symbol(NonterminalSymbolType::ExpressionRest)
            },
            {
                Symbol(NonterminalSymbolType::Op1),
                Symbol(NonterminalSymbolType::Term),
                Symbol(NonterminalSymbolType::ExpressionRest)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::ExpressionRest),
        {
            {
                Symbol(NonterminalSymbolType::Op1),
                Symbol(NonterminalSymbolType::Term),
                Symbol(NonterminalSymbolType::ExpressionRest)
            },
            {
                Symbol(SymbolType::EPSILON)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Op1),
        {
            {
                Symbol(TerminalSymbolType::keyword, "+")
            },
            {
                Symbol(TerminalSymbolType::keyword, "-")
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Term),
        {
            {
                Symbol(NonterminalSymbolType::Factor),
                Symbol(NonterminalSymbolType::TermRest)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::TermRest),
        {
            {
                Symbol(NonterminalSymbolType::Op2),
                Symbol(NonterminalSymbolType::Factor),
                Symbol(NonterminalSymbolType::TermRest)
            },
            {
                Symbol(SymbolType::EPSILON)
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Op2),
        {
            {
                Symbol(TerminalSymbolType::keyword, "*")
            },
            {
                Symbol(TerminalSymbolType::keyword, "/")
            }
        }
    },
    {
        Symbol(NonterminalSymbolType::Factor),
        {
            {
                Symbol(TerminalSymbolType::ident)
            },
            {
                Symbol(TerminalSymbolType::number)
            },
            {
                Symbol(TerminalSymbolType::keyword, "("),
                Symbol(NonterminalSymbolType::Expression),
                Symbol(TerminalSymbolType::keyword, ")")
            }
        }
    }
};

#endif

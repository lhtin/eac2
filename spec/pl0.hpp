#ifndef EAC2_PL0_HPP
#define EAC2_PL0_HPP

#include <vector>
#include <utility>
#include <string>
#include "spec.hpp"

using namespace std;

using SymbolType = Spec::SymbolType;

enum class TerminalSymbolType {
  ident,
  keyword,
  space,
  number,
  none,
  eof
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

const map<TerminalSymbolType, string> TokenDesc{
    {TerminalSymbolType::ident,   "ident"},
    {TerminalSymbolType::keyword, "keyword"},
    {TerminalSymbolType::space,   "space"},
    {TerminalSymbolType::number,  "number"}
};

template <typename NonterminalSymbolType, typename TerminalSymbolType>
map<TerminalSymbolType, string> Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>::TerminalDesc = {
    {TerminalSymbolType::ident,   "ident"},
    {TerminalSymbolType::keyword, "keyword"},
    {TerminalSymbolType::space,   "space"},
    {TerminalSymbolType::number,  "number"},
    {TerminalSymbolType::eof,  "eof"},
    {TerminalSymbolType::none,  "none"}
};

template <typename NonterminalSymbolType, typename TerminalSymbolType>
map<NonterminalSymbolType, string> Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>::NonterminalDesc = {

};

using Symbol = Spec::Symbol<NonterminalSymbolType, TerminalSymbolType>;

Symbol getPureSymbol (const Symbol& a) {
  if (a.type == SymbolType::TERMINAL_SYMBOL && (a.t_type == TerminalSymbolType::ident || a.t_type == TerminalSymbolType::number)) {
    return Symbol(a.t_type);
  }
  return a;
}

using Lex = Spec::Lex<TerminalSymbolType>;

const Lex PL0_LEX{
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
        TerminalSymbolType::space
    }
};

using Production = Spec::Production<Symbol>;
using ProductionList = Spec::ProductionList<Symbol>;
using CFG = Spec::CFG<Symbol>;
/*
const CFG PL0_CFG{
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
                Symbol(NonterminalSymbolType::Assign)
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
    }
};
 */

const CFG PL0_CFG{
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
        ProductionList {
            Production {
                Symbol(TerminalSymbolType::keyword, "*")
            },
            Production {
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

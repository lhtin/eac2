# EaC2

Engineering a Compiler, Second Edition

代码实现，通过定义好词法规则和语法规则，解析相应源码生成抽象语法树。

## 目录结构

```
- lex // 生成最小DFA，从RE -> NFA -> DFA -> minDFA
- spec // 定义所解析语言的词法规则和语法规则
  - spec.hpp // 定义词法规则和语法规则的模版
  - pl0 // PL/0语言
    - pl0.hpp // 定义好的PL/0词法规则和语法规则
    - test.cpp // 示例展示解析源码的程序，输出token流和ast
  - ()  // EaC2中的括号语言
  - 01  // 随意定义的语言
- src
  - Lex.hpp // 词法生成器模版类
  - LR1.hpp // LR(1)语法生成器模版类
  - parser.hpp // 整合起来的parser模版类
- utils // 帮助函数
```

## 示例

下面示例展示PL/0语言的解析。首先是定义词法和语法规则，终结符和非终结符等请看`spec/pl0/pl0.hpp`文件

词法定义:

```c++
const Lex PL0_LEX{
    {
        R"(const|var|procedure|call|begin|end|while|do|odd|if|=|,|;|:=|?|!|#|<|<=|>|>=|\+|-|\*|/|\(|\)|.)",
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
```

语法定义:

```c++
const Spec::CFG<Symbol> PL0_CFG{
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
```

定义好了之后，就可以调用写好的词法和语法分析生成器模版类、及parser解析源代码了（代码文件为`spec/pl0/test.cpp`）：

```c++
#include <iostream>
#include "utils.hpp"
#include "Lex.hpp"
#include "LR1.hpp"
#include "parser.hpp"
#include "pl0.hpp" // 定义好了终结符、非终结符、词法规则、语法规则等信息

using namespace std;

using My_Lex = Lex<Symbol>; // 生成PL/0词法分析器的class
using My_LR1 = LR1<Symbol>; // 生成PL/0 LR(1)语法分析器的class
using Parser = parser<My_Lex, My_LR1>; // 生成编译器class

int main () {
  My_Lex lex(LEX);
  My_LR1 lr1(CFG, START_SYMBOL);
  Parser c = Parser(lex, lr1);
  printNow();
  Parser::Output o = c.getOutput("test1.pl0");
  printNow();
  cout << "Tokens:\n" << o.tokens << endl;
  cout << "AST:\n" << o.ast << endl; // 输出抽象语法树
  return 0;
}
```

例如解析`spec/pl0/test1.pl0`源码的输出可以查看根目录的`output.txt`文件。



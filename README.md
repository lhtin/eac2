# EaC2

Engineering a Compiler, Second Edition

C++实现，目前可以根据定义编程语言，解析对于的源码到抽象语法树。下面示例展示PL/0语言的解析。更多内容请看`spec/pl0/pl0.hpp`文件

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
        TerminalSymbolType::space
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

定义好了之后，就可以调用写好的词法分析生成器和语法分析生成器来解析源码了：
```c++
#include <cassert>
#include <string>

#include "utils/utils.hpp"
#include "lex/lex.hpp"
#include "syntax/LR1.hpp"

#include "spec/pl0.hpp" // 定义PL/0语言的文件

using namespace std;

using My_Lex = WrapLex<Symbol>; // 生成PL/0词法分析器的class
using My_LR1 = LR1<Symbol, My_Lex>; // 生成PL/0 LR(1)语法分析器的class

int main (int argc, char *argv[]) {
  assert(argc >= 2);

  printNow();
  My_Lex lex(PL0_LEX, string(argv[1])); // 第二个参数为需要编译的源码文件路径
  My_LR1 lr1(PL0_CFG, NonterminalSymbolType::Program, lex);
  printNow();

  My_LR1::AST ast = lr1.getAST();
  cout << ast.toString() << endl; // 输出抽象语法树
  return 0;
}
```

例如解析`spec/pl0/test.pl0`源码的输出可以查看根目录的output.txt文件。



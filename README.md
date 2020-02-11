# EaC2

Engineering a Compiler, Second Edition

## PL/0

LEX:
```
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

CFG:
```
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
```


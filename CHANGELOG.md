```sh
main.ent
```

  - Updated code example to test loop + or

```sh
src/daedalus-entropia/lexer/lexer.cpp
```

  - Added `OR` token

```sh
src/daedalus-entropia/parser/ast.cpp
```

  - Added `OrExpression` class and its members and methods
  - Added `orExpression` field to `LoopExpression`, `WhileExpression` and `ForExpression`

```sh
src/include/daedalus/Entropia/parser/ast.hpp
```

  - Implemented `OrExpression` class and its methods
  - Implemented `orExpression` checks to `LoopExpression`, `WhileExpression` and `ForExpression`

```sh
src/daedalus-entropia/interpreter/interpreter.cpp
```

  - Added or case to loop evaluations

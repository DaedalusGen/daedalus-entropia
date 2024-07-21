```sh
main.ent
```

  - Updated code example to test while loop

```sh
src/daedalus-entropia/lexer/lexer.cpp
```

	- Added `WHILE` token

```sh
src/incude/daedalus/Entropia/parser/ast.hpp
```

  - Added `WhileExpression` class and its members and methods

```sh
src/daedalus-entropia/parser/ast.cpp
```

  - Implemented `WhileExpression` class and its methods

```sh
src/incude/daedalus/Entropia/parser/parser.hpp
```

  - Added `parse_while_expression` method

```sh
src/daedalus-entropia/parser/parser.cpp
```

  - Implemented `parse_while_expression` method and added it to parser config


```sh
src/incude/daedalus/Entropia/interpreter/interpreter.hpp
```

  - Added `evaluate_while_expression` method

```sh
src/daedalus-entropia/interpreter/interpreter.cpp
```

  - Implemented `evaluate_while_expression` method and added it to interpreter config

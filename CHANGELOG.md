```sh
main.ent
```

  - Updated code example to test for loop + scoping

```sh
src/daedalus-entropia/lexer/lexer.cpp
```

	- Added `FOR` token

```sh
src/incude/daedalus/Entropia/parser/ast.hpp
```

  - Added `ForExpression` class and its members and methods

```sh
src/daedalus-entropia/parser/ast.cpp
```

  - Implemented `ForExpression` class and its methods

```sh
src/incude/daedalus/Entropia/parser/parser.hpp
```

  - Added `parse_for_expression` method

```sh
src/daedalus-entropia/parser/parser.cpp
```

  - Added missing semicolon check (when parsing identifier from `parse_assignation_expression`)
  - Implemented `parse_for_expression` method and added it to parser config


```sh
src/incude/daedalus/Entropia/interpreter/interpreter.hpp
```

  - Added `evaluate_for_expression` method

```sh
src/daedalus-entropia/interpreter/interpreter.cpp
```

  - Implemented `evaluate_for_expression` method and added it to interpreter config

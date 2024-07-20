```sh
main.t
```

- Updated code to be loop + continue example

```sh
src/daedalus-entropia/lexer/lexer.cpp
```

- Added `CONTINUE` token

```sh
src/daedalus-entropia/parser/ast.hpp
```

- Added `ContinueExpression` class and its members and methods

```sh
src/daedalus-entropia/parser/ast.cpp
```

- Implemented methods for `ContinueExpression` class

```sh
src/daedalus-entropia/parser/parser.hpp
```

- Added `parse_continue_expression` method

```sh
src/daedalus-entropia/parser/parser.cpp
```

- Implemented `parse_continue_expression` method and added it to parser config

```sh
src/daedalus-entropia/interpreter/interpreter.hpp
```

- Added `CONTINUE` flag to `ValueEscapeFlags` enum
- Added `evaluate_continue_expression` method

```sh
src/daedalus-entropia/interpreter/interpreter.cpp
```

- Implemented `evaluate_continue_expression` method and added it to interpreter config
- Added support for `CONTINUE` flag to `evaluate_loop_expression` method

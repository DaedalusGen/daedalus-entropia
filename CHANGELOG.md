```sh
README.md
```

- Added missing types and operators

```sh
main.t
```

- Updated code to be loop + break example

```sh
src/daedalus-entropia/lexer/lexer.cpp
```

- Added `BREAK` token

```sh
src/daedalus-entropia/parser/ast.hpp
```

- Added `BreakExpression` class and its members and methods

```sh
src/daedalus-entropia/parser/ast.cpp
```

- Implemented methods for `BreakExpression` class

```sh
src/daedalus-entropia/parser/parser.hpp
```

- Added `parse_break_expression` method

```sh
src/daedalus-entropia/parser/parser.cpp
```

- Implemented `parse_break_expression` method and added it to parser config

```sh
src/daedalus-entropia/interpreter/interpreter.hpp
```

- Added `ValueEscapeFlags` enum to identify scope escaping expressions
- Implemented `RuntimeValueWrapper` from core instead of `RuntimeValue` to include flags
- Added `evaluate_break_expression` method

```sh
src/daedalus-entropia/interpreter/interpreter.cpp
```

- Implemented `wrap` from core to return `RuntimeValueWrapper`
- Updated `evaluate_loop_expression` method to check for break flags using `flag_contains` from core
- Implemented `evaluate_break_expression` method and added it to interpreter config
- Added flag forwarding and break escaping to `evaluate_conditionnal_structure` method

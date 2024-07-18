```sh
CHANGELOG.md
```

- Created changelog

```sh
main.t
```

- Updated source code to test loops and comparisons

```sh
src/daedalus-entropia/lexer/lexer.hpp
```

- Reordered includes

```sh
src/daedalus-entropia/lexer/lexer.cpp
```

  - Added token `if`
  - Added token `else`
  - Added token `==`
  - Added token `!=`
  - Added token `<=`
  - Added token `>=`
  - Added token `<`
  - Added token `>`
  - Removed auto includes

```sh
src/daedalus-entropia/parser/ast.hpp
```

  - Added class ConditionnalExpression and its members/methods
  - Added class ConditionnalStructure and its members/methods
  - Reordered includes

```sh
src/daedalus-entropia/parser/ast.cpp
```

  - Implemented methods for the ConditionnalExpression class
  - Implemented methods for the ConditionnalStructure class
  - Updated the `repr` method for the BinaryExpression class
  - Removed auto includes

```sh
src/daedalus-entropia/parser/parser.hpp
```

  - Added needsSemicolon to all parsing methods
  - Added method `parse_conditionnal_expression`
  - Added method `parse_conditionnal_structure`

```sh
src/daedalus-entropia/parser/parser.cpp
```

  - Updated all parsing methods' implementation to use needsSemicolon
  - Added method `parse_conditionnal_structure` to parser configuration
  - Added new aforementioned logical operators to `parse_logical_expression`'s impementation
  - Implemented method `parse_conditionnal_expression`
  - Implemented method `parse_conditionnal_structure`
  - Removed auto includes

```sh
src/daedalus-entropia/interpreter/interpreter.hpp
```

  - Added method `evaluate_conditionnal_structure`
  - Reordered includes

```sh
src/daedalus-entropia/interpreter/interpreter.cpp
```

  - Added new aforementioned logical operators to `evaluate_binary_expression`
  - Implemented method `evaluate_conditionnal_structure`
  - Added method `evaluate_conditionnal_structure` to interpreter configuration
  - Removed auto includes

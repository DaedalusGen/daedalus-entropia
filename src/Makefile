default: run

build:
	g++ -I..daedalus-core/src/include .\langgen.dll .\Tlang\lexer\lexer.cpp .\Tlang\parser\ast.cpp .\Tlang\parser\parser.cpp .\Tlang\interpreter\env.cpp .\Tlang\interpreter\values.cpp .\Tlang\interpreter\interpreter.cpp main.cpp -o tlang

build_for_run:
	g++ -I../daedalus-core/src/include-Iinclude -Wall -Wpedantic -Werror langgen/lexer/lexer.cpp langgen/parser/ast.cpp langgen/parser/parser.cpp langgen/interpreter/values.cpp langgen/interpreter/env.cpp langgen/interpreter/interpreter.cpp main.cpp -o lg

run: build
	./tlang
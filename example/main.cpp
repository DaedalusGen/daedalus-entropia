#include <iostream>
#include <string>
#include <vector>

#include <AquIce/Tlang/core.hpp>

int main(int argc, char** argv) {

	std::string src = "const i: boolean = true && false || !false i = false";
	// std::string src = "3 + 100 * .2 / 1 - 2";
	// std::string src = "(3 + 100 * .2) / (1 - 2)";

	// * LEXER

	daedalus::lexer::Lexer lexer;

	setup_lexer(lexer);

	std::vector<daedalus::lexer::Token> tokens;

	std::cout << src << std::endl;

	daedalus::lexer::lex(
		lexer,
		tokens,
		src
	);

	for(const daedalus::lexer::Token& token : tokens) {
		std::cout << repr(token) << "\n\n";
	}

	// * PARSER

	daedalus::parser::Parser parser;

	setup_parser(parser);

	std::shared_ptr<daedalus::ast::Scope> program = std::make_shared<daedalus::ast::Scope>();

	daedalus::parser::parse(
		parser,
		program,
		tokens
	);

	std::cout << program->repr() << std::endl;

	// * INTERPRETER

	daedalus::interpreter::Interpreter interpreter;

	setup_interpreter(interpreter);

	std::unordered_map<std::string, std::string> results;

	daedalus::interpreter::interpret(
		interpreter,
		results,
		program
	);

	for(const auto& [node, result] : results) {
		std::cout << node << " -> " << result << std::endl;
	}

	return 0;
}
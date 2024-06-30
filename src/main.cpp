#include <iostream>
#include <string>
#include <vector>

#include <AquIce/daedalus/core.hpp>
#include <AquIce/Tlang/core.hpp>

int main(int argc, char** argv) {

	daedalus::Daedalus daedalusConfig = daedalus::setup_daedalus(
		&setup_lexer,
		&setup_parser,
		&setup_interpreter
	);

	std::string src = "let i: i32 = (12 - (-2 - 1)) * 4";
	// std::string src = "3 + 100 * .2 / 1 - 2";
	// std::string src = "(3 + 100 * .2) / (1 - 2)";

	// * LEXER

	std::vector<daedalus::lexer::Token> tokens;

	std::cout << src << std::endl;

	daedalus::lexer::lex(
		daedalusConfig.lexer,
		tokens,
		src
	);

	for(const daedalus::lexer::Token& token : tokens) {
		std::cout << repr(token) << "\n\n";
	}

	// * PARSER

	std::shared_ptr<daedalus::ast::Scope> program = std::make_shared<daedalus::ast::Scope>();

	daedalus::parser::parse(
		daedalusConfig.parser,
		program,
		tokens
	);

	std::cout << "PROGRAM " << program->repr() << std::endl;

	// * INTERPRETER

	std::unordered_map<std::string, std::string> results;

	daedalus::interpreter::interpret(
		daedalusConfig.interpreter,
		results,
		program
	);

	for(const auto& [node, result] : results) {
		std::cout << node << " -> " << result << std::endl;
	}

	return 0;
}
#include <AquIce/Tlang/core.hpp>

#include <AquIce/daedalus/core.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <stdexcept>

int main(int argc, char** argv) {

	DAE_ASSERT_TRUE(
		argc == 2,
		std::runtime_error("Invalid number of arguments\nUsage: tlang <filename>: ")
	)

	std::string filename = argv[1];

	DAE_ASSERT_TRUE(
		(&filename)->rfind(".t") != std::string::npos,
		std::runtime_error("Invalid file format")
	)

	std::ifstream file = std::ifstream(argv[1]);
	std::string src = std::string(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);

	daedalus::Daedalus daedalusConfig = daedalus::setup_daedalus(
		&setup_lexer,
		&setup_parser,
		&setup_interpreter
	);

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

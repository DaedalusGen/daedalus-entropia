#include <daedalus/Entropia/core.hpp>

#include <daedalus/core/core.hpp>

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

	daedalus::core::Daedalus daedalusConfig = daedalus::core::setup_daedalus(
		&setup_lexer,
		&setup_parser,
		&setup_interpreter
	);

	// * LEXER

	std::vector<daedalus::core::lexer::Token> tokens;

	std::cout << src << std::endl;

	daedalus::core::lexer::lex(
		daedalusConfig.lexer,
		tokens,
		src
	);

	for(const daedalus::core::lexer::Token& token : tokens) {
		std::cout << repr(token) << "\n\n";
	}

	// * PARSER

	std::shared_ptr<daedalus::core::ast::Scope> program = std::make_shared<daedalus::core::ast::Scope>();

	daedalus::core::parser::parse(
		daedalusConfig.parser,
		program,
		tokens
	);

	std::cout << "PROGRAM " << program->repr() << std::endl;

	// * INTERPRETER

	std::unordered_map<std::string, std::string> results;

	daedalus::core::interpreter::interpret(
		daedalusConfig.interpreter,
		results,
		program
	);

	for(const auto& [node, result] : results) {
		std::cout << node << " -> " << result << std::endl;
	}

	return 0;
}

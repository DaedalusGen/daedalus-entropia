#include <daedalus/Entropia/core.hpp>

#include <daedalus/core/core.hpp>

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char** argv) {

	DAE_ASSERT_TRUE(
		argc == 2,
		std::runtime_error("Invalid number of arguments\nUsage: tlang <filename>: ")
	)

	std::string filename = argv[1];

	DAE_ASSERT_TRUE(
		(&filename)->rfind(".ent") != std::string::npos,
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

	DAE_DEBUG_LOG(src)

	daedalus::core::lexer::lex(
		daedalusConfig.lexer,
		tokens,
		src
	);

	DAE_DEBUG({
	    for(const daedalus::core::lexer::Token& token : tokens) {
			DAE_DEBUG_LOG(repr(token))
		}
	})

	// * PARSER

	std::shared_ptr<daedalus::core::ast::Scope> program = std::make_shared<daedalus::core::ast::Scope>();

	daedalus::core::parser::parse(
		daedalusConfig.parser,
		program,
		tokens
	);

	DAE_DEBUG_LOG("PROGRAM " + program->repr())

	// * INTERPRETER

	std::vector<daedalus::core::interpreter::RuntimeResult> results;

	daedalus::core::interpreter::interpret(
		daedalusConfig.interpreter,
		results,
		program
	);

	DAE_DEBUG({
    	for(const auto& [node, result] : results) {
    	    DAE_DEBUG_LOG(node + " -> " + result);
    	}
	})

	return 0;
}

#ifndef __T_PARSER__
#define __T_PARSER__

#include <AquIce/daedalus/parser.hpp>
#include <AquIce/daedalus/unit.hpp>

#include "ast.hpp"

#include <unordered_map>
#include <limits>
#include <float.h>

void setup_parser(daedalus::parser::Parser& parser);

namespace tlang {
	namespace parser {
		extern std::unordered_map<std::string, std::string> identifiers;

		std::shared_ptr<daedalus::ast::Expression> parse_identifier(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_boolean_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_parenthesis_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_unary_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_multiplicative_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_additive_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_logical_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_binary_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Statement> parse_assignation_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Statement> parse_declaration_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens);
	}
}

#endif // __T_PARSER__
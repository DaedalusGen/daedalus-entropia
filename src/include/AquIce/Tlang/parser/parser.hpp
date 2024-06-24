#ifndef __T_PARSER__
#define __T_PARSER__

#include <AquIce/daedalus/parser.hpp>

#include "ast.hpp"

#include <limits>

void setup_parser(daedalus::parser::Parser& parser);

namespace tlang {
	namespace parser {
		std::shared_ptr<daedalus::ast::Expression> parse_identifier(std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_boolean_expression(std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_unary_expression(std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_multiplicative_expression(std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_additive_expression(std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_logical_expression(std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Expression> parse_binary_expression(std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Statement> parse_assignation_expression(std::vector<daedalus::lexer::Token>& tokens);

		std::shared_ptr<daedalus::ast::Statement> parse_declaration_expression(std::vector<daedalus::lexer::Token>& tokens);
	}
}

#endif // __T_PARSER__
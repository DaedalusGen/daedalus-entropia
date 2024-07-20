#ifndef __DAEDALUS_ENTROPIA_PARSER__
#define __DAEDALUS_ENTROPIA_PARSER__

#include <daedalus/Entropia/parser/ast.hpp>

#include <daedalus/core/parser/parser.hpp>
#include <daedalus/core/tools/assert.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <float.h>

void setup_parser(daedalus::core::parser::Parser& parser);

char get_char(std::string src);

namespace daedalus {
    namespace entropia {
    	namespace parser {
    		extern std::unordered_map<std::string, std::string> identifiers;

    		std::shared_ptr<daedalus::core::ast::Expression> parse_identifier(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_boolean_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_char_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_str_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_parenthesis_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_unary_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_multiplicative_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_additive_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_logical_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_binary_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_assignation_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

    		std::shared_ptr<daedalus::core::ast::Expression> parse_declaration_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

            std::shared_ptr<daedalus::core::ast::Expression> parse_loop_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

            std::shared_ptr<daedalus::core::ast::Expression> parse_break_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);

            std::shared_ptr<daedalus::core::ast::Expression> parse_conditionnal_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, std::shared_ptr<daedalus::entropia::ast::ConditionnalExpression> before = nullptr);

            std::shared_ptr<daedalus::core::ast::Expression> parse_conditionnal_structure(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon);
    	}
    }
}

#endif // __DAEDALUS_ENTROPIA_PARSER__

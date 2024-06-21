#include <AquIce/Tlang/parser/parser.hpp>

void setup_parser(daedalus::parser::Parser& parser) {

	daedalus::parser::setup_parser(
		parser,
		{
			{
				"Identifier",
				daedalus::parser::make_node(&tlang::parser::parse_identifier, false)
			},
			{
				"BooleanExpression",
				daedalus::parser::make_node(&tlang::parser::parse_boolean_expression, false)
			},
			{
				"UnaryExpression",
				daedalus::parser::make_node(&tlang::parser::parse_unary_expression, false)
			},
			{
				"BinaryExpression",
				daedalus::parser::make_node(&tlang::parser::parse_binary_expression, false)
			},
			{
				"AssignationExpression",
				daedalus::parser::make_node(&tlang::parser::parse_assignation_expression, false)
			},
			{
				"DeclarationExpression",
				daedalus::parser::make_node(&tlang::parser::parse_declaration_expression)
			},
		}
	);

	daedalus::parser::demoteTopNode(parser, "NumberExpression");
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_identifier(std::vector<daedalus::lexer::Token>& tokens) {
	if(peek(tokens).type == "IDENTIFIER") {
		return std::make_shared<tlang::ast::Identifier>(eat(tokens).value);
	}
	return daedalus::parser::parse_number_expression(tokens);
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_boolean_expression(std::vector<daedalus::lexer::Token>& tokens) {
	if(peek(tokens).type == "BOOL") {
		return std::make_shared<tlang::ast::BooleanExpression>(eat(tokens).value == "true");
	}
	return tlang::parser::parse_identifier(tokens);
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_unary_expression(std::vector<daedalus::lexer::Token>& tokens) {

	std::string operator_symbol = "";

	if(peek(tokens).type == "UNARY_OPERATOR") {
		operator_symbol = eat(tokens).value;
	}
	
	std::shared_ptr<daedalus::ast::Expression> term = tlang::parser::parse_boolean_expression(tokens);

	return operator_symbol.size() == 0 ? term : std::make_shared<tlang::ast::UnaryExpression>(term, operator_symbol);
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_multiplicative_expression(std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = tlang::parser::parse_unary_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "*" || peek(tokens).value == "/")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::ast::Expression> right = tlang::parser::parse_multiplicative_expression(tokens);
		
		return std::make_shared<tlang::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_additive_expression(std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = tlang::parser::parse_multiplicative_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "+" || peek(tokens).value == "-")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::ast::Expression> right = tlang::parser::parse_additive_expression(tokens);
		
		return std::make_shared<tlang::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_logical_expression(std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = tlang::parser::parse_additive_expression(tokens);

	if(left == nullptr) {
		return nullptr;
	}

	if(
		peek(tokens).type == "OPERATOR" &&
		(peek(tokens).value == "&&" || peek(tokens).value == "||")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::ast::Expression> right = tlang::parser::parse_logical_expression(tokens);
		
		return std::make_shared<tlang::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_binary_expression(std::vector<daedalus::lexer::Token>& tokens) {
	return tlang::parser::parse_logical_expression(tokens);
}

std::shared_ptr<daedalus::ast::Statement> tlang::parser::parse_assignation_expression(std::vector<daedalus::lexer::Token>& tokens) {
	
	std::shared_ptr<daedalus::ast::Expression> pseudoIdentifier = tlang::parser::parse_identifier(tokens);

	if(pseudoIdentifier->type() != "Identifier") {
		return tlang::parser::parse_binary_expression(tokens);
	}

	std::shared_ptr<tlang::ast::Identifier> identifier = std::dynamic_pointer_cast<tlang::ast::Identifier>(pseudoIdentifier);

	if(peek(tokens).type != "ASSIGN") {
		return tlang::parser::parse_identifier(tokens);
	}
	(void)eat(tokens);

	std::shared_ptr<daedalus::ast::Expression> expression = tlang::parser::parse_binary_expression(tokens);

	return std::make_shared<tlang::ast::AssignationExpression>(identifier, expression);
}

std::shared_ptr<daedalus::ast::Statement> tlang::parser::parse_declaration_expression(std::vector<daedalus::lexer::Token>& tokens) {
	if(peek(tokens).type != "ASSIGN_KEYWORD") {
		return parse_assignation_expression(tokens);
	}

	bool isMutable = eat(tokens).value == "let";
	
	std::shared_ptr<daedalus::ast::Expression> pseudoIdentifier = tlang::parser::parse_identifier(tokens);

	if(pseudoIdentifier->type() != "Identifier") {
		throw std::runtime_error("Expected identifier");
	}

	std::shared_ptr<tlang::ast::Identifier> identifier = std::dynamic_pointer_cast<tlang::ast::Identifier>(pseudoIdentifier);

	if(peek(tokens).type != "COLON") {
		throw std::runtime_error("Expected colon");
	}
	(void)eat(tokens);

	if(peek(tokens).type != "TYPE") {
		throw std::runtime_error("Expected type specifier");
	}
	std::string type = eat(tokens).value;

	if(peek(tokens).type != "ASSIGN") {
		throw std::runtime_error("Expected assignment symbol");
	}
	(void)eat(tokens);

	std::shared_ptr<daedalus::ast::Expression> expression = tlang::parser::parse_binary_expression(tokens);

	return std::make_shared<tlang::ast::DeclarationExpression>(identifier, expression, type, isMutable);
}
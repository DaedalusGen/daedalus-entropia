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

std::unordered_map<std::string, std::string> tlang::parser::identifiers = std::unordered_map<std::string, std::string>();

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_identifier(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {
	if(peek(tokens).type == "IDENTIFIER") {
		return std::make_shared<tlang::ast::Identifier>(eat(tokens).value);
	}
	return daedalus::parser::parse_number_expression(parser, tokens);
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_boolean_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {
	if(peek(tokens).type == "BOOL") {
		return std::make_shared<tlang::ast::BooleanExpression>(eat(tokens).value == "true");
	}
	return tlang::parser::parse_identifier(parser, tokens);
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_parenthesis_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {
	if(peek(tokens).type != "OPEN_PAREN") {
		return parse_boolean_expression(parser, tokens);
	}
	(void)eat(tokens);

	std::shared_ptr<daedalus::ast::Statement> parenthesisStatement = daedalus::parser::parse_statement(parser, tokens);

	(void)expect(tokens, "CLOSE_PAREN", std::runtime_error("Expected closed parenthesis"));

	if(std::shared_ptr<daedalus::ast::Expression> parenthesisExpression = std::dynamic_pointer_cast<daedalus::ast::Expression>(parenthesisStatement)) {
		return parenthesisExpression;
	}
	
	throw std::runtime_error("Invalid parenthesis expression");
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_unary_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {

	std::string operator_symbol = "";

	if(peek(tokens).type == "UNARY_OPERATOR") {
		operator_symbol = eat(tokens).value;
		DAE_ASSERT_TRUE(
			operator_symbol == "!",
			std::runtime_error("Invalid unary operator")
		)
	} else if(peek(tokens).value == "-") {
		(void)eat(tokens);
		std::shared_ptr<daedalus::ast::Expression> term = parse_parenthesis_expression(parser, tokens)->get_constexpr();
		if(term->type() == "Identifier") {
			return std::make_shared<tlang::ast::BinaryExpression>(
				std::make_shared<daedalus::ast::NumberExpression>(0),
				"-",
				term
			);
		}
		DAE_ASSERT_TRUE(
			term->type() == "NumberExpression",
			std::runtime_error("Invalid or not supported negative term")
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(term);
		numberExpression->value *= -1;

		return numberExpression;
	}
	
	std::shared_ptr<daedalus::ast::Expression> term = tlang::parser::parse_parenthesis_expression(parser, tokens);

	return operator_symbol.size() == 0 ? term : std::make_shared<tlang::ast::UnaryExpression>(term, operator_symbol);
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_multiplicative_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = tlang::parser::parse_unary_expression(parser, tokens);

	if(
		peek(tokens).type == "BINARY_OPERATOR" &&
		(peek(tokens).value == "*" || peek(tokens).value == "/")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::ast::Expression> right = tlang::parser::parse_multiplicative_expression(parser, tokens);
		
		return std::make_shared<tlang::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_additive_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = tlang::parser::parse_multiplicative_expression(parser, tokens);

	if(
		peek(tokens).type == "BINARY_OPERATOR" &&
		(peek(tokens).value == "+" || peek(tokens).value == "-")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::ast::Expression> right = tlang::parser::parse_additive_expression(parser, tokens);
		
		return std::make_shared<tlang::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_logical_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {
	std::shared_ptr<daedalus::ast::Expression> left = tlang::parser::parse_additive_expression(parser, tokens);

	if(
		peek(tokens).type == "BINARY_OPERATOR" &&
		(peek(tokens).value == "&&" || peek(tokens).value == "||")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::ast::Expression> right = tlang::parser::parse_logical_expression(parser, tokens);
		
		return std::make_shared<tlang::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}
	
	return left;
}

std::shared_ptr<daedalus::ast::Expression> tlang::parser::parse_binary_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {
	return tlang::parser::parse_logical_expression(parser, tokens);
}

std::shared_ptr<daedalus::ast::Statement> tlang::parser::parse_assignation_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {
	
	std::shared_ptr<daedalus::ast::Expression> pseudoIdentifier = tlang::parser::parse_binary_expression(parser, tokens);

	if(pseudoIdentifier->type() != "Identifier") {
		return pseudoIdentifier;
	}

	std::shared_ptr<tlang::ast::Identifier> identifier = std::dynamic_pointer_cast<tlang::ast::Identifier>(pseudoIdentifier);

	if(peek(tokens).type != "ASSIGN") {
		return identifier;
	}
	(void)eat(tokens);

	std::shared_ptr<daedalus::ast::Expression> expression = tlang::parser::parse_binary_expression(parser, tokens);

	if(std::shared_ptr<tlang::ast::ContainerExpression> containerExpression = std::dynamic_pointer_cast<tlang::ast::ContainerExpression>(expression)) {
		if(std::shared_ptr<tlang::ast::Identifier> source_identifier = std::dynamic_pointer_cast<tlang::ast::ContainerExpression>(expression)->contains_identifier()) {
			return std::make_shared<tlang::ast::AssignationExpression>(identifier, expression);
		}
	}
	
	std::string type = tlang::parser::identifiers.at(identifier->get_name());

	if(type == "i8") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid i8 value (" + std::to_string(_I8_MIN) + " - " + std::to_string(_I8_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= _I8_MIN && numberExpression->value <= _I8_MAX),
			std::runtime_error("Expected valid i8 value (" + std::to_string(_I8_MIN) + " - " + std::to_string(_I8_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "i16") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid i16 value (" + std::to_string(_I16_MIN) + " - " + std::to_string(_I16_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= _I16_MIN && numberExpression->value <= _I16_MAX),
			std::runtime_error("Expected valid i16 value (" + std::to_string(_I16_MIN) + " - " + std::to_string(_I16_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "i32") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid i32 value (" + std::to_string(_I32_MIN) + " - " + std::to_string(_I32_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= _I32_MIN && numberExpression->value <= _I32_MAX),
			std::runtime_error("Expected valid i32 value (" + std::to_string(_I32_MIN) + " - " + std::to_string(_I32_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "i64") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid i64 value (" + std::to_string(_I64_MIN) + " - " + std::to_string(_I64_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= _I64_MIN && numberExpression->value <= _I64_MAX),
			std::runtime_error("Expected valid i64 value (" + std::to_string(_I64_MIN) + " - " + std::to_string(_I64_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "u8") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid u8 value (0 - " + std::to_string(_UI8_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= 0 && numberExpression->value <= _UI8_MAX),
			std::runtime_error("Expected valid u8 value (0 - " + std::to_string(_UI8_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "u16") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid u16 value (0 - " + std::to_string(_UI16_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= 0 && numberExpression->value <= _UI16_MAX),
			std::runtime_error("Expected valid u16 value (0 - " + std::to_string(_UI16_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "u32") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid u32 value (0 - " + std::to_string(_UI32_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= 0 && numberExpression->value <= _UI32_MAX),
			std::runtime_error("Expected valid u32 value (0 - " + std::to_string(_UI32_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "u64") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid u64 value (0 - " + std::to_string(_UI64_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= 0 && numberExpression->value <= _UI64_MAX),
			std::runtime_error("Expected valid u64 value (0 - " + std::to_string(_UI64_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "f32") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid f32 value (" + std::to_string(FLT_MIN) + " - " + std::to_string(FLT_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= FLT_MIN && numberExpression->value <= FLT_MAX),
			std::runtime_error("Expected valid u64 value (" + std::to_string(FLT_MIN) + " - " + std::to_string(FLT_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "f64") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid f64 value (" + std::to_string(DBL_MIN) + " - " + std::to_string(DBL_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= DBL_MIN && numberExpression->value <= DBL_MAX),
			std::runtime_error("Expected valid u64 value (" + std::to_string(DBL_MIN) + " - " + std::to_string(DBL_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "bool") {
		DAE_ASSERT_TRUE(
			expression->type() == "BooleanExpression",
			std::runtime_error("Expected valid boolean value (true / false), got " + expression->repr())
		)
	}

	return std::make_shared<tlang::ast::AssignationExpression>(identifier, expression);
}

std::shared_ptr<daedalus::ast::Statement> tlang::parser::parse_declaration_expression(daedalus::parser::Parser& parser, std::vector<daedalus::lexer::Token>& tokens) {

	if(peek(tokens).type != "DECLARE_KEYWORD") {
		return parse_assignation_expression(parser, tokens);
	}

	bool isMutable = eat(tokens).value == "let";
	
	std::shared_ptr<daedalus::ast::Expression> pseudoIdentifier = tlang::parser::parse_identifier(parser, tokens);

	DAE_ASSERT_TRUE(
		pseudoIdentifier->type() == "Identifier",
		std::runtime_error("Expected identifier")
	)

	std::shared_ptr<tlang::ast::Identifier> identifier = std::dynamic_pointer_cast<tlang::ast::Identifier>(pseudoIdentifier);

	(void)expect(tokens, "COLON", std::runtime_error("Expected colon"));

	std::string type = expect(tokens, "TYPE", std::runtime_error("Expected type specifier")).value;

	(void)expect(tokens, "ASSIGN", std::runtime_error("Expected assignment symbol"));

	std::shared_ptr<daedalus::ast::Expression> expression = tlang::parser::parse_binary_expression(parser, tokens)->get_constexpr();

	if(std::shared_ptr<tlang::ast::ContainerExpression> containerExpression = std::dynamic_pointer_cast<tlang::ast::ContainerExpression>(expression)) {
		if(std::shared_ptr<tlang::ast::Identifier> source_identifier = std::dynamic_pointer_cast<tlang::ast::ContainerExpression>(expression)->contains_identifier()) {
			tlang::parser::identifiers[identifier->get_name()] = tlang::parser::identifiers.at(source_identifier->get_name());
			return std::make_shared<tlang::ast::DeclarationExpression>(identifier, expression, type, isMutable);
		}
	}

	if(type == "i8") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid i8 value (" + std::to_string(_I8_MIN) + " - " + std::to_string(_I8_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= _I8_MIN && numberExpression->value <= _I8_MAX),
			std::runtime_error("Expected valid i8 value (" + std::to_string(_I8_MIN) + " - " + std::to_string(_I8_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "i16") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid i16 value (" + std::to_string(_I16_MIN) + " - " + std::to_string(_I16_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= _I16_MIN && numberExpression->value <= _I16_MAX),
			std::runtime_error("Expected valid i16 value (" + std::to_string(_I16_MIN) + " - " + std::to_string(_I16_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "i32") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid i32 value (" + std::to_string(_I32_MIN) + " - " + std::to_string(_I32_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= _I32_MIN && numberExpression->value <= _I32_MAX),
			std::runtime_error("Expected valid i32 value (" + std::to_string(_I32_MIN) + " - " + std::to_string(_I32_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "i64") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid i64 value (" + std::to_string(_I64_MIN) + " - " + std::to_string(_I64_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= _I64_MIN && numberExpression->value <= _I64_MAX),
			std::runtime_error("Expected valid i64 value (" + std::to_string(_I64_MIN) + " - " + std::to_string(_I64_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "u8") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid u8 value (0 - " + std::to_string(_UI8_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= 0 && numberExpression->value <= _UI8_MAX),
			std::runtime_error("Expected valid u8 value (0 - " + std::to_string(_UI8_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "u16") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid u16 value (0 - " + std::to_string(_UI16_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= 0 && numberExpression->value <= _UI16_MAX),
			std::runtime_error("Expected valid u16 value (0 - " + std::to_string(_UI16_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "u32") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid u32 value (0 - " + std::to_string(_UI32_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= 0 && numberExpression->value <= _UI32_MAX),
			std::runtime_error("Expected valid u32 value (0 - " + std::to_string(_UI32_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "u64") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid u64 value (0 - " + std::to_string(_UI64_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= 0 && numberExpression->value <= _UI64_MAX),
			std::runtime_error("Expected valid u64 value (0 - " + std::to_string(_UI64_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "f32") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid f32 value (" + std::to_string(FLT_MIN) + " - " + std::to_string(FLT_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= FLT_MIN && numberExpression->value <= FLT_MAX),
			std::runtime_error("Expected valid u64 value (" + std::to_string(FLT_MIN) + " - " + std::to_string(FLT_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "f64") {
		DAE_ASSERT_TRUE(
			expression->type() == "NumberExpression",
			std::runtime_error("Expected valid f64 value (" + std::to_string(DBL_MIN) + " - " + std::to_string(DBL_MAX) + "), got " + expression->repr())
		)
		std::shared_ptr<daedalus::ast::NumberExpression> numberExpression = std::dynamic_pointer_cast<daedalus::ast::NumberExpression>(expression);
		DAE_ASSERT_TRUE(
			(numberExpression->value >= DBL_MIN && numberExpression->value <= DBL_MAX),
			std::runtime_error("Expected valid u64 value (" + std::to_string(DBL_MIN) + " - " + std::to_string(DBL_MAX) + "), got " + std::to_string(numberExpression->value))
		)
	}
	else if(type == "bool") {
		DAE_ASSERT_TRUE(
			expression->type() == "BooleanExpression",
			std::runtime_error("Expected valid boolean value (true / false), got " + expression->repr())
		)
	}

	tlang::parser::identifiers[identifier->get_name()] = type;
	return std::make_shared<tlang::ast::DeclarationExpression>(identifier, expression, type, isMutable);
}
#include <daedalus/Entropia/parser/parser.hpp>

void setup_parser(daedalus::core::parser::Parser& parser) {

	daedalus::core::parser::setup_parser(
		parser,
		{
			{
				"Identifier",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_identifier, false)
			},
			{
				"BooleanExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_boolean_expression, false)
			},
			{
				"CharExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_char_expression, false)
			},
			{
				"StrExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_str_expression, false)
			},
			{
				"UnaryExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_unary_expression, false)
			},
			{
				"BinaryExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_binary_expression, false)
			},
			{
				"AssignationExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_assignation_expression, false)
			},
			{
				"DeclarationExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_declaration_expression, false)
			},
			{
			    "LoopExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_loop_expression, false)
			},
			{
			    "WhileExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_while_expression, false)
			},
			{
			    "BreakExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_break_expression, false)
			},
			{
			    "ContinueExpression",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_continue_expression, false)
			},
			{
			    "ConditionnalStructure",
				daedalus::core::parser::make_node(&daedalus::entropia::parser::parse_conditionnal_structure)
			}
		}
	);

	daedalus::core::parser::demoteTopNode(parser, "NumberExpression");
}

char get_char(std::string src) {
	if(src.at(0) == '\\') {
		switch(src.at(1)) {
			case 'n':
				return '\n';
			case 't':
				return '\t';
			case 'r':
				return '\r';
			case '\'':
				return '\'';
			case '\\':
				return '\\';
			default:
				throw std::runtime_error("Invalid escape sequence (should not have been validated by lexer)");
		}
	}
	return src.at(0);
}

std::unordered_map<std::string, std::string> daedalus::entropia::parser::identifiers = std::unordered_map<std::string, std::string>();

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_identifier(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	if(peek(tokens).type == "IDENTIFIER") {
		return std::make_shared<daedalus::entropia::ast::Identifier>(eat(tokens).value);
	}
	return daedalus::core::parser::parse_number_expression(parser, tokens, needsSemicolon);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_boolean_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	if(peek(tokens).type == "BOOL") {
		return std::make_shared<daedalus::entropia::ast::BooleanExpression>(eat(tokens).value == "true");
	}
	return daedalus::entropia::parser::parse_identifier(parser, tokens, needsSemicolon);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_char_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	if(peek(tokens).type == "CHAR") {
		std::string value = eat(tokens).value;
		return std::make_shared<daedalus::entropia::ast::CharExpression>(get_char(value.substr(1, value.length() - 2)));
	}
	return daedalus::entropia::parser::parse_boolean_expression(parser, tokens, needsSemicolon);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_str_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	if(peek(tokens).type == "STR") {
		std::string str = "";
		size_t i = 0;
		std::string value = eat(tokens).value;
		value = value.substr(1, value.length() - 2);
		while(i < value.length()) {
			char c = get_char(value.substr(i));
			i += value.at(i) == '\\' ? 2 : 1;
			str += c;
		}
		return std::make_shared<daedalus::entropia::ast::StrExpression>(str);
	}
	return daedalus::entropia::parser::parse_char_expression(parser, tokens, needsSemicolon);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_parenthesis_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	if(peek(tokens).type != "OPEN_PAREN") {
		return parse_str_expression(parser, tokens, needsSemicolon);
	}
	(void)eat(tokens);

	std::shared_ptr<daedalus::core::ast::Statement> parenthesisStatement = daedalus::core::parser::parse_expression(parser, tokens, needsSemicolon);

	(void)expect(tokens, "CLOSE_PAREN", std::runtime_error("Expected closed parenthesis"));

	if(std::shared_ptr<daedalus::core::ast::Expression> parenthesisExpression = std::dynamic_pointer_cast<daedalus::core::ast::Expression>(parenthesisStatement)) {
	    return parenthesisExpression;
	}

	throw std::runtime_error("Invalid parenthesis expression");
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_unary_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {

	std::string operator_symbol = "";

	if(peek(tokens).type == "UNARY_OPERATOR") {
		operator_symbol = eat(tokens).value;
		DAE_ASSERT_TRUE(
			operator_symbol == "!",
			std::runtime_error("Invalid unary operator")
		)
	} else if(peek(tokens).value == "-") {
		(void)eat(tokens);
		std::shared_ptr<daedalus::core::ast::Expression> term = parse_parenthesis_expression(parser, tokens, needsSemicolon)->get_constexpr();
		if(term->type() == "Identifier") {
			return std::make_shared<daedalus::entropia::ast::BinaryExpression>(
				std::make_shared<daedalus::core::ast::NumberExpression>(0),
				"-",
				term
			);
		}
		DAE_ASSERT_TRUE(
			term->type() == "NumberExpression",
			std::runtime_error("Invalid or not supported negative term")
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(term);
		numberExpression->set_value(-numberExpression->get_value());

		return numberExpression;
	}

	std::shared_ptr<daedalus::core::ast::Expression> term = daedalus::entropia::parser::parse_parenthesis_expression(parser, tokens, needsSemicolon);

	return operator_symbol.size() == 0 ? term : std::make_shared<daedalus::entropia::ast::UnaryExpression>(term, operator_symbol);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_multiplicative_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	std::shared_ptr<daedalus::core::ast::Expression> left = daedalus::entropia::parser::parse_unary_expression(parser, tokens, needsSemicolon);

	if(
		peek(tokens).type == "BINARY_OPERATOR" &&
		(peek(tokens).value == "*" || peek(tokens).value == "/")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::core::ast::Expression> right = daedalus::entropia::parser::parse_multiplicative_expression(parser, tokens, needsSemicolon);

		return std::make_shared<daedalus::entropia::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}

	return left;
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_additive_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	std::shared_ptr<daedalus::core::ast::Expression> left = daedalus::entropia::parser::parse_multiplicative_expression(parser, tokens, needsSemicolon);

	if(
		peek(tokens).type == "BINARY_OPERATOR" &&
		(peek(tokens).value == "+" || peek(tokens).value == "-")
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::core::ast::Expression> right = daedalus::entropia::parser::parse_additive_expression(parser, tokens, needsSemicolon);

		return std::make_shared<daedalus::entropia::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}

	return left;
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_logical_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	std::shared_ptr<daedalus::core::ast::Expression> left = daedalus::entropia::parser::parse_additive_expression(parser, tokens, needsSemicolon);

	if(
		peek(tokens).type == "BINARY_OPERATOR" &&
		(
		    peek(tokens).value == "&&" || peek(tokens).value == "||" ||
		    peek(tokens).value == "==" || peek(tokens).value == "!=" ||
		    peek(tokens).value == "<=" || peek(tokens).value == ">=" ||
		    peek(tokens).value == "<" || peek(tokens).value == ">"
		)
	) {
		std::string operator_symbol = eat(tokens).value;
		std::shared_ptr<daedalus::core::ast::Expression> right = daedalus::entropia::parser::parse_logical_expression(parser, tokens, needsSemicolon);

		return std::make_shared<daedalus::entropia::ast::BinaryExpression>(
			left,
			operator_symbol,
			right
		);
	}

	return left;
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_binary_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {
	return daedalus::entropia::parser::parse_logical_expression(parser, tokens, needsSemicolon);
}

void daedalus::entropia::parser::check_assignation_type(std::shared_ptr<daedalus::core::ast::Expression> expression, std::string type) {
    std::shared_ptr<daedalus::core::ast::Expression> typeExpression = (
	    expression->type() == "LoopExpression" ||
	    expression->type() == "WhileExpression" ||
	    expression->type() == "ForExpression"
	) ?
	    std::dynamic_pointer_cast<daedalus::entropia::ast::LoopExpression>(expression)->get_or_expression()->get_value() :
		expression->type() == "ConditionnalStructure" ?
		    std::dynamic_pointer_cast<daedalus::entropia::ast::ConditionnalStructure>(expression)->get_expressions().back()->get_body().back() : // Replace with <ConditionnalStructure>.get_last_value (gets the last value before break)
		    expression;

	if(type == "i8") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid i8 value (" + std::to_string(_I8_MIN) + " - " + std::to_string(_I8_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= _I8_MIN && numberExpression->get_value() <= _I8_MAX),
			std::runtime_error("Expected valid i8 value (" + std::to_string(_I8_MIN) + " - " + std::to_string(_I8_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "i16") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid i16 value (" + std::to_string(_I16_MIN) + " - " + std::to_string(_I16_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= _I16_MIN && numberExpression->get_value() <= _I16_MAX),
			std::runtime_error("Expected valid i16 value (" + std::to_string(_I16_MIN) + " - " + std::to_string(_I16_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "i32") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid i32 value (" + std::to_string(_I32_MIN) + " - " + std::to_string(_I32_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= _I32_MIN && numberExpression->get_value() <= _I32_MAX),
			std::runtime_error("Expected valid i32 value (" + std::to_string(_I32_MIN) + " - " + std::to_string(_I32_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "i64") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid i64 value (" + std::to_string(_I64_MIN) + " - " + std::to_string(_I64_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= _I64_MIN && numberExpression->get_value() <= _I64_MAX),
			std::runtime_error("Expected valid i64 value (" + std::to_string(_I64_MIN) + " - " + std::to_string(_I64_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "u8") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid u8 value (0 - " + std::to_string(_UI8_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= 0 && numberExpression->get_value() <= _UI8_MAX),
			std::runtime_error("Expected valid u8 value (0 - " + std::to_string(_UI8_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "u16") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid u16 value (0 - " + std::to_string(_UI16_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= 0 && numberExpression->get_value() <= _UI16_MAX),
			std::runtime_error("Expected valid u16 value (0 - " + std::to_string(_UI16_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "u32") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid u32 value (0 - " + std::to_string(_UI32_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= 0 && numberExpression->get_value() <= _UI32_MAX),
			std::runtime_error("Expected valid u32 value (0 - " + std::to_string(_UI32_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "u64") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid u64 value (0 - " + std::to_string(_UI64_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= 0 && numberExpression->get_value() <= _UI64_MAX),
			std::runtime_error("Expected valid u64 value (0 - " + std::to_string(_UI64_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "f32") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid f32 value (" + std::to_string(FLT_MIN) + " - " + std::to_string(FLT_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= FLT_MIN && numberExpression->get_value() <= FLT_MAX),
			std::runtime_error("Expected valid u64 value (" + std::to_string(FLT_MIN) + " - " + std::to_string(FLT_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "f64") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "NumberExpression",
			std::runtime_error("Expected valid f64 value (" + std::to_string(DBL_MIN) + " - " + std::to_string(DBL_MAX) + "), got " + typeExpression->repr())
		)
		auto numberExpression = std::dynamic_pointer_cast<daedalus::core::ast::NumberExpression>(typeExpression);
		DAE_ASSERT_TRUE(
			(numberExpression->get_value() >= DBL_MIN && numberExpression->get_value() <= DBL_MAX),
			std::runtime_error("Expected valid u64 value (" + std::to_string(DBL_MIN) + " - " + std::to_string(DBL_MAX) + "), got " + std::to_string(numberExpression->get_value()))
		)
	}
	else if(type == "bool") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "BooleanExpression",
			std::runtime_error("Expected valid boolean value (true / false), got " + typeExpression->repr())
		)
	}
	else if(type == "char") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "CharExpression",
			std::runtime_error("Expected valid char value, got " + typeExpression->repr())
		)
	}
	else if(type == "str") {
		DAE_ASSERT_TRUE(
			typeExpression->type() == "StrExpression",
			std::runtime_error("Expected valid string value, got " + typeExpression->repr())
		)
	}
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_assignation_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {

	std::shared_ptr<daedalus::core::ast::Expression> pseudoIdentifier = daedalus::entropia::parser::parse_binary_expression(parser, tokens, needsSemicolon);

	if(pseudoIdentifier->type() != "Identifier") {
	    if(needsSemicolon) {
	       (void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon at the end of line"));
		}
		return pseudoIdentifier;
	}

	auto identifier = std::dynamic_pointer_cast<daedalus::entropia::ast::Identifier>(pseudoIdentifier);

	if(peek(tokens).type != "ASSIGN") {
    	if(needsSemicolon) {
            (void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon at the end of line"));
        }
		return identifier;
	}
	(void)eat(tokens);

	std::shared_ptr<daedalus::core::ast::Expression> expression = daedalus::entropia::parser::parse_binary_expression(parser, tokens, false)->get_constexpr();

	if(auto containerExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::ContainerExpression>(expression)) {
		if(auto source_identifier = std::dynamic_pointer_cast<daedalus::entropia::ast::ContainerExpression>(expression)->get_inner_identifier()) {
		    if(needsSemicolon) {
				(void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon at the end of line"));
			}
		    return std::make_shared<daedalus::entropia::ast::AssignationExpression>(identifier, expression);
		}
	}

	std::string type = daedalus::entropia::parser::identifiers.at(identifier->get_name());

	check_assignation_type(expression, type);

	if(needsSemicolon) {
	   (void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon at the end of line"));
	}

	return std::make_shared<daedalus::entropia::ast::AssignationExpression>(identifier, expression);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_declaration_expression(daedalus::core::parser::Parser& parser, std::vector<daedalus::core::lexer::Token>& tokens, bool needsSemicolon) {

	if(peek(tokens).type != "LET") {
		return parse_assignation_expression(parser, tokens, needsSemicolon);
	}
	(void)eat(tokens);

	bool isMutable = false;

	if(peek(tokens).type == "MUT") {
		isMutable = true;
		(void)eat(tokens);
	}

	std::shared_ptr<daedalus::core::ast::Expression> pseudoIdentifier = daedalus::entropia::parser::parse_identifier(parser, tokens, needsSemicolon);

	DAE_ASSERT_TRUE(
		pseudoIdentifier->type() == "Identifier",
		std::runtime_error("Expected identifier")
	)

	auto identifier = std::dynamic_pointer_cast<daedalus::entropia::ast::Identifier>(pseudoIdentifier);

	(void)expect(tokens, "COLON", std::runtime_error("Expected colon"));

	std::string type = expect(tokens, "TYPE", std::runtime_error("Expected type specifier")).value;

	(void)expect(tokens, "ASSIGN", std::runtime_error("Expected assignment symbol"));

	std::shared_ptr<daedalus::core::ast::Expression> expression = daedalus::core::parser::parse_expression(parser, tokens, false)->get_constexpr();

	daedalus::entropia::parser::identifiers[identifier->get_name()] = type;

	if(auto containerExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::ContainerExpression>(expression)) {
		if(auto source_identifier = std::dynamic_pointer_cast<daedalus::entropia::ast::ContainerExpression>(expression)->get_inner_identifier()) {
		    if(needsSemicolon) {
				(void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon at the end of line"));
			}
			return std::make_shared<daedalus::entropia::ast::DeclarationExpression>(identifier, expression, type, isMutable);
		}
	}

	check_assignation_type(expression, type);

	if(needsSemicolon) {
	   (void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon at the end of line"));
	}

	return std::make_shared<daedalus::entropia::ast::DeclarationExpression>(identifier, expression, type, isMutable);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_or_expression(daedalus::core::parser::Parser &parser, std::vector<daedalus::core::lexer::Token> &tokens, bool needsSemicolon) {
    (void)expect(tokens, "OR", std::runtime_error("Expected or expression"));

    std::shared_ptr<daedalus::core::ast::Expression> value = daedalus::core::parser::parse_expression(
        parser,
        tokens,
        false
    );

    if(needsSemicolon) {
	   (void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon at the end of line"));
	}

	return std::make_shared<daedalus::entropia::ast::OrExpression>(value);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_loop_expression(daedalus::core::parser::Parser &parser, std::vector<daedalus::core::lexer::Token> &tokens, bool needsSemicolon) {
    if(peek(tokens).type != "LOOP") {
		return parse_declaration_expression(parser, tokens, needsSemicolon);
	}
	(void)eat(tokens);

	(void)expect(tokens, "OPEN_BRACE", std::runtime_error("Expected open brace before loop body"));

	auto body = std::vector<std::shared_ptr<daedalus::core::ast::Expression>>();

	// TODO Add max iteration to prevent unclosed loop
	while(peek(tokens).type != "CLOSE_BRACE") {
	    body.push_back(daedalus::core::parser::parse_expression(parser, tokens, true));
	}
	(void)eat(tokens);

	auto orExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::OrExpression>(
	    parse_or_expression(parser, tokens, needsSemicolon)
	);

	return std::make_shared<daedalus::entropia::ast::LoopExpression>(
	    body,
		orExpression
	);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_while_expression(daedalus::core::parser::Parser &parser, std::vector<daedalus::core::lexer::Token> &tokens, bool needsSemicolon) {
    if(peek(tokens).type != "WHILE") {
		return parse_loop_expression(parser, tokens, needsSemicolon);
	}
	(void)eat(tokens);

	(void)expect(tokens, "OPEN_PAREN", std::runtime_error("Expected open parenthesis before while condition"));

	std::shared_ptr<daedalus::core::ast::Expression> condition = daedalus::core::parser::parse_expression(parser, tokens, false);

	(void)expect(tokens, "CLOSE_PAREN", std::runtime_error("Expected close parenthesis after while condition"));
	(void)expect(tokens, "OPEN_BRACE", std::runtime_error("Expected open brace before while body"));

	auto body = std::vector<std::shared_ptr<daedalus::core::ast::Expression>>();

	// TODO Add max iteration to prevent unclosed loop
	while(peek(tokens).type != "CLOSE_BRACE") {
	   body.push_back(daedalus::core::parser::parse_expression(parser, tokens, true));
	}
	(void)eat(tokens);

	auto orExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::OrExpression>(
	    parse_or_expression(parser, tokens, needsSemicolon)
	);

	return std::make_shared<daedalus::entropia::ast::WhileExpression>(
	    body,
		condition,
		orExpression
	);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_for_expression(daedalus::core::parser::Parser &parser, std::vector<daedalus::core::lexer::Token> &tokens, bool needsSemicolon) {
    if(peek(tokens).type != "FOR") {
		return parse_while_expression(parser, tokens, needsSemicolon);
	}
	(void)eat(tokens);

	(void)expect(tokens, "OPEN_PAREN", std::runtime_error("Expected open parenthesis before for initial expression"));

	std::shared_ptr<daedalus::core::ast::Expression> initial_expression = daedalus::core::parser::parse_expression(parser, tokens, false);
	(void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon after for initial expression"));

	std::shared_ptr<daedalus::core::ast::Expression> condition = daedalus::core::parser::parse_expression(parser, tokens, false);
	(void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon after for condition"));

	std::shared_ptr<daedalus::core::ast::Expression> update_expression = daedalus::core::parser::parse_expression(parser, tokens, false);

	(void)expect(tokens, "CLOSE_PAREN", std::runtime_error("Expected close parenthesis after for update expression"));
	(void)expect(tokens, "OPEN_BRACE", std::runtime_error("Expected open brace before for body"));

	auto body = std::vector<std::shared_ptr<daedalus::core::ast::Expression>>();

	// TODO Add max iteration to prevent unclosed loop
	while(peek(tokens).type != "CLOSE_BRACE") {
	   body.push_back(daedalus::core::parser::parse_expression(parser, tokens, true));
	}
	(void)eat(tokens);

	auto orExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::OrExpression>(
	    parse_or_expression(parser, tokens, needsSemicolon)
	);

	return std::make_shared<daedalus::entropia::ast::ForExpression>(
	    body,
		initial_expression,
		condition,
		update_expression,
		orExpression
	);
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_break_expression(daedalus::core::parser::Parser &parser, std::vector<daedalus::core::lexer::Token> &tokens, bool needsSemicolon) {
    if(peek(tokens).type != "BREAK") {
        return parse_for_expression(parser, tokens, needsSemicolon);
    }
    (void)eat(tokens);

    if(needsSemicolon) {
	   (void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon at the end of line"));
	}

	return std::make_shared<daedalus::entropia::ast::BreakExpression>();
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_continue_expression(daedalus::core::parser::Parser &parser, std::vector<daedalus::core::lexer::Token> &tokens, bool needsSemicolon) {
    if(peek(tokens).type != "CONTINUE") {
        return parse_break_expression(parser, tokens, needsSemicolon);
    }
    (void)eat(tokens);

    if(needsSemicolon) {
	   (void)expect(tokens, "SEMICOLON", std::runtime_error("Expected semicolon at the end of line"));
	}

	return std::make_shared<daedalus::entropia::ast::ContinueExpression>();
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_conditionnal_expression(daedalus::core::parser::Parser &parser, std::vector<daedalus::core::lexer::Token> &tokens, std::shared_ptr<daedalus::entropia::ast::ConditionnalExpression> before) {

    if(peek(tokens).type == "ELSE") {
        DAE_ASSERT_TRUE(
            before != nullptr,
            std::runtime_error("else statement needs an if statement before itself")
        )
        (void)eat(tokens);
    } else {
        before = nullptr;
    }

    std::shared_ptr<daedalus::core::ast::Expression> condition = nullptr;
    if(peek(tokens).type == "IF") {
        (void)eat(tokens);
        (void)expect(tokens, "OPEN_PAREN", std::runtime_error("Expected open parentesis before if condition"));
        condition = daedalus::core::parser::parse_expression(parser, tokens, false);
        (void)expect(tokens, "CLOSE_PAREN", std::runtime_error("Expected close parentesis after if condition"));
    }

    (void)expect(tokens, "OPEN_BRACE", std::runtime_error("Expected open brace before conditionnal body"));

    auto body = std::vector<std::shared_ptr<daedalus::core::ast::Expression>>();

    // TODO Add max iteration to prevent unclosed body
    while(peek(tokens).type != "CLOSE_BRACE") {
        body.push_back(daedalus::core::parser::parse_expression(parser, tokens, true));
	}

	DAE_ASSERT_TRUE(
	    body.size() > 0,
		std::runtime_error("Empty if/else block")
	)

	(void)eat(tokens);

    return std::make_shared<daedalus::entropia::ast::ConditionnalExpression>(
        body,
        condition,
        before
    );
}

std::shared_ptr<daedalus::core::ast::Expression> daedalus::entropia::parser::parse_conditionnal_structure(daedalus::core::parser::Parser &parser, std::vector<daedalus::core::lexer::Token> &tokens, bool needsSemicolon) {
    if(peek(tokens).type != "IF" && peek(tokens).type != "ELSE") {
        return parse_continue_expression(parser, tokens, needsSemicolon);
    }
    auto expressions = std::vector<std::shared_ptr<daedalus::entropia::ast::ConditionnalExpression>>();
    std::shared_ptr<daedalus::entropia::ast::ConditionnalExpression> before = nullptr;
    do {
        expressions.push_back(
            std::dynamic_pointer_cast<daedalus::entropia::ast::ConditionnalExpression>(parse_conditionnal_expression(parser, tokens, before))
        );
        before = expressions.back();
        if(before->get_condition() == nullptr) {
            break;
        }
    } while(peek(tokens).type == "IF" || peek(tokens).type == "ELSE");

    DAE_ASSERT_TRUE(
        expressions.back()->get_before() != nullptr,
        std::runtime_error("Else block missing")
    )

    return std::make_shared<daedalus::entropia::ast::ConditionnalStructure>(
        expressions
    );
}

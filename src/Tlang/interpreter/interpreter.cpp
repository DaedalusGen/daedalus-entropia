#include <AquIce/Tlang/interpreter/interpreter.hpp>

std::shared_ptr<daedalus::values::RuntimeValue> tlang::interpreter::evaluate_identifier(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<tlang::ast::Identifier> identifier = std::dynamic_pointer_cast<tlang::ast::Identifier>(statement);
	return env->get_value(identifier->get_name());
}

std::shared_ptr<daedalus::values::RuntimeValue> tlang::interpreter::evaluate_boolean_expression(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<tlang::ast::BooleanExpression> booleanExpression = std::dynamic_pointer_cast<tlang::ast::BooleanExpression>(statement);

	return std::make_shared<tlang::values::BooleanValue>(booleanExpression->value);
};

std::shared_ptr<daedalus::values::RuntimeValue> tlang::interpreter::evaluate_char_expression(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<tlang::ast::CharExpression> charExpression = std::dynamic_pointer_cast<tlang::ast::CharExpression>(statement);
	return std::make_shared<tlang::values::CharValue>(charExpression->value);
};

std::shared_ptr<daedalus::values::RuntimeValue> tlang::interpreter::evaluate_str_expression(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<tlang::ast::StrExpression> strExpression = std::dynamic_pointer_cast<tlang::ast::StrExpression>(statement);
	return std::make_shared<tlang::values::StrValue>(strExpression->value);
};

std::shared_ptr<daedalus::values::RuntimeValue> tlang::interpreter::evaluate_unary_expression(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<tlang::ast::UnaryExpression> unaryExpression = std::dynamic_pointer_cast<tlang::ast::UnaryExpression>(statement);

	std::shared_ptr<daedalus::values::RuntimeValue> term = daedalus::interpreter::evaluate_statement(interpreter, unaryExpression->get_term(), env);
	std::string operator_symbol = unaryExpression->get_operator_symbol();

	if(operator_symbol == "!") {
		return std::make_shared<tlang::values::BooleanValue>(!term->IsTrue());
	}

	throw std::runtime_error("Unknown unary operator " + operator_symbol);
}

std::shared_ptr<daedalus::values::RuntimeValue> tlang::interpreter::evaluate_binary_expression(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<tlang::ast::BinaryExpression> binaryExpression = std::dynamic_pointer_cast<tlang::ast::BinaryExpression>(statement);

	std::shared_ptr<daedalus::values::RuntimeValue> left = daedalus::interpreter::evaluate_statement(interpreter, binaryExpression->get_left(), env);
	std::shared_ptr<daedalus::values::RuntimeValue> right = daedalus::interpreter::evaluate_statement(interpreter, binaryExpression->get_right(), env);

	std::string operator_symbol = binaryExpression->get_operator_symbol();

	if(operator_symbol == "+") {
		DAE_ASSERT_TRUE(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue",
			std::runtime_error("Trying to add invalid operands : " + left->type() + " - " + right->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(right)->get();
		
		return std::make_shared<daedalus::values::NumberValue>(left_nb + right_nb);
	}
	if(operator_symbol == "-") {
		DAE_ASSERT_TRUE(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue",
			std::runtime_error("Trying to subtract invalid operands : " + left->type() + " - " + right->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(right)->get();
		
		return std::make_shared<daedalus::values::NumberValue>(left_nb - right_nb);		
	}
	if(operator_symbol == "*") {
		DAE_ASSERT_TRUE(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue",
			std::runtime_error("Trying to multiply invalid invalid operands : " + left->type() + " - " + right->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(right)->get();
		
		return std::make_shared<daedalus::values::NumberValue>(left_nb * right_nb);		
	}
	if(operator_symbol == "/") {
		DAE_ASSERT_TRUE(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue",
			std::runtime_error("Trying to divide invalid invalid operands : " + left->type() + " - " + right->type());
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::values::NumberValue>(right)->get();

		DAE_ASSERT_TRUE(
			right_nb != 0,
			std::runtime_error("Trying to divide by zero");
		)
		
		return std::make_shared<daedalus::values::NumberValue>(left_nb / right_nb);
	}
	if(operator_symbol == "&&") {
		return std::make_shared<tlang::values::BooleanValue>(left->IsTrue() && right->IsTrue());
	}
	if(operator_symbol == "||") {
		return std::make_shared<tlang::values::BooleanValue>(left->IsTrue() || right->IsTrue());
	}

	throw std::runtime_error("Unknown operator " + operator_symbol);
}

std::shared_ptr<daedalus::values::RuntimeValue> tlang::interpreter::evaluate_assignation_expression(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<tlang::ast::AssignationExpression> assignationExpression = std::dynamic_pointer_cast<tlang::ast::AssignationExpression>(statement);

	return env->set_value(
		assignationExpression->get_identifier()->get_name(),
		daedalus::interpreter::evaluate_statement(
			interpreter,
			assignationExpression->get_value(),
			env
		)
	);
}

std::shared_ptr<daedalus::values::RuntimeValue> tlang::interpreter::evaluate_declaration_expression(
	daedalus::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::ast::Statement> statement,
	std::shared_ptr<daedalus::env::Environment> env
) {
	std::shared_ptr<tlang::ast::DeclarationExpression> declarationExpression = std::dynamic_pointer_cast<tlang::ast::DeclarationExpression>(statement);

	return env->init_value(
		declarationExpression->get_identifier()->get_name(),
		daedalus::interpreter::evaluate_statement(
			interpreter,
			declarationExpression->get_value(),
			env
		),
		{
			{ "isMutable", declarationExpression->get_mutability() ? "true" : "false" }
		}
	);
}

void setup_interpreter(daedalus::interpreter::Interpreter& interpreter) {

	std::unordered_map<std::string, daedalus::interpreter::ParseStatementFunction> nodeEvaluationFunctions = std::unordered_map<std::string, daedalus::interpreter::ParseStatementFunction>({
		{
			"DeclarationExpression",
			&tlang::interpreter::evaluate_declaration_expression
		},
		{
			"AssignationExpression",
			&tlang::interpreter::evaluate_assignation_expression
		},
		{
			"Identifier",
			&tlang::interpreter::evaluate_identifier
		},
		{
			"BooleanExpression",
			&tlang::interpreter::evaluate_boolean_expression
		},
		{
			"CharExpression",
			&tlang::interpreter::evaluate_char_expression
		},
		{
			"StrExpression",
			&tlang::interpreter::evaluate_str_expression
		},
		{
			"UnaryExpression",
			&tlang::interpreter::evaluate_unary_expression
		},
		{
			"BinaryExpression",
			&tlang::interpreter::evaluate_binary_expression
		},
	});

	std::vector<std::string> envValuesProperties = std::vector<std::string>({
		"isMutable"
	});

	daedalus::env::EnvValidationRule mutabilityValidation = {
		&tlang::env::validate_mutability,
		std::vector<daedalus::env::ValidationRuleSensitivity>({
			daedalus::env::ValidationRuleSensitivity::SET
		})
	};

	daedalus::env::EnvValidationRule typeCastValidation = {
		&tlang::env::validate_type_cast,
		std::vector<daedalus::env::ValidationRuleSensitivity>({
			daedalus::env::ValidationRuleSensitivity::SET
		})
	};

	std::vector<daedalus::env::EnvValidationRule> validationRules = std::vector<daedalus::env::EnvValidationRule>({
		mutabilityValidation,
		typeCastValidation
	});

	daedalus::interpreter::setup_interpreter(
		interpreter,
		nodeEvaluationFunctions,
		envValuesProperties,
		validationRules
	);
}
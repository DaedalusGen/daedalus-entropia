#include <daedalus/Entropia/interpreter/interpreter.hpp>

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_identifier(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	std::shared_ptr<daedalus::entropia::ast::Identifier> identifier = std::dynamic_pointer_cast<daedalus::entropia::ast::Identifier>(statement);
	return env->get_value(identifier->get_name());
}

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_boolean_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	std::shared_ptr<daedalus::entropia::ast::BooleanExpression> booleanExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::BooleanExpression>(statement);

	return std::make_shared<daedalus::entropia::values::BooleanValue>(booleanExpression->value);
};

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_char_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	std::shared_ptr<daedalus::entropia::ast::CharExpression> charExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::CharExpression>(statement);
	return std::make_shared<daedalus::entropia::values::CharValue>(charExpression->value);
};

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_str_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	std::shared_ptr<daedalus::entropia::ast::StrExpression> strExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::StrExpression>(statement);
	return std::make_shared<daedalus::entropia::values::StrValue>(strExpression->value);
};

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_unary_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	std::shared_ptr<daedalus::entropia::ast::UnaryExpression> unaryExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::UnaryExpression>(statement);

	std::shared_ptr<daedalus::core::values::RuntimeValue> term = daedalus::core::interpreter::evaluate_statement(interpreter, unaryExpression->get_term(), env);
	std::string operator_symbol = unaryExpression->get_operator_symbol();

	if(operator_symbol == "!") {
		return std::make_shared<daedalus::entropia::values::BooleanValue>(!term->IsTrue());
	}

	throw std::runtime_error("Unknown unary operator " + operator_symbol);
}

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_binary_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	std::shared_ptr<daedalus::entropia::ast::BinaryExpression> binaryExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::BinaryExpression>(statement);

	std::shared_ptr<daedalus::core::values::RuntimeValue> left = daedalus::core::interpreter::evaluate_statement(interpreter, binaryExpression->get_left(), env);
	std::shared_ptr<daedalus::core::values::RuntimeValue> right = daedalus::core::interpreter::evaluate_statement(interpreter, binaryExpression->get_right(), env);

	std::string operator_symbol = binaryExpression->get_operator_symbol();

	if(operator_symbol == "+") {
		DAE_ASSERT_TRUE(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue",
			std::runtime_error("Trying to add invalid operands : " + left->type() + " - " + right->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

		return std::make_shared<daedalus::core::values::NumberValue>(left_nb + right_nb);
	}
	if(operator_symbol == "-") {
		DAE_ASSERT_TRUE(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue",
			std::runtime_error("Trying to subtract invalid operands : " + left->type() + " - " + right->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

		return std::make_shared<daedalus::core::values::NumberValue>(left_nb - right_nb);
	}
	if(operator_symbol == "*") {
		DAE_ASSERT_TRUE(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue",
			std::runtime_error("Trying to multiply invalid invalid operands : " + left->type() + " - " + right->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

		return std::make_shared<daedalus::core::values::NumberValue>(left_nb * right_nb);
	}
	if(operator_symbol == "/") {
		DAE_ASSERT_TRUE(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue",
			std::runtime_error("Trying to divide invalid invalid operands : " + left->type() + " - " + right->type());
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

		DAE_ASSERT_TRUE(
			right_nb != 0,
			std::runtime_error("Trying to divide by zero");
		)

		return std::make_shared<daedalus::core::values::NumberValue>(left_nb / right_nb);
	}
	if(operator_symbol == "&&") {
		return std::make_shared<daedalus::entropia::values::BooleanValue>(left->IsTrue() && right->IsTrue());
	}
	if(operator_symbol == "||") {
		return std::make_shared<daedalus::entropia::values::BooleanValue>(left->IsTrue() || right->IsTrue());
	}

	throw std::runtime_error("Unknown operator " + operator_symbol);
}

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_assignation_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	std::shared_ptr<daedalus::entropia::ast::AssignationExpression> assignationExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::AssignationExpression>(statement);

	return env->set_value(
		assignationExpression->get_identifier()->get_name(),
		daedalus::core::interpreter::evaluate_statement(
			interpreter,
			assignationExpression->get_value(),
			env
		)
	);
}

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_declaration_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	std::shared_ptr<daedalus::entropia::ast::DeclarationExpression> declarationExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::DeclarationExpression>(statement);

	return env->init_value(
		declarationExpression->get_identifier()->get_name(),
		daedalus::core::interpreter::evaluate_statement(
			interpreter,
			declarationExpression->get_value(),
			env
		),
		{
			{ "isMutable", declarationExpression->get_mutability() ? "true" : "false" }
		}
	);
}

void setup_interpreter(daedalus::core::interpreter::Interpreter& interpreter) {

	std::unordered_map<std::string, daedalus::core::interpreter::ParseStatementFunction> nodeEvaluationFunctions = std::unordered_map<std::string, daedalus::core::interpreter::ParseStatementFunction>({
		{
			"DeclarationExpression",
			&daedalus::entropia::interpreter::evaluate_declaration_expression
		},
		{
			"AssignationExpression",
			&daedalus::entropia::interpreter::evaluate_assignation_expression
		},
		{
			"Identifier",
			&daedalus::entropia::interpreter::evaluate_identifier
		},
		{
			"BooleanExpression",
			&daedalus::entropia::interpreter::evaluate_boolean_expression
		},
		{
			"CharExpression",
			&daedalus::entropia::interpreter::evaluate_char_expression
		},
		{
			"StrExpression",
			&daedalus::entropia::interpreter::evaluate_str_expression
		},
		{
			"UnaryExpression",
			&daedalus::entropia::interpreter::evaluate_unary_expression
		},
		{
			"BinaryExpression",
			&daedalus::entropia::interpreter::evaluate_binary_expression
		},
	});

	std::vector<std::string> envValuesProperties = std::vector<std::string>({
		"isMutable"
	});

	daedalus::core::env::EnvValidationRule mutabilityValidation = {
		&daedalus::entropia::env::validate_mutability,
		std::vector<daedalus::core::env::ValidationRuleSensitivity>({
			daedalus::core::env::ValidationRuleSensitivity::SET
		})
	};

	daedalus::core::env::EnvValidationRule typeCastValidation = {
		&daedalus::entropia::env::validate_type_cast,
		std::vector<daedalus::core::env::ValidationRuleSensitivity>({
			daedalus::core::env::ValidationRuleSensitivity::SET
		})
	};

	std::vector<daedalus::core::env::EnvValidationRule> validationRules = std::vector<daedalus::core::env::EnvValidationRule>({
		mutabilityValidation,
		typeCastValidation
	});

	daedalus::core::interpreter::setup_interpreter(
		interpreter,
		nodeEvaluationFunctions,
		envValuesProperties,
		validationRules
	);
}

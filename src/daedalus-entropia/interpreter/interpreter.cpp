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
			std::runtime_error("Trying to add invalid operands : " + left->type() + " + " + right->type())
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
			std::runtime_error("Trying to multiply invalid invalid operands : " + left->type() + " * " + right->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

		return std::make_shared<daedalus::core::values::NumberValue>(left_nb * right_nb);
	}
	if(operator_symbol == "/") {
		DAE_ASSERT_TRUE(
			left->type() == "NumberValue" &&
			right->type() == "NumberValue",
			std::runtime_error("Trying to divide invalid invalid operands : " + left->type() + " / " + right->type());
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

		DAE_ASSERT_TRUE(
			right_nb != 0,
			std::runtime_error("Trying to divide by zero");
		)

		return std::make_shared<daedalus::core::values::NumberValue>(left_nb / right_nb);
	}
	if(operator_symbol == "==") {
        if(
    		left->type() == "NumberValue" &&
    		right->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb == right_nb);
		}
        if(
    		left->type() == "BooleanValue" &&
    		right->type() == "BooleanValue"
		) {
            bool left_bool = std::dynamic_pointer_cast<daedalus::entropia::values::BooleanValue>(left)->get();
    		bool right_bool = std::dynamic_pointer_cast<daedalus::entropia::values::BooleanValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_bool == right_bool);
		}
		if(
    		left->type() == "CharValue" &&
    		right->type() == "CharValue"
		) {
            char left_char = std::dynamic_pointer_cast<daedalus::entropia::values::CharValue>(left)->get();
    		char right_char = std::dynamic_pointer_cast<daedalus::entropia::values::CharValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_char == right_char);
        }
        if(
      		left->type() == "StrValue" &&
      		right->type() == "StrValue"
		) {
            std::string left_str = std::dynamic_pointer_cast<daedalus::entropia::values::StrValue>(left)->get();
      		std::string right_str = std::dynamic_pointer_cast<daedalus::entropia::values::StrValue>(right)->get();

      		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_str == right_str);
        }

		throw std::runtime_error("Trying to check equality on invalid invalid operands : " + left->type() + " == " + right->type());
	}
	if(operator_symbol == "!=") {
        if(
    		left->type() == "NumberValue" &&
    		right->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb != right_nb);
		}
        if(
    		left->type() == "BooleanValue" &&
    		right->type() == "BooleanValue"
		) {
            bool left_bool = std::dynamic_pointer_cast<daedalus::entropia::values::BooleanValue>(left)->get();
    		bool right_bool = std::dynamic_pointer_cast<daedalus::entropia::values::BooleanValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_bool != right_bool);
		}
		if(
    		left->type() == "CharValue" &&
    		right->type() == "CharValue"
		) {
            char left_char = std::dynamic_pointer_cast<daedalus::entropia::values::CharValue>(left)->get();
    		char right_char = std::dynamic_pointer_cast<daedalus::entropia::values::CharValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_char != right_char);
        }
        if(
      		left->type() == "StrValue" &&
      		right->type() == "StrValue"
		) {
            std::string left_str = std::dynamic_pointer_cast<daedalus::entropia::values::StrValue>(left)->get();
      		std::string right_str = std::dynamic_pointer_cast<daedalus::entropia::values::StrValue>(right)->get();

      		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_str != right_str);
        }

		throw std::runtime_error("Trying to check inequality on invalid invalid operands : " + left->type() + " != " + right->type());
	}
	if(operator_symbol == "<") {
        if(
    		left->type() == "NumberValue" &&
    		right->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb < right_nb);
		}

		throw std::runtime_error("Trying to check inferiority on invalid invalid operands : " + left->type() + " < " + right->type());
	}
	if(operator_symbol == ">") {
        if(
    		left->type() == "NumberValue" &&
    		right->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb > right_nb);
		}

		throw std::runtime_error("Trying to check superiority on invalid invalid operands : " + left->type() + " != " + right->type());
	}
	if(operator_symbol == "<=") {
        if(
    		left->type() == "NumberValue" &&
    		right->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb != right_nb);
		}

		throw std::runtime_error("Trying to check equality/inferiority on invalid invalid operands : " + left->type() + " != " + right->type());
	}
	if(operator_symbol == "!=") {
        if(
    		left->type() == "NumberValue" &&
    		right->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right)->get();

    		return std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb != right_nb);
		}

		throw std::runtime_error("Trying to check equality/superiority on invalid invalid operands : " + left->type() + " != " + right->type());
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

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_loop_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
    std::shared_ptr<daedalus::entropia::ast::LoopExpression> loopExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::LoopExpression>(statement);

    std::shared_ptr<daedalus::core::values::RuntimeValue> last;
	while(true) {
	    std::vector<daedalus::core::interpreter::RuntimeResult> results = std::vector<daedalus::core::interpreter::RuntimeResult>();
        last = daedalus::core::interpreter::evaluate_scope(interpreter, loopExpression, results, nullptr, env);
        std::cout << last->repr() << std::endl;
	}

	return last;
}

std::shared_ptr<daedalus::core::values::RuntimeValue> daedalus::entropia::interpreter::evaluate_conditionnal_structure(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
    std::shared_ptr<daedalus::entropia::ast::ConditionnalStructure> conditionnalExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::ConditionnalStructure>(statement);

    for(std::shared_ptr<daedalus::entropia::ast::ConditionnalExpression> expression : conditionnalExpression->get_expressions()) {
        bool isTrue = expression->get_condition() == nullptr;
        if(!isTrue) {
            isTrue = daedalus::core::interpreter::evaluate_statement(interpreter, expression->get_condition(), env)->IsTrue();
        }
        if(isTrue) {
            std::vector<daedalus::core::interpreter::RuntimeResult> results = std::vector<daedalus::core::interpreter::RuntimeResult>();
            std::shared_ptr<daedalus::core::values::RuntimeValue> last = daedalus::core::interpreter::evaluate_scope(interpreter, expression, results, nullptr, env);
            std::cout << last->repr() << std::endl;
            return last;
        }
	}

	throw std::runtime_error("No condition matched (should not have been parsed");
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
		{
		    "LoopExpression",
			&daedalus::entropia::interpreter::evaluate_loop_expression
		},
		{
		    "ConditionnalStructure",
			&daedalus::entropia::interpreter::evaluate_conditionnal_structure
		}
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

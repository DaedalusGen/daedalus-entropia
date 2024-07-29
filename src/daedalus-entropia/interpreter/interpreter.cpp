#include <daedalus/Entropia/interpreter/interpreter.hpp>

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_identifier(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	auto identifier = std::dynamic_pointer_cast<daedalus::entropia::ast::Identifier>(statement);
	return daedalus::core::interpreter::wrap(
	   env->get_value(identifier->get_name())
	);
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_boolean_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	auto booleanExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::BooleanExpression>(statement);
	return daedalus::core::interpreter::wrap(
	   std::make_shared<daedalus::entropia::values::BooleanValue>(booleanExpression->get_value())
	);
};

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_char_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	auto charExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::CharExpression>(statement);
	return daedalus::core::interpreter::wrap(
	   std::make_shared<daedalus::entropia::values::CharValue>(charExpression->get_value())
	);
};

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_str_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	auto strExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::StrExpression>(statement);
	return daedalus::core::interpreter::wrap(
	   std::make_shared<daedalus::entropia::values::StrValue>(strExpression->get_value())
	);
};

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_unary_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	auto unaryExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::UnaryExpression>(statement);

	daedalus::core::interpreter::RuntimeValueWrapper term = daedalus::core::interpreter::evaluate_statement(interpreter, unaryExpression->get_term(), env);
	std::string operator_symbol = unaryExpression->get_operator_symbol();

	if(operator_symbol == "!") {
		return daedalus::core::interpreter::wrap(
		   std::make_shared<daedalus::entropia::values::BooleanValue>(!term.value->IsTrue())
		);
	}

	throw std::runtime_error("Unknown unary operator " + operator_symbol);
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_binary_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	auto binaryExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::BinaryExpression>(statement);

	daedalus::core::interpreter::RuntimeValueWrapper left = daedalus::core::interpreter::evaluate_statement(interpreter, binaryExpression->get_left(), env);
	daedalus::core::interpreter::RuntimeValueWrapper right = daedalus::core::interpreter::evaluate_statement(interpreter, binaryExpression->get_right(), env);

	std::string operator_symbol = binaryExpression->get_operator_symbol();

	if(operator_symbol == "+") {
		DAE_ASSERT_TRUE(
			left.value->type() == "NumberValue" &&
			right.value->type() == "NumberValue",
			std::runtime_error("Trying to add invalid operands : " + left.value->type() + " + " + right.value->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

		return daedalus::core::interpreter::wrap(
		   std::make_shared<daedalus::core::values::NumberValue>(left_nb + right_nb)
		);
	}
	if(operator_symbol == "-") {
		DAE_ASSERT_TRUE(
			left.value->type() == "NumberValue" &&
			right.value->type() == "NumberValue",
			std::runtime_error("Trying to subtract invalid operands : " + left.value->type() + " - " + right.value->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

		return daedalus::core::interpreter::wrap(
		   std::make_shared<daedalus::core::values::NumberValue>(left_nb - right_nb)
		);
	}
	if(operator_symbol == "*") {
		DAE_ASSERT_TRUE(
			left.value->type() == "NumberValue" &&
			right.value->type() == "NumberValue",
			std::runtime_error("Trying to multiply invalid invalid operands : " + left.value->type() + " * " + right.value->type())
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

		return daedalus::core::interpreter::wrap(
		   std::make_shared<daedalus::core::values::NumberValue>(left_nb * right_nb)
		);
	}
	if(operator_symbol == "/") {
		DAE_ASSERT_TRUE(
			left.value->type() == "NumberValue" &&
			right.value->type() == "NumberValue",
			std::runtime_error("Trying to divide invalid invalid operands : " + left.value->type() + " / " + right.value->type());
		)

		double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

		DAE_ASSERT_TRUE(
			right_nb != 0,
			std::runtime_error("Trying to divide by zero");
		)

		return daedalus::core::interpreter::wrap(
		   std::make_shared<daedalus::core::values::NumberValue>(left_nb / right_nb)
		);
	}
	if(operator_symbol == "==") {
        if(
    		left.value->type() == "NumberValue" &&
    		right.value->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb == right_nb)
            );
		}
        if(
    		left.value->type() == "BooleanValue" &&
    		right.value->type() == "BooleanValue"
		) {
            bool left_bool = std::dynamic_pointer_cast<daedalus::entropia::values::BooleanValue>(left.value)->get();
    		bool right_bool = std::dynamic_pointer_cast<daedalus::entropia::values::BooleanValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_bool == right_bool)
            );
		}
		if(
    		left.value->type() == "CharValue" &&
    		right.value->type() == "CharValue"
		) {
            char left_char = std::dynamic_pointer_cast<daedalus::entropia::values::CharValue>(left.value)->get();
    		char right_char = std::dynamic_pointer_cast<daedalus::entropia::values::CharValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_char == right_char)
            );
        }
        if(
      		left.value->type() == "StrValue" &&
      		right.value->type() == "StrValue"
		) {
            std::string left_str = std::dynamic_pointer_cast<daedalus::entropia::values::StrValue>(left.value)->get();
      		std::string right_str = std::dynamic_pointer_cast<daedalus::entropia::values::StrValue>(right.value)->get();

      		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_str == right_str)
            );
        }

		throw std::runtime_error("Trying to check equality on invalid invalid operands : " + left.value->type() + " == " + right.value->type());
	}
	if(operator_symbol == "!=") {
        if(
    		left.value->type() == "NumberValue" &&
    		right.value->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb != right_nb)
            );
		}
        if(
    		left.value->type() == "BooleanValue" &&
    		right.value->type() == "BooleanValue"
		) {
            bool left_bool = std::dynamic_pointer_cast<daedalus::entropia::values::BooleanValue>(left.value)->get();
    		bool right_bool = std::dynamic_pointer_cast<daedalus::entropia::values::BooleanValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_bool != right_bool)
            );
		}
		if(
    		left.value->type() == "CharValue" &&
    		right.value->type() == "CharValue"
		) {
            char left_char = std::dynamic_pointer_cast<daedalus::entropia::values::CharValue>(left.value)->get();
    		char right_char = std::dynamic_pointer_cast<daedalus::entropia::values::CharValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_char != right_char)
            );
        }
        if(
      		left.value->type() == "StrValue" &&
      		right.value->type() == "StrValue"
		) {
            std::string left_str = std::dynamic_pointer_cast<daedalus::entropia::values::StrValue>(left.value)->get();
      		std::string right_str = std::dynamic_pointer_cast<daedalus::entropia::values::StrValue>(right.value)->get();

      		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_str != right_str)
            );
        }

		throw std::runtime_error("Trying to check inequality on invalid invalid operands : " + left.value->type() + " != " + right.value->type());
	}
	if(operator_symbol == "<") {
        if(
    		left.value->type() == "NumberValue" &&
    		right.value->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb < right_nb)
            );
		}

		throw std::runtime_error("Trying to check inferiority on invalid invalid operands : " + left.value->type() + " < " + right.value->type());
	}
	if(operator_symbol == ">") {
        if(
    		left.value->type() == "NumberValue" &&
    		right.value->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb > right_nb)
            );
		}

		throw std::runtime_error("Trying to check superiority on invalid invalid operands : " + left.value->type() + " != " + right.value->type());
	}
	if(operator_symbol == "<=") {
        if(
    		left.value->type() == "NumberValue" &&
    		right.value->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb != right_nb)
            );
		}

		throw std::runtime_error("Trying to check equality/inferiority on invalid invalid operands : " + left.value->type() + " != " + right.value->type());
	}
	if(operator_symbol == "!=") {
        if(
    		left.value->type() == "NumberValue" &&
    		right.value->type() == "NumberValue"
		) {
            double left_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(left.value)->get();
    		double right_nb = std::dynamic_pointer_cast<daedalus::core::values::NumberValue>(right.value)->get();

    		return daedalus::core::interpreter::wrap(
                std::make_shared<daedalus::entropia::values::BooleanValue>(left_nb != right_nb)
            );
		}

		throw std::runtime_error("Trying to check equality/superiority on invalid invalid operands : " + left.value->type() + " != " + right.value->type());
	}
	if(operator_symbol == "&&") {
		return daedalus::core::interpreter::wrap(
		   std::make_shared<daedalus::entropia::values::BooleanValue>(left.value->IsTrue() && right.value->IsTrue())
		);
	}
	if(operator_symbol == "||") {
		return daedalus::core::interpreter::wrap(
		   std::make_shared<daedalus::entropia::values::BooleanValue>(left.value->IsTrue() || right.value->IsTrue())
		);
	}

	throw std::runtime_error("Unknown operator " + operator_symbol);
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_assignation_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	auto assignationExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::AssignationExpression>(statement);

	return daedalus::core::interpreter::wrap(
	    env->set_value(
    		assignationExpression->get_identifier()->get_name(),
    		daedalus::core::interpreter::evaluate_statement(
    			interpreter,
    			assignationExpression->get_value(),
    			env
    		).value
    	)
	);
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_declaration_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
	auto declarationExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::DeclarationExpression>(statement);

	return daedalus::core::interpreter::wrap(
	   env->init_value(
    		declarationExpression->get_identifier()->get_name(),
    		daedalus::core::interpreter::evaluate_statement(
    			interpreter,
    			declarationExpression->get_value(),
    			env
    		).value,
    		{
    			{ "isMutable", declarationExpression->get_mutability() ? "true" : "false" }
    		}
    	)
	);
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_loop_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
    auto loopExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::LoopExpression>(statement);

    daedalus::core::interpreter::RuntimeValueWrapper scope_result = daedalus::core::interpreter::wrap(nullptr);
	while(true) {
	    auto results = std::vector<daedalus::core::interpreter::RuntimeResult>();
		scope_result = daedalus::core::interpreter::evaluate_scope(
		    interpreter,
			loopExpression,
			results,
			nullptr,
			env,
			static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
		);

        if(
            daedalus::core::interpreter::flag_contains(
                static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
            )
        ) {
            scope_result.flags = static_cast<daedalus::core::interpreter::Flags>(
                daedalus::core::interpreter::flag_remove(
                    static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                    static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
                )
            );
            break;
        }
        if(
            daedalus::core::interpreter::flag_contains(
                static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::CONTINUE)
            )
        ) {
            scope_result.flags = static_cast<daedalus::core::interpreter::Flags>(
                daedalus::core::interpreter::flag_remove(
                    static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                    static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::CONTINUE)
                )
            );
            continue;
        }
	}

	if(scope_result.value == nullptr) {
	    scope_result = daedalus::core::interpreter::evaluate_statement(interpreter, loopExpression->get_or_expression(), env);
	}

	return scope_result;
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_while_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
    auto whileExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::WhileExpression>(statement);

    daedalus::core::interpreter::RuntimeValueWrapper scope_result = daedalus::core::interpreter::wrap(nullptr);
	while(daedalus::core::interpreter::evaluate_statement(interpreter, whileExpression->get_condition(), env).value->IsTrue()) {
	    auto results = std::vector<daedalus::core::interpreter::RuntimeResult>();
		scope_result = daedalus::core::interpreter::evaluate_scope(
		    interpreter,
			whileExpression,
			results,
			nullptr,
			env,
			static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
		);

        if(
            daedalus::core::interpreter::flag_contains(
                static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
            )
        ) {
            scope_result.flags = static_cast<daedalus::core::interpreter::Flags>(
                daedalus::core::interpreter::flag_remove(
                    static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                    static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
                )
            );
            break;
        }
        if(
            daedalus::core::interpreter::flag_contains(
                static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::CONTINUE)
            )
        ) {
            scope_result.flags = static_cast<daedalus::core::interpreter::Flags>(
                daedalus::core::interpreter::flag_remove(
                    static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                    static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::CONTINUE)
                )
            );
            continue;
        }
	}

	if(scope_result.value == nullptr) {
	    scope_result = daedalus::core::interpreter::evaluate_statement(interpreter, whileExpression->get_or_expression(), env);
	}

	return scope_result;
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_for_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
    auto forExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::ForExpression>(statement);

    auto for_env = std::make_shared<daedalus::core::env::Environment>(
		interpreter.envValuesProperties,
		interpreter.validationRules,
		env
	);

	daedalus::core::interpreter::evaluate_statement(interpreter, forExpression->get_initial_expression(), for_env);
    forExpression->push_back_body(forExpression->get_update_expression());

    daedalus::core::interpreter::RuntimeValueWrapper scope_result = daedalus::core::interpreter::wrap(nullptr);

	while(daedalus::core::interpreter::evaluate_statement(interpreter, forExpression->get_condition(), for_env).value->IsTrue()) {
	    std::vector<daedalus::core::interpreter::RuntimeResult> results = std::vector<daedalus::core::interpreter::RuntimeResult>();
		scope_result = daedalus::core::interpreter::evaluate_scope(
		    interpreter,
			forExpression,
			results,
			nullptr,
			for_env,
			static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
		);

        if(
            daedalus::core::interpreter::flag_contains(
                static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
            )
        ) {
            scope_result.flags = static_cast<daedalus::core::interpreter::Flags>(
                daedalus::core::interpreter::flag_remove(
                    static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                    static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
                )
            );
            break;
        }
        if(
            daedalus::core::interpreter::flag_contains(
                static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::CONTINUE)
            )
        ) {
            scope_result.flags = static_cast<daedalus::core::interpreter::Flags>(
                daedalus::core::interpreter::flag_remove(
                    static_cast<daedalus::core::interpreter::Flags>(scope_result.flags),
                    static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::CONTINUE)
                )
            );
            continue;
        }
	}

	if(scope_result.value == nullptr) {
	    scope_result = daedalus::core::interpreter::evaluate_statement(interpreter, forExpression->get_or_expression()->get_value(), env);
	}

	return scope_result;
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_break_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
    return daedalus::core::interpreter::wrap(
        std::make_shared<daedalus::core::values::NullValue>(),
        static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK),
        true
    );
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_continue_expression(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
    return daedalus::core::interpreter::wrap(
        std::make_shared<daedalus::core::values::NullValue>(),
        static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::CONTINUE),
        true
    );
}

daedalus::core::interpreter::RuntimeValueWrapper daedalus::entropia::interpreter::evaluate_conditionnal_structure(
	daedalus::core::interpreter::Interpreter& interpreter,
	std::shared_ptr<daedalus::core::ast::Statement> statement,
	std::shared_ptr<daedalus::core::env::Environment> env
) {
    auto conditionnalExpression = std::dynamic_pointer_cast<daedalus::entropia::ast::ConditionnalStructure>(statement);

    for(std::shared_ptr<daedalus::entropia::ast::ConditionnalExpression> expression : conditionnalExpression->get_expressions()) {
        bool isTrue = expression->get_condition() == nullptr;
        if(!isTrue) {
            isTrue = daedalus::core::interpreter::evaluate_statement(interpreter, expression->get_condition(), env).value->IsTrue();
        }
        if(isTrue) {
            auto results = std::vector<daedalus::core::interpreter::RuntimeResult>();
            daedalus::core::interpreter::RuntimeValueWrapper last = daedalus::core::interpreter::evaluate_scope(
                interpreter,
                expression,
                results,
                nullptr,
                env,
                static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK)
            );
            return last;
        }
	}

	throw std::runtime_error("No condition matched (should not have been parsed");
}

void setup_interpreter(daedalus::core::interpreter::Interpreter& interpreter) {

	auto nodeEvaluationFunctions = std::unordered_map<std::string, daedalus::core::interpreter::ParseStatementFunction>({
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
		    "WhileExpression",
			&daedalus::entropia::interpreter::evaluate_while_expression
		},
		{
		    "ForExpression",
			&daedalus::entropia::interpreter::evaluate_for_expression
		},
		{
		    "BreakExpression",
			&daedalus::entropia::interpreter::evaluate_break_expression
		},
		{
		    "ContinueExpression",
			&daedalus::entropia::interpreter::evaluate_continue_expression
		},
		{
		    "ConditionnalStructure",
			&daedalus::entropia::interpreter::evaluate_conditionnal_structure
		}
	});

	auto envValuesProperties = std::vector<std::string>({
		"isMutable"
	});

	auto mutabilityValidation = daedalus::core::env::EnvValidationRule{
		&daedalus::entropia::env::validate_mutability,
		std::vector<daedalus::core::env::ValidationRuleSensitivity>({
			daedalus::core::env::ValidationRuleSensitivity::SET
		})
	};

	auto typeCastValidation = daedalus::core::env::EnvValidationRule{
		&daedalus::entropia::env::validate_type_cast,
		std::vector<daedalus::core::env::ValidationRuleSensitivity>({
			daedalus::core::env::ValidationRuleSensitivity::SET
		})
	};

	auto validationRules = std::vector<daedalus::core::env::EnvValidationRule>({
		mutabilityValidation,
		typeCastValidation
	});

	auto flagsReturnStatementBefore = std::unordered_map<daedalus::core::interpreter::Flags, bool>({
	    {
			static_cast<daedalus::core::interpreter::Flags>(daedalus::entropia::interpreter::ValueEscapeFlags::BREAK),
			true
		}
	});

	daedalus::core::interpreter::setup_interpreter(
		interpreter,
		nodeEvaluationFunctions,
		envValuesProperties,
		validationRules
	);
}

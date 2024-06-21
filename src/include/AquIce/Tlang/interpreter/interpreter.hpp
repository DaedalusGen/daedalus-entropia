#ifndef __T_INTERPRETER__
#define __T_INTERPRETER__

#include "../parser/ast.hpp"
#include "values.hpp"
#include "env.hpp"
#include <AquIce/daedalus/interpreter.hpp>

void setup_interpreter(daedalus::interpreter::Interpreter& interpreter);

namespace tlang {
	namespace interpreter {
		std::shared_ptr<daedalus::values::RuntimeValue> evaluate_identifier(
			daedalus::interpreter::Interpreter& interpreter,
			std::shared_ptr<daedalus::ast::Statement> statement,
			std::shared_ptr<daedalus::env::Environment> env
		);
		std::shared_ptr<daedalus::values::RuntimeValue> evaluate_boolean_expression(
			daedalus::interpreter::Interpreter& interpreter,
			std::shared_ptr<daedalus::ast::Statement> statement,
			std::shared_ptr<daedalus::env::Environment> env
		);
		std::shared_ptr<daedalus::values::RuntimeValue> evaluate_unary_expression(
			daedalus::interpreter::Interpreter& interpreter,
			std::shared_ptr<daedalus::ast::Statement> statement,
			std::shared_ptr<daedalus::env::Environment> env
		);
		std::shared_ptr<daedalus::values::RuntimeValue> evaluate_binary_expression(
			daedalus::interpreter::Interpreter& interpreter,
			std::shared_ptr<daedalus::ast::Statement> statement,
			std::shared_ptr<daedalus::env::Environment> env
		);
		std::shared_ptr<daedalus::values::RuntimeValue> evaluate_assignation_expression(
			daedalus::interpreter::Interpreter& interpreter,
			std::shared_ptr<daedalus::ast::Statement> statement,
			std::shared_ptr<daedalus::env::Environment> env
		);
		std::shared_ptr<daedalus::values::RuntimeValue> evaluate_declaration_expression(
			daedalus::interpreter::Interpreter& interpreter,
			std::shared_ptr<daedalus::ast::Statement> statement,
			std::shared_ptr<daedalus::env::Environment> env
		);
	}
}

#endif // __T_INTERPRETER__
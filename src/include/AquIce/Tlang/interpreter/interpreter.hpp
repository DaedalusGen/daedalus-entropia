#ifndef __T_INTERPRETER__
#define __T_INTERPRETER__

#include <AquIce/Tlang/parser/ast.hpp>
#include <AquIce/Tlang/interpreter/values.hpp>
#include <AquIce/Tlang/interpreter/env.hpp>

#include <AquIce/daedalus/interpreter.hpp>

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <unordered_map>

void setup_interpreter(daedalus::interpreter::Interpreter& interpreter);

// TODO Add i + u types to interpreter

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
		std::shared_ptr<daedalus::values::RuntimeValue> evaluate_char_expression(
			daedalus::interpreter::Interpreter& interpreter,
			std::shared_ptr<daedalus::ast::Statement> statement,
			std::shared_ptr<daedalus::env::Environment> env
		);
		std::shared_ptr<daedalus::values::RuntimeValue> evaluate_str_expression(
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

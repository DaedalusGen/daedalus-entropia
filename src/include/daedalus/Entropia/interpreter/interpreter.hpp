#ifndef __DAEDALUS_ENTROPIA_INTERPRETER__
#define __DAEDALUS_ENTROPIA_INTERPRETER__

#include <daedalus/Entropia/parser/ast.hpp>
#include <daedalus/Entropia/interpreter/values.hpp>
#include <daedalus/Entropia/interpreter/env.hpp>

#include <daedalus/core/interpreter/interpreter.hpp>

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <unordered_map>

void setup_interpreter(daedalus::core::interpreter::Interpreter& interpreter);

// TODO Add i + u types to interpreter

namespace daedalus {
    namespace entropia {
    	namespace interpreter {
    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_identifier(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_boolean_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_char_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_str_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_unary_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_binary_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_assignation_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_declaration_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		std::shared_ptr<daedalus::core::values::RuntimeValue> evaluate_loop_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    	}
    }
}

#endif // __DAEDALUS_ENTROPIA_INTERPRETER__

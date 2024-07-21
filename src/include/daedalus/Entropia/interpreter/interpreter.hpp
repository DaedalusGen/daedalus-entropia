#ifndef __DAEDALUS_ENTROPIA_INTERPRETER__
#define __DAEDALUS_ENTROPIA_INTERPRETER__

#include <daedalus/Entropia/parser/ast.hpp>
#include <daedalus/Entropia/interpreter/values.hpp>
#include <daedalus/Entropia/interpreter/env.hpp>

#include <daedalus/core/interpreter/interpreter.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

void setup_interpreter(daedalus::core::interpreter::Interpreter& interpreter);

// TODO Add i + u types to interpreter

namespace daedalus {
    namespace entropia {
    	namespace interpreter {

            enum class ValueEscapeFlags {
                CONTINUE = 1,
                BREAK = 2
            };

    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_identifier(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_boolean_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_char_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_str_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_unary_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_binary_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_assignation_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_declaration_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_loop_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_while_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_break_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_continue_expression(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    		daedalus::core::interpreter::RuntimeValueWrapper evaluate_conditionnal_structure(
    			daedalus::core::interpreter::Interpreter& interpreter,
    			std::shared_ptr<daedalus::core::ast::Statement> statement,
    			std::shared_ptr<daedalus::core::env::Environment> env
    		);
    	}
    }
}

#endif // __DAEDALUS_ENTROPIA_INTERPRETER__

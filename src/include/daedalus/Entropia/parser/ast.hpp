#ifndef __DAEDALUS_ENTROPIA_AST__
#define __DAEDALUS_ENTROPIA_AST__

#include <daedalus/core/parser/ast.hpp>
#include <daedalus/core/tools/assert.hpp>

#include <string>
#include <memory>
#include <stdexcept>

namespace daedalus {
    namespace entropia {
    	namespace ast {

    		/**
    		 * TODO
    		 * FunctionCallExpression
    		 * FunctionDeclarationExpression
    		 * ConditionnalBlock
    		 * ConditionnalStructure
    		 * MatchStructure
    		 * BreakStatement
    		 * Loop
    		 * ForLoop
    		 * WhileLoop
    		 * TypeDeclaration
    		 * TypeImplementation
    		 */

    		// From daedalus core
    		// class Statement;
    		// class Scope;
    		// class Expression;
    		// class NumberExpression;

    		class Identifier;

    		class AssignationExpression;
    		class DeclarationExpression;

    		class BooleanExpression;
    		class CharExpression;

    		class ContainerExpression;
    		class UnaryExpression;
    		class BinaryExpression;

    		class Identifier : public daedalus::core::ast::Expression {
    		public:
    			Identifier(std::string name);

    			std::string get_name();

    			virtual std::string type() override;
    			virtual std::shared_ptr<daedalus::core::ast::Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;

    		private:
    			std::string name;
    		};

    		class AssignationExpression : public daedalus::core::ast::Expression {
    		public:
    			AssignationExpression(
    				std::shared_ptr<Identifier> identifier,
    				std::shared_ptr<daedalus::core::ast::Expression> value
    			);

    			std::shared_ptr<Identifier> get_identifier();
    			std::shared_ptr<daedalus::core::ast::Expression> get_value();

    			virtual std::string type() override;
    			virtual std::shared_ptr<daedalus::core::ast::Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;

    		protected:
    			std::shared_ptr<Identifier> identifier;
    			std::shared_ptr<daedalus::core::ast::Expression> value;
    		};

    		class DeclarationExpression : public AssignationExpression {
    		public:
    			DeclarationExpression(std::shared_ptr<Identifier> identifier, std::shared_ptr<daedalus::core::ast::Expression> value, std::string value_type, bool isMutable);

    			std::string get_value_type();
    			bool get_mutability();

    			virtual std::string type() override;
    			virtual std::shared_ptr<daedalus::core::ast::Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;

    		private:
    			std::string value_type;
    			bool isMutable;
    		};

    		class BooleanExpression : public daedalus::core::ast::Expression {
    		public:
    			bool value;

    			BooleanExpression(bool value);

    			virtual std::string type() override;
    			virtual std::shared_ptr<daedalus::core::ast::Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;
    		};

    		class CharExpression : public daedalus::core::ast::Expression {
    		public:
    			char value;

    			CharExpression(char value);

    			virtual std::string type() override;
    			virtual std::shared_ptr<daedalus::core::ast::Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;
    		};

    		class StrExpression : public daedalus::core::ast::Expression {
    		public:
    			std::string value;

    			StrExpression(std::string value);

    			virtual std::string type() override;
    			virtual std::shared_ptr<daedalus::core::ast::Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;
    		};

    		class ContainerExpression : public daedalus::core::ast::Expression {
    		public:
    			virtual std::shared_ptr<Identifier> contains_identifier();
    		};

    		class UnaryExpression : public ContainerExpression {
    		public:
    			UnaryExpression(
    				std::shared_ptr<Expression> term,
    				std::string operator_symbol
    			);

    			std::shared_ptr<Expression> get_term();
    			std::string get_operator_symbol();

    			virtual std::shared_ptr<Identifier> contains_identifier() override;

    			virtual std::string type() override;
    			virtual std::shared_ptr<daedalus::core::ast::Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;

    		private:
    			std::shared_ptr<Expression> term;
    			std::string operator_symbol;
    		};

    		class BinaryExpression : public ContainerExpression {
    		public:
    			BinaryExpression(
    				std::shared_ptr<Expression> left,
    				std::string operator_symbol,
    				std::shared_ptr<Expression> right
    			);

    			std::shared_ptr<Expression> get_left();
    			std::string get_operator_symbol();
    			std::shared_ptr<Expression> get_right();

    			virtual std::shared_ptr<Identifier> contains_identifier() override;

    			virtual std::string type() override;
    			virtual std::shared_ptr<daedalus::core::ast::Expression> get_constexpr() override;
    			virtual std::string repr(int indent = 0) override;

    		private:
    			std::shared_ptr<Identifier> left_contains_identifier();
    			std::shared_ptr<Identifier> right_contains_identifier();

    		private:
    			std::shared_ptr<Expression> left;
    			std::string operator_symbol;
    			std::shared_ptr<Expression> right;
    		};
    	}
    }
}

#endif // __DAEDALUS_ENTROPIA_AST__

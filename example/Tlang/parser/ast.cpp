#include <AquIce/Tlang/parser/ast.hpp>

#pragma region AssignationExpression

tlang::ast::AssignationExpression::AssignationExpression(
	std::shared_ptr<Identifier> identifier,
	std::shared_ptr<daedalus::ast::Expression> value
) {
	this->identifier = identifier;
	this->value = value;
}

std::shared_ptr<tlang::ast::Identifier> tlang::ast::AssignationExpression::get_identifier() {
	return this->identifier;
}
std::shared_ptr<daedalus::ast::Expression> tlang::ast::AssignationExpression::get_value() {
	return this->value;
}

std::string tlang::ast::AssignationExpression::type() {
	return "AssignationExpression";
}
std::string tlang::ast::AssignationExpression::repr(int indent) {
	return
		this->identifier->repr(indent) + "\n" +
		std::string(indent, '\t') + "=\n" +
		this->value->repr(indent);
}

#pragma endregion

#pragma region DeclarationExpression

tlang::ast::DeclarationExpression::DeclarationExpression(std::shared_ptr<Identifier> identifier, std::shared_ptr<daedalus::ast::Expression> value, std::string value_type, bool isMutable)
: AssignationExpression(identifier, value) {
	this->value_type = value_type;
	this->isMutable = isMutable;
}

std::string tlang::ast::DeclarationExpression::get_value_type() {
	return this->value_type;
}
bool tlang::ast::DeclarationExpression::get_mutability() {
	return this->isMutable;
}

std::string tlang::ast::DeclarationExpression::type() {
	return "DeclarationExpression";
}
std::string tlang::ast::DeclarationExpression::repr(int indent) {
	return
		std::string(indent, '\t') + std::string(this->isMutable ? "let " : "const ") + "\n" +
		this->identifier->repr(indent) + "\n" +
		std::string(indent, '\t') + ": " + this->value_type + "\n" +
		this->value->repr(indent);
}

#pragma endregion

#pragma region Identifier

tlang::ast::Identifier::Identifier(std::string name) {
	this->name = name;
}

std::string tlang::ast::Identifier::get_name() {
	return this->name;
}
std::string tlang::ast::Identifier::type() {
	return "Identifier";
}
std::string tlang::ast::Identifier::repr(int indent) {
	return std::string(indent, '\t') + this->name;
}

#pragma endregion

#pragma region BooleanExpression

tlang::ast::BooleanExpression::BooleanExpression(bool value) {
	this->value = value;
}

std::string tlang::ast::BooleanExpression::BooleanExpression::type() {
	return "BooleanExpression";
}
std::string tlang::ast::BooleanExpression::repr(int indent) {
	return std::string(indent, '\t') + std::string(this->value ? "true" : "false");
}

#pragma endregion

#pragma region UnaryExpression

tlang::ast::UnaryExpression::UnaryExpression(
	std::shared_ptr<Expression> term,
	std::string operator_symbol
) {
	this->term = term;
	this->operator_symbol = operator_symbol;
}

std::shared_ptr<daedalus::ast::Expression> tlang::ast::UnaryExpression::get_term() {
	return this->term;
}

std::string tlang::ast::UnaryExpression::get_operator_symbol() {
	return this->operator_symbol;
}

std::string tlang::ast::UnaryExpression::type() {
	return "UnaryExpression";
}
std::string tlang::ast::UnaryExpression::repr(int indent) {
	return
		std::string(indent, '\t') + "(\n" +
		std::string(indent + 1, '\t') + this->operator_symbol + "\n" +
		this->term->repr(indent + 1) + "\n" +
		std::string(indent, '\t') + ")";
}

#pragma endregion

#pragma region BinaryExpression

tlang::ast::BinaryExpression::BinaryExpression(
	std::shared_ptr<daedalus::ast::Expression> left,
	std::string operator_symbol,
	std::shared_ptr<daedalus::ast::Expression> right
) {
	this->left = left;
	this->operator_symbol = operator_symbol;
	this->right = right;
}

std::shared_ptr<daedalus::ast::Expression> tlang::ast::BinaryExpression::get_left() {
	return this->left;
}
std::string tlang::ast::BinaryExpression::get_operator_symbol() {
	return this->operator_symbol;
}
std::shared_ptr<daedalus::ast::Expression> tlang::ast::BinaryExpression::get_right() {
	return this->right;
}

std::string tlang::ast::BinaryExpression::type() {
	return "BinaryExpression";
}
std::string tlang::ast::BinaryExpression::repr(int indent) {
	return
		std::string(indent, '\t') + "(\n" +
		this->left->repr(indent + 1) + "\n" +
		std::string(indent + 1, '\t') + this->operator_symbol + "\n" +
		this->right->repr(indent + 1) + "\n" +
		std::string(indent, '\t') + ")";
}

#pragma endregion
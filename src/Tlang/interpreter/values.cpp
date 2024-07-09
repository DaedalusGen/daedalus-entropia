#include <AquIce/Tlang/interpreter/values.hpp>

#pragma region BooleanValue

tlang::values::BooleanValue::BooleanValue(bool value) {
	this->value = value;
}

bool tlang::values::BooleanValue::get() {
	return this->value;
}

std::string tlang::values::BooleanValue::type() {
	return "BooleanValue";
}
std::string tlang::values::BooleanValue::repr() {
	return this->value ? "true" : "false";
}
bool tlang::values::BooleanValue::IsTrue() {
	return this->value;
}

#pragma endregion

#pragma region CharValue

tlang::values::CharValue::CharValue(char value) {
	this->value = value;
}

char tlang::values::CharValue::get() {
	return this->value;
}

std::string tlang::values::CharValue::type() {
	return "CharValue";
}
std::string tlang::values::CharValue::repr() {
	return "'" + std::string(1, this->value) + "'";
}
bool tlang::values::CharValue::IsTrue() {
	return this->value != '\0';
}

#pragma endregion
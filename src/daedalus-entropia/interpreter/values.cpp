#include <daedalus/Entropia/interpreter/values.hpp>

#pragma region BooleanValue

daedalus::entropia::values::BooleanValue::BooleanValue(bool value) :
	value(value)
{}

bool daedalus::entropia::values::BooleanValue::get() {
	return this->value;
}

std::string daedalus::entropia::values::BooleanValue::type() {
	return "BooleanValue";
}
std::string daedalus::entropia::values::BooleanValue::repr() {
	return this->value ? "true" : "false";
}
bool daedalus::entropia::values::BooleanValue::IsTrue() {
	return this->value;
}

#pragma endregion

#pragma region CharValue

daedalus::entropia::values::CharValue::CharValue(char value) :
	value(value)
{}

char daedalus::entropia::values::CharValue::get() {
	return this->value;
}

std::string daedalus::entropia::values::CharValue::type() {
	return "CharValue";
}
std::string daedalus::entropia::values::CharValue::repr() {
	return "'" + std::string(1, this->value) + "'";
}
bool daedalus::entropia::values::CharValue::IsTrue() {
	return this->value != '\0';
}

#pragma endregion

#pragma region StrValue

daedalus::entropia::values::StrValue::StrValue(std::string value) :
	value(value)
{}

std::string daedalus::entropia::values::StrValue::get() {
	return this->value;
}

std::string daedalus::entropia::values::StrValue::type() {
	return "StrValue";
}
std::string daedalus::entropia::values::StrValue::repr() {
	return "\"" + this->value + "\"";
}
bool daedalus::entropia::values::StrValue::IsTrue() {
	return !this->value.empty();
}

#pragma endregion

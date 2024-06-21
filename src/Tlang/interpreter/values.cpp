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
	return this->get();
}

#pragma endregion
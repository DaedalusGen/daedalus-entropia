#include <AquIce/Tlang/interpreter/env.hpp>

bool tlang::env::validate_mutability(daedalus::env::EnvValue value) {
	try {
		return value.properties.at("isMutable") == "true";
	} catch(const std::exception& e) {
		throw std::runtime_error("Trying to access undeclared property \"isMutable\"");
	}
}

std::string tlang::env::get_mutability_error_message(std::string key, std::string on) {
	return "Trying to set immutable value \"" + key + "\"";
}
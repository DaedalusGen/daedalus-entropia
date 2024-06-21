#include <AquIce/Tlang/interpreter/env.hpp>

bool tlang::env::validate_mutability(daedalus::env::EnvValue value) {
	try {
		return value.properties.at("isMutable") == "true";
	} catch(const std::exception& e) {
		throw std::runtime_error("Trying to access undeclared property \"isMutable\"");
	}
}
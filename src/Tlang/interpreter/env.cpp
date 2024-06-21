#include <AquIce/Tlang/interpreter/env.hpp>

#include <iostream>

daedalus::env::EnvValue tlang::env::validate_mutability(daedalus::env::EnvValue env_value, std::shared_ptr<daedalus::values::RuntimeValue> new_value, std::string key) {
	try {
		if(env_value.properties.at("isMutable") == "true") {
			return daedalus::env::EnvValue{ new_value, env_value.properties };
		}
	} catch(const std::exception& e) {
		throw std::runtime_error("Trying to access undeclared property \"isMutable\"");
	}
	throw std::runtime_error("Trying to assign to immutable value \"" + key + "\"");
}
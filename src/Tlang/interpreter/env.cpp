#include <AquIce/Tlang/interpreter/env.hpp>

daedalus::env::EnvValue tlang::env::validate_mutability(daedalus::env::EnvValue env_value, std::shared_ptr<daedalus::values::RuntimeValue> new_value, std::string key) {
	DAE_ASSERT_TRY(
		{
			if(env_value.properties.at("isMutable") == "true") {
				return (daedalus::env::EnvValue{ new_value, env_value.properties });
			}
		},
		std::runtime_error("Trying to access undeclared property \"isMutable\"")
	)
	throw std::runtime_error("Trying to assign to immutable value \"" + key + "\"");
}

daedalus::env::EnvValue tlang::env::validate_type_cast(daedalus::env::EnvValue env_value, std::shared_ptr<daedalus::values::RuntimeValue> new_value, std::string key) {
	DAE_ASSERT_TRY(
		{
			if(env_value.value->type() == new_value->type()) {
				return (daedalus::env::EnvValue{ new_value, env_value.properties });
			}
		},
		std::runtime_error("Trying to access undeclared property \"isMutable\"");
	)
	throw std::runtime_error("Trying to assign value of type \"" + new_value->type() + "\" to value of type \"" + env_value.value->type() + "\"");
}

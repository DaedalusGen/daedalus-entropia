#include <daedalus/Entropia/interpreter/env.hpp>

daedalus::core::env::EnvValue daedalus::entropia::env::validate_mutability(daedalus::core::env::EnvValue env_value, std::shared_ptr<daedalus::core::values::RuntimeValue> new_value, std::string key) {
	DAE_ASSERT_TRY(
		{
			if(env_value.properties.at("isMutable") == "true") {
				return (daedalus::core::env::EnvValue{ new_value, env_value.properties });
			}
		},
		std::runtime_error("Trying to access undeclared property \"isMutable\"")
	)
	throw std::runtime_error("Trying to assign to immutable value \"" + key + "\"");
}

daedalus::core::env::EnvValue daedalus::entropia::env::validate_type_cast(daedalus::core::env::EnvValue env_value, std::shared_ptr<daedalus::core::values::RuntimeValue> new_value, std::string key) {
	DAE_ASSERT_TRY(
		{
			if(env_value.value->type() == new_value->type()) {
				return (daedalus::core::env::EnvValue{ new_value, env_value.properties });
			}
		},
		std::runtime_error("Trying to access undeclared property \"isMutable\"");
	)
	throw std::runtime_error("Trying to assign value of type \"" + new_value->type() + "\" to value of type \"" + env_value.value->type() + "\"");
}

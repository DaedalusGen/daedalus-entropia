#ifndef __T_ENV__
#define __T_ENV__

#include <AquIce/daedalus/env.hpp>

#include <memory>
#include <stdexcept>

namespace tlang {
	namespace env {
		daedalus::env::EnvValue validate_mutability(daedalus::env::EnvValue env_value, std::shared_ptr<daedalus::values::RuntimeValue> new_value, std::string key);
		daedalus::env::EnvValue validate_type_cast(daedalus::env::EnvValue env_value, std::shared_ptr<daedalus::values::RuntimeValue> new_value, std::string key);
	}
}

#endif // __T_ENV__

#ifndef __DAEDALUS_ENTROPIA_ENV__
#define __DAEDALUS_ENTROPIA_ENV__

#include <daedalus/core/interpreter/env.hpp>

#include <memory>
#include <stdexcept>

namespace daedalus {
    namespace entropia {
        namespace env {
    		daedalus::core::env::EnvValue validate_mutability(daedalus::core::env::EnvValue env_value, std::shared_ptr<daedalus::core::values::RuntimeValue> new_value, std::string key);
    		daedalus::core::env::EnvValue validate_type_cast(daedalus::core::env::EnvValue env_value, std::shared_ptr<daedalus::core::values::RuntimeValue> new_value, std::string key);
    	}
    }
}

#endif // __DAEDALUS_ENTROPIA_ENV__

#ifndef __T_ENV__
#define __T_ENV__

#include <AquIce/daedalus/env.hpp>

namespace tlang {
	namespace env {
		bool validate_mutability(daedalus::env::EnvValue value);
		std::string get_mutability_error_message(std::string key, std::string on);
	}
}

#endif // __T_ENV__
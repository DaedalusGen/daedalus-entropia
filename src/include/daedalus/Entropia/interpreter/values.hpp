#ifndef __DAEDALUS_ENTROPIA_VALUES__
#define __DAEDALUS_ENTROPIA_VALUES__

#include <daedalus/core/interpreter/values.hpp>

#include <string>

namespace daedalus {
    namespace entropia {
    	namespace values {
    		class BooleanValue : public daedalus::core::values::RuntimeValue {
    		public:
    			/**
    			 * Create a new Boolean Value
    			 */
    			BooleanValue(bool value = false);

    			bool get();

    			virtual std::string type() override;
    			virtual std::string repr() override;
    			virtual bool IsTrue() override;

    		private:
    			bool value;
    		};

    		class CharValue : public daedalus::core::values::RuntimeValue {
    		public:
    			/**
    			 * Create a new Char Value
    			 */
    			CharValue(char value = '\0');

    			char get();

    			virtual std::string type() override;
    			virtual std::string repr() override;
    			virtual bool IsTrue() override;

    		private:
    			char value;
    		};

    		class StrValue : public daedalus::core::values::RuntimeValue {
    		public:
    			/**
    			 * Create a new String Value
    			 */
    			StrValue(std::string value = "");

    			std::string get();

    			virtual std::string type() override;
    			virtual std::string repr() override;
    			virtual bool IsTrue() override;

    		private:
    			std::string value;
    		};
    	}
    }
}

#endif

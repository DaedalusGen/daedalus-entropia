#ifndef __DAEDALUS_ENTROPIA_LEXER__
#define __DAEDALUS_ENTROPIA_LEXER__

#include <daedalus/core/lexer/lexer.hpp>

#include <string>
#include <stdexcept>

std::string parse_character(std::string src);

void setup_lexer(daedalus::core::lexer::Lexer& lexer);

#endif // __DAEDALUS_ENTROPIA_LEXER__

#ifndef __T_LEXER__
#define __T_LEXER__

#include <AquIce/daedalus/lexer.hpp>

std::string parse_character(std::string& src);

void setup_lexer(daedalus::lexer::Lexer& lexer);

#endif // __T_LEXER__
#include <AquIce/Tlang/lexer/lexer.hpp>

void setup_lexer(daedalus::lexer::Lexer& lexer) {
	std::vector<daedalus::lexer::TokenType> tokenTypes = std::vector<daedalus::lexer::TokenType>({
		daedalus::lexer::make_token_type("ASSIGN_KEYWORD", "let"),
		daedalus::lexer::make_token_type("ASSIGN_KEYWORD", "const"),
		daedalus::lexer::make_token_type("SEMICOLON", ";"),
		daedalus::lexer::make_token_type("COLON", ":"),
		daedalus::lexer::make_token_type("ASSIGN", "="),
		daedalus::lexer::make_token_type("OPEN_PAREN", "("),
		daedalus::lexer::make_token_type("CLOSE_PAREN", ")"),
		daedalus::lexer::make_token_type("OPERATOR", "+"),
		daedalus::lexer::make_token_type("OPERATOR", "-"),
		daedalus::lexer::make_token_type("OPERATOR", "*"),
		daedalus::lexer::make_token_type("OPERATOR", "/"),
		daedalus::lexer::make_token_type("OPERATOR", "&&"),
		daedalus::lexer::make_token_type("OPERATOR", "||"),
		daedalus::lexer::make_token_type("UNARY_OPERATOR", "!"),
		daedalus::lexer::make_token_type("BOOL", "true"),
		daedalus::lexer::make_token_type("BOOL", "false"),
		daedalus::lexer::make_token_type("TYPE", "number"),
		daedalus::lexer::make_token_type("TYPE", "boolean"),
		daedalus::lexer::make_token_type(
			"NUMBER",
			[lexer](std::string src) -> std::string {
				std::string number = "";
				size_t i = 0;
				bool isInteger = true;
				while(isdigit(src.at(i)) || src.at(i) == lexer.decimalSeparator) {
					if(src.at(i) == lexer.decimalSeparator) {
						if(!isInteger) {
							throw std::runtime_error("Invalid number format");
						}
						isInteger = false;
					}
					number += src.at(i);
					if(++i == src.length()) {
						break;
					}
				}
				return number;
			}
		),
		daedalus::lexer::make_token_type(
			"IDENTIFIER",
			[lexer](std::string src) -> std::string {
				std::string identifier = "";
				size_t i = 0;
				while(isalpha(src.at(i)) || src.at(i) == '_') {
					identifier += src.at(i);
					if(++i == src.length()) {
						break;
					}
				}
				return identifier;
			}
		)
	});

	daedalus::lexer::setup_lexer(
		lexer,
		tokenTypes
	);
}
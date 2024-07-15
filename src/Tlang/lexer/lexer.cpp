#include <AquIce/Tlang/lexer/lexer.hpp>

std::string parse_character(std::string src) {
	std::string chr = "";

	if(src.at(0) == '\\') {
		char c = src.at(1);
		switch(c) {
			case 'n':
				return "\\n";
			case 't':
				return "\\t";
			case 'r':
				return "\\r";
			case '\'':
				return "\\'";
			case '\\':
				return "\\\\";
			case '"':
				return "\\\"";
			default:
				throw std::runtime_error("Invalid escape character '" + std::string(1, c) + "' in " + chr + "\\" + c + src.at(2));
		}
	}

	chr += src.at(0);

	return chr;
}

void setup_lexer(daedalus::lexer::Lexer& lexer) {
	std::vector<daedalus::lexer::TokenType> tokenTypes = std::vector<daedalus::lexer::TokenType>({
		daedalus::lexer::make_token_type("LET", "let"),
		daedalus::lexer::make_token_type("MUT", "mut"),

		daedalus::lexer::make_token_type("SEMICOLON", ";"),
		
		daedalus::lexer::make_token_type("COLON", ":"),
		
		daedalus::lexer::make_token_type("ASSIGN", "="),

		daedalus::lexer::make_token_type("OPEN_PAREN", "("),
		daedalus::lexer::make_token_type("CLOSE_PAREN", ")"),

		daedalus::lexer::make_token_type("BINARY_OPERATOR", "+"),
		daedalus::lexer::make_token_type("BINARY_OPERATOR", "-"),
		daedalus::lexer::make_token_type("BINARY_OPERATOR", "*"),
		daedalus::lexer::make_token_type("BINARY_OPERATOR", "/"),

		daedalus::lexer::make_token_type("BINARY_OPERATOR", "&&"),
		daedalus::lexer::make_token_type("BINARY_OPERATOR", "||"),

		daedalus::lexer::make_token_type("UNARY_OPERATOR", "!"),
		
		daedalus::lexer::make_token_type("BOOL", "true"),
		daedalus::lexer::make_token_type("BOOL", "false"),

		daedalus::lexer::make_token_type("TYPE", "i8"),
		daedalus::lexer::make_token_type("TYPE", "i16"),
		daedalus::lexer::make_token_type("TYPE", "i32"),
		daedalus::lexer::make_token_type("TYPE", "i64"),
		daedalus::lexer::make_token_type("TYPE", "u8"),
		daedalus::lexer::make_token_type("TYPE", "u16"),
		daedalus::lexer::make_token_type("TYPE", "u32"),
		daedalus::lexer::make_token_type("TYPE", "u64"),
		daedalus::lexer::make_token_type("TYPE", "f32"),
		daedalus::lexer::make_token_type("TYPE", "f64"),
		
		daedalus::lexer::make_token_type("TYPE", "bool"),
		daedalus::lexer::make_token_type("TYPE", "char"),
		daedalus::lexer::make_token_type("TYPE", "str"),

		daedalus::lexer::make_token_type(
			"NUMBER",
			[lexer](std::string src) -> std::string {
				std::string number = "";
				size_t i = 0;
				bool isInteger = true;
				while(isdigit(src.at(i)) || src.at(i) == lexer.decimalSeparator) {
					if(src.at(i) == lexer.decimalSeparator) {
						DAE_ASSERT_TRUE(
							isInteger,
							std::runtime_error("Invalid number format: double decimal separator in \"" + number + src.at(i) + "\"");
						)
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
			"CHAR",
			[lexer](std::string src) -> std::string {
				std::string chr = "";

				size_t index = 0;

				if(src.at(index) == '\'') {

					chr += src.at(index);
					index++;

					std::string parsed_character = parse_character(src.substr(index));

					DAE_ASSERT_TRUE(
						parsed_character.at(0) != '\'',
						std::runtime_error("Invalid character format: empty character in " + parsed_character)
					)

					chr += parsed_character;
					index += parsed_character.length();

					char c = src.at(index);
					chr += c;
					
					DAE_ASSERT_TRUE(
						c == '\'',
						std::runtime_error("Invalid character format: expected closing character instead of " + std::string(1, c) + " in " + chr + c)
					)
				}
				
				return chr;
			}
		),
		daedalus::lexer::make_token_type(
			"STR",
			[lexer](std::string src) -> std::string {
				std::string str = "";
				
				if(peek(src) == '"') {
					str += peek(src);
					size_t i = 1;

					while(src.at(i) != '"') {
						std::string parsed_character = parse_character(src.substr(i));
						i += parsed_character.length();
						str += parsed_character;
					}
					
					str += '"';
				}

				return str;
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
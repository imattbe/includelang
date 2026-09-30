#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
bool hasSuffix(const std::string& name, const std::string& suffix) {
	int suffixLen = suffix.length();
	int nameLen = name.length();
	if (nameLen > suffixLen && name.substr(nameLen-suffixLen,suffixLen).compare(suffix) == 0) {
		return true;
	} else {
		return false;
	}
}

enum class TokenType {
	PLUS,
	STAR,
	MINUS,
	SLASH,
	PRINT,
	EQUALITY,
	NUMBER,
	LEFT_PAREN,
	RIGHT_PAREN,
	SEMICOLON,
	END_OF_FILE,
};

class LexerToken{
	public:
		int line;
		int column;
		TokenType type;
		std::string contents;
		LexerToken(int line, int column, TokenType type, std::string contents) : line(line), column(column), type(type), contents(contents) {}
};

int main(int argc, char** argv) {
	int inputFileCount = 0;
	std::string inputName;
	std::string outputName;
	bool outputTokens = false;
	

	// CLI argument parsing, add all other options before the input name.
	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]).compare("-o") == 0) {
			if (i + 1 < argc) {
				outputName = argv[i + 1];
				i++;
			} else {
				std::cerr << "-o requires an argument\n";
				return 1;
			}
		} else if (std::string(argv[i]).compare("--tokens") == 0) {
			outputTokens = true;
		} else if (argv[i][0] == '-') {
			std::cerr << "unknown option: " << argv[i] << "\n";
			return 1;
		} else {
			if (inputFileCount == 0) {
				if (hasSuffix(argv[i], ".ilf")) {
					inputName = argv[i];
				} else {
					std::cerr << "input file must have .ilf suffix\n";
					return 1;
				}
			}
			inputFileCount++;
		}
		
	}
	if (outputName.empty()) {
		outputName = inputName + ".out";
	}
	
	if (inputFileCount == 0) {
		std::cerr << "no input file specified\n";
		return 1;
	} else if (inputFileCount > 1) {
		std::cerr << inputFileCount-1 << " too many input files specified\n";
		return 1;
	} 
	else if (outputName.compare(inputName) == 0) {
		std::cerr << "output file name cannot be the same as input file name\n";
		return 1;
	}


	std::ifstream in(inputName);
	if (!in) { std::cerr << "could not open " << inputName << "\n"; return 1; }
	
	std::stringstream buffer;
	buffer << in.rdbuf();
	std::string source = buffer.str();
	std::vector<LexerToken> tokens;
	int line = 1;
	int column = 1;
	for (int i = 0; i < source.length(); i++) {
		if (source[i] == '(') {
			tokens.push_back(LexerToken(line, column, TokenType::LEFT_PAREN, "("));
		} else if (source[i] == ')') {
			tokens.push_back(LexerToken(line, column, TokenType::RIGHT_PAREN, ")"));
		} else if (source[i] == '+') {
			tokens.push_back(LexerToken(line, column, TokenType::PLUS, "+"));
		} else if (source[i] == '-') {
			tokens.push_back(LexerToken(line, column, TokenType::MINUS, "-"));
		} else if (source[i] == '*') {
			tokens.push_back(LexerToken(line, column, TokenType::STAR, "*"));
		} else if (source[i] == '/') {
			tokens.push_back(LexerToken(line, column, TokenType::SLASH, "/"));
		} else if (source[i] == '=') {
			tokens.push_back(LexerToken(line, column, TokenType::EQUALITY, "="));
		} else if (source[i] == ';') {
			tokens.push_back(LexerToken(line, column, TokenType::SEMICOLON, ";"));
		} else if (source[i] >= '0' && source[i] <= '9') {
			std::string number;
			while (i < source.length() && source[i] >= '0' && source[i] <= '9') {
				number += source[i];
				i++;
				column++;
			}
			i--;
			tokens.push_back(LexerToken(line, column - number.length(), TokenType::NUMBER, number));
		} else if (source[i] == '\n') {
			line++;
			column = 1;
		} else if (source.substr(i, 5).compare("print") == 0) {
			tokens.push_back(LexerToken(line, column, TokenType::PRINT, "print"));
			i += 4;
			column += 5;

		} else if (source[i] == ' ' || source[i] == '\t' || source[i] == '\r') {
			// ignore whitespace
			column++;
		} else {
			std::cerr << "unknown token: " << source[i] << "line:" << line << "column:" << column << "\n";
			return 1;
		}
	}
	tokens.push_back(LexerToken(line, column, TokenType::END_OF_FILE, ""));

	if (outputTokens) {
		std::cout << "line " << " column " << " type " << " contents\n";
		for (const auto& token : tokens) {
			std::cout << token.line << " " << token.column << " ";
			switch (token.type) {
				case TokenType::PLUS:
					std::cout << "PLUS";
					break;
				case TokenType::MINUS:
					std::cout << "MINUS";
					break;
				case TokenType::STAR:
					std::cout << "STAR";
					break;
				case TokenType::SLASH:
					std::cout << "SLASH";
					break;
				case TokenType::PRINT:
					std::cout << "PRINT";
					break;
				case TokenType::EQUALITY:
					std::cout << "EQUALITY";
					break;
				case TokenType::NUMBER:
					std::cout << "NUMBER";
					break;
				case TokenType::LEFT_PAREN:
					std::cout << "LEFT_PAREN";
					break;
				case TokenType::RIGHT_PAREN:
					std::cout << "RIGHT_PAREN";
					break;
				case TokenType::SEMICOLON:
					std::cout << "SEMICOLON";
					break;
				case TokenType::END_OF_FILE:
					std::cout << "END_OF_FILE";
					break;
			}
			std::cout << " \"" << token.contents << "\"\n";
		}
	}


	std::ofstream out(outputName);
	if (!out) { std::cerr << "could not create " << outputName << "\n"; return 3; }
	out.close();
	return 0;
}

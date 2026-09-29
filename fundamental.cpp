#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
using namespace std;
bool hasSuffix(std::string name, std::string suffix) {
	int suffixLen = suffix.length();
	int nameLen = name.length();
	if (nameLen > suffixLen && name.substr(nameLen-suffixLen,suffixLen).compare(suffix) == 0) {
		return true;
	} else {
		return false;
	}
}

enum class TokenType {
	IDENTIFIER,
	KEYWORD,
	NUMBER,
	STRING,
	OPERATOR,
	DELIMITER,
	COMMENT,
	WHITESPACE,
	UNKNOWN,
	END_OF_FILE
};

class LexerToken{
	int line;
	int character;
	TokenType type;
	std::string contents;
};

int main(int argc, char** argv) {
	cout << "hello world" << '\n';
	
	

	std::string inputName = argv[1];
	std::string outputName;
	std::ifstream in(inputName);

	if (!in) { std::cerr << "could not open " << inputName << "\n"; return 1; }

	for (int i = 2; i < argc; i++) {
		if (std::string(argv[i]).compare("-o") == 0) {
			if (i + 1 < argc) {
				outputName = argv[i + 1];
				i++;
			} else {
				std::cerr << "missing output file name after -o\n";
				return 1;
			}
		}
	}

	if (outputName.compare(inputName) == 0) {
		std::cerr << "output file name cannot be the same as input file name\n";
		return 2;
	} else if (outputName.empty()) {
		outputName = inputName + ".out";
	}

	std::stringstream buffer;
	buffer << in.rdbuf();
	std::string source = buffer.str();


	FILE* out = fopen(outputName.c_str(), "w");
	if (!out) { std::cerr << "could not create " << outputName << "\n"; return 3; }
	fclose(out);
	return 0;
}

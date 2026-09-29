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
	NUMBER
};

class LexerToken{
	public:
		int line;
		int column;
		TokenType type;
		std::string contents;
};

int main(int argc, char** argv) {
	int inputFileCount = 0;
	std::string inputName;
	std::string outputName;
	

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
		} else if (argv[i][0] != '-') {
			std::cerr << "unknown option: " << argv[i] << "\n";
			return 1;
		} else {
			if (inputFileCount ==0) {
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
		std::cerr << inputFileCount << " too many input files specified\n";
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


	std::ofstream out(outputName);
	if (!out) { std::cerr << "could not create " << outputName << "\n"; return 3; }
	out.close();
	return 0;
}

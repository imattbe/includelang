#include <iostream>
#include <stdio.h>
using namespace std;
int main(int argc, char** argv) {
	cout << "hello world";
	
	if (argc > 1) {
		std::string name(argv[1]);
		if (name.length() > 4 && name.substr(name.length()-5,4).compare(".ilf")) {
				try {
					FILE *file = fopen(name.c_str(), "w");
					cout << file;
					if (file == NULL) {
						throw 500;
					} else {
						fclose(file);
					}	
				}
				catch (...){
					cout << "failed to create file";
				}
		} else {
			try {
                                FILE *file = fopen((name+".ilf").c_str(), "w");
				cout << file;
                                if (file == NULL) {
                                        throw 500;
                                } else {
                                        fclose(file);
                                }
                        }
                        catch (...){
                                cout << "failed to create file";
                        }
		}
	} else {
		cout << "incorrect arguments";
	}
	return 0;
}

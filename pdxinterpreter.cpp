#include"pdxinterpreter.h"
#include<algorithm>
#include<sstream>

void divide_statement(const std::string& line, std::string& identifier, std::string& token) {
	std::string reducedline = line;
	reducedline.erase(0, reducedline.find_first_not_of(" \t"));
	reducedline.erase(reducedline.find_last_not_of(" \t") + 1);
	
	identifier = reducedline.substr(0, reducedline.find_first_of("="));
	identifier.erase(0, identifier.find_first_not_of(" \t"));
	identifier.erase(identifier.find_last_not_of(" \t") + 1);

	token = reducedline.substr(reducedline.find_first_of("=") + 1, reducedline.find_first_of("#") - reducedline.find_first_of("=") - 1);
	token.erase(0, token.find_first_not_of(" \t"));
	token.erase(token.find_last_not_of(" \t") + 1);
}

void get_color(const std::string& line, unsigned char& r, unsigned char& b, unsigned char& g) {
	using namespace std;
	
	string colorline = line.substr(line.find_first_of("{") + 1, line.find_first_of("}") - line.find_first_of("{") - 1);
	stringstream lineStream(colorline);

	lineStream >> r >> g >> b;
}
#include"main.h"
#include"popdata.h"
#include<iostream>
#include<filesystem>
#include"pdxinterpreter.h"

std::vector<poptemplate> poptemplates;

void load_templates() {
	using namespace std;
	
	poptemplates.clear();

	cout << "\n\nLoading templates...\n";

	for (filesystem::directory_entry entry : filesystem::directory_iterator("templates")) {
		ifstream templatefile(entry.path());
		
		poptemplate poptemp;
		
		int indent = 0;

		while (!templatefile.eof()) {
			string line;
			getline(templatefile, line);

			//line.erase(0, line.find_first_not_of(" \t"));
			if (line.find_first_not_of(" \t") == std::string::npos) continue;

			string identifier, token;
			divide_statement(line, identifier, token);

			if (token == "{") indent++;
			else if (identifier == "}") indent--;

			if (token == "{" && indent == 1) {
				poptemp.name = identifier;
			}
			else if (token == "{" && indent == 2) {
				pop newpop(templatefile, identifier);
				indent--;
				poptemp.poplist.push_back(newpop);
			}
		}
		
		templatefile.close();

		poptemplates.push_back(poptemp);
	}

	std::cout << "Loaded templates:\n";
	for (poptemplate t : poptemplates) {
		std::cout << t.name << " ";
	}
	std::cout << "\n";
}
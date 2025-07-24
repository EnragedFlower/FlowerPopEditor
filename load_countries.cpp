#include"main.h"
#include"popdata.h"
#include<iostream>

std::vector<country> countrylist;

void load_countries() {
	std::cout << "\n\nLoading countries...\n";
	
	std::ifstream countriesfile(commonfolder_path + "\\countries.txt");
	while (!countriesfile.eof()) {
		std::string line;
		getline(countriesfile, line);

		line.erase(0, line.find_first_not_of(" \t"));

		if (line.find_first_not_of(" \t") == std::string::npos) continue;
		if (line[0] == '#') continue;
		
		std::string token = line.substr(0, line.find_first_of(" \t"));
		if (token == "dynamic_tags") break;

		std::string tag = token;
		countrylist.push_back(*new country(tag));
	}
	countriesfile.close();
	
	std::cout << "Loaded countries:\n";
	for (country c : countrylist) {
		std::cout << c.GetTag() << " ";
	}
	std::cout << "\n";
}
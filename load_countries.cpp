#include"main.h"
#include"popdata.h"
#include<filesystem>
#include<iostream>
#include"pdxinterpreter.h"

std::vector<country> countrylist;

void load_country_history(const std::string& path) {
	using namespace std;

	ifstream countryfile(path);

	string filename = path;

	filename.erase(0, filename.find_last_of("\\") + 1);
	if (filename[0] == 'D' && isdigit(filename[1])) return; // dynamic country
	filename = filename.substr(0, filename.find_first_of(" -"));

	int index = -1;
	for (int i = 0; i < countrylist.size(); i++) {
		if (countrylist[i].GetTag() == filename) {
			index = i;
			break;
		}
	}
	if (index < 0) cout << "ERROR: couldn't find country with tag " << filename << ", from file\n" << path << "\n";

	string primaryculture = "", religion = "";
	vector<string> secondarycultures;

	while (!countryfile.eof()) {
		string line;
		getline(countryfile, line);

		line.erase(0, line.find_first_not_of(" \t"));

		string identifier, token;
		divide_statement(line, identifier, token);

		if (identifier == "primary_culture") primaryculture = token;
		else if (identifier == "culture") secondarycultures.push_back(token);
		else if (identifier == "religion") religion = token;
	}

	countrylist[index].Load(primaryculture, secondarycultures, religion);
}

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
	
	std::cout << "Loading histories...\n";

	for (std::filesystem::directory_entry entry : std:: filesystem::directory_iterator(historyfolder_path + "\\countries")) {
		load_country_history(entry.path().string());
	}

	std::cout << "Loaded countries:\n";
	for (country c : countrylist) {
		std::cout << c.GetTag() << " ";
	}
	std::cout << "\n";
}
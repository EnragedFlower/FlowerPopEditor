#include"main.h"
#include"popdata.h"
#include<iostream>
#include<fstream>
#include<filesystem>
#include"pdxinterpreter.h"

std::string basefolder_path;
std::string commonfolder_path;
std::string historyfolder_path;
std::string popfolder_path;

void get_folders() {
	using namespace std;
	
	ifstream foldersfile("gamefolders.txt");
	if (foldersfile) {

		while (!foldersfile.eof())
		{
			string line;
			getline(foldersfile, line);

			if (line[0] == '#') continue;

			string identifier, token;
			divide_statement(line, identifier, token);

			if (identifier == "basefolder") basefolder_path = token;
			if (identifier == "commonfolder") commonfolder_path = token;
			if (identifier == "historyfolder") historyfolder_path = token;
			if (identifier == "popfolder") popfolder_path = token;
		}
		
		cout << "base folder:\n" << basefolder_path << "\n";
		if (!std::filesystem::exists(basefolder_path + "\\map\\terrain.txt"))
			cout << "ERROR: No useful map folder was found with specified basefolder path\n";

		cout << "common folder:\n" << commonfolder_path << "\n";
		if (!std::filesystem::exists(commonfolder_path + "\\countries.txt"))
			cout << "ERROR: No valid file was found with specified commonfolder path\n";

		cout << "history folder:\n" << historyfolder_path << "\n";
		if (!std::filesystem::exists(historyfolder_path))
			cout << "ERROR: No folder was found with specified historyfolder path\n";

		cout << "pop folder:\n" << popfolder_path << "\n";
		if (!std::filesystem::exists(popfolder_path))
			cout << "ERROR: No folder was found with specified popfolder path\n";

		return;
	}

	bool error = false;

	do {
		cout << "\nInsert path from root to the base of the map folder you want to consider.\n"
			<< "Base of the map folder is used for terrain types.\n"
			<< "\nExample: \"C:\\...\\Victoria II\" - to use vanilla terrain\n"
			<< "Example: \"C:\\...\\Victoria II\\mod\\MyCoolMod\" - to use modded terrain\n"
			<< "path: ";
		getline(cin, basefolder_path);
		
		error = !std::filesystem::exists(basefolder_path + "\\map\\terrain.txt");
		if (error) cout << "\nERROR: No useful map folder was found with this path\n";
	}
	while(error);

	do {
		cout << "\nInsert path from root to the common folder with the countries.txt file you want to consider.\n"
			<< "countries.txt is used to obtain a list of valid countries.\n"
			<< "\nExample: \"C:\\...\\Victoria II\\mod\\MyCoolMod\\common\"\n"
			<< "path: ";
		getline(cin, commonfolder_path);
		
		error = !std::filesystem::exists(commonfolder_path + "\\countries.txt");
		if(error) cout << "\nERROR: No valid file was found with this path\n";
	}
	while(error);

	do {
		cout << "\nInsert path from root to the history folder you want to consider.\n"
			<< "The folder is used for province owners and RGOs.\n"
			<< "\nExample: \"C:\\...\\Victoria II\\mod\\MyCoolMod\\history\"\n"
			<< "path: ";
		getline(cin, historyfolder_path);

		error = !std::filesystem::exists(historyfolder_path);
		if (error) cout << "\nERROR: No folder was found with this path\n";
	} while (error);

	do {
		cout << "\nInsert path from root to the pop folder you want to consider.\n"
			<< "The folder is used for the orginal pops to edit, and to inherit some pop values.\n"
			<< "\nExample: \"C:\\...\\Victoria II\\mod\\MyCoolMod\\history\\pops\\1836.1.1\"\n"
			<< "path: ";
		getline(cin, popfolder_path);

		error = !std::filesystem::exists(popfolder_path);
		if (error) cout << "\nERROR: No folder was found with this path\n";
	} while (error);

	error = !std::filesystem::exists("terrain.bmp");
	if (error) cout << "\nERROR: the program expects a copy of terrain.bmp in the root folder\n";
}

#include"main.h"
#include"popdata.h"
#include<filesystem>
#include<fstream>
#include<iostream>

void save_provinces() {
	std::cout << "Saving pops...\n";

	std::vector<std::string> saved_files;
	std::ofstream popfile;

	for (int i = 0; i < provincelist.size(); i++) {
		std::string filename;
		std::filesystem::path og_popfile_path(provincelist[i].popfile);
		filename = og_popfile_path.filename().string();

		bool already_saved = false;
		for (int j = 0; j < saved_files.size(); j++) {

			if (filename == saved_files[j]) {
				already_saved = true;
				break;
			}
		}

		if (already_saved) continue;
		else {
			popfile.open("output\\" + filename, std::ios::trunc);
			if (!popfile.is_open()) {
				std::cout << "\nERROR: failed to open or create output\\" << filename << ", does output folder exist?\n";
				std::cout << i << "/" << provincelist.size() << ": " << provincelist[i].id << "\n" << provincelist[i].popfile << "\n";
			}
			saved_files.push_back(filename);
		}

		for (int j = i; j < provincelist.size(); j++) {
			std::filesystem::path og_popfile_path(provincelist[j].popfile);
			std::string province_filename = og_popfile_path.filename().string();
			if (province_filename == filename) provincelist[j].SaveInFile(popfile);
		}

		popfile.close();
	}

	std::cout << "Pops saved in output folder!\n\n";
}
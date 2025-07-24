#include"main.h"
#include"popdata.h"
#include<iostream>
#include<filesystem>
#include"pdxinterpreter.h"
#include"bmpreader.h"
#include<unordered_map>

std::vector<province> provincelist;

struct temp_province {
	short unsigned int id;
	rgbcolor color;
	rgbcolor terraincolor;
};

std::unordered_map<rgbcolor, temp_province> temp_provincelist;

void load_temp_provinces() {
	BMP terrainmap("terrain.bmp");
	BMP provincemap(basefolder_path + "\\map\\provinces.bmp");
	if (terrainmap.bmp_info_header.bit_count != 24) std::cout << "WARNING: root folder's terrain.bmp should be saved in 24 bit format\n";

	for (int i = 0; i < provincemap.bmp_info_header.width; i++) {
		for (int j = 0; j < provincemap.bmp_info_header.height; j++) {
			rgbcolor provincecolor = provincemap.pixel24(i, j);
			rgbcolor terraincolor = terrainmap.pixel24(i, j);
			
			temp_province* p = new temp_province{ 0, provincecolor, terraincolor };
			temp_provincelist[provincecolor] = *p;
		}
	}
}

void assign_province_ids() {
	using namespace std;

	ifstream definitionfile(basefolder_path + "\\map\\definition.csv");

	string header;
	getline(definitionfile, header);

	while (!definitionfile.eof()) {
		string line;
		getline(definitionfile, line);

		stringstream lineStream(line);

		string id;
		getline(lineStream, id, ';');

		if (id.empty())continue;

		string r, g, b;
		getline(lineStream, r, ';');
		getline(lineStream, g, ';');
		getline(lineStream, b, ';');

		rgbcolor color;
		color.r = stoi(r);
		color.g = stoi(g);
		color.b = stoi(b);

		if (temp_provincelist.count(color)) temp_provincelist[color].id = stoi(id);
		else cout << "ERROR: defined province color not included in provinces.bmp\n";
	}

	definitionfile.close();
}

std::string provinceterrain(rgbcolor& terraincolor) {
	for (terrain_t t : terraintypes) {
		if (t.mapcolor == terraincolor) {
			if (t.is_water) return "water";
			else return t.terrain;
		}
	}
	 
	return terraintypes[0].terrain;
}

void create_provincelist() {
	for (std::pair<rgbcolor, temp_province> p : temp_provincelist) {
		if (p.second.id <= 0) continue;
		
		province* prov = new province();
		prov->id = p.second.id;
		prov->terrain = provinceterrain(p.second.terraincolor);
		if (prov->terrain == "water") {
			prov->~province();
			continue;
		}

		provincelist.push_back(*prov);
	}
}

void load_provincefile(std::ifstream& provincefile, province& prov) {
	using namespace std;

	int indent = 0;

	while (!provincefile.eof()) {
		string line;
		getline(provincefile, line);

		line.erase(0, line.find_first_not_of(" \t"));

		string identifier, token;
		divide_statement(line, identifier, token);

		if (token == "{") indent++;
		else if (identifier == "}") indent--;

		if (identifier == "owner" && indent == 0) {
			prov.ownertag.assign(token);
		}
		if (identifier == "add_core" && indent == 0) {
			prov.coretags.push_back(token);
		}
		if (identifier == "trade_goods" && indent == 0) {
			if (token == "coal" || token == "iron" || token == "sulphur" || token == "precious_metal" || token == "oil")
				prov.mine_not_farm = true;
		}
	}
}

void load_province_info(const std::string& path) {
	using namespace std;

	ifstream provincefile(path);

	string filename = path;

	filename.erase(0, filename.find_last_of("\\") + 1);
	filename = filename.substr(0, filename.find_first_of(" -"));

	short unsigned int id = stoi(filename);

	int index = 0;
	for (int i = 0; i < provincelist.size(); i++) {
		if (provincelist[i].id == id) {
			index = i;
			break;
		}
	}

	load_provincefile(provincefile, provincelist[index]);

	provincefile.close();
}

void load_pops_from_files(const std::string& popfile_path) {
	using namespace std;

	ifstream popfile(popfile_path);

	int indent = 0;

	int index = 0;

	while (!popfile.eof()) {
		string line;
		getline(popfile, line);

		//line.erase(0, line.find_first_not_of(" \t"));
		if (line.find_first_not_of(" \t") == std::string::npos) continue;

		string identifier, token;
		divide_statement(line, identifier, token);

		if (token == "{") indent++;
		else if (identifier == "}") indent--;

		if (token == "{" && indent == 1) {

			for (int i = 0; i < provincelist.size(); i++) {
				if (provincelist[i].id == stoi(identifier)) {
					index = i;
					provincelist[index].popfile = popfile_path;
					break;
				}
			}
		}
		else if (token == "{" && indent == 2) {
			pop newpop(popfile, identifier);
			indent--;
			provincelist[index].poplist.push_back(newpop);
		}
	}
}

void load_provinces() {
	using namespace std;

	cout << "\n\nLoading provinces from map...\n";

	load_temp_provinces();

	cout << "Assigning province IDs...\n";

	assign_province_ids();

	cout << "Creating province list...\n";

	create_provincelist();

	cout << "Loading province histories...\n";

	for (filesystem::directory_entry subfolder : filesystem::directory_iterator(historyfolder_path + "\\provinces")) {
		
		for (filesystem::directory_entry entry : filesystem::directory_iterator(subfolder.path())) {
			load_province_info(entry.path().string());
		}
	}
	
	cout << "Loaded province owners and RGOs!\n";

	cout << "Loading original pops for inheriting...\n";

	for (filesystem::directory_entry entry : filesystem::directory_iterator(popfolder_path)) {
		load_pops_from_files(entry.path().string());
	}

	cout << "Loaded original pops!\n";
}
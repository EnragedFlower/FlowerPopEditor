#include"main.h"
#include"popdata.h"
#include<iostream>
#include"pdxinterpreter.h"
std::vector<terrain_t> terraintypes;

void create_terraintypes() {
	terraintypes = {
		{
			"arctic",
			{112, 112, 112},
			false
		},
		{
			"arctic",
			{140, 140, 140},
			false
		},
		{
			"arctic",
			{176, 176, 176},
			false
		},
		{
			"arctic",
			{210, 210, 210},
			false
		},
		{
			"arctic",
			{236, 236, 236},
			false
		},
		{
			"plains",
			{86, 124, 27},
			false
		},
		{
			"plains",
			{111, 162, 57},
			false
		},
		{
			"plains",
			{134, 191, 92},
			false
		},
		{
			"plains",
			{152, 211, 131},
			false
		},
		{
			"forest",
			{33, 40, 0},
			false
		},
		{
			"forest",
			{39, 66, 0},
			false
		},
		{
			"forest",
			{76, 86, 4},
			false
		},
		{
			"forest",
			{64, 97, 12},
			false
		},
		{
			"hills",
			{45, 119, 146},
			false
		},
		{
			"hills",
			{75, 147, 174},
			false
		},
		{
			"hills",
			{120, 180, 202},
			false
		},
		{
			"hills",
			{160, 212, 220},
			false
		},
		{
			"woods",
			{2, 20, 41},
			false
		},
		{
			"woods",
			{6, 41, 78},
			false
		},
		{
			"woods",
			{15, 63, 90},
			false
		},
		{
			"woods",
			{37, 96, 126},
			false
		},
		{
			"mountain",
			{127, 24, 60},
			false
		},
		{
			"mountain",
			{162, 39, 83},
			false
		},
		{
			"mountain",
			{180, 86, 179},
			false
		},
		{
			"mountain",
			{181, 111, 177},
			false
		},
		{
			"mountain",
			{213, 144, 199},
			false
		},
		{
			"mountain",
			{235, 179, 233},
			false
		},
		{
			"mountain",
			{162, 39, 83},
			false
		},
		{
			"mountain",
			{173, 59, 83},
			false
		},
		{
			"mountain",
			{192, 90, 117},
			false
		},
		{
			"steppe",
			{39, 0, 2},
			false
		},
		{
			"steppe",
			{82, 4, 8},
			false
		},
		{
			"steppe",
			{99, 7, 11},
			false
		},
		{
			"plains",
			{116, 11, 16},
			false
		},
		{
			"plains",
			{138, 11, 26},
			false
		},
		{
			"plains",
			{179, 11, 27},
			false
		},
		{
			"plains",
			{231, 32, 55},
			false
		},
		{
			"plains",
			{56, 56, 56},
			false
		},
		{
			"plains",
			{78, 78, 78},
			false
		},
		{
			"plains",
			{86, 86, 86},
			false
		},
		{
			"jungle",
			{48, 175, 147},
			false
		},
		{
			"jungle",
			{56, 199, 197},
			false
		},
		{
			"jungle",
			{97, 220, 193},
			false
		},
		{
			"jungle",
			{118, 245, 217},
			false
		},
		{
			"marsh",
			{0, 73, 57},
			false
		},
		{
			"marsh",
			{2, 94, 74},
			false
		},
		{
			"marsh",
			{16, 122, 99},
			false
		},
		{
			"marsh",
			{31, 154, 127},
			false
		},
		{
			"desert",
			{172, 136, 67},
			false
		},
		{
			"desert",
			{206, 169, 99},
			false
		},
		{
			"desert",
			{225, 192, 130},
			false
		},
		{
			"desert",
			{241, 210, 151},
			false
		},
		{
			"water",
			{255, 255, 255},
			true
		},
	};
}

void load_terraintypes() {
	using namespace std;

	cout << "\n\nLoading terrain types...\n";

	ifstream terrainfile(basefolder_path + "\\map\\terrain.txt");

	int indent = 0;
	terrain_t terraintype;
	bool is_water = false;

	while (!terrainfile.eof()) {
		string line;
		getline(terrainfile, line);
		
		line.erase(0, line.find_first_not_of(" \t"));

		string identifier, token;
		divide_statement(line, identifier, token);

		if (token == "{") indent++;
		else if (identifier == "}") indent--;

		if (token == "{" && indent == 2) terraintype.terrain = identifier;
		if (identifier == "color") get_color(line, terraintype.mapcolor.r, terraintype.mapcolor.g, terraintype.mapcolor.b);

		if (identifier == "is_water")
			if (token == "yes") terraintype.is_water = true;
			else terraintype.is_water = false;
		if (identifier == "}" && indent == 1)
			terraintypes.push_back(terraintype);
		if (identifier == "}" && indent == 0 && terraintypes.size() > 0) break;
	}
	terrainfile.close();

	cout << "Loaded terrain types:\n";
	for (terrain_t t : terraintypes) {
		cout << t.terrain << " ";
	}
}
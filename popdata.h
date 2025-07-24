#pragma once
#include<vector>
#include<string>
#include<fstream>
#include"rgbcolor.h"

struct pop {
	std::string type;
	std::string culture;
	std::string religion;
	int size;

	pop(std::ifstream& file, bool echo = false);
	pop(std::ifstream& file, const std::string& type, bool echo = false);
	void Load(std::ifstream& file, bool echo = false);
	void SaveInFile(std::ofstream& file);
};

struct poptemplate {
	std::string name;
	std::vector<pop> poplist;
};

struct province {
	short unsigned int id;
	std::string popfile;
	std::string ownertag;
	std::vector<std::string> coretags;
	std::string terrain;
	bool mine_not_farm;
	std::vector<pop> poplist;
	
	int GetPopulation() const;
	bool HasCore(const std::string& coretag) const;
	void ResetPopulation(const poptemplate& target_template);
	void SetPopulation(int target);
	void MultiplyPopulation(double factor);
	void SaveInFile(std::ofstream& file);
};

struct country {
	std::string GetTag();
	int GetPopulation(bool use_cores = false) const;
	void ResetPopulation(bool use_cores = false);
	void SetPopulation(int target, bool use_cores = false);

	country(std::string countrytag);

	private:
	std::string tag;
};

struct assignedtemplate {
	std::string terrain;
	poptemplate* poptemplate;
};

struct terrain_t {
	std::string terrain;
	rgbcolor mapcolor;
	bool is_water;
};

void MultiplyPoplist(std::vector<pop>& poplist, double factor);

extern std::vector<country> countrylist;
extern std::vector<province> provincelist;
extern std::vector<poptemplate> poptemplates;
extern std::vector<terrain_t> terraintypes;
extern std::vector<assignedtemplate> usedtemplates;
#include"popdata.h"
#include"pdxinterpreter.h"
#include<iostream>

std::vector<pop> adapt_poplist(const std::vector<pop>& poplist_to_adapt, const province& province, const country& country) {
	std::vector<pop> new_poplist = poplist_to_adapt;

	for (int i = 0; i < new_poplist.size(); i++) {
		if (new_poplist[i].culture == "primary_culture") new_poplist[i].culture = country.GetPrimaryCulture();
		else if (new_poplist[i].culture == "secondary_culture") {
			std::vector<std::string> secondarycultures = country.GetSecondaryCultures();
			if (secondarycultures.empty())new_poplist[i].culture = country.GetPrimaryCulture();
			else new_poplist[i].culture = secondarycultures[0];
		}

		if (new_poplist[i].religion == "state_religion") new_poplist[i].religion = country.GetReligion();
		
		if (province.mine_not_farm) {
			if (new_poplist[i].type == "farmer") new_poplist[i].type = "labourer";
		}
		else {
			if (new_poplist[i].type == "labourer") new_poplist[i].type = "farmer";
		}
	}

	MultiplyPoplist(new_poplist, (double)province.GetPopulation() / 1000);

	return new_poplist;
}

bool ProvinceHasTag(const province& prov, const std::string& tag, bool use_cores) {
	if (use_cores) {
		return prov.HasCore(tag);
	}
	else return prov.ownertag == tag;
}

country::country(std::string countrytag) : tag(countrytag) {}

void country::Load(std::string primaryculture, const std::vector<std::string>& secondarycultures, std::string statereligion) {
	primary_culture = primaryculture;
	secondary_cultures = secondarycultures;
	religion = statereligion;
}

std::string country::GetTag() const {
	return tag;
}

std::string country::GetPrimaryCulture() const {
	return primary_culture;
}

std::vector<std::string> country::GetSecondaryCultures() const {
	return secondary_cultures;
}

std::string country::GetReligion() const {
	return religion;
}

int country::GetPopulation(bool use_cores) const {
	int population = 0;
	for (province p : provincelist) if(ProvinceHasTag(p, tag, use_cores)) population += p.GetPopulation();
	return population;
}

void country::ResetPopulation(bool use_cores) {
	
	for (int j = 0; j < provincelist.size(); j++) if (ProvinceHasTag(provincelist[j], tag, use_cores)) {
		
		int index = 0;
		for (int i = 0; i < usedtemplates.size(); i++) {
			assignedtemplate& t = usedtemplates[i];
			if (t.terrain == provincelist[j].terrain) {
				index = i;
				break;
			}
		}
		provincelist[j].ResetPopulation(*usedtemplates[index].poptemplate, *this);
	}
}

void country::SetPopulation(int target, bool use_cores) {
	double factor = (double)target / GetPopulation(use_cores);
	for (int j = 0; j < provincelist.size(); j++) if (ProvinceHasTag(provincelist[j], tag, use_cores)) provincelist[j].MultiplyPopulation(factor);
}

int province::GetPopulation() const {
	int population = 0;
	for (pop p : poplist) {
		population += p.size;
	}
	return population;
}

bool province::HasCore(const std::string& coretag) const {
	for (int i = 0; i < coretags.size(); i++) {
		if (coretags[i] == coretag) return true;
	}
	return false;
}

void province::ResetPopulation(const poptemplate& target_template, const country& callingcountry) {
	std::vector<pop> adapted_poplist = adapt_poplist(target_template.poplist, *this, callingcountry);
	poplist.assign(adapted_poplist.size(), adapted_poplist[0]);

	for (int i = 0; i < adapted_poplist.size(); i++) {
		poplist[i] = adapted_poplist[i];
	}
}

void province::SetPopulation(int target) {
	double factor = (double)target / GetPopulation();
	MultiplyPopulation(factor);
}

void MultiplyPoplist(std::vector<pop>& poplist, double factor) {
	for (int i = 0; i < poplist.size(); i++) poplist[i].size *= factor;
}

void province::MultiplyPopulation(double factor) {
	MultiplyPoplist(poplist, factor);
}

void province::SaveInFile(std::ofstream& file) {
	file << id << " = {\n";

	for (int i = 0; i < poplist.size(); i++) {
		poplist[i].SaveInFile(file);
	}

	file << "}\n\n";
}

pop::pop(std::ifstream& file, bool echo) {
	Load(file, echo);
}

pop::pop(std::ifstream& file, const std::string& identifier_type, bool echo) {
	type = identifier_type;
	Load(file, echo);
}

void pop::Load(std::ifstream& file, bool echo) {
	while (!file.eof()) {
		std::string line;
		getline(file, line);

		//line.erase(0, line.find_first_not_of(" \t"));

		if (line.find_first_not_of(" \t") == std::string::npos) continue;
		//if (line[0] == '#') continue;

		std::string identifier, token;
		divide_statement(line, identifier, token);

		if (echo) std::cout << "pop " << identifier << " " << token << "\n";

		if (identifier == "}" || token == "}") return;

		if (identifier == "culture") culture = token;
		else if (identifier == "religion") religion = token;
		else if (identifier == "size") {
			try {
				size = stoi(token);
			}
			catch (const std::exception& e) {
				std::cout << "ERROR: standard exception in\nLine: " << line << "\nWhile trying to load pop data.\n";
				std::cout << "Exception: " << e.what();
			}
		}
		else if (token == "{") type = identifier;
	}
}

void pop::SaveInFile(std::ofstream& file) {
	file << "\t" << type << " = {\n";
	file << "\t\t" << "culture" << " = " << culture << "\n";
	file << "\t\t" << "religion" << " = " << religion << "\n";
	file << "\t\t" << "size" << " = " << size << "\n";
	file << "\t}\n\n";
}
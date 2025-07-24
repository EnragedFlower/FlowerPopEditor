#include"main.h"
#include"popdata.h"
#include<iostream>

std::vector<assignedtemplate> usedtemplates;

void adapt_templates() {
	std::string prev_terrain;

	usedtemplates.clear();

	for (terrain_t terrain : terraintypes) {
		if (terrain.terrain == prev_terrain)continue;
		if (terrain.is_water) continue;
		assignedtemplate temp;
		temp.terrain = terrain.terrain;
		
		bool error = false;

		std::cout << "\nAvailable templates:\n";
		for (poptemplate t : poptemplates) {
			std::cout << t.name << "\n";
		}

		do {
			std::cout << "Select template for '" << terrain.terrain << "': ";
			std::string templatename;
			if(poptemplates.size() > 1) getline(std::cin, templatename);
			else {
				templatename = poptemplates[0].name;
				std::cout << "only " << templatename << " available!\n";
			}

			for (int i = 0; i < poptemplates.size(); i++) if (poptemplates[i].name == templatename) temp.poptemplate = &poptemplates[i];

			error = temp.poptemplate == nullptr;
			if (error) std::cout << "ERROR: No template with this name was found\n";
		}
		while (error);

		usedtemplates.push_back(temp);
		prev_terrain = terrain.terrain;
	}
	
	std::cout << "\nAssigned templates to all terrain types!\n";
}
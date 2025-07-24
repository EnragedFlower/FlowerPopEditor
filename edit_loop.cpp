#include"main.h"
#include"popdata.h"
#include<iostream>

using namespace std;

void edit_country(bool reset_to_template, bool use_cores) {
	
	string tag;
	int index = -1;

	do {
		cout << "\ninsert country TAG: ";
		cin >> tag;

		for (int i = 0; i < countrylist.size(); i++) if (tag == countrylist[i].GetTag()) {
			index = i;
			break;
		}
		if (index == -1) cout << "\nERROR: no country with this TAG\n";
	} while (index == -1);

	cout << " current population: " << countrylist[index].GetPopulation(use_cores);

	int population_target;
	cout << "\ninsert new population (0 or less to escape): ";
	cin >> population_target;

	if (population_target > 0) {
		if (reset_to_template)countrylist[index].ResetPopulation(use_cores);
		countrylist[index].SetPopulation(population_target, use_cores);
	}

	cout << "\n" << countrylist[index].GetTag() << " population is now " << countrylist[index].GetPopulation(use_cores) << "\n";
}

void edit_loop() {
	
	bool editing = true;
	while (editing) {
		
		cout << "0 - Save pops & close program\n1 - Change templates\n2 - Multiply pops by country\n3 - Edit pops by country\n4 - Multiply pops by core\n5 - Edit pops by core\n6 - Save pops to file\naction: ";
		int action;
		cin >> action;

		switch (action)
		{
			default:
				break;
			case 0:
				editing = false;
				break;
			case 1:
				load_templates();
				adapt_templates();
				break;
			case 2:
				edit_country(false, false);
				break;
			case 3:
				edit_country(true, false);
				break;
			case 4:
				edit_country(false, true);
				break;
			case 5:
				edit_country(true, true);
				break;
			case 6:
				save_provinces();
				break;
		}
	}
}
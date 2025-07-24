#include"main.h"
#include"bmpreader.h"
#include<iostream>

void load_data() {
	get_folders();
	create_terraintypes();
	//load_terraintypes(); // TOO DIFFICULT TO FIGURE OUT, SORRY
	load_provinces();
	load_countries();
	load_templates();
}

void save_data() {
	save_provinces();
}

int main() {
	load_data();
	adapt_templates();
	edit_loop();
	save_data();
}

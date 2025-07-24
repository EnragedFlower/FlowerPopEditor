#pragma once
#include<string>

void get_folders();

void create_terraintypes();
void load_terraintypes();
void load_provinces();
void load_countries();
void load_templates();

void adapt_templates();

void edit_loop();
void save_provinces();

extern std::string basefolder_path;
extern std::string commonfolder_path;
extern std::string historyfolder_path;
extern std::string popfolder_path;
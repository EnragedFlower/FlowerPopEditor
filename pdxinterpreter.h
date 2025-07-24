#pragma once
#include<string>

void divide_statement(const std::string& line, std::string& identifier, std::string& token);
void get_color(const std::string& line, unsigned char& r, unsigned char& b, unsigned char& g);
#include "tarot.h"

vector<string> Tarot::major;
vector<string> Tarot::major_meaning_a;
vector<string> Tarot::major_meaning_b;

void Tarot::init() {
	stringstream ss;
	ss << get_file("resources/tarot.txt").begin();
	vector<string> parts = split(strip(ss.str()), '-');
	Tarot::major = split(strip(parts[0]), '\n'); 
	Tarot::major_meaning_a = split(strip(parts[1]), '\n');
	Tarot::major_meaning_b = split(strip(parts[2]), '\n');
}

string Tarot::get_card() {
	int n = common::random(0, 21);
	int side = common::random(0, 9);
	string card = strip(major[n]);
	string meaning, flag;
	if (side % 2 == 0) {
		meaning = strip(major_meaning_a[n]);
		flag = string("正位");
	} else {
		meaning = strip(major_meaning_b[n]);
		flag = string("逆位");
	}
	stringstream result;
	result << "抽到了: " << endl;
	result << card << ", " << flag << endl;
	result << "释义: " << meaning;
	return result.str();
}

string Tarot::get_card(string msg) {
	stringstream result;
	result << "因为" << msg << ", ";
	result << Tarot::get_card();
	return result.str();
}
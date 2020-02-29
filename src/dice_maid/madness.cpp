#include "madness.h"

vector<string> Madness::mad_a;
vector<string> Madness::mad_b;
vector<string> Madness::phobia;
vector<string> Madness::mania;

void Madness::init() {
	stringstream ss;
	ss << get_file("resources/madness.txt").begin();
	vector<string> parts = split(strip(ss.str()), '-');
	Madness::mad_a = split(strip(parts[0]), '\n');
	Madness::mad_b = split(strip(parts[1]), '\n');
	Madness::phobia = split(strip(parts[2]), '\n');
	Madness::mania = split(strip(parts[3]), '\n');
}

string Madness::get_symptom(int type) {
	stringstream result;
	result << "疯狂发作-";
	int num = common::random(0, 9);
	int time = common::random(10);
	if (type == 0)
		result << "临时症状: 1d10=" << (num + 1) << endl << strip(Madness::mad_a[num]);
	else
		result << "总结症状: 1d10=" << (num + 1) << endl << strip(Madness::mad_b[num]);

	if (num >= 8) {
		int sub = common::random(0, 99);
		result << endl << "具体症状: 1d100=" << (sub + 1) << endl;
		if (num == 8)
			result << Madness::phobia[sub];
		else
			result << Madness::mania[sub];
	}
	result << endl << "持续时间: 1d10=" << time << "轮";
	return result.str();
}
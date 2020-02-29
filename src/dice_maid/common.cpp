#include "common.h"

using namespace std;

boost::random::mt19937 engine;

string& strip(string& s) {
	if (s.empty()) return s;
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

int common::random(int min, int max) {
	boost::random::uniform_int_distribution dist(min, max);
	return dist(engine);
}

int common::random(int sides) {
	return (sides <= 0) ? 0 : common::random(1, sides);
}

static void _split(const string& s, char delim, vector<string>& elems) {
	stringstream ss(s);
	string item;

	while (getline(ss, item, delim)) { elems.push_back(item); }
}

vector<string> split(const string& s, char delim) {
	vector<string> elems;
	_split(s, delim, elems);
	return elems;
}

string extract(string& values, int index, char delim = ' ') {
	if (values.length() == 0) return string("");

	vector<string> x = split(values, delim);
	try {
		return x.at(index);
	} catch (const out_of_range&) { return string(""); }
}

bool start_with(const string& str, const string& head) {
	int srclen = (int) str.size();
	int startlen = (int) head.size();
	if (srclen >= startlen) {
		string temp = str.substr(0, startlen);
		return temp == head;
	}
	return false;
}

inline string get_directory() {
	char* buffer;
	return ((buffer = _getcwd(NULL, 0)) == NULL) ? string("") : string(buffer);
}

string get_data_path() {
	string cur = get_directory();
	if (cur != "") {
		stringstream path;
		path << cur << "\\data\\app\\me.cpq.samarium.dice_maid";
		return path.str();
	} else
		return "";
}

cmrc::file get_file(const string& name) {
	return cmrc::app::get_filesystem().open(name);
}
#include "fortune.h"

using namespace std;

string Fortune::path = "";

void Fortune::init() {
	string data = get_data_path();
	if (data != "") {
		stringstream ss;
		ss << data << "\\fortune_data";
		Fortune::path = ss.str();
		if (_access(Fortune::path.c_str(), 0) == -1) CreateDirectory(Fortune::path.c_str(), NULL);
	}
}

string get_timestamp() {
	time_t t;
	time(&t);
	struct tm temp;
	localtime_s(&temp, &t);
	char s[11];
	strftime(s, 11, "%F", &temp);
	return string(s);
}

inline bool Fortune::check(string time) {
	return time.compare(get_timestamp()) != 0;
}

string Fortune::get_luck(const int64_t qq) {
	int luck = 0;
	stringstream ss, response;
	ss << Fortune::path << "\\" << qq << ".txt";
	const char* p = ss.str().c_str();
	if (_access(p, 0) == -1) { 
		luck = common::random(100);
		ofstream file;
		file.open(p);
		file << get_timestamp() << endl << luck;
		file.close();
	} else {
		ifstream file;
		file.open(p);
		char time[11], buffer[256];
		file.getline(time, 11);
		file.getline(buffer, 256);
		file.close();
		string t = time;
		if (check(t)) { 
			remove(p);
			return get_luck(qq);
		}
		luck = atoi(buffer);
	}
	response << "今日人品: " << luck;
	return response.str();
}
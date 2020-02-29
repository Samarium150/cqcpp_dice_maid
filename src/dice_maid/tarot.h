#pragma once
#include "common.h"

namespace Tarot {
	extern vector<string> major;
	extern vector<string> major_meaning_a;
	extern vector<string> major_meaning_b;
	void init();
	string get_card();
	string get_card(string msg);
}  // namespace Tarot
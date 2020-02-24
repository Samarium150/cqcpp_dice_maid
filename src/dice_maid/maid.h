#pragma once
#include "common.h"
#include "dice.h"
#include "fortune.h"
#include "madness.h"
#include "tarot.h"

namespace Maid {
	extern int64_t master;
	extern string banned;
    extern string cursed;
    extern string setting;
    void init();
	string get_help();
	string get_info();
	string command(int64_t id, string msg);
	string command(int64_t id, string name, string msg);
	string ban(int64_t id);
	string unban(int64_t id);
	bool check_banned(const int64_t id);
	bool check_cursed(const int64_t id);
    int get_rate(int64_t id);
    void set_rate(int64_t id, int set);
}  // namespace Maid

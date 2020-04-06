#pragma once

#include "common.h"
#include "dice.h"
#include "fortune.h"
#include "madness.h"
#include "tarot.h"

namespace Maid {
    extern string setting;
    void init();
    string get_help();
    string get_info();
    string command(int64_t id, string msg);
    string command(int64_t id, string name, string msg);
    int get_rate(int64_t id);
    void set_rate(int64_t id, int set);
} // namespace Maid

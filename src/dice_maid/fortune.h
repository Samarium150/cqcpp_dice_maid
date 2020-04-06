#pragma once
#include "common.h"

using namespace std;

namespace Fortune {
    extern string path;
    void init();
    inline bool check(string time);
    string get_luck(const int64_t qq);
} // namespace Fortune
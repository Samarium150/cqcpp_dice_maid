#pragma once

#include "common.h"

namespace Madness {
    extern vector<string> mad_a;
    extern vector<string> mad_b;
    extern vector<string> phobia;
    extern vector<string> mania;
    void init();
    string get_symptom(int type);
} // namespace Madness
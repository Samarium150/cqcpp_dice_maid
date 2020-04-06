#pragma once
#include "common.h"

namespace Dice {
    void init();
    int roll(int times, int sides, int div);
    string roll(int times, int sides, int div, bool output, int &s);
    string check_result(int result, int skill, int type);
    string check(string msg, int num, int rate, int type = 1);
    string check(string msg, int num, int rate, int type, int count);
    string coc();
    string coc(int n);
    string toss(string msg, int type, int &s);
    string toss(string msg);
    string san_check(string msg, int rate);
} // namespace Dice

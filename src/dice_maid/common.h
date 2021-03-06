#pragma once

#include <direct.h>
#include <fcntl.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <windows.h>
#include <algorithm>
#include <boost/random.hpp>
#include <cmath>
#include <cmrc/cmrc.hpp>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

CMRC_DECLARE(app);

extern boost::random::mt19937 engine;

namespace common {
    int random(int, int);
    int random(int);
} // namespace common

constexpr double ABS(double);

constexpr double active(double);

string& strip(string&);

static void _split(const string&, char, vector<string>&);

vector<string> split(const string&, char);

string extract(string&, int, char);

bool start_with(const string&, const string&);

string get_directory();

string get_data_path();

cmrc::file get_file(const string&);

template <class Type>
Type string_to_num(const string& str) {
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}
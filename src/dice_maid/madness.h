#pragma once
#include "common.h"

namespace Madness {
	extern string mad_a[];
	extern string mad_b[];
	extern string phobia[];
	extern string mania[];
	void init();
	string get_symptom(int type);
}  // namespace Madness
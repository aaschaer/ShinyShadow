#pragma once

#include <iostream>

#include "lcg.h"

class PyriteNoise {
public:
	PyriteNoise(LCG& global_lcg);
	LCG& lcg;
	float noise_state[3];
	void step();
	void print_state();
};

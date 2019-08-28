#pragma once

using namespace std;

class LCG {
public:
	LCG(int seed);
	unsigned int state;
	double generate();
};
#include "lcg.h"
#include <string.h>
#include <iostream>

LCG::LCG(int seed){
    state = seed;
};

double LCG::generate() {
	//simulates subroutine 801adc7c
	float generation_constant = 65536.0;
	state = (state * 214013 + 2531011) & 0xffffffff;
	unsigned long long shifted = 0x4330000000000000 | (state >> 16);
	double n;
	memcpy(&n, &shifted, 8);
	n -= 4503599627370496.0;
	n /= generation_constant;

	//cout << "generated " << n << endl;

	return n;
}

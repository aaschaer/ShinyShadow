#include "noise.h"

PyriteNoise::PyriteNoise(LCG& global_lcg) : lcg(global_lcg) {
	int initial_val = 0x3F7FFFFE;
	for (int i = 0; i < 3; i++) {
		memcpy(&noise_state[i], &initial_val, 4);
	}
}

void PyriteNoise::step() {

	for (int i = 0; i < 3; i++) {
		double prn = lcg.generate();

		double val = (double)noise_state[i] + (0.5 * prn);
		noise_state[i] = (float)val; 

		if (noise_state[i] > 1.0) {
			for (int j = 0; j < 8; j++) {
				lcg.generate();
			}
			noise_state[i] -= 1.0;
		}
	}
}

void PyriteNoise::print_state() {
	for (int i = 0; i < 3; i++) {
		int val;
		memcpy(&val, &noise_state[i], 4);
		cout << "noise" << dec << i << ": " << hex << val << endl;
	}
}
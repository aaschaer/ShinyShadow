#pragma once

#include <utility>

#include "ppc_exc.h"
#include "lcg.h"
#include "ram.h"

using namespace std;

enum states { WAITING = 0, FIRST_WALK = 1, WALKING = 2, STOPPING = 3 };

class NPC {
public:
	NPC(LCG& global_lcg,
		RAM& global_ram,
		int npc_adr,
		int pos_adr,
		int npc_num,
		float startX,
		float startY);
	LCG& lcg;
	RAM& ram;
	int npc_address;
	int pos_address;
	int npc_number;
	int state;
	float startingX;
	float startingY;
	float currentX;
	float currentY;
	float destX;
	float destY;
	float nextX;
	float nextY;
	float walk_speed;
	float wait_time;
	int step_count;
	bool first_step;
	bool debug;

	void step();
	void set_wait_time();
	void wait();
	void set_walk_params();
	pair <float, float> calc_walk(float givenX, float givenY);

	void print_state();
};

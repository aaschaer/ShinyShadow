#include "npc.h"

NPC::NPC(
		LCG& global_lcg,
		RAM& global_ram,
		int npc_adr,
		int pos_adr,
		int npc_num,
		float startX,
		float startY
		) : 
	lcg(global_lcg),
	ram(global_ram) {
	npc_address = npc_adr;
	pos_address = pos_adr;
	npc_number = npc_num;
	startingX = startX;
	startingY = startY;
	currentX = startX;
	currentY = startY;
	wait_time = 0.0;
	walk_speed = 0.0;
	nextX = 0.0;
	nextY = 0.0;
	destX = 0.0;
	destY = 0.0;
	state = WAITING;
	step_count = 0;
	first_step = true;
	debug = false;

	//write constants to ram
	ram.write_int(npc_address + 4, npc_address);
	ram.write_int(npc_address + 8, pos_address);
	ram.write_int(npc_address + 40, 0x0000002d);
	ram.write_int(npc_address + 44, npc_number);
	ram.write_int(npc_address + 52, 0x3e94a529);
	ram.write_single(npc_address + 116, startingX);
	ram.write_int(npc_address + 120, 0x00000000);
	ram.write_single(npc_address + 124, startingY);
	ram.write_int(npc_address + 128, 0x41700000);

	ram.write_int(pos_address, 0x00000000); //unused
	ram.write_int(pos_address + 28, 0x00000000);
	ram.write_int(pos_address + 36, 0x00000000);
	ram.write_int(pos_address + 40, 0x00000000); //unused
	ram.write_int(pos_address + 44, 0x00000000);
	ram.write_int(pos_address + 160, 0x41f00000); //constant?
};

void NPC::step() {
    // simulates one frame of NPC action
	if (state == FIRST_WALK) {
		// calc the next position in the walk
		pair <float, float> calced;
		if (first_step) {
			calced = calc_walk(currentX, currentY);
			nextX = calced.first;
			nextY = calced.second;
			first_step = false;
		}
		calced = calc_walk(nextX, nextY);
		float calcedX = calced.first;
		float calcedY = calced.second;

		// check if we would pass the destination next step and if so start stopping
		if ((nextX <= destX and destX <= calcedX) or
			(nextX >= destX and destX >= calcedX)) {
			set_wait_time();
			state = STOPPING;
		}
		currentX = nextX;
		currentY = nextY;
		nextX = calcedX;
		nextY = calcedY;
	}

	else if (state == WALKING) {
		// calc the next position in the walk
		pair <float, float> calced = calc_walk(currentX, currentY);
		float calcedX = calced.first;
		float calcedY = calced.second;

		// check if we will pass the destination now and if so start stopping
		if ((currentX <= destX and destX <= calcedX) or
			(currentX >= destX and destX >= calcedX)) {
			set_wait_time();
			state = STOPPING;
		}
		currentX = calcedX;
		currentY = calcedY;
	}

	else if (state == STOPPING) {
		currentX = destX;
		currentY = destY;
		wait();
		state = WAITING;
	}

	else if (state == WAITING) {
		if (wait_time <= 0.0) {
			set_walk_params();
			if (first_step) {
				state = FIRST_WALK;
			}
			else {
				state = WALKING;
			}
		}
		wait();
	}
}

void NPC::set_wait_time() {
	//simulates logic for selecting a pseudo random wait time
	double prn1 = lcg.generate();
	double prn2 = lcg.generate();
	double rand = (prn1 + prn2) - 1.0; // constant from rtoc
	wait_time = (float)((3.0 * rand) + 5.0); //constants from r31
}

void NPC::wait() {
	//simulates waiting one frame
	double val = (double)wait_time;
	val -= 0.03333333507180214; //0x3fa1111120000000
	if (val >= 0) {
		wait_time = (float)val;
	}
	else {
		wait_time = 0.0;
	}
}

void NPC::set_walk_params() {
	//executes logic for selecting a pseudo random walk destination and speed
	
	unordered_map<string, int> starting_int_registers = {
		{"sp", 0x8048e5e0},
		{"r13", 0x80480820},
		{"r30", npc_address},
		{"r31", npc_address},
	};

	double prn = lcg.generate();
	unordered_map<string, double> starting_double_registers = {
		{"f1", prn},
	};

	ram.write_single(pos_address + 24, currentX);
	ram.write_single(pos_address + 32, currentY);
	
	PPC_exc exc = PPC_exc(ram, 0x80184e64, starting_int_registers, starting_double_registers);
	exc.execute();
	walk_speed = ram.read_single(npc_address + 64);
	destX = ram.read_single(npc_address + 92);
	destY = ram.read_single(npc_address + 100);
}

pair <float, float> NPC::calc_walk(float givenX, float givenY) {
	//calculates the next walk position from a given x and y

	unordered_map<string, int> starting_int_registers = {
		{"lr", 0x80185afc},
		{"sp", 0x80185afc},
		{"r13", 0x80480820},
		{"r30", 0x0000002d},
		{"r31", npc_number},
	};

	unordered_map<string, double> starting_double_registers = {
		{"f30", 1.0},
	};

	ram.write_single(npc_address + 64, walk_speed);
	ram.write_single(pos_address + 24, givenX);
	ram.write_single(pos_address + 32, givenY);
	
    // strange out of bounds value, seems related to how many steps taken in walk?
	if (first_step) {
		ram.write_int(0xe0000054, 0x00000001);
	}
    else {
		ram.write_int(0xe0000054, 0x00000002);
	}

	PPC_exc exc = PPC_exc(ram, 0x8018824c, starting_int_registers, starting_double_registers);
	exc.execute();
	float calcedX = ram.read_single(exc.int_registers.at("sp") + 0x10);
	float calcedY = ram.read_single(exc.int_registers.at("sp") + 0x18);
	return make_pair(calcedX, calcedY);
}

void NPC::print_state() {
	int val;

	memcpy(&val, &destX, 4);
	cout << "destX: " << hex << val << endl;
	memcpy(&val, &destY, 4);
	cout << "destY: " << hex << val << endl;
	memcpy(&val, &walk_speed, 4);
	cout << "speed: " << hex << val << endl;

	memcpy(&val, &currentX, 4);
	cout << "currentX: " << hex << val << endl;
	memcpy(&val, &currentY, 4);
	cout << "currentY: " << hex << val << endl;

	memcpy(&val, &wait_time, 4);
	cout << "wait_time: " << hex << val << endl;

	cout << "\n";
}
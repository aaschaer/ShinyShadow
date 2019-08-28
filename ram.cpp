# include "ram.h"

RAM::RAM() {};

void RAM::write_byte(int address, unsigned char val) {
	ram[address] = val;
}

void RAM::write_int(int address, int val) {
	unsigned char bytes[4];
	memcpy(&bytes, &val, 4);
	for (int i = 0; i < 4; i++) {
		ram[address + i] = bytes[3 - i];
	}
}

void RAM::write_long(int address, long long val) {
	unsigned char bytes[8];
	memcpy(&bytes, &val, 8);
	for (int i = 0; i < 8; i++) {
		ram[address + i] = bytes[7 - i];
	}
}

void RAM::write_single(int address, float val) {
	unsigned char bytes[4];
	memcpy(&bytes, &val, 4);
	for (int i = 0; i < 4; i++) {
		ram[address + i] = bytes[3 - i];
	}
}

void RAM::write_double(int address, double val) {
	unsigned char bytes[8];
	memcpy(&bytes, &val, 8);
	for (int i = 0; i < 8; i++) {
		ram[address + i] = bytes[7 - i];
	}
}

unsigned char RAM::read_byte(int address) {
	try {
		return ram.at(address);
	}
	catch (...) {
		cout << hex << address;
		throw address;
	}
}

int RAM::read_int(int address) {
	try {
		unsigned char bytes[4];
		for (int i = 0; i < 4; i++) {
			bytes[3 - i] = ram.at(address + i);
		}
		int ret;
		memcpy(&ret, &bytes, 4);
		return ret;
	}
	catch (...) {
		cout << hex << address;
		throw address;
	}
}

float RAM::read_single(int address) {
	try {
		unsigned char bytes[4];
		for (int i = 0; i < 4; i++) {
			bytes[3 - i] = ram.at(address + i);
		}
		float ret;
		memcpy(&ret, &bytes, 4);
		return ret;
	}
	catch (...) {
		cout << hex << address;
		throw address;
	}
}

double RAM::read_double(int address) {
	try {
		unsigned char bytes[8];
		for (int i = 0; i < 8; i++) {
			bytes[7 - i] = ram.at(address + i);
		}
		double ret;
		memcpy(&ret, &bytes, 8);
		return ret;
	}
	catch (...) {
		cout << hex << address;
		throw address;
	}
}

void RAM::print_ram() {
	//no longer ordered, needs sorting

	int last_address = 0;
	for (auto const& a : ram) {
		if (last_address + 1 == a.first) {
			cout << setw(2) << setfill('0') << hex << (int)a.second;
			last_address = a.first;
		}
		else {
			cout << '\n';
			last_address = a.first;
			cout << hex << a.first << ": ";
			cout << setw(2) << setfill('0') << hex << (int)a.second;
		}
	}
	cout << "\n\n";
}

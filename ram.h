#pragma once
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class RAM {
public:
	RAM();
	unordered_map<int, unsigned char> ram;
	void write_byte(int address, unsigned char val);
	void write_int(int address, int val);
	void write_long(int address, long long val);
	void write_single(int address, float val);
	void write_double(int address, double val);

	unsigned char read_byte(int address);
	int read_int(int address);
	float read_single(int address);
	double read_double(int address);

	void print_ram();
};
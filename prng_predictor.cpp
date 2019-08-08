#include "ppc_exc.h"

int main() {

	int start_address = 0x80184e64;
	map<string, int> starting_int_registers = {};
	map<string, double> starting_double_registers = {};
	map<int, unsigned char> starting_ram = {};

	PPC_exc exc = PPC_exc(start_address, starting_int_registers, starting_double_registers, starting_ram);
	exc.debug = true;
	exc.execute();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

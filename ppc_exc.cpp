#include "source.h"
#include "ppc_exc.h"

PPC_exc::PPC_exc(
	int start_address,
	map<string, int> starting_int_registers,
	map<string, double> starting_double_registers,
	map<int, unsigned char> starting_ram
) {
	current_address = start_address;
	int_registers = starting_int_registers;
	double_registers = starting_double_registers;
	ram = starting_ram;
}

void PPC_exc::write_int_to_ram(int address, int val) {
	unsigned char* bytes = reinterpret_cast<unsigned char*>(&val);
	for (int i = 0; i < sizeof(int); i++) {
		ram[address + i] = bytes[i];
	}
}

void PPC_exc::write_single_to_ram(int address, float val) {
	unsigned char* bytes = reinterpret_cast<unsigned char*>(&val);
	for (int i = 0; i < sizeof(float); i++) {
		ram[address + i] = bytes[i];
	}
}

void PPC_exc::write_double_to_ram(int address, double val) {
	unsigned char* bytes = reinterpret_cast<unsigned char*>(&val);
	for (int i = 0; i < sizeof(double); i++) {
		ram[address + i] = bytes[i];
	}
}

int PPC_exc::read_int_from_ram(int address) {

	unsigned char bytes[sizeof(int)];
	for (int i = 0; i < sizeof(int); i++) {
		bytes[i] = ram[address + i];
	}
	int ret;
	memcpy(&ret, bytes, sizeof(int));
	return ret;
}

float PPC_exc::read_single_from_ram(int address) {

	unsigned char bytes[sizeof(float)];
	for (int i = 0; i < sizeof(float); i++) {
		bytes[i] = ram[address + i];
	}
	float ret;
	memcpy(&ret, bytes, sizeof(float));
	return ret;
}

double PPC_exc::read_double_from_ram(int address) {

	unsigned char bytes[sizeof(double)];
	for (int i = 0; i < sizeof(double); i++) {
		bytes[i] = ram[address + i];
	}
	float ret;
	memcpy(&ret, bytes, sizeof(double));
	return ret;
}

void PPC_exc::execute() {
	while (!end_reached) {

		if (current_address == breakpoint) {
			debug = true;
		}

		if (debug) {
			print_current_line();
			cout << "press Enter to run line\n";
			string enter;
			getline(cin, enter);
		}

		string code = source[current_address];
		replace(code.begin(), code.end(), ',', ' ');
		istringstream f_args(code);
		current_address += 4;
		string f_name;
		f_args >> f_name;

		if (f_name == "add") { add(f_args); }
		else if (f_name == "addi") { addi(f_args); }
		else if (f_name == "b") { b(f_args); }
		else if (f_name == "bdnz") { bdnz(f_args); }
		else if (f_name == "beq") { beq(f_args); }
		else if (f_name == "beqlr") { beqlr(f_args); }
		else if (f_name == "bge") { bge(f_args); }
		else if (f_name == "bgt") { bgt(f_args); }
		else if (f_name == "bl") { bl(f_args); }
		else if (f_name == "ble") { ble(f_args); }
		else if (f_name == "blr") { blr(f_args); }
		else if (f_name == "blt") { blt(f_args); }
		else if (f_name == "bne") { bne(f_args); }
		else if (f_name == "cmplw") { cmplw(f_args); }
		else if (f_name == "cmplwi") { cmplwi(f_args); }
		else if (f_name == "cmpw") { cmpw(f_args); }
		else if (f_name == "cmpwi") { cmpwi(f_args); }
		else if (f_name == "crclr") { crclr(f_args); }
		else if (f_name == "cror") { cror(f_args); }
		else if (f_name == "end") { end_reached = true; }
		else if (f_name == "fabs") { fabs(f_args); }
		else if (f_name == "fadd") { fadd(f_args, false); }
		else if (f_name == "fadds") { fadd(f_args, true); }
		else if (f_name == "fcmpo") { fcmpo(f_args); }
		else if (f_name == "fcmpu") { fcmpu(f_args); }
		else if (f_name == "fctiwz") { fctiwz(f_args); }
		else if (f_name == "fdiv") { fdiv(f_args, false); }
		else if (f_name == "fdivs") { fdiv(f_args, true); }
		else if (f_name == "fmadd") { fmadd(f_args, false); }
		else if (f_name == "fmadds") {fmadd(f_args, true); }
		else if (f_name == "fmr") { fmr(f_args); }
		else if (f_name == "fmsub") { fmsub(f_args); }
		else if (f_name == "fmul") { fmul(f_args, false); }
		else if (f_name == "fmuls") { fmul(f_args, true); }
		else if (f_name == "fneg") { fneg(f_args); }
		else if (f_name == "fnmsub") { fnmsub(f_args, false); }
		else if (f_name == "fnmsubs") { fnmsub(f_args, true); }
		else if (f_name == "frsp") { frsp(f_args); }
		else if (f_name == "frsqrte") { frsqrte(f_args); }
		else if (f_name == "fsub") { fsub(f_args, false); }
		else if (f_name == "fsubs") { fsub(f_args, true); }
		else if (f_name == "lbz") { lbz(f_args); }
		else if (f_name == "lbzu") { lbzu(f_args); }
		else if (f_name == "lfd") { lfd(f_args); }
		else if (f_name == "lfdx") { lfdx(f_args); }
		else if (f_name == "lfs") { lfs(f_args); }
		else if (f_name == "lhz") { lhz(f_args); }
		else if (f_name == "li") { li(f_args); }
		else if (f_name == "lis") { lis(f_args); }
		else if (f_name == "lwz") { lwz(f_args); }
		else if (f_name == "mflr") { mflr(f_args); }
		else if (f_name == "mr") { mr(f_args, false); }
		else if (f_name == "mr.") { mr(f_args, true); }
		else if (f_name == "mtctr") { mtctr(f_args); }
		else if (f_name == "mtlr") { mtlr(f_args); }
		else if (f_name == "mulli") { mulli(f_args); }
		else if (f_name == "neg") { neg(f_args); }
		else if (f_name == "oris") { oris(f_args); }
		else if (f_name == "ps_add") { ps_add(f_args); }
		else if (f_name == "ps_madd") { ps_madd(f_args); }
		else if (f_name == "ps_mul") { ps_mul(f_args); }
		else if (f_name == "ps_muls0") { ps_muls0(f_args); }
		else if (f_name == "ps_sub") { ps_sub(f_args); }
		else if (f_name == "ps_sum0") { ps_sum0(f_args); }
		else if (f_name == "psq_l") { psq_l(f_args); }
		else if (f_name == "psq_st") { psq_st(f_args); }
		else if (f_name == "rlwimi") { rlwimi(f_args); }
		else if (f_name == "rlwinm") { rlwinm(f_args, false); }
		else if (f_name == "rlwinm.") { rlwinm(f_args, true); }
		else if (f_name == "srawi") { srawi(f_args); }
		else if (f_name == "stb") { stb(f_args); }
		else if (f_name == "stbu") { stbu(f_args); }
		else if (f_name == "stfd") { stfd(f_args); }
		else if (f_name == "stfs") { stfs(f_args); }
		else if (f_name == "stmw") { stmw(f_args); }
		else if (f_name == "stw") { stw(f_args); }
		else if (f_name == "stwu") { stwu(f_args); }
		else if (f_name == "sub") { sub(f_args, false); }
		else if (f_name == "sub.") { sub(f_args, true); }
		else if (f_name == "subi") { subi(f_args); }
		else if (f_name == "subic") { subic(f_args, false); }
		else if (f_name == "subic.") { subic(f_args, true); }
		else if (f_name == "subis") { subis(f_args); }
		else if (f_name == "xor") { xor_f(f_args); }
		else if (f_name == "xoris") { xoris(f_args); }
		else {throw f_name; }
	
		if (debug) {
			print_registers();
			print_ram();
		}
	}
}

	void PPC_exc::print_current_line() {
		cout << current_address << R"(: )" << source[current_address] << '\n';
	}

	void PPC_exc::print_registers() {
		for (auto const& r : int_registers) {
			cout << r.first << R"(: )" << r.second << '\n';
		}
		for (auto const& fr : double_registers) {
			cout << fr.first << R"(: )" << fr.second << '\n';
		}
		cout << '\n';
	}

	void PPC_exc::print_ram() {
		// this could be fancier
		for (auto const& a : ram) {
			cout << a.first << ':' << a.second << '\n';
		}
		cout << '\n';
	}

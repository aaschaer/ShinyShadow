#pragma once

#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <iterator>

using namespace std;

class PPC_exc {
public:
	PPC_exc(
		int start_address,
		map<string, int> starting_int_registers,
		map<string, double> starting_double_registers,
		map<int, unsigned char> starting_ram
	);
	bool debug;
	int breakpoint;
	void write_int_to_ram(int address, int val);
	void write_single_to_ram(int address, float val);
	void write_double_to_ram(int address, double val);
	int read_int_from_ram(int address);
	float read_single_from_ram(int address);
	double read_double_from_ram(int address);
	void execute();
	void print_current_line();
	void print_ram();
	void print_registers();

	bool end_reached;
	int current_address;
	map<string, int> int_registers;
	map<string, double> double_registers;
	map<int, unsigned char> ram;

	//ppc functions
	void add(istringstream& args);
	void addi(istringstream& args);
	void b(istringstream& args);
	void bdnz(istringstream& args);
	void beq(istringstream& args);
	void beqlr(istringstream& args);
	void bge(istringstream& args);
	void bgt(istringstream& args);
	void bl(istringstream& args);
	void ble(istringstream& args);
	void blr(istringstream& args);
	void blt(istringstream& args);
	void bne(istringstream& args);
	void cmplw(istringstream& args);
	void cmplwi(istringstream& args);
	void cmpw(istringstream& args);
	void cmpwi(istringstream& args);
	void crclr(istringstream& args);
	void cror(istringstream& args);
	void end(istringstream& args);
	void fabs(istringstream& args);
	void fadd(istringstream& args, bool s);
	void fcmpo(istringstream& args);
	void fcmpu(istringstream& args);
	void fctiwz(istringstream& args);
	void fdiv(istringstream& args, bool s);
	void fmadd(istringstream& args, bool s);
	void fmr(istringstream& args);
	void fmsub(istringstream& args);
	void fmul(istringstream& args, bool s);
	void fneg(istringstream& args);
	void fnmsub(istringstream& args, bool s);
	void frsp(istringstream& args);
	void frsqrte(istringstream& args);
	void fsub(istringstream& args, bool s);
	void lbz(istringstream& args, bool u);
	void lfd(istringstream& args);
	void lfdx(istringstream& args);
	void lfs(istringstream& args);
	void lhz(istringstream& args);
	void li(istringstream& args);
	void lis(istringstream& args);
	void lwz(istringstream& args);
	void mflr(istringstream& args);
	void mr(istringstream& args, bool cr);
	void mtctr(istringstream& args);
	void mtlr(istringstream& args);
	void mulli(istringstream& args);
	void neg(istringstream& args);
	void or_f (istringstream& args, bool cr);
	void oris(istringstream& args);
	void ps_add(istringstream& args);
	void ps_madd(istringstream& args);
	void ps_mul(istringstream& args);
	void ps_muls0(istringstream& args);
	void ps_sub(istringstream& args);
	void ps_sum0(istringstream& args);
	void psq_l(istringstream& args);
	void psq_st(istringstream& args);
	void rlwimi(istringstream& args);
	void rlwinm(istringstream& args, bool cr);
	void srawi(istringstream& args);
	void stb(istringstream& args, bool u);
	void stfd(istringstream& args);
	void stfs(istringstream& args);
	void stmw(istringstream& args);
	void stw(istringstream& args, bool u);
	void sub(istringstream& args, bool cr);
	void subi(istringstream& args);
	void subic(istringstream& args, bool cr);
	void subis(istringstream& args);
	void xor_f (istringstream& args);
	void xoris(istringstream& args);
};

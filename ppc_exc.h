#pragma once

#include <unordered_map>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip> 
#include <string>
#include <iterator>

# include "ram.h"

using namespace std;

class PPC_exc {
public:
	PPC_exc(
		RAM& global_ram,
		int start_address,
		unordered_map<string, int> starting_int_registers,
		unordered_map<string, double> starting_double_registers
	);
	RAM& ram;
	bool debug;
	int breakpoint;
	void execute();
	void print_current_line();
	void print_registers();
	bool end_reached;
	int current_address;
	unordered_map<string, int> int_registers;
	unordered_map<string, double> double_registers;

	//ppc functions
	void add(stringstream& args);
	void addi(stringstream& args);
	void b(stringstream& args);
	void bdnz(stringstream& args);
	void beq(stringstream& args);
	void beqlr(stringstream& args);
	void bge(stringstream& args);
	void bgt(stringstream& args);
	void bl(stringstream& args);
	void ble(stringstream& args);
	void blr(stringstream& args);
	void blt(stringstream& args);
	void bne(stringstream& args);
	void cmplw(stringstream& args);
	void cmplwi(stringstream& args);
	void cmpw(stringstream& args);
	void cmpwi(stringstream& args);
	void cror(stringstream& args);
	void fabs(stringstream& args);
	void fadd(stringstream& args, bool s);
	void fcmp(stringstream& args);
	void fctiwz(stringstream& args);
	void fdiv(stringstream& args, bool s);
	void fmadd(stringstream& args, bool s);
	void fmr(stringstream& args);
	void fmsub(stringstream& args);
	void fmul(stringstream& args, bool s);
	void fneg(stringstream& args);
	void fnmsub(stringstream& args, bool s);
	void frsp(stringstream& args);
	void frsqrte(stringstream& args);
	void fsub(stringstream& args, bool s);
	void lbz(stringstream& args, bool u);
	void lfd(stringstream& args);
	void lfdx(stringstream& args);
	void lfs(stringstream& args);
	void li(stringstream& args);
	void lis(stringstream& args);
	void lwz(stringstream& args);
	void mflr(stringstream& args);
	void mr(stringstream& args, bool cr);
	void mtctr(stringstream& args);
	void mtlr(stringstream& args);
	void mulli(stringstream& args);
	void neg(stringstream& args);
	void or_f (stringstream& args, bool cr);
	void oris(stringstream& args);
	void ps_add(stringstream& args);
	void ps_madd(stringstream& args);
	void ps_mul(stringstream& args);
	void ps_muls0(stringstream& args);
	void ps_sub(stringstream& args);
	void ps_sum0(stringstream& args);
	void psq_l(stringstream& args);
	void psq_st(stringstream& args);
	void rlwimi(stringstream& args);
	void rlwinm(stringstream& args, bool cr);
	void srawi(stringstream& args);
	void stb(stringstream& args, bool u);
	void stfd(stringstream& args);
	void stfs(stringstream& args);
	void stw(stringstream& args, bool u);
	void sub(stringstream& args, bool cr);
	void subi(stringstream& args);
	void subic(stringstream& args, bool cr);
	void subis(stringstream& args);
	void xor_f (stringstream& args);
	void xoris(stringstream& args);
};

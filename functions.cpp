#include "ppc_exc.h"
#include "rtoc.h"

enum CR { LT = 8, GT = 4, EQ = 2 };

int parse_int_str(string str) {
	bool negative = false;
	unsigned int x;

	if (str.at(0) == '-') {
		negative = true;
		str.erase(0, 1);
	}
	if (str.substr(0, 2) == "0x") {
		str.erase(0, 2);
		std::stringstream ss;
		ss << hex << str;
		ss >> x;
	}
	else {
		x = stoi(str);
	}
	if (negative) {
		return ((int)x) * -1;
	}
	else {
		return (int) x;
	}
}

void PPC_exc::add(stringstream& args) {
	string rt, ra, rb;
	args >> rt >> ra >> rb;
	int_registers[rt] = int_registers.at(ra) + int_registers.at(rb);
}

void PPC_exc::addi(stringstream& args) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers.at(ra) + i;
}

void PPC_exc::b(stringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	current_address = pointer;
}

void PPC_exc::bdnz(stringstream& args) {
	int_registers["ctr"] = int_registers.at("ctr") - 1;
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers.at("ctr") != 0)
		current_address = pointer;
}

void PPC_exc::beq(stringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers.at("cr") & EQ)
		current_address = pointer;
}

void PPC_exc::beqlr(stringstream& args) {
	if (int_registers.at("cr") & EQ)
		current_address = int_registers.at("lr");
}

void PPC_exc::bge(stringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers.at("cr") & (GT | EQ))
		current_address = pointer;
}

void PPC_exc::bgt(stringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers.at("cr") & GT)
		current_address = pointer;
}

void PPC_exc::bl(stringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	int_registers["lr"] = current_address;
	current_address = pointer;
}

void PPC_exc::ble(stringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers.at("cr") & (LT | EQ))
		current_address = pointer;
}

void PPC_exc::blr(stringstream& args) {
	current_address = int_registers.at("lr");
}

void PPC_exc::blt(stringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers.at("cr") & LT)
		current_address = pointer;
}

void PPC_exc::bne(stringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (!(int_registers.at("cr") & EQ))
		current_address = pointer;
}

void PPC_exc::cmplw(stringstream& args) {
	string ra, rb;
	args >> ra >> rb;
	unsigned int val_a = (unsigned int)int_registers.at(ra);
	unsigned int val_b = (unsigned int)int_registers.at(rb);
	if (val_a < val_b) {
		int_registers["cr"] = LT;
	}
	else if (val_a > val_b){
		int_registers["cr"] = GT;
	}
	else {
		int_registers["cr"] = EQ;
	}
}

void PPC_exc::cmplwi(stringstream& args) {
	string ra;
	unsigned int i;
	args >> ra >> i;
	unsigned int val_a = (unsigned int)int_registers.at(ra);
	if (val_a < i) {
		int_registers["cr"] = LT;
	}
	else if (val_a > i){
		int_registers["cr"] = GT;
	}
	else {
		int_registers["cr"] = EQ;
	}
}

void PPC_exc::cmpw(stringstream& args) {
	string ra, rb;
	args >> ra >> rb;
	if (int_registers.at(ra) < int_registers.at(rb)) {
		int_registers["cr"] = LT;
	}
	else if (int_registers.at(ra) > int_registers.at(rb)){
		int_registers["cr"] = GT;
	}
	else {
		int_registers["cr"] = EQ;
	}
}

void PPC_exc::cmpwi(stringstream& args) {
	string ra;
	int i;
	args >> ra >> i;
	if (int_registers.at(ra) < i) {
		int_registers["cr"] = LT;
	}
	else if (int_registers.at(ra) > i){
		int_registers["cr"] = GT;
	}
	else {
		int_registers["cr"] = EQ;
	}
}

void PPC_exc::cror(stringstream& args) {
	int ct;
	args >> ct;
	if (ct == 0){
		throw ct;
	}
}

void PPC_exc::fabs(stringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = abs(double_registers.at(fra));
	// does not effect prt?
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::fadd(stringstream& args, bool s) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;
	double val = double_registers.at(fra) + double_registers.at(frb);
	if (s) {
		double_registers[frt] = (double)((float)val);
	}
	else {
		double_registers[frt] = val;
	}
	//does not effect prt?
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::fcmp(stringstream& args) {
	string crt, fra, frb;
	args >> crt >> fra >> frb;
	if (crt != "cr0") {
		throw crt;
	}
	if (isnan(double_registers.at(fra))) {
		throw fra;
	}
	if (isnan(double_registers.at(frb))) {
		throw frb;
	}
	if (double_registers.at(fra) < double_registers.at(frb)) {
		int_registers["cr"] = LT;
	}
	else if (double_registers.at(fra) > double_registers.at(frb)){
		int_registers["cr"] = GT;
	}
	else {
		int_registers["cr"] = EQ;
	}
}

void PPC_exc::fctiwz(stringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	long long val_a = (long long)((int)double_registers.at(fra));
	memcpy(&double_registers.at(frt), &val_a, 8);
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::fdiv(stringstream& args, bool s) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;
	double val = double_registers.at(fra) / double_registers.at(frb);
	if (s) {
		double_registers[frt] = (double)((float)val);
	}
	else {
		double_registers[frt] = val;	
	}
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
	
}

void PPC_exc::fmadd(stringstream& args, bool s) {
	string frt, fra, frb, frc;
	args >> frt >> fra >> frb >> frc;
	double val = (double_registers.at(fra) * double_registers.at(frb)) + double_registers.at(frc);
	if (s) {
		double_registers[frt] = (double)((float)val);
	}
	else {
		double_registers[frt] = val;
	}
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::fmr(stringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = double_registers.at(fra);
	// does not effect prt?
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);

}

void PPC_exc::fmsub(stringstream& args) {
	string frt, fra, frb, frc;
	args >> frt >> fra >> frb >> frc;
	double_registers[frt] = (double_registers.at(fra) * double_registers.at(frb)) - double_registers.at(frc);
	//undefined prt behavior ?
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::fmul(stringstream& args, bool s) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;
	double val = double_registers.at(fra) * double_registers.at(frb);
	if (s) {
		double_registers[frt] = (double)((float)val);
	}
	else {
		double_registers[frt] = val;
	}
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::fneg(stringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = double_registers.at(fra) * -1.0;
}

void PPC_exc::fnmsub(stringstream& args, bool s) {
	string frt, fra, frb, frc;
	args >> frt >> fra >> frb >> frc;
	double val = ((double_registers.at(fra) * double_registers.at(frb)) - double_registers.at(frc)) * -1;
	if (s) {
		double_registers[frt] = (double)((float)val);
	}
	else {
		double_registers[frt] = val;	
	}
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::frsp(stringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = (double)((float) double_registers.at(fra));
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::frsqrte(stringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = 1.0 / sqrt(double_registers.at(fra));
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::fsub(stringstream& args, bool s) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;
	double_registers[frt] = double_registers.at(fra) - double_registers.at(frb);
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::lbz(stringstream& args, bool u) {
	string rt, offset_s, ra;
	args >> rt >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	int ea = int_registers.at(ra) + offset;
	int_registers[rt] = ram.read_byte(ea);
	if (u) {
		int_registers[ra] = ea;
	}
}

void PPC_exc::lfd(stringstream& args) {
	string frt, offset_s, ra;
	args >> frt >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	double val;
	if (ra == "rtoc") {
		int ea = 0x804836a0 + offset;
		val = double_rtoc.at(ea);
	}
	else {
		int ea = int_registers.at(ra) + offset;
		val = ram.read_double(ea);
	}
	double_registers[frt] = val;
	// does not effect prt?
	//undefined prt behavior
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers.erase(prt);
}

void PPC_exc::lfdx(stringstream& args) {
	string frt, ra, rb;
	args >> frt >> ra >> rb;
	int ea = int_registers.at(ra) + int_registers.at(rb);
	double val = ram.read_double(ea);
	double_registers[frt] = val;
}

void PPC_exc::lfs(stringstream& args) {
	string frt, offset_s, ra;
	args >> frt >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	float val;
	if (ra == "rtoc") {
		int ea = 0x804836a0 + offset;
		val = single_rtoc.at(ea);
	}
	else {
		int ea = int_registers.at(ra) + offset;
		val = ram.read_single(ea);
	}
	double_registers[frt] = (double)val;

	//loaded into both frt and prt
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = (double)val;
}

void PPC_exc::li(stringstream& args) {
	string rt;
	int i;
	args >> rt >> i;
	int_registers[rt] = i;
}

void PPC_exc::lis(stringstream& args) {
	string rt, i_s;
	args >> rt >> i_s;
	int i = parse_int_str(i_s);
	int_registers[rt] = i << 16;
}

void PPC_exc::lwz(stringstream& args) {
	string rt, offset_s, ra;
	args >> rt >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	int ea = int_registers.at(ra) + offset;
	int val = ram.read_int(ea);
	int_registers[rt] = val;
}

void PPC_exc::mflr(stringstream& args) {
	string rt;
	args >> rt;
	int_registers[rt] = int_registers.at("lr");
}

void PPC_exc::mr(stringstream& args, bool cr) {
	string rt, ra;
	args >> rt >> ra;
	int_registers[rt] = int_registers.at(ra);
	if (cr) {
		if (int_registers.at(rt) < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers.at(rt) > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::mtctr(stringstream& args) {
	string ra;
	args >> ra;
	int_registers["ctr"] = int_registers.at(ra);
}

void PPC_exc::mtlr(stringstream& args) {
	string ra;
	args >> ra;
	int_registers["lr"] = int_registers.at(ra);
}

void PPC_exc::mulli(stringstream& args) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers.at(ra) * i;
}

void PPC_exc::neg(stringstream& args) {
	string rt, ra;
	args >> rt >> ra;
	int_registers[rt] = (~int_registers.at(ra)) + 1;
}

void PPC_exc::or_f(stringstream& args, bool cr) {
	string rt, ra, rb;
	args >> rt >> ra >> rb;
	int_registers[rt] = int_registers.at(ra) | int_registers.at(rb);
	if (cr) {
		if (int_registers.at(rt) < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers.at(rt) > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::oris(stringstream& args) {
	string rt, ra, i_s;
	args >> rt >> ra >> i_s;
	int i = parse_int_str(i_s);
	int_registers[rt] = int_registers.at(ra) | (i << 16);
}

void PPC_exc::ps_add(stringstream& args) {
	string prt, pra, prb;
	args >> prt >> pra >> prb;

	string frt = prt;
	replace(frt.begin(), frt.end(), 'p', 'f');
	string fra = pra;
	replace(fra.begin(), fra.end(), 'p', 'f');
	string frb = prb;
	replace(frb.begin(), frb.end(), 'p', 'f');

	double_registers[frt] = double_registers.at(fra) + double_registers.at(frb);
	double_registers[prt] = double_registers.at(pra) + double_registers.at(prb);
}

void PPC_exc::ps_madd(stringstream& args) {
	string prt, pra, prb, prc;
	args >> prt >> pra >> prb >> prc;

	string frt = prt;
	replace(frt.begin(), frt.end(), 'p', 'f');
	string fra = pra;
	replace(fra.begin(), fra.end(), 'p', 'f');
	string frb = prb;
	replace(frb.begin(), frb.end(), 'p', 'f');
	string frc = prc;
	replace(frc.begin(), frc.end(), 'p', 'f');

	double_registers[frt] = (double_registers.at(fra) * double_registers.at(frb)) + double_registers.at(frc);
	double_registers[prt] = (double_registers.at(pra) * double_registers.at(prb)) + double_registers.at(prc);
}

void PPC_exc::ps_mul(stringstream& args) {
	string prt, pra, prb;
	args >> prt >> pra >> prb;

	string frt = prt;
	replace(frt.begin(), frt.end(), 'p', 'f');
	string fra = pra;
	replace(fra.begin(), fra.end(), 'p', 'f');
	string frb = prb;
	replace(frb.begin(), frb.end(), 'p', 'f');

	double_registers[frt] = double_registers.at(fra) * double_registers.at(frb);
	double_registers[prt] = double_registers.at(pra) * double_registers.at(prb);
}

void PPC_exc::ps_muls0(stringstream& args) {
	string prt, pra, prb;
	args >> prt >> pra >> prb;

	string frt = prt;
	replace(frt.begin(), frt.end(), 'p', 'f');
	string fra = pra;
	replace(fra.begin(), fra.end(), 'p', 'f');
	string frb = prb;
	replace(frb.begin(), frb.end(), 'p', 'f');

    double_registers[frt] = double_registers.at(fra) * double_registers.at(frb);
    double_registers[prt] = double_registers.at(pra) * double_registers.at(frb);
}

void PPC_exc::ps_sub(stringstream& args) {
	string prt, pra, prb;
	args >> prt >> pra >> prb;

	string frt = prt;
	replace(frt.begin(), frt.end(), 'p', 'f');
	string fra = pra;
	replace(fra.begin(), fra.end(), 'p', 'f');
	string frb = prb;
	replace(frb.begin(), frb.end(), 'p', 'f');

	double_registers[frt] = double_registers.at(fra) - double_registers.at(frb);
	double_registers[prt] = double_registers.at(pra) - double_registers.at(prb);
}

void PPC_exc::ps_sum0(stringstream& args) {
	string prt, pra, prb, prc;
	args >> prt >> pra >> prb >> prc;

	string frt = prt;
	replace(frt.begin(), frt.end(), 'p', 'f');
	string fra = pra;
	replace(fra.begin(), fra.end(), 'p', 'f');
	string frb = prb;
	replace(frb.begin(), frb.end(), 'p', 'f');
	string frc = prc;
	replace(frc.begin(), frc.end(), 'p', 'f');

	double_registers[frt] = double_registers.at(fra) + double_registers.at(prb);
	double_registers[prt] = double_registers.at(prc);
}

void PPC_exc::psq_l(stringstream& args) {
	string prt, ra;
	int offset, w;
	args >> prt >> offset >> ra >> w;
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	string frt = prt;
	replace(frt.begin(), frt.end(), 'p', 'f');

	int ea = int_registers.at(ra) + offset;
	double_registers[frt] = (double)ram.read_single(ea);

	if (w) {
		double_registers[prt] = 1.0;
	}
	else {
		double_registers[prt] = (double)ram.read_single(ea + 4);
	}
}

void PPC_exc::psq_st(stringstream& args) {
	int offset;
	string ra, prs;
	args >> offset >> ra >> prs;
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	string frs = prs;
	replace(frs.begin(), frs.end(), 'p', 'f');
	int ea = int_registers.at(ra) + offset;
	ram.write_single(ea, (float)double_registers.at(frs));
	ram.write_single(ea+4, (float)double_registers.at(prs));
}

void PPC_exc::rlwimi(stringstream& args) {
	string rt, ra;
	int sh, mb, me;
	args >> rt >> ra >> sh >> mb >> me;

	unsigned int val = (unsigned int) int_registers.at(ra);
	val = (val << sh) | val >> (32 - sh);

	int mask = 0;
	if (mb < me + 1) {
		for (int i = mb; i < me + 1; i++) {
			mask = mask | (0x80000000 >> i);
		}
	} else if (mb == me + 1) {
		mask = 0xffffffff;
	} else {
		throw "rlwimi";
	}

	int_registers[rt] = (int)((int_registers.at(rt) & ~mask) | (val & mask));
}

void PPC_exc::rlwinm(stringstream& args, bool cr) {
	string rt, ra;
	int sh, mb, me;
	args >> rt >> ra >> sh >> mb >> me;

	unsigned int val = (unsigned int) int_registers.at(ra);
	val = (val << sh) | val >> (32 - sh);

	int mask = 0;
	if (mb < me + 1) {
		for (int i = mb; i < me + 1; i++) {
			mask = mask | (0x80000000 >> i);
		}
	} else if (mb == me + 1) {
		mask = 0xffffffff;
	} else {
		throw "rlwinm";
	}

	int_registers[rt] = (int)(val & mask);

	if (cr) {
		if (int_registers.at(rt) < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers.at(rt) > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::srawi(stringstream& args) {
	string rt, ra;
	int sh;
	args >> rt >> ra >> sh;
	unsigned int val = (unsigned int) int_registers.at(ra);
	unsigned int mask = val & 0x80000000;
	for (int i = 0; i < sh; i++) {
		mask = mask | (mask >> 1);
	}
	int_registers[rt] = (int)((val >> sh) | mask);
}

void PPC_exc::stb(stringstream& args, bool u) {
	string rs, offset_s, ra;
	args >> rs >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	int ea = int_registers.at(ra) + offset;
	ram.write_byte(ea, (unsigned char)int_registers.at(rs));
	if (u) {
		int_registers[ra] = ea;
	}
}

void PPC_exc::stfd(stringstream& args) {
	string rs, offset_s, ra;
	args >> rs >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	int ea = int_registers.at(ra) + offset;
	ram.write_double(ea, double_registers.at(rs));
}

void PPC_exc::stfs(stringstream& args) {
	string rs, offset_s, ra;
	args >> rs >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	int ea = int_registers.at(ra) + offset;
	ram.write_single(ea, (float)double_registers.at(rs));
}

void PPC_exc::stw(stringstream& args, bool u) {
	string rs, offset_s, ra;
	args >> rs >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	int ea = int_registers.at(ra) + offset;
	ram.write_int(ea, int_registers.at(rs));
	if (u) {
		int_registers[ra] = ea;
	}
}

void PPC_exc::sub(stringstream& args, bool cr) {
	string rt, ra, rb;
	args >> rt >> ra >> rb;
	int_registers[rt] = int_registers.at(ra) - int_registers.at(rb);
	if (cr) {
		if (int_registers.at(rt) < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers.at(rt) > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::subi(stringstream& args) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers.at(ra) - i;
}

void PPC_exc::subic(stringstream& args, bool cr) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers.at(ra) - i;
	if (cr) {
		if (int_registers.at(rt) < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers.at(rt) > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::subis(stringstream& args) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers.at(ra) - (i << 16);
}

void PPC_exc::xor_f(stringstream& args) {
	string rt, ra, rb;
	args >> rt >> ra >> rb;
	int_registers[rt] = int_registers.at(ra) ^ int_registers.at(rb);
}

void PPC_exc::xoris(stringstream& args) {
	string rt, ra, i_s;
	args >> rt >> ra >> i_s;
	int i = parse_int_str(i_s);
	int_registers[rt] = int_registers.at(ra) ^ (i << 16);
}

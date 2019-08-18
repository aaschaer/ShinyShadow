#include "ppc_exc.h"
#include "rtoc.h"

enum CR { LT = 8, GT = 4, EQ = 2 };

int parse_int_str(string str) {
	bool negative = false;
	int x;

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
		x = atoi(c_str(str));
	}
	if (negative)
		x *= -1;
	return x;
}

void PPC_exc::add(istringstream& args) {
	string rt, ra, rb;
	args >> rt >> ra >> rb;
	int_registers[rt] = int_registers[ra] + int_registers[rb];
}

void PPC_exc::addi(istringstream& args) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers[ra] + i;
}

void PPC_exc::b(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	current_address = pointer;
}

void PPC_exc::bdnz(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers["ctr"] == 0)
		current_address = pointer;
}

void PPC_exc::beq(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers["cr"] | EQ)
		current_address = pointer;
}

void PPC_exc::beqlr(istringstream& args) {
	if (int_registers["cr"] | EQ)
		current_address = int_registers["lr"];
}

void PPC_exc::bge(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers["cr"] | (GT | EQ))
		current_address = pointer;
}

void PPC_exc::bgt(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers["cr"] | GT)
		current_address = pointer;
}

void PPC_exc::bl(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	int_registers["lr"] = current_address + 4;
	current_address = pointer;
}

void PPC_exc::ble(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers["cr"] | (LT | EQ))
		current_address = pointer;
}

void PPC_exc::blr(istringstream& args) {
	current_address = int_registers["lr"];
}

void PPC_exc::blt(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (int_registers["cr"] | LT)
		current_address = pointer;
}

void PPC_exc::bne(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_int_str(pointer_s.erase(0, 2));
	if (!(int_registers["cr"] | EQ))
		current_address = pointer;
}

void PPC_exc::cmplw(istringstream& args) {
	string ra, rb;
	args >> ra >> rb;
	unsigned int val_a = (unsigned int) int_registers[ra];
	unsigned int val_b = (unsigned int) int_registers[rb]
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

void PPC_exc::cmplwi(istringstream& args) {
	string ra;
	int i;
	args >> ra >> i;
	unsigned int val_a = (unsigned int) int_registers[ra];
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

void PPC_exc::cmpw(istringstream& args) {
	string ra, rb;
	args >> ra >> rb;
	if (int_registers[ra] < int_registers[rb]) {
		int_registers["cr"] = LT;
	}
	else if (int_registers[ra] > int_registers[rb]){
		int_registers["cr"] = GT;
	}
	else {
		int_registers["cr"] = EQ;
	}
}

void PPC_exc::cmpwi(istringstream& args) {
	string ra;
	int i;
	args >> ra >> i;
	if (int_registers[ra] < i) {
		int_registers["cr"] = LT;
	}
	else if (int_registers[ra] > i){
		int_registers["cr"] = GT;
	}
	else {
		int_registers["cr"] = EQ;
	}
}

void PPC_exc::crclr(istringstream& args) {
	//TODO: test, this seems wrong
	int_registers["cr"] = LT;
}

void PPC_exc::cror(istringstream& args) {
	int ct;
	args >> ct;
	if (ct == 0){
		throw ct;
	}
}

void PPC_exc::fabs(istringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = abs(double_registers[fra]);
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fadd(istringstream& args, bool s) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;
	double val = double_registers[fra] + double_registers[frb];
	if (s) {
		double_registers[frt] = (double)((float)val);
	}
	else {
		double_registers[frt] = val;
	}
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fcmpo(istringstream& args) {
	string crt, fra, frb;
	args >> crt >> fra >> frb;
	if (crt != "cr0")
		throw crt;
	if (double_registers[fra] < double_registers[frb]) {
		int_registers["cr"] = LT;
	}
	else if (double_registers[fra] > double_registers[frb]){
		int_registers["cr"] = GT;
	}
	else {
		int_registers["cr"] = EQ;
	}
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fcmpu(istringstream& args) {
	string crt, fra, frb;
	args >> crt >> fra >> frb;
	if (crt != "cr0")
		throw crt;
	if isnan(double_registers[fra])
		throw fra;
	if isnan(double_registers[frb])
		throw frb;
	if (double_registers[fra] < double_registers[frb]) {
		int_registers["cr"] = LT;
	}
	else if (double_registers[fra] > double_registers[frb]){
		int_registers["cr"] = GT;
	}
	else {
		int_registers["cr"] = EQ;
	}
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fctiwz(istringstream& args) {
	// TODO: test
	string frt, fra;
	args >> frt >> fra;
	int val_a = (int) double_registers[fra];
	unsigned char* val_a_bytes = reinterpret_cast<unsigned char *>(&);
	unsigned char* bytes = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	for (int i = 0; i < 4; i++) {
		bytes[4+i] = val_a_bytes[i];
	}
	memcpy(&double_registers[frt], bytes, 8);
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fdiv(istringstream& args, bool s) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;
	double val = double_registers[fra] / double_registers[frb];
	if (s) {
		double_registers[frt] = (double)((float)val);
	}
	else {
		double_registers[frt] = val;	
	}
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fmadd(istringstream& args, bool s) {
	string frt, fra, frb, frc;
	args >> frt >> fra >> frb >> frc;
	double val = (double_registers[fra] * double_registers[frb]) + double_registers[frc];
	if (s) {
		double_registers[frt] = (double)((float)val)
	}
	else {
		double_registers[frt] = val;
	}
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fmr(istringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = double_registers[fra];
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fmsub(istringstream& args) {
	string frt, fra, frb, frc;
	args >> frt >> fra >> frb >> frc;
	double_registers[frt] = (double_registers[fra] * double_registers[frb]) - double_registers[frc];
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fmul(istringstream& args, bool s) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;
	double val = double_registers[fra] * double_registers[frb];
	if (s) {
		double_registers[frt] = (double)((float)val)
	}
	else {
		double_registers[frt] = val;
	}
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fneg(istringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = double_registers[fra] * -1.0;
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fnmsub(istringstream& args, bool s) {
	string frt, fra, frb, frc;
	args >> frt >> fra >> frb >> frc;
	double val = ((double_registers[fra] * double_registers[frb]) - double_registers[frc]) * -1;
	if (s) {
		double_registers[frt] = (double)((float)val);
	}
	else {
		double_registers[frt] = val;	
	}
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::frsp(istringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = (double)((float) double_registers[fra]);
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::frsqrte(istringstream& args) {
	string frt, fra;
	args >> frt >> fra;
	double_registers[frt] = 1.0 / sqrt(double_registers[fra]);
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::fsub(istringstream& args, bool s) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;
	double_registers[frt] = double_registers[fra] - double_registers[frb];
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = 0.0;
}

void PPC_exc::lbz(istringstream& args, bool u) {
	string rt, offset_s, ra;
	args >> rt >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	int ea = int_registers[ra] + offset;
	unsigned char val = ram[ea];
	int_registers[rt] = int(val);
	if (u) {
		int_registers[ra] = ea;
	}
}

void PPC_exc::lfd(istringstream& args) {
	string frt, offset_s, ra;
	args >> frt >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	double val;
	if (ra == "rtoc") {
		int ea = 0x804836a0 + offset;
		val = double_rtoc[ea];
	}
	else {
		int ea = int_registers[ra] + offset;
		val = read_single_from_ram(ea);
	}
	double_registers[frt] = val;
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = val;
}

void PPC_exc::lfdx(istringstream& args) {
	string frt, ra, rb;
	args >> frt >> ra >> rb;
	ea = int_registers[ra] + int_registers[rb];
	val = read_double_from_ram(ea);
	double_registers[frt] = val;
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = val;
}

void PPC_exc::lfs(istringstream& args) {
	string frt, offset_s, ra;
	args >> frt >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	fra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	fra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	float val;
	if (ra == "rtoc") {
		int ea = 0x804836a0 + offset;
		val = single_rtoc[ea];
	}
	else {
		int ea = int_registers[ra] + offset;
		val = read_single_from_ram(ea);
	}
	double_registers[frt] = (double)val;
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = (double)val;
}

void PPC_exc::li(istringstream& args) {
	string rt;
	int i;
	args >> rt >> i;
	int_registers[ra] = i;
}

void PPC_exc::lis(istringstream& args) {
	string rt, i_s;
	args >> rt >> i_s;
	int i = parse_int_str(i_s);
	int_registers[rt] = i << 16
}

void PPC_exc::lwz(istringstream& args) {
	string rt, offset_s, ra;
	args >> rt >> offset_s >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	int val;
	if (ra == "rtoc") {
		int ea = 0x804836a0 + offset;
		val = double_rtoc[ea];
	}
	else {
		int ea = int_registers[ra] + offset;
		val = read_single_from_ram(ea);
	}
	double_registers[frt] = val;
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	double_registers[prt] = val;
}

void PPC_exc::mflr(istringstream& args) {
	string rt;
	args >> rt;
	int_registers[rt] = int_registers["lr"];
}

void PPC_exc::mr(istringstream& args, bool cr) {
	string rt, ra;
	args >> rt >> ra;
	int_registers[rt] = int_registers[ra];
	if (cr) {
		if (int_registers[rt] < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers[rt] > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::mtctr(istringstream& args) {
	string ra;
	args >> ra;
	int_registers["ctr"] = int_registers[ra];
}

void PPC_exc::mtlr(istringstream& args) {
	string ra;
	args >> ra;
	int_registers["lr"] = int_registers[ra];
}

void PPC_exc::mulli(istringstream& args) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers[ra] * i;
}

void PPC_exc::neg(istringstream& args) {
	string rt, ra;
	args >> rt >> ra;
	int_registers[rt] = (~int_registers[ra]) + 1;
}

void PPC_exc::or_f(istringstream& args, bool cr) {
	string rt, ra, rb;
	args >> rt >> ra >> rb;
	int_registers[rt] = int_registers[ra] | int_registers [rb];
	if (cr) {
		if (int_registers[rt] < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers[rt] > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::oris(istringstream& args) {
	string rt, ra, i_s;
	ars >> rt >> ra >> i_s;
	int i = parse_int_str(i_s);
	int_registers[rt] = int_registers[ra] | (i << 16);
}

void PPC_exc::ps_add(istringstream& args) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;

	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	string pra = fra;
	replace(pra.begin(), pra.end(), 'f', 'p');
	string prb = frb;
	replace(prb.begin(), prb.end(), 'f', 'p');

	double_registers[frt] = double_registers[fra] + double_registers[frb];
	double_registers[prt] = double_registers[pra] + double_registers[prb];
}

void PPC_exc::ps_madd(istringstream& args) {
	string frt, fra, frb, frc;
	args >> frt >> fra >> frb >> frc;

	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	string pra = fra;
	replace(pra.begin(), pra.end(), 'f', 'p');
	string prb = frb;
	replace(prb.begin(), prb.end(), 'f', 'p');
	string prc = frc;
	replace(prc.begin(), prc.end(), 'f', 'p');

	double_registers[frt] = (double_registers[fra] * double_registers[frb]) + double_registers[frc];
	double_registers[prt] = (double_registers[pra] * double_registers[prb]) + double_registers[prc];
}

void PPC_exc::ps_mul(istringstream& args) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;

	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	string pra = fra;
	replace(pra.begin(), pra.end(), 'f', 'p');
	string prb = frb;
	replace(prb.begin(), prb.end(), 'f', 'p');

	double_registers[frt] = double_registers[fra] * double_registers[frb];
	double_registers[prt] = double_registers[pra] * double_registers[prb];
}

void PPC_exc::ps_muls0(istringstream& args) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;

	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	string pra = fra;
	replace(pra.begin(), pra.end(), 'f', 'p');
	string prb = frb;
	replace(prb.begin(), prb.end(), 'f', 'p');

    double_registers[frt] = double_registers[fra] * double_registers[frb];
    double_registers[prt] = double_registers[pra] * double_registers[frb];
}

void PPC_exc::ps_sub(istringstream& args) {
	string frt, fra, frb;
	args >> frt >> fra >> frb;

	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	string pra = fra;
	replace(pra.begin(), pra.end(), 'f', 'p');
	string prb = frb;
	replace(prb.begin(), prb.end(), 'f', 'p');

	double_registers[frt] = double_registers[fra] - double_registers[frb];
	double_registers[prt] = double_registers[pra] - double_registers[prb];
}

void PPC_exc::ps_sum0(istringstream& args) {
	string frt, fra, frb, frc;
	args >> frt >> fra >> frb >> frc;

	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');
	string pra = fra;
	replace(pra.begin(), pra.end(), 'f', 'p');
	string prb = frb;
	replace(prb.begin(), prb.end(), 'f', 'p');
	string prc = frc;
	replace(prc.begin(), prc.end(), 'f', 'p');

	double_registers[frt] = double_registers[fra] + double_registers[prb];
	double_registers[prt] = double_registers[prc];
}

void PPC_exc::psq_l(istringstream& args) {
	string frt, ra
	int offset, w;
	args >> frt >> offset >> ra >> w;
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	string prt = frt;
	replace(prt.begin(), prt.end(), 'f', 'p');

	int ea = int_registers[ra] + offset;
	double_registers[frt] = (double) read_single_from_ram(ea);

	if (w) {
		double_registers[prt] = 1.0;
	}
	else {
		double_registers[prt] = (double) read_single_from_ram(ea + 4);
	}
}

void PPC_exc::psq_st(istringstream& args) {
	int offset;
	string ra, frs;
	args >> offset >> ra >> frs;
	string prs = frs;
	replace(prs.begin(), prs.end(), 'f', 'p');
	int ea = int_registers[ra] + offset;
	write_single_to_ram(ea, (float)double_registers[frs]);
	write_single_to_ram(ea+4, (float)double_registers[prs]);
}

void PPC_exc::rlwimi(istringstream& args) {
	string rt, ra;
	int sh, mb, me;
	args >> rt >> ra >> sh >> mb >> me

	int val = int_registers[ra];
	val = (val << sh) | val >> (32 - sh);

	int mask = 0;
	if (mb < me + 1) {
		for (int i = mb; i < me; i++) {
			mask = mask | (0x80000000 >> i);
		}
	} else if (mb == me + 1) {
		mask = 0xffffffff;
	} else {
		throw "rlwimi";
	}

	int_registers[rt] = (val & ~mask) | (val & mask);
}

void PPC_exc::rlwinm(istringstream& args, bool cr) {
	string rt, ra;
	int sh, mb, me;
	args >> rt >> ra >> sh >> mb >> me

	int val = int_registers[ra];
	val = (val << sh) | val >> (32 - sh);

	int mask = 0;
	if (mb < me + 1) {
		for (int i = mb; i < me; i++) {
			mask = mask | (0x80000000 >> i);
		}
	} else if (mb == me + 1) {
		mask = 0xffffffff;
	} else {
		throw "rlwinm";
	}

	int_registers[rt] = val & mask;

	if (cr) {
		if (int_registers[rt] < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers[rt] > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::srawi(istringstream& args) {
	string rt, ra;
	int sh;
	args >> rt >> ra >> sh;
	int val = int_registers[ra];
	int mask = val & 0x80000000;
	for (int i = 0; i < sh; i++) {
		mask = mask | (mask >> 1);
	}
	int_registers[rt] = (val >> sh) | mask;
}

void PPC_exc::stb(istringstream& args, bool u) {
	string rs, offset_s, ra;
	args >> rs >> offset >> ra;
	int offset = parse_int_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	int ea = int_registers[ra] + offset
	ram[ea] = (unsigned char) int_registers[rs];
	if (u) {
		int_registers[ra] = ea;
	}
}

void PPC_exc::stfd(istringstream& args) {
	string rs, offset_s, ra;
	args >> rs >> offset >> ra;
	int offset = parse_int_str(offset_s);
	int ea = int_registers[ra] + offset;
	write_double_to_ram(ea, double_registers[rs]);
}

void PPC_exc::stfs(istringstream& args) {
	string rs, offset_s, ra;
	args >> rs >> offset >> ra;
	int offset = parse_int_str(offset_s);
	int ea = int_registers[ra] + offset;
	write_single_to_ram(ea, (float)double_registers[rs]);
}

void PPC_exc::stw(istringstream& args, bool u) {
	string rs, offset_s, ra;
	args >> rs >> offset >> ra;
	int offset = parse_int_str(offset_s);
	int ea = int_registers[ra] + offset;
	write_int_to_ram(ea, int_registers[rs]);
	if (u) {
		int_registers[ra] = ea;
	}
}

void PPC_exc::sub(istringstream& args, bool cr) {
	string rt, ra, rb;
	args >> rt >> ra >> rb;
	int_registers[rt] = int_registers[ra] - int_registers[rb];
	if (cr) {
		if (int_registers[rt] < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers[rt] > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::subi(istringstream& args) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers[ra] - i;
}

void PPC_exc::subic(istringstream& args, bool cr) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers[ra] - i;
	if (cr) {
		if (int_registers[rt] < 0) {
			int_registers["cr"] = LT;
		}
		else if (int_registers[rt] > 0){
			int_registers["cr"] = GT;
		}
		else {
			int_registers["cr"] = EQ;
		}
	}
}

void PPC_exc::subis(istringstream& args) {
	string rt, ra;
	int i;
	args >> rt >> ra >> i;
	int_registers[rt] = int_registers[ra] - (i << 16);
}

void PPC_exc::xor_f(istringstream& args) {
	string rt, ra, rb;
	args >> rt >> ra >> rb;
	int_registers[rt] = int_registers[ra] ^ int_registers [rb];
}

void PPC_exc::xoris(istringstream& args) {
	string rt, ra, i_s;
	ars >> rt >> ra >> i_s;
	int i = parse_int_str(i_s);
	int_registers[rt] = int_registers[ra] ^ (i << 16);
}

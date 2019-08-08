#include "ppc_exc.h"
#include "rtoc.h"

enum CR { LT = 8, GT = 4, EQ = 2 };

int parse_hex_str(string str) {
	bool negative = false;
	if (str.at(0) == '-') {
		negative = true;
		str.erase(0, 1);
	}
	if (str.substr(0, 2) != "0x") {
		throw str;
	}
	str.erase(0, 2);

	int x;
	std::stringstream ss;
	ss << hex << str;
	ss >> x;

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
	int pointer = parse_hex_str(pointer_s.erase(0, 2));
	current_address = pointer;
}

void PPC_exc::bdnz(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_hex_str(pointer_s.erase(0, 2));
	if (int_registers["ctr"] == 0)
		current_address = pointer;
}

void PPC_exc::beq(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_hex_str(pointer_s.erase(0, 2));
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
	int pointer = parse_hex_str(pointer_s.erase(0, 2));
	if (int_registers["cr"] | (GT | EQ))
		current_address = pointer;
}

void PPC_exc::bgt(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_hex_str(pointer_s.erase(0, 2));
	if (int_registers["cr"] | GT)
		current_address = pointer;
}

void PPC_exc::bl(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_hex_str(pointer_s.erase(0, 2));
	int_registers["lr"] = current_address + 4;
	current_address = pointer;
}

void PPC_exc::ble(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_hex_str(pointer_s.erase(0, 2));
	if (int_registers["cr"] | (LT | EQ))
		current_address = pointer;
}

void PPC_exc::blr(istringstream& args) {
	current_address = int_registers["lr"];
}

void PPC_exc::blt(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_hex_str(pointer_s.erase(0, 2));
	if (int_registers["cr"] | LT)
		current_address = pointer;
}

void PPC_exc::bne(istringstream& args) {
	string pointer_s;
	args >> pointer_s;
	int pointer = parse_hex_str(pointer_s.erase(0, 2));
	if (!(int_registers["cr"] | EQ))
		current_address = pointer;
}

void PPC_exc::cmplw(istringstream& args) {

}

void PPC_exc::cmplwi(istringstream& args) {

}

void PPC_exc::cmpw(istringstream& args) {

}

void PPC_exc::cmpwi(istringstream& args) {

}

void PPC_exc::crclr(istringstream& args) {

}

void PPC_exc::cror(istringstream& args) {

}

void PPC_exc::end(istringstream& args) {

}

void PPC_exc::fabs(istringstream& args) {

}

void PPC_exc::fadd(istringstream& args, bool s) {

}

void PPC_exc::fcmpo(istringstream& args) {

}

void PPC_exc::fcmpu(istringstream& args) {

}

void PPC_exc::fctiwz(istringstream& args) {

}

void PPC_exc::fdiv(istringstream& args, bool s) {

}

void PPC_exc::fmadd(istringstream& args, bool s) {

}

void PPC_exc::fmr(istringstream& args) {

}

void PPC_exc::fmsub(istringstream& args) {

}

void PPC_exc::fmul(istringstream& args, bool s) {

}

void PPC_exc::fneg(istringstream& args) {

}

void PPC_exc::fnmsub(istringstream& args, bool s) {

}

void PPC_exc::frsp(istringstream& args) {

}

void PPC_exc::frsqrte(istringstream& args) {

}

void PPC_exc::fsub(istringstream& args, bool s) {

}

void PPC_exc::lbz(istringstream& args) {

}

void PPC_exc::lbzu(istringstream& args) {

}

void PPC_exc::lfd(istringstream& args) {

}

void PPC_exc::lfdx(istringstream& args) {

}


void PPC_exc::lfs(istringstream& args) {
	string rt, offset_s, ra;
	args >> rt >> offset_s >> ra;
	int offset = parse_hex_str(offset_s);
	ra.erase(remove(ra.begin(), ra.end(), '('), ra.end());
	ra.erase(remove(ra.begin(), ra.end(), ')'), ra.end());
	string rp = rt;
	replace(rp.begin(), rp.end(), 'f', 'p');

	float val;
	if (ra == "rtoc") {
		int ea = 0x804836a0 + offset;
		val = single_rtoc[ea];
	}
	else {
		int ea = int_registers[ra] + offset;
		val = read_single_from_ram(ea);
	}
	double_registers[rt] = (double)val;
	double_registers[rp] = (double)val;
}

void PPC_exc::lhz(istringstream& args) {

}

void PPC_exc::li(istringstream& args) {

}

void PPC_exc::lis(istringstream& args) {

}

void PPC_exc::lwz(istringstream& args) {

}

void PPC_exc::mflr(istringstream& args) {

}

void PPC_exc::mr(istringstream& args, bool cr) {

}

void PPC_exc::mtctr(istringstream& args) {
	string ra;
	args >> ra;
	int_registers["ctr"] = int_registers[ra];
}

void PPC_exc::mtlr(istringstream& args) {

}

void PPC_exc::mulli(istringstream& args) {

}

void PPC_exc::neg(istringstream& args) {

}

void PPC_exc::or_f(istringstream& args, bool cr) {

}

void PPC_exc::oris(istringstream& args) {

}

void PPC_exc::ps_add(istringstream& args) {

}

void PPC_exc::ps_madd(istringstream& args) {

}

void PPC_exc::ps_mul(istringstream& args) {

}

void PPC_exc::ps_muls0(istringstream& args) {

}

void PPC_exc::ps_sub(istringstream& args) {

}

void PPC_exc::ps_sum0(istringstream& args) {

}

void PPC_exc::psq_l(istringstream& args) {

}

void PPC_exc::psq_st(istringstream& args) {

}

void PPC_exc::rlwimi(istringstream& args) {

}

void PPC_exc::rlwinm(istringstream& args, bool cr) {

}

void PPC_exc::srawi(istringstream& args) {

}

void PPC_exc::stb(istringstream& args) {

}

void PPC_exc::stbu(istringstream& args) {

}

void PPC_exc::stfd(istringstream& args) {

}

void PPC_exc::stfs(istringstream& args) {

}

void PPC_exc::stmw(istringstream& args) {

}

void PPC_exc::stw(istringstream& args) {

}

void PPC_exc::stwu(istringstream& args) {

}

void PPC_exc::sub(istringstream& args, bool cr) {

}

void PPC_exc::subi(istringstream& args) {

}

void PPC_exc::subic(istringstream& args, bool cr) {

}

void PPC_exc::subis(istringstream& args) {

}

void PPC_exc::xor_f(istringstream& args) {

}

void PPC_exc::xoris(istringstream& args) {

}



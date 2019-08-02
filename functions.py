from conversion import *
from rtoc import RTOC
import math


def end(state):
    state.end_reached = True


def nop(state):
    state.current_address += 4


def b(state, pointer):
    state.current_address = int(pointer[2:], 16)


def bl(state, pointer):
    state.registers["lr"] = state.current_address + 4
    state.current_address = int(pointer[2:], 16)


def blr(state):
    state.current_address = state.registers["lr"]


def lfs(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    if ra == "rtoc":
        val = RTOC[offset]
    else:
        ea = state.registers[ra] + offset
        hex_str = state.read_hex_str_from_ram(ea, 4)
        val = hex_str_to_single(hex_str)
    state.registers[rt] = single_to_double(val)
    state.registers[rt.replace("f", "p")] = single_to_double(val)
    state.current_address += 4


def lfd(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    if ra == "rtoc":
        val = RTOC[offset]
        assert isinstance(val, float)
    else:
        ea = state.registers[ra] + offset
        hex_str = state.read_hex_str_from_ram(ea, 8)
        val = hex_str_to_double(hex_str)
    state.registers[rt] = val 
    state.current_address += 4


def lwz(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    hex_str = state.read_hex_str_from_ram(ea, 4)
    val = hex_str_to_int(hex_str)
    state.registers[rt] = val
    state.current_address += 4


def lhz(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    hex_str = state.read_hex_str_from_ram(ea, 2)
    hex_str = "0000" + hex_str
    val = hex_str_to_int(hex_str)
    state.registers[rt] = val
    state.current_address += 4


def lbz(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    hex_str = state.read_hex_str_from_ram(ea, 1)
    hex_str = "000000" + hex_str
    val = hex_str_to_int(hex_str)
    state.registers[rt] = val
    state.current_address += 4


def lbzu(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    hex_str = state.read_hex_str_from_ram(ea, 1)
    hex_str = "000000" + hex_str
    val = hex_str_to_int(hex_str)
    state.registers[rt] = val
    state.registers[ra] = ea
    state.current_address += 4


def stfd(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    val = state.registers[rs]
    hex_str = double_to_hex_str(val)
    state.write_hex_str_to_ram(ea, hex_str)
    state.current_address += 4


def stfs(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    val = state.registers[rs]
    hex_str = single_to_hex_str(double_to_single(val))
    state.write_hex_str_to_ram(ea, hex_str)
    state.current_address += 4


def stw(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    val = state.registers[rs]
    hex_str = int_to_hex_str(val)
    state.write_hex_str_to_ram(ea, hex_str)
    state.current_address += 4


def stwu(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    val = state.registers[rs]
    hex_str = int_to_hex_str(val)
    state.write_hex_str_to_ram(ea, hex_str)
    state.registers[ra] = ea
    state.current_address += 4


def stmw(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)

    ea = state.registers[ra] + offset
    start_i = int(rs[1:])
    for i in range (start_i,31):
        val = state.registers["r" + str(i)]
        hex_str = int_to_hex_str(val)
        state.write_hex_str_to_ram(ea, hex_str)
        ea += 4

    state.current_address += 4


def stb(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    val = state.registers[rs]
    hex_str = int_to_hex_str(val)
    state.write_hex_str_to_ram(ea, hex_str[6:])
    state.current_address += 4


def stbu(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    ea = state.registers[ra] + offset
    val = state.registers[rs]
    hex_str = int_to_hex_str(val)
    state.write_hex_str_to_ram(ea, hex_str[6:])
    state.registers[ra] = ea
    state.current_address += 4


def fmul(state, rt, ra, rc):
    val_a = state.registers[ra]
    val_c = state.registers[rc]
    assert isinstance(val_a, float)
    assert isinstance(val_c, float)
    state.registers[rt] = val_a * val_c
    state.current_address += 4


def fmuls(state, rt, ra, rc):
    val_a = state.registers[ra]
    val_c = state.registers[rc]
    assert isinstance(val_a, float)
    assert isinstance(val_c, float)
    state.registers[rt] = single_to_double(
        double_to_single(val_a * val_c))
    state.current_address += 4


def fdivs(state, rt, ra, rc):
    val_a = state.registers[ra]
    val_c = state.registers[rc]
    assert isinstance(val_a, float)
    assert isinstance(val_c, float)
    state.registers[rt] = single_to_double(
        double_to_single(val_a / val_c))
    state.current_address += 4


def fnmsubs(state, rt, ra, rc, rb):
    val_a = state.registers[ra]
    val_c = state.registers[rc]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_c, float)
    assert isinstance(val_b, float)
    state.registers[rt] = single_to_double(
        double_to_single(-(val_a * val_c - val_b)))
    state.current_address += 4


def mulli(state, rt, ra, i):
    i = int(i)
    val = state.registers[ra]
    assert isinstance(val, int)
    state.registers[rt] = val * i
    state.current_address += 4


def fmr(state, rt, ra):
    val_a = state.registers[ra]
    assert isinstance(val_a, float)
    state.registers[rt] = val_a
    state.current_address += 4


def frsp(state, rt, ra):
    state.registers[rt] = single_to_double(double_to_single(
        state.registers[ra]))
    state.current_address += 4


def frsqrte(state, rt, ra):
    val_a = state.registers[ra]
    assert isinstance(val_a, float)
    state.registers[rt] = 1.0 / math.sqrt(val_a)
    state.current_address += 4


def mflr(state, ra):
    state.registers[ra] = state.registers["lr"]
    state.current_address += 4


def mtlr(state, ra):
    state.registers["lr"] = state.registers[ra]
    state.current_address += 4


def li(state, ra, i):
    i = int(i)
    state.registers[ra] = i
    state.current_address += 4


def lis(state, ra, i):
    try:
        i = int(i)
    except:
        i = int(i, 16)
    assert i <= 0xffff 
    state.registers[ra] = i << 16
    state.current_address += 4


def add(state, rt, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, int)
    assert isinstance(val_b, int)
    state.registers[rt] = val_a + val_b
    state.current_address += 4


def addi(state, rt, ra, i):
    i = int(i)
    val = state.registers[ra]
    assert isinstance(val, int)
    state.registers[rt] = val + i
    state.current_address += 4


def sub(state, rt, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, int)
    assert isinstance(val_b, int)
    state.registers[rt] = val_a - val_b
    state.current_address += 4


def subi(state, rt, ra, i):
    i = int(i)
    val = state.registers[ra]
    assert isinstance(val, int)
    state.registers[rt] = val - i
    state.current_address += 4


def subic(state, rt, ra, i):
    i = int(i)
    val = state.registers[ra]
    assert isinstance(val, int)
    state.registers[rt] = val - i

    # fix this?
    state.cr0 = val - i
    state.cr1 = 0

    state.current_address += 4


def subis(state, rt, ra, i):
    i = int(i)
    val = state.registers[ra]
    assert isinstance(val, int)
    state.registers[rt] = val - (i << 16)
    state.current_address += 4


def rlwinm(state, ra, rs, sh, mb, me, m):
    sh = int(sh)
    m = int(m.strip("()"), 16)
    val = state.registers[rs]
    assert isinstance(val, int)
    
    # bit rotate left
    assert(sh < 32)
    b = bin(val)[2:]
    b = "0" * (32- len(b)) + b
    assert len(b) == 32
    b = b[sh:] + b[:sh]
    val = int(b, 2)

    # mask
    state.registers[ra] = val & m
    state.current_address += 4


def rlwimi(state, ra, rs, sh, mb, me, m):
    sh = int(sh)
    m = int(m.strip("()"), 16)
    val_a = state.registers[ra]
    val_s = state.registers[rs]
    assert isinstance(val_a, int)
    assert isinstance(val_s, int)
    
    # bit rotate left
    assert(sh < 32)
    b = bin(val_s)[2:]
    b = "0" * (32- len(b)) + b
    assert len(b) == 32
    b = b[sh:] + b[:sh]
    val_s = int(b, 2)

    # mask insert
    state.registers[ra] = (val_a & ~m) | (val_s & m)
    state.current_address += 4


def cmpw(state, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, int)
    assert isinstance(val_b, int)
    state.cr0 = val_a
    state.cr1 = val_b
    state.current_address += 4


def cmpwi(state, ra, i):
    val = state.registers[ra]
    assert isinstance(val, int)
    state.cr0 = val
    state.cr1 = int(i)
    state.current_address += 4


def cmplw(state, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, int)
    assert isinstance(val_b, int)
    state.cr0 = val_a
    state.cr1 = val_b
    state.current_address += 4


def cmplwi(state, ra, i):
    i = int(i)
    val = state.registers[ra]
    assert isinstance(val, int)
    if i < 0:
        raise Exception("implement cmplwi")
    if val < 0:
        val += 2**32 # negative two's compliment to unsigned 32 bit
    state.cr0 = val
    state.cr1 = i
    state.current_address += 4


def fcmpo(state, cr, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_b, float)
    state.cr0 = val_a
    state.cr1 = val_b
    state.current_address += 4


def fcmpu(state, cr, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_b, float)
    state.cr0 = val_a
    state.cr1 = val_b
    state.current_address += 4


def cror(state, bt, ba, bb):
    if bt == "0":
        raise Exception("implement condition registers")
    state.current_address += 4


def crclr(state, a, b):
    state.cr0 = 0
    state.cr1 = 1
    state.current_address += 4


def bgt(state, pointer):
    if state.cr0 > state.cr1:
        state.current_address = int(pointer[2:], 16)
    else:
        state.current_address += 4


def blt(state, pointer):
    if state.cr0 < state.cr1:
        state.current_address = int(pointer[2:], 16)
    else:
        state.current_address += 4


def bge(state, pointer):
    if state.cr0 >= state.cr1:
        state.current_address = int(pointer[2:], 16)
    else:
        state.current_address += 4


def ble(state, pointer):
    if state.cr0 <= state.cr1:
        state.current_address = int(pointer[2:], 16)
    else:
        state.current_address += 4


def bne(state, pointer):
    if state.cr0 != state.cr1:
        state.current_address = int(pointer[2:], 16)
    else:
        state.current_address += 4


def beq(state, pointer):
    if state.cr0 == state.cr1:
        state.current_address = int(pointer[2:], 16)
    else:
        state.current_address += 4


def beqlr(state):
    if state.cr0 == state.cr1:
        state.current_address = state.registers["lr"]
    else:
        state.current_address += 4


def fctiwz(state, rt, rb):
    val = state.registers[rb]
    assert isinstance(val, float)
    val = int(val)
    val = hex_str_to_double("00000000" + int_to_hex_str(val))
    state.registers[rt] = val
    state.current_address += 4


def fmadd(state, rt, ra, rc, rb):
    val_a = state.registers[ra]
    val_c = state.registers[rc]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_c, float)
    assert isinstance(val_b, float)
    state.registers[rt] = (val_a * val_c) + val_b 
    state.current_address += 4


def fmadds(state, rt, ra, rc, rb):
    val_a = state.registers[ra]
    val_c = state.registers[rc]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_c, float)
    assert isinstance(val_b, float)
    state.registers[rt] = single_to_double(double_to_single(
        (val_a * val_c) + val_b))
    state.current_address += 4


def fmsub(state, rt, ra, rc, rb):
    val_a = state.registers[ra]
    val_c = state.registers[rc]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_c, float)
    assert isinstance(val_b, float)
    state.registers[rt] = (val_a * val_c) - val_b 
    state.current_address += 4


def mr(state, rt, rb):
    state.registers[rt] = state.registers[rb]
    state.current_address += 4


def fabs(state, rt, rb):
    val = state.registers[rb]
    assert isinstance(val, float)
    state.registers[rt] = abs(val)
    state.current_address += 4


def xoris(state, ra, rs, i):
    try:
        i = int(i)
    except:
        i = int(i, 16)
    val = state.registers[rs]
    assert isinstance(val, int)
    state.registers[ra] = val ^ (i << 16)
    state.current_address += 4


def fadd(state, rt, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_b, float)
    state.registers[rt] = val_a + val_b
    state.current_address += 4


def fadds(state, rt, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_b, float)
    state.registers[rt] = single_to_double(
        double_to_single(val_a + val_b))
    state.current_address += 4


def fsub(state, rt, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_b, float)
    state.registers[rt] = val_a - val_b
    state.current_address += 4


def fsubs(state, rt, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_b, float)
    state.registers[rt] = single_to_double(
        double_to_single(val_a - val_b))
    state.current_address += 4


def fnmsub(state, rt, ra, rc, rb):
    val_a = state.registers[ra]
    val_c = state.registers[rc]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_c, float)
    assert isinstance(val_b, float)
    state.registers[rt] = ((val_a * val_c) - val_b) * -1
    state.current_address += 4


def fneg(state, rt, rb):
    val = state.registers[rb]
    assert isinstance(val, float)
    state.registers[rb] = val * -1
    state.current_address += 4


def srawi(state, rt, rs, sh):
    sh = int(sh)
    assert sh < 32
    val = state.registers[rs]
    assert isinstance(val, int)
    # shirt right then fill with previous sign bit
    sign_bit = int_to_bin_str(val)[0]
    val = val >> sh
    val = val | int(sign_bit * sh + "0" * (32 - sh), 2)
    state.registers[rt] = val
    state.current_address += 4


def psq_l(state, rt, dra, w):
    offset, ra = dra.split("(")
    offset = int(offset)
    ra = ra.strip(")")
    ea1 = state.registers[ra] + offset
    hex_str1 = state.read_hex_str_from_ram(ea1, 4)
    val1 = single_to_double(hex_str_to_single(hex_str1))
    state.registers[rt.replace("p", "f")] = val1

    if int(w) != 1:
        ea2 = ea1 + 4
        hex_str2 = state.read_hex_str_from_ram(ea2, 4)
        val2 = single_to_double(hex_str_to_single(hex_str2))
        state.registers[rt] = val2
    else:
        state.registers[rt] = 1.0
    state.current_address += 4
    

def psq_st(state, dra, rs):
    offset, ra = dra.split("(")
    offset = int(offset)
    ra = ra.strip(")")
    ea1 = state.registers[ra] + offset
    val1 = state.registers[rs.replace("p", "f")]
    hex_str1 = single_to_hex_str(double_to_single(val1))
    state.write_hex_str_to_ram(ea1, hex_str1)
    ea2 = ea1 + 4
    val2 = state.registers[rs]
    hex_str2 = single_to_hex_str(double_to_single(val2))
    state.write_hex_str_to_ram(ea2, hex_str2)
    state.current_address += 4


def ps_add(state, rt, rab):
    ra, rb = rab.split("+")
    val_a0 = state.registers[ra.replace("p", "f")]
    val_b0 = state.registers[rb.replace("p", "f")]
    state.registers[rt.replace("p", "f")] = val_a0 + val_b0
    val_a1 = state.registers[ra]
    val_b1 = state.registers[rb]
    state.registers[rt] = val_a1 + val_b1
    state.current_address += 4


def ps_sub(state, rt, rab):
    ra, rb = rab.split("-")
    val_a0 = state.registers[ra.replace("p", "f")]
    val_b0 = state.registers[rb.replace("p", "f")]
    state.registers[rt.replace("p", "f")] = val_a0 - val_b0
    val_a1 = state.registers[ra]
    val_b1 = state.registers[rb]
    state.registers[rt] = val_a1 - val_b1
    state.current_address += 4


def ps_mul(state, rt, rab):
    ra, rb = rab.split("*")
    val_a0 = state.registers[ra.replace("p", "f")]
    val_b0 = state.registers[rb.replace("p", "f")]
    state.registers[rt.replace("p", "f")] = val_a0 * val_b0
    val_a1 = state.registers[ra]
    val_b1 = state.registers[rb]
    state.registers[rt] = val_a1 * val_b1
    state.current_address += 4


def ps_madd(state, rt, rabc):
    ra, rbc = rabc.split("*")
    rb, rc = rbc.split("+")
    val_a0 = state.registers[ra.replace("p", "f")]
    val_b0 = state.registers[rb.replace("p", "f")]
    val_c0 = state.registers[rc.replace("p", "f")]
    state.registers[rt.replace("p", "f")] = val_a0 * val_b0 + val_c0
    val_a1 = state.registers[ra]
    val_b1 = state.registers[rb]
    val_c1 = state.registers[rc]
    state.registers[rt] = val_a1 * val_b1 + val_c1
    state.current_address += 4


def ps_sum0(state, rt, rab, rc):
    ra, rb = rab[2:].split("+")
    rc = rc[2:]
    val_a0 = state.registers[ra.replace("p", "f")]
    val_b1 = state.registers[rb]
    val_c1 = state.registers[rc]
    state.registers[rt.replace("p", "f")] = val_a0 + val_b1
    state.registers[rt] = val_c1
    state.current_address += 4


def ps_muls0(state, rt, rab):
    ra, rb = rab[:-3].split("*")
    val_a0 = state.registers[ra.replace("p", "f")]
    val_a1 = state.registers[ra]
    val_b0 = state.registers[rb.replace("p", "f")]
    state.registers[rt.replace("p", "f")] = val_a0 * val_b0
    state.registers[rt] = val_a1 * val_b0
    state.current_address += 4

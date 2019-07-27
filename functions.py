from conversion import *
from rtoc import RTOC

def end(state):
    state.end_reached = True

def ret(state, rs):
    val = double_to_single(state.registers[rs])
    state.ret.append(val)
    state.current_address += 4

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
        hex_str = state.ram[state.registers[ra] + offset]
        val = hex_str_to_single(hex_str)
    state.registers[rt] = single_to_double(val) 
    state.current_address += 4

def lfd(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    if ra == "rtoc":
        val = RTOC[offset]
        assert isinstance(val, float)
    else:
        ea = state.registers[ra] + offset
        hex_str = state.ram[ea] + state.ram[ea + 4]
        val = hex_str_to_double(hex_str)
    state.registers[rt] = val 
    state.current_address += 4

def stfd(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    val = state.registers[rs]
    hex_str = double_to_hex_str(val)
    # store in two words to allow lwz calls later
    state.ram[state.registers[ra] + offset] = hex_str[0:8]
    state.ram[state.registers[ra] + offset + 4] = hex_str[8:16]
    state.current_address += 4

def stfs(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    val = state.registers[rs]
    hex_str = single_to_hex_str(val)
    state.ram[state.registers[ra] + offset] = hex_str
    state.current_address += 4

def stw(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    val = state.registers[rs]
    hex_str = int_to_hex_str(val)
    state.ram[state.registers[ra] + offset] = hex_str
    state.current_address += 4

def stwu(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    val = state.registers[rs]
    hex_str = int_to_hex_str(val)
    ea = state.registers[ra] + offset
    state.ram[ea] = hex_str
    state.registers[ra] = ea
    state.current_address += 4

def lwz(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    hex_str = state.ram[state.registers[ra] + offset]
    val = hex_str_to_int(hex_str)
    state.registers[rt] = val
    state.current_address += 4

def fmul(state, rt, ra, rc):
    val_a = state.registers[ra]
    val_c = state.registers[rc]
    assert isinstance(val_a, float)
    assert isinstance(val_c, float)
    state.registers[rt] = val_a * val_c
    state.current_address += 4

def fmuls(state, rt, ra, rc):
    val_a = double_to_single(state.registers[ra])
    val_c = double_to_single(state.registers[rc])
    state.registers[rt] = single_to_double(val_a * val_c)
    state.current_address += 4

def fmr(state, rt, rb):
    val_b = state.registers[rb]
    assert isinstance(val_b, float)
    state.registers[rt] = val_b
    state.current_address += 4

def frsp(state, rt, rb):
    state.registers[rt] = double_to_single(state.registers[rb])
    state.current_address += 4

def mflr(state, ra):
    state.registers[ra] = state.registers["lr"]
    state.current_address += 4

def mtlr(state, ra):
    state.registers["lr"] = state.registers[ra]
    state.current_address += 4

def li(state, ra, i):
    i = int(i, 16)
    state.registers[ra] = i
    state.current_address += 4

def lis(state, ra, i):
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

def subis(state, rt, ra, i):
    i = int(i)
    val = state.registers[ra]
    assert isinstance(val, int)
    state.registers[rt] = val - (i << 16)
    state.current_address += 4

def rlwinm(state, ra, rs, sh, mb, me, m):
    mb = int(mb)
    me = int(me)
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
    state.registers[ra] = val & m
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

def cmplwi(state, ra, i):
    # NEEDS TESTING
    i = int(i)
    val = state.registers[ra]
    assert isinstance(val, int)
    if i < 0:
        raise Exception("implement cmplwi")
    if val < 0:
        val += 2**32 # negative two's compliment to unsigned 32 bit
    state.cr0 = val
    state.cr1 = i  # << 16 ?
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

def fsub(state, rt, ra, rb):
    val_a = state.registers[ra]
    val_b = state.registers[rb]
    assert isinstance(val_a, float)
    assert isinstance(val_b, float)
    state.registers[rt] = val_a - val_b
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

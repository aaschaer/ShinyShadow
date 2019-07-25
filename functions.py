from conversion import double_to_single
from rtoc import RTOC

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
        val = double_to_single(RTOC[offset])
    else:
        ea = state.registers[ra] + offset
        val = double_to_single(state.ram[ea])
    state.registers[rt] = val 
    state.current_address += 4

def lfd(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    if ra == "rtoc":
        val = RTOC[offset]
        assert isinstance(val, float)
    else:
        ea = state.registers[ra] + offset
        val = state.ram[ea]
        assert isinstance(val, float)
    state.registers[rt] = val 
    state.current_address += 4

def stfs(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    val = double_to_single(state.registers[rs])
    state.ram[state.registers[ra] + offset] = val
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
    state.registers[rt] = double_to_single(val_a * val_c)
    state.current_address += 4

def fmr(state, rt, rb):
    val_b = state.registers[rb]
    assert isinstance(val_b, float)
    state.registers[rt] = val_b
    state.current_address += 4

def frsp(state, rt, rb):
    state.registers[rt] = double_to_single(state.registers[rb])
    state.current_address += 4

def stw(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    val = state.registers[rs]
    assert isinstance(val, int)
    state.ram[state.registers[ra] + offset] = val
    state.current_address += 4

def stwu(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    val = state.registers[rs]
    assert isinstance(val, int)
    ea = state.registers[ra] + offset
    state.ram[ea] = val
    state.registers[ra] = ea
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

def stfd(state, rs, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    val = state.registers[rs]
    # assert isinstance(val, float)
    state.ram[state.registers[ra] + offset] = val
    state.current_address += 4

def addi(state, rt, ra, i):
    i = int(i)
    val = state.registers[ra]
    assert isinstance(val, int)
    state.registers[rt] = val + i
    state.current_address += 4

def lwz(state, rt, offset, ra):
    ra = ra.strip("()")
    offset = int(offset, 16)
    val = state.ram[state.registers[ra] + offset]
    print(state.registers[ra] + offset)
    print(val)
    assert isinstance(val, int)
    state.registers[rt] = val
    state.current_address += 4

def rlwinm(state, ra, rs, sh, mb, me, m):
    mb = int(mb)
    me = int(me)
    m = int(m.strip("()"), 16)
    val = state.registers[rs]
    assert isinstance(val, int)
    if int(sh) != 0:
        raise Exception("implement rlwinm")
    if mb < (me + 1):
        gen = "1" * ((me - mb) + 1)
        gen += "0" * (31 - me)
        assert int(gen, 2) == m
    else:
        raise Exception("implement rlwinm")
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

def bne(state, pointer):
    if state.cr0 != state.cr1:
        state.current_address = int(pointer[2:], 16)
    else:
        state.current_address += 4

def fctiwz(state, rt, rb):
    # this breaks some float/int assumptions
    val = state.registers[rb]
    assert isinstance(val, float)
    state.registers[rt] = int(val)
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
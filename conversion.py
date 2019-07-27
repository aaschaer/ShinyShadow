import struct
from numpy import float32

def int_to_float(val):
    assert isinstance(val, int)
    return struct.unpack('<f', struct.pack('<i', val))[0]


def float_to_int(val):
    assert isinstance(val, float)
    return struct.unpack('<i', struct.pack('<f', val))[0]


def int_to_double(val):
    assert isinstance(val, int)
    return struct.unpack('<d', struct.pack('<q', val))[0]


def double_to_int(val):
    assert isinstance(val, float)
    return struct.unpack('<q', struct.pack('<d', val))[0]


def double_to_single(val):
    assert isinstance(val, float)
    return float32(val)


def single_to_double(val):
    assert isinstance(val, float32)
    return float(val)


def int_to_hex_str(i):
    assert isinstance(i, int)
    val =  hex(i)[2:]
    val = "0" * (8 - len(val)) + val
    assert(len(val) == 8)
    return val


def hex_str_to_int(s):
    assert(len(s) == 8)
    return int(s, 16)


def hex_str_to_single(s):
    assert(len(s) == 8)
    return float32(struct.unpack('!f', bytes.fromhex(s))[0])


def hex_str_to_double(s):
    assert(len(s) == 16)
    return struct.unpack('!d', bytes.fromhex(s))[0]


def single_to_hex_str(f):
    assert isinstance(f, float32)
    val = hex(struct.unpack('<I', struct.pack('<f', f))[0])[2:]
    val = "0" * (8 - len(val)) + val
    assert(len(val) == 8)
    return val


def double_to_hex_str(f):
    assert isinstance(f, float)
    val = hex(struct.unpack('<Q', struct.pack('<d', f))[0])[2:]
    val = "0" * (16 - len(val)) + val
    assert(len(val) == 16)
    return val

def int_to_bin_str(i):
    assert isinstance(i, int)
    if i < 0:
        i += 2**32 # two's compliment
    val = bin(i)[2:]
    val = "0" * (32 - len(val)) + val
    assert len(val) == 32
    return val

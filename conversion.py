import struct

def hex_to_float(val):
    assert isinstance(val, int)
    return struct.unpack('<f', struct.pack('<i', val))[0]


def float_to_hex(val):
    assert isinstance(val, float)
    return struct.unpack('<i', struct.pack('<f', val))[0]


def hex_to_double(val):
    assert isinstance(val, int)
    return struct.unpack('<d', struct.pack('<q', val))[0]


def double_to_hex(val):
    assert isinstance(val, float)
    return struct.unpack('<q', struct.pack('<d', val))[0]


def double_to_single(val):
    assert isinstance(val, float)
    return struct.unpack('<f', struct.pack('<f', val))[0]


def hex_to_string(h):
    assert isinstance(h, int)
    return hex(h)[2:]


def str_to_hex(s):
    assert isinstance(s, str)
    return int(s, 16)

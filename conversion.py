import struct

def double_to_single(val):
    assert isinstance(val, float)
    return struct.unpack('<f', struct.pack('<f', val))[0]

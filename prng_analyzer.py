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


def LCG(prng_state, generation_constant=65536.0):
    """
    simulates subroutine 801adc7c
    """
    assert isinstance(prng_state, int)
    assert isinstance(generation_constant, float)

    prng_state = (prng_state * 214013 + 2531011) & 0xffffffff

    n = 0x4330000000000000 | (prng_state >> 16)
    n = hex_to_double(n)
    n -= 4503599627370496.0
    n /= generation_constant

    return prng_state, n


def prng_range(prng_state, n):
    prg_states = []
    for i in range(n):
        vals.append(prng_state)
        prng_state, _ = LCG(prng_state)
    return prng_states


def prng_difference(prng_state, target):
    difference = 0
    while prng_state != target:
        difference += 1
        prng_state, _ = LCG(prng_state)
    return difference


def pyrite_noise(prng_state, noise_state):
    """
    simulates 1 frame of background noise in Pyrite Town
    """
    for i in range(3):
        prng_state, prn = LCG(prng_state)
        prn = double_to_single(prn)

        noise_state[i] = (0.5 * prn) + noise_state[i]

        if noise_state[i] > 1.0:
            for j in range(8):
                prng_state, _ = LCG(prng_state)
            noise_state[i] -= 1.0

        noise_state[i] = double_to_single(noise_state[i])

    return prng_state, noise_state


if __name__ == "__main__":

    print(LCG(0x5e86b02b))

    """
    prng_state = 0x9187a4a2
    noise_state = [hex_to_float(0x3f65557e),
                   hex_to_float(0x3f5ac27e),
                   hex_to_float(0x3ea0a9fc)]

    while True:
        prng_state, noise_state = pyrite_noise(prng_state, noise_state)
        print("predicted prng_state: ", hex_to_string(prng_state))
        print("predicted noise_state: ", noise_state)
        input("press ENTER to continue")
    """

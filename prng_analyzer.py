from conversion import *
from lcg import LCG
from npc import NPC


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

    """
    prng_state = 0x9187a4a2
    noise_state = [int_to_float(0x3f65557e),
                   int_to_float(0x3f5ac27e),
                   int_to_float(0x3ea0a9fc)]

    while True:
        prng_state, noise_state = pyrite_noise(prng_state, noise_state)
        print("predicted prng_state: ", int_to_hex_str(prng_state))
        print("predicted noise_state: ", noise_state)
        input("press ENTER to continue")
    """

    lcg = LCG(0xed60636d)
    npc = NPC(lcg, 4.0, 24.0)
    npc.step() # calc dest
    assert single_to_hex_str(npc.destX) == "40327ef7"
    assert single_to_hex_str(npc.destY) == "421bcddc"

    npc.step() # walk a step
    assert single_to_hex_str(npc.currentX) == "407e7ffb"
    assert single_to_hex_str(npc.currentY) == "41c250a4"

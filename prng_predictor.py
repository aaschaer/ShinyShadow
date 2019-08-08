from conversion import *
from lcg import LCG
from npc import NPC
import random

from prediction_vid import create_frame, create_video


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

    seed = 0xed60636d

    lcg = LCG(seed)
    npc = NPC(lcg, float32(4.0), float32(24.0))
    
    for i in range(1, 60*60*2):
        npc.step()
        # create_frame(i, npc.currentX, npc.currentY)
    
        if i >= 368:
            print("step: ", i)
            print("prng: ", int_to_hex_str(lcg.state))
            print("destX: ", single_to_hex_str(npc.destX))
            print("destY: ", single_to_hex_str(npc.destY))
            print("speed: ", single_to_hex_str(npc.walk_speed))
            print("currentX: ", single_to_hex_str(npc.currentX))
            print("currentY: ", single_to_hex_str(npc.currentY))
            try:
                print("nextX: ", single_to_hex_str(npc.nextX))
                print("nextY: ", single_to_hex_str(npc.nextY))
            except:
                print("nextX: None")
                print("nextY: None")
            print("wait_time: ", single_to_hex_str(npc.wait_time))
        
            npc.debug = True

            # input("press ENTER to continue")
            print()

    # create_video(fps=18)
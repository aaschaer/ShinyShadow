from source import SOURCE
from state import State
import functions
from prng_analyzer import LCG
from conversion import *

def execute(prn):

    DEBUG = False

    # these all seem to be constants other than f1 which is the prn
    starting_ram = {
        0x809e5458: "41700000",
        0x80270184: "400921fb",
        0x80270188: "4012d97c",
        0x8027018c: "401921fb",

        # constants?
        0x809e544c: "40800000",
        0x809e5454: "41c00000",
    }
    starting_registers = {
        "sp": 0x8048e5e0,
        "r30": 0x809e53d8,
        "r31": 0x809e53d8,
        "f1": prn,
    }
    state = State(0x80184e64, starting_ram, starting_registers)

    breakpoints = []

    while not state.end_reached:

        if state.current_address in breakpoints:
            DEBUG = True

        code = SOURCE[state.current_address]
        args = code.replace(",", " ").split()

        if DEBUG:
            print(hex(state.current_address), code)
            input("press Enter to run line")
        f = method_to_call = getattr(functions, args[0].strip("-"))
        f(state, *args[1:])

        if DEBUG:
            for key in sorted(state.registers.keys()):
                val = state.registers[key]
                if isinstance(val, int):
                    print("{}: {}".format(key, int_to_hex_str(val)))
                elif isinstance(val, float):
                    print("{}: {}".format(key,  double_to_hex_str(val)))
                else:
                    raise TypeError(val)
            # print(state.ram)
            print("")

    return state.ret


if __name__ == "__main__":
    
    prng_state = 0xed60636d

    for i in range (10):

        prng_state, prn = LCG(prng_state)
        targetX, targetY = execute(prn)
        print(single_to_hex_str(targetX), single_to_hex_str(targetY))

        prng_state, prn1 = LCG(prng_state)
        prng_state, prn2 = LCG(prng_state)
        print(NPC_wait_time(prn1, prn2))

    '''
    import random
    for i in range(54000):
        execute(random.random())
    '''
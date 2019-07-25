from source import SOURCE
from state import State
import functions

def execute(prn):

    # f1 is the pseudo random number from LCG
    state = State(start_address=0x80184e64, f1=prn, sp=0x8048e5e0)

    while not state.end_reached:
        code = SOURCE[state.current_address]
        args = code.replace(",", " ").split()
        print(hex(state.current_address), code)
        f = method_to_call = getattr(functions, args[0].strip("-"))
        f(state, *args[1:])
        print(state.registers)
        print(state.ram)
        print("")

if __name__ == "__main__":
    execute(prn=1.0)

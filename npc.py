- calc target
- walk
- walk pass target, calc wait
- wait
- wait hits 0


def NPC_wait_time(prn1, prn2):
    """
    simulates logic starting at 800e0bc0
    """
    rand = (prn1 + prn2) - 1.0 # constant from rtoc
    return (3.0 * rand) + 5.0 # constants from r31


def wait()

    -= 0x3fa1111120000000

    >= 0
        return
    else:
        == 0
"""
class for simulating NPC interactions with pRNG
"""
from numpy import float32
from conversion import *
from execution import PPC_executor

CALCING_DEST = 0
WALKING = 1
WAITING = 2


class NPC():
    
    def __init__(self, lcg, startingX, startingY):
        self.lcg = lcg
        self.wait_time = 0
        self.currentX = startingX
        self.currentY = startingY
        self.state = CALCING_DEST

    def step(self):
        """
        simulates one frame of NPC action
        """
        if self.state == CALCING_DEST:
            self.set_walk_dest()
            self.state = WALKING

        elif self.state == WALKING:
            self.walk()
            if passed_dest:
                self.set_wait_time()
                self.state = WAITING

        elif self.state == WAITING:
            if self.wait_time == 0:
                self.state == CALCING_DEST
            else:
                self.wait()

    def set_wait_time(self):
        """
        simulates logic for selecting a pseudo random wait time
        """
        prn1 = self.lcg.generate()
        prn2 = self.lcg.generate()
        rand = (prn1 + prn2) - 1.0 # constant from rtoc
        self.wait_time = (3.0 * rand) + 5.0 # constants from r31

    def wait(self):
        """
        simulates waiting one frame
        """
        val = single_to_double(self.wait_time)
        val -= 0.03333333507180214 # 0x3fa1111120000000
        if val >= 0:
            self.wait_time = double_to_single(val)
        else:
            self.wait_time == float32(0.0)

    def set_walk_dest(self):
        """
        executes logic for selecting a pseudo random walk destination
        """
        prn = self.lcg.generate()
        starting_ram = {
            0x809e5458: "41700000",
            0x80270184: "400921fb",
            0x80270188: "4012d97c",
            0x8027018c: "401921fb",
            # TODO: get the real pair constants
            0x809e544c: "4080000000000000",
            0x809e5454: "41c0000000000000",
            0x8048e610: "00000000",
            0x8048e618: "00000000",
        }
        starting_registers = {
            "sp": 0x8048e5e0,
            "r30": 0x809e53d8,
            "r31": 0x809e53d8,
            "f1": prn,
        }
        exc = PPC_executor(0x80184e64, starting_ram, starting_registers)
        exc.execute()
        self.destX = hex_str_to_single(
            exc.read_hex_str_from_ram(0x809e5434, 4))
        self.destY = hex_str_to_single(
            exc.read_hex_str_from_ram(0x809e543c, 4))

    def walk(self):
        """
        executes logic for walking one frame
        """
        starting_ram = {
            0x8047b1f8: "00000030",
            0x8047b200: "809e5220",
        }
        # are these all constants?
        starting_registers = {
            "sp": 0x8048e6e0,
            "lr": 0x80005fe4,
            "r1": 0x8048e6e0,
            "r13": 0x80480820,
            "r26": 0x00000000,
            "r27": 0x00000000,
            "r28": 0x00000000,
            "r29": 0x00000000,
            "r30": 0x00000006,
            "r31": 0x00000000,
            "f31": hex_str_to_double("ffffffffffffffff")
        }
        exc = PPC_executor(0x80181850, starting_ram, starting_registers)
        self.currentX, self.currentY = exc.execute()

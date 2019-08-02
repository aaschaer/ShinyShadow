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
        self.walk_speed = float32(0.0)
        self.currentX = startingX
        self.currentY = startingY
        self.state = CALCING_DEST
        self.first_step = True

    def step(self):
        """
        simulates one frame of NPC action
        """
        if self.state == CALCING_DEST:
            self.set_walk_dest()
            self.set_walk_speed()
            self.state = WALKING

        elif self.state == WALKING:
            self.walk()
            if (self.currentX == self.destX and
                    self.currentY == self.destY):
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
        print(prn, double_to_hex_str(prn))

        starting_ram = {
            0x809e5458: "41700000",
            0x80270184: "400921fb",
            0x80270188: "4012d97c",
            0x8027018c: "401921fb",
            0x809e544c: "4080000000000000",
            0x809e5454: "41c0000041700000",
            0x8048e610: "00000000",
            0x8048e618: "ffffffff",
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

    def set_walk_speed(self):
        """
        executes logic for selecting a walk speed
        """
        starting_ram = {
            0x80273fc0: "00000000",
            0x80273fc4: "00000000",
            0x80273fc8: "00000000",
            0x809e542d: "00", # non constant?
            0x809e53e0: "8050f0c0",
            0x809e5434: "40327ef700000000",
            0x8050f0d8: "4080000000000000", # current x?
            0x809e543c: "421bcddc00000000",
            0x8050f0e0: "41c0000000000000", # current y?
            0x809e5400: "00000020",
            0x809e5404: "00000000",
            0x809e5430: "3f800000",
            0x8047b1f8: "00000030",
            0x8047b200: "809e5220",
            0x809e5220: "01",
            0x809e5248: "00000000",
            0x809e52fc: "01",
            0x809e5324: "00000000",
            0x809e53d8: "01",
            0x809e53dc: "809e53d8",
            0x809e5224: "809e5220",
            0x809e5300: "809e52fc",
            0x8050f0c0: "10113027", # variable, but unused?
            0x8050f160: "41f00000",
            0x8047aa94: "0000003c",
            0x809e540c: "3e94a529",
            0x8031554c: "00000000",
            0x80315550: "3f800000",
            0x80315550: "00000000",
            0x8047aa80: "e0000000",
            0x809e545c: "00000000",

            0x809e5418: single_to_hex_str(self.walk_speed),

            0xe0000054: "00000001", # ????

        }
        starting_registers = {
            "sp": 0x8048e5e0,
            "r3": 0x809e53d8,
            "r13": 0x80480820,
            "r28": 0x00000001,
            "r29": 0x807f1048,
            "r30": 0x809e53d8,
            "f30": hex_str_to_double("ffffffffffffffff"),
            "p30": hex_str_to_double("4070000000000000"),
            "f31": hex_str_to_double("0000000000000000"),
            "p31": hex_str_to_double("0000000000000000"),
        }
        exc = PPC_executor(0x80184d98, starting_ram, starting_registers)
        exc.execute()
        self.walk_speed = hex_str_to_single(
            exc.read_hex_str_from_ram(0x809e5418, 4))

    def walk(self):
        """
        executes logic for walking one frame
        """
        starting_registers = {
            "lr": 0x80185afc,
            "sp": 0x8048e5b0,
            "r3": 0x00000020,
            "r4": 0x00000000,
            "r13": 0x80480820,
            "r28": 0x00000001, # 0x809e53d8, # unused?
            "r29": 0x807f1048, # 0x00000002, # unused?
            "r30": 0x809e53d8,
            "r31": 0x809e53d8,
            "f1": hex_str_to_double("3ff0000000000000"),
            "f30": hex_str_to_double("ffffffffffffffff"),
            "p30": hex_str_to_double("4070000000000000"),
            "f31": hex_str_to_double("402d6b5aa0000000"), # unused? seems speed related
            "p31": hex_str_to_double("0000000000000000"),
        }
        starting_ram = {
            # constants
            0x8047b1f8: "00000030",
            0x8047b200: "809e5220",
            0x8047aa94: "0000003c",
            0x809e5220: "01",
            0x809e5248: "00000000",
            0x809e52fc: "01",
            0x809e5324: "00000000",
            0x809e53d8: "01",
            0x809e5400: "00000020",
            0x809e5404: "00000000",
            0x809e540c: "3e94a529",
            0x809e53dc: "809e53d8",
            0x809e5224: "809e5220",
            0x809e5300: "809e52fc",
            0x809e53e0: "8050f0c0",
            0x8050f0dc: "00000000",
            0x8050f160: "41f00000",
            0x8031554c: "000000003f800000",
            0x80315554: "0000000000000000",
            0x8047aa80: "e0000000",

            0x809e5418: single_to_hex_str(self.walk_speed),
            0x8050f0d8: single_to_hex_str(self.currentX),
            0x8050f0e0: single_to_hex_str(self.currentY),
            
            # changes between some steps, but doesn't seem to be used
            0x8050f0c0: "10102027",
        }

        # fist step on load changes this out of memory value, very strange
        if self.first_step:
            starting_ram[0xe0000054] = "00000001"
            self.first_step = False
        else:
            starting_ram[0xe0000054] = "00000002"

        exc = PPC_executor(0x80188214, starting_ram, starting_registers)
        exc.execute()

        nextX = hex_str_to_single(
            exc.read_hex_str_from_ram(0x8048e540, 4))
        nextY = hex_str_to_single(
            exc.read_hex_str_from_ram(0x8048e548, 4))

        # check if we would pass the destination and if so set to it
        if ((self.currentX <= self.destX and self.destX <= nextX) or
            (self.currentX >= self.destX and self.destX >= nextX)):
            self.currentX = self.destX
        else:
            self.currentX = nextX

        if ((self.currentY <= self.destY and self.destY <= nextY) or
            (self.currentY >= self.destY and self.destY >= nextY)):
            self.currentY = self.destY
        else:
            self.currentY = nextY
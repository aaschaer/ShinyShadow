// class for simulating NPC interactions with pRNG
#include "npc.h"

enum states { WAITING = 0, WALKING = 1 };

class NPC {
    LCG lcg;
    float destX;
    float destY;
    float currentX;
    float currentY;
    float nextX;
    float nextY;
    float walk_speed;
    float wait_time;
	bool walking;
    bool first_step;
    bool debug;

	NPC(LCG lcg, float startX, float startY) {
		wait_time = 0.0;
		walk_speed = 0.0;
		currentX = startX
		currentY = startY
		self.nextX = None
		self.nextY = None
		self.state = WAITING
		self.first_step = True
		self.debug = False

	}

void NPC::step() {
    // simulates one frame of NPC action
	if (state == WALKING) {
		walk();
		if (nextX == destX and nextY == destY) {
			set_wait_time();
			walking = false;
		}		
	}
    else if (state == WAITING){
            if (nextX) {
                currentX = destX;
                nextX = null;
            }
            if nextY {
                currentY = destY;
                nextY = null;
            }
        }
            
            if wait_time <= 0.0 {
                set_walk_params();
                walk();
                state = WALKING;
            }
}
    

   
        
        


    def set_wait_time(self):
        """
        simulates logic for selecting a pseudo random wait time
        """
        prn1 = self.lcg.generate()
        prn2 = self.lcg.generate()
        rand = (prn1 + prn2) - 1.0 # constant from rtoc
        self.wait_time = float32((3.0 * rand) + 5.0) # constants from r31

    def wait(self):
        """
        simulates waiting one frame
        """
        val = single_to_double(self.wait_time)
        val -= 0.03333333507180214 # 0x3fa1111120000000
        if val >= 0:
            self.wait_time = double_to_single(val)
        else:
            self.wait_time = float32(0.0)

    def set_walk_params(self):
        """
        executes logic for selecting a pseudo random walk destination
        and walk speed
        """
        prn = self.lcg.generate()

        starting_ram = {
            0x809e5458: "41700000",
            0x80270184: "400921fb",
            0x80270188: "4012d97c",
            0x8027018c: "401921fb",
            0x809e544c: "4080000000000000",
            0x809e5454: "41c0000041700000",
            0x8048e610: "00000000",
            0x8048e618: "ffffffff",

            0x809e53e0: "8050f0c0",
            0x8050f0dc: "00000000",
            0x809e5404: "00000000",
            0x809e5400: "00000020",
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
            
            0x8050f0e4: "000000000000000000000000",

            0x80270348: "3f90ad3ae322da11",
            0x80270338: "3fa97b4b24760deb",
            0x80270328: "3fb10d66a0d03d51",
            0x80270340: "bfa2b4442c6a6c2f",
            0x80270330: "bfadde2d52defd9a",
            0x80270318: "3fb745cdc54c206e",
            0x80270320: "bfb3b0f2af749a6d",
            0x80270308: "3fc24924920083ff",
            0x80270310: "bfbc71c6fe231671",
            0x802702f8: "3fd555555555550d",
            0x80270300: "bfc999999998ebc4",
            0x802702f0: "3c9aa62633145c07",
            0x802702d0: "3ff921fb54442d18",
            0x802702d8: "3c7a2b7f222f65e2",
            0x802702b8: "3fddac670561bb4f",
            0x802702e0: "3c81a62633145c07",
            0x802702c0: "3fe921fb54442d18",
            0x802702e8: "3c7007887af0cbbd",
            0x802702c8: "3fef730bd281f69b",

            0x8050f0d8: single_to_hex_str(self.currentX),
            0x8050f0e0: single_to_hex_str(self.currentY),
        }
        starting_registers = {
            "sp": "8048e5e0",
            "r13": "80480820",
            "r30": "809e53d8",
            "r31": "809e53d8",
            "f1": single_to_hex_str(prn),
        }
        exc = PPC_executor(0x80184e64, starting_ram, starting_registers)
        exc.execute()
        self.destX = hex_str_to_single(
            exc.read_hex_str_from_ram(0x809e5434, 4))
        self.destY = hex_str_to_single(
            exc.read_hex_str_from_ram(0x809e543c, 4))
        self.walk_speed = hex_str_to_single(
            exc.read_hex_str_from_ram(0x809e5418, 4))

    def walk(self):
        """
        executes logic for walking one frame
        """
        starting_registers = {
            "lr": "80185afc",
            "sp": "8048e5b0",
            "r3": "00000020",
            "r4": "00000000",
            "r13": "80480820",
            "r28": "00000001", # 0x809e53d8, # unused?
            "r29": "807f1048", # 0x00000002, # unused?
            "r30": "809e53d8",
            "r31": "809e53d8",
            "f1": "3ff0000000000000",
            "f30": "ffffffffffffffff",
            "p30": "4070000000000000",
            "f31": "402d6b5aa0000000", # unused?
            "p31": "0000000000000000",
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
            
            # changes between some steps, but doesn't seem to be used
            0x8050f0c0: "10102027",
        }

        # fist step on load changes this out of memory value, very strange
        if self.first_step:
            starting_ram[0xe0000054] = "00000001"
            self.first_step = False
        else:
            starting_ram[0xe0000054] = "00000002"

        # if this is the first step in the walk, use current vals for calc
        if self.nextX is None:
            starting_ram[0x8050f0d8] = single_to_hex_str(self.currentX)
        else:
            starting_ram[0x8050f0d8] = single_to_hex_str(self.nextX)

        if self.nextY is None:
            starting_ram[0x8050f0e0] = single_to_hex_str(self.currentY)
        else:
            starting_ram[0x8050f0e0] = single_to_hex_str(self.nextY)

        exc = PPC_executor(0x80188214, starting_ram, starting_registers)

        if self.debug:
            exc.breakpoints = [0x800cc2f0]

        exc.execute()

        calcedX = hex_str_to_single(
            exc.read_hex_str_from_ram(0x8048e540, 4))
        calcedY = hex_str_to_single(
            exc.read_hex_str_from_ram(0x8048e548, 4))

        # check if we would pass the destination and if so stop at it
        if (self.nextX and ((self.nextX <= self.destX and self.destX <= calcedX) or
                            (self.nextX >= self.destX and self.destX >= calcedX))):
            print("stopping instead of going to ", single_to_hex_str(calcedX), single_to_hex_str(calcedY))
            calcedX = self.destX

        if (self.nextY and ((self.nextY <= self.destY and self.destY <= calcedY) or
                            (self.nextY >= self.destY and self.destY >= calcedY))):
            calcedY = self.destY

        # move to next vals
        if self.nextX:
            self.currentX = self.nextX
        self.nextX = calcedX
        if self.nextY:
            self.currentY = self.nextY
        self.nextY = calcedY


};
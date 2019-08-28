#include <chrono>
using namespace std::chrono;

#include "lcg.h"
#include "noise.h"
#include "ram.h"
#include "npc.h"


int main() {

	LCG global_lcg = LCG(0x2F0FFF1C);

	PyriteNoise noise = PyriteNoise(global_lcg);

	RAM global_ram = RAM();
	global_ram.write_byte(0x809e5220, 0x01);
	global_ram.write_byte(0x809e52fc, 0x01);
	global_ram.write_byte(0x809e53d8, 0x01);
	global_ram.write_byte(0x809E54B4, 0x01);
	global_ram.write_byte(0x809E5590, 0x01);
	global_ram.write_byte(0x809E566C, 0x01);
	global_ram.write_byte(0x809E5748, 0x01);
	global_ram.write_byte(0x809E5824, 0x01);
	global_ram.write_byte(0x809E5900, 0x01);
	global_ram.write_byte(0x809E59DC, 0x01);
	global_ram.write_byte(0x809E5AB8, 0x01);
	global_ram.write_byte(0x809E5B94, 0x01);
	global_ram.write_byte(0x809E5C70, 0x01);
	global_ram.write_byte(0x809E5D4C, 0x01);

	global_ram.write_int(0x80270184, 0x400921fb);
	global_ram.write_int(0x80270188, 0x4012d97c);
	global_ram.write_int(0x8027018c, 0x401921fb);
	global_ram.write_int(0x8047b1f8, 0x00000030);
	global_ram.write_int(0x8047b200, 0x809e5220);
	global_ram.write_int(0x8048E540, 0x00000000); //semi-volatile
	global_ram.write_int(0x8048E548, 0x00000000); //semi-volatile
	global_ram.write_int(0x8048e610, 0x00000000); //semi-volatile
	global_ram.write_int(0x8047aa80, 0xe0000000);
	global_ram.write_int(0x8047aa94, 0x0000003c);
	global_ram.write_int(0x8047b1f8, 0x00000030);
	global_ram.write_int(0x8047b200, 0x809e5220);
	global_ram.write_int(0x809e5224, 0x809e5220);
	global_ram.write_int(0x809e5248, 0x00000000);
	global_ram.write_int(0x809e5300, 0x809e52fc);
	global_ram.write_int(0x809e5324, 0x00000000);
	global_ram.write_int(0x809e53dc, 0x809e53d8);
	global_ram.write_int(0x809E54B8, 0x809e54b4);
	global_ram.write_int(0x809E5594, 0x809e5590);
	global_ram.write_int(0x809E5670, 0x809e566c);
	global_ram.write_int(0x809E574C, 0x809e5748);
	global_ram.write_int(0x809E5828, 0x809e5824);
	global_ram.write_int(0x809E5904, 0x809e5900);
	global_ram.write_int(0x809E59E0, 0x809e59dc);
	global_ram.write_int(0x809E5ABC, 0x809e5ab8);
	global_ram.write_int(0x809E5B98, 0x809e5b94);
	global_ram.write_int(0x809E5C74, 0x809e5c70);
	global_ram.write_int(0x809e5d50, 0x809e5d4c);

	global_ram.write_long(0x80270348, 0x3f90ad3ae322da11);
	global_ram.write_long(0x80270338, 0x3fa97b4b24760deb);
	global_ram.write_long(0x80270328, 0x3fb10d66a0d03d51);
	global_ram.write_long(0x80270340, 0xbfa2b4442c6a6c2f);
	global_ram.write_long(0x80270330, 0xbfadde2d52defd9a);
	global_ram.write_long(0x80270318, 0x3fb745cdc54c206e);
	global_ram.write_long(0x80270320, 0xbfb3b0f2af749a6d);
	global_ram.write_long(0x80270308, 0x3fc24924920083ff);
	global_ram.write_long(0x80270310, 0xbfbc71c6fe231671);
	global_ram.write_long(0x802702f8, 0x3fd555555555550d);
	global_ram.write_long(0x80270300, 0xbfc999999998ebc4);
	global_ram.write_long(0x802702f0, 0x3c9aa62633145c07);
	global_ram.write_long(0x802702d0, 0x3ff921fb54442d18);
	global_ram.write_long(0x802702d8, 0x3c7a2b7f222f65e2);
	global_ram.write_long(0x802702b8, 0x3fddac670561bb4f);
	global_ram.write_long(0x802702e0, 0x3c81a62633145c07);
	global_ram.write_long(0x802702c0, 0x3fe921fb54442d18);
	global_ram.write_long(0x802702e8, 0x3c7007887af0cbbd);
	global_ram.write_long(0x802702c8, 0x3fef730bd281f69b);
	global_ram.write_long(0x8031554c, 0x000000003f800000);
	global_ram.write_long(0x80315554, 0x0000000000000000);
	global_ram.write_long(0x809E5400, 0x0000002d00000000);
	global_ram.write_long(0x809E54DC, 0x0000002d00000001);
	global_ram.write_long(0x809E55B8, 0x0000002d00000002);
	global_ram.write_long(0x809E5694, 0x0000002d00000003);
	global_ram.write_long(0x809E5770, 0x0000002d00000004);
	global_ram.write_long(0x809E584C, 0x0000002d00000005);
	global_ram.write_long(0x809E5928, 0x0000002d00000006);
	global_ram.write_long(0x809E5A04, 0x0000002d00000007);
	global_ram.write_long(0x809E5AE0, 0x0000002d00000008);
	global_ram.write_long(0x809E5BBC, 0x0000002d00000009);
	global_ram.write_long(0x809E5C98, 0x0000002d0000000a);

	NPC npc0 = NPC(global_lcg, global_ram, 0x809e5d4c, 0x805111d0, 11, 120.0, -580.0);
	NPC npc1 = NPC(global_lcg, global_ram, 0x809e5c70, 0x80511060, 10, 0.0, -228.0);
	NPC npc2 = NPC(global_lcg, global_ram, 0x809e5748, 0x805107c0, 4, -80.0, -87.0);
	NPC npc3 = NPC(global_lcg, global_ram, 0x809e566c, 0x80510650, 3, -150.0, 6.0);
	NPC npc4 = NPC(global_lcg, global_ram, 0x809e5590, 0x805104e0, 2, 4.0, 70.0);
	NPC npc5 = NPC(global_lcg, global_ram, 0x809e54b4, 0x80510370, 1, 60.0, 170.0);

	auto start = high_resolution_clock::now();
	for (int frame = 0; frame < 5; frame++) {

		//cout << "frame " << dec << frame << endl;

		npc0.step();
		npc1.step();
		npc2.step();
		npc3.step();
		npc4.step();
		npc5.step();
		if (frame >= 2) {
			noise.step();
		}

		cout << hex << global_lcg.state << endl;
		npc0.print_state();
		noise.print_state();

		//string enter;
		//getline(cin, enter);
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	cout << duration.count() << endl;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

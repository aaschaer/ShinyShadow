#pragma once

#include "ppc_exc.h"
#include "lcg.h"

using namespace std;

class NPC {
public:
    NPC(LCG lcg, float startX, float startY);
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

    void step();
    void set_wait_time();
    void wait();
    void set_walk_params();
    void walk();
}
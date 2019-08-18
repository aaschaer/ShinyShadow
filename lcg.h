#pragma once

using namespace std;

clas LCG {
public:
    LCG(int seed);
    int state;
    float generate();
}
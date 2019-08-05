//
// Created by jon on 01/08/2019.
//

#include "Rand.h"
#include <cmath>


int randInt(int n1, int n2){
    return  rand() % (n2-n1) +n1;
}

float randFloatClamped(){
    float rndfltcmp = float(randInt(0, 100000) / 100000.0f);
    return rndfltcmp;
}

float randFloat(float n1, float n2){
    return randFloatClamped() * (n2 - n1) + n1;
}


bool randBool() {
    return rand() % 2;
}

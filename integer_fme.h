#ifndef INTEGER_FME_H
#define INTEGER_FME_H

#include "system_types.h"

int gcd(int a, int b);

int lcm(int a, int b);

IntSystem eliminateVariableInt(IntSystem sys, int col);

void generateLoopNest(IntSystem sys);

void solveIntegerFME(IntSystem sys);

#endif

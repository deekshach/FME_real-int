#ifndef REAL_FME_H
#define REAL_FME_H

#include "system_types.h"

bool hasContradiction(System sys);

System eliminateVariable(System sys, int col);

bool solveFME(System sys);

#endif

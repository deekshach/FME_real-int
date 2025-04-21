#ifndef SYSTEM_READER_H
#define SYSTEM_READER_H

#include <string>
#include "system_types.h"

bool readSystem(const std::string& filename, System& sys);

bool readIntSystem(const std::string& filename, IntSystem& sys);

void printSystem(const System& sys);

void printIntSystem(const IntSystem& sys);

#endif

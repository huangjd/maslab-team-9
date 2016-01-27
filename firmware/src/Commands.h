#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <cstdint>
#include <cstdio>
#include "Config.h"

extern bool(*commandsRegister[64])();

#endif // __COMMAND_H__

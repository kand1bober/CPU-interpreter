#ifndef DSL_HEADER
#define DSL_HEADER

#include "instructions.h"

#define CMD_CASE(code, ...) \
case(code): \
{ \
    do_##code(cpu_state, __VA_ARGS__); \
    break; \
}\

#endif
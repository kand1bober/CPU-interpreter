#ifndef INSTRUCTIONS_HEADER
#define INSTRUCTIONS_HEADER

#include "cpu_header.h"

void do_kAdd(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt);
void do_kSub(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt);
void do_kOr(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt);
void do_kBext(CpuState* cpu_state, uint8_t rd, uint8_t rs1, uint8_t rs2);

#endif 
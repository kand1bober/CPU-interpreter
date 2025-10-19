#include "dsl.h"

#ifndef STAGES_HEADER
#define STAGES_HEADER

    
void fetch(CpuState* cpu_state, BufInfo* input, uint32_t* curr_cmd);
void decode_exec(CpuState* cpu_state, Memory* memory, uint32_t curr_cmd);
void advance_pc(CpuState* cpu_state, uint32_t curr_cmd);
void write_to_mem(Memory* memory, Register addr, Register val);
Register read_from_mem(Memory* memory, Register addr);

#endif
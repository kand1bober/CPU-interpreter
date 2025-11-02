#include "dsl.hpp"
#include "cpu_header.hpp"

#ifndef STAGES_HEADER
#define STAGES_HEADER

#define MAX_OPERANDS 4

typedef enum
{   
    //High type
    kHighType = 0,
    kAdd = 24,
    kOr = 16,
    kBext = 15,
    kSub = 25,
    kSyscall = 21,
    kClz = 28,

    //Other
    kSlti = 61,
    kSt = 56,
    kSsat = 63,    
    kLdp = 53,
    kBeq = 22,
    kLd = 62,
    kJ = 48,
    kUsat = 34,
} Opcode;

typedef struct
{
    Opcode opcode;
    uint32_t operands[MAX_OPERANDS];
} DecodedResult;

void fetch(CpuState* cpu_state, BufInfo* input, uint32_t* curr_cmd);
void decode(CpuState* cpu_state, uint32_t curr_cmd, DecodedResult* decoded);
void execute(CpuState* cpu_state, Memory* memory, DecodedResult* instr_arr, int instr_num);
void advance_pc(CpuState* cpu_state, uint32_t curr_cmd);
void write_to_mem(Memory* memory, Register addr, Register val);
Register read_from_mem(Memory* memory, Register addr);

#endif

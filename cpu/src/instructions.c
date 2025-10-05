#include "../include/instructions.h"


void do_kAdd(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt)
{
    Register* regs = cpu_state->gpr_regs;
    regs[rd] = regs[rs] + regs[rt];
}


void do_kSub(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt)
{
    Register* regs = cpu_state->gpr_regs;
    regs[rd] = regs[rs] - regs[rt];
}


void do_kSub(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt)
{
    Register* regs = cpu_state->gpr_regs;
    regs[rd] = regs[rs] & regs[rt];
}


void do_kBext(CpuState* cpu_state, uint8_t rd, uint8_t rs1, uint8_t rs2)
{
    Register* regs = cpu_state->gpr_regs;
    
    uint32_t mask = regs[rs1] & regs[rs2]; //select bits
    uint32_t invert_mask = regs[rs2] ^ (-1); 

    regs[rd] &= invert_mask; //zero certain bits
    regs[rd] |= mask; //copy certan bits from rs1
}


void do_sysacll()
{

}


void do_clz()
{

}


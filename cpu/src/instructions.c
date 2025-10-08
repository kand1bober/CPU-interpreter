#include "../include/instructions.h"

#define EXEC_ERROR   printf("error in execution\n" \
                            " :file: %s\n" \
                            "  func: %s\n" \
                            "  line: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); \
                     exit(1); 

#define EXEC_ASSERT(cond, text)     if (!(cond)) \
                                    { \
                                        EXEC_ERROR \
                                        printf("details: bad %s\n\n", text); \
                                    }

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


void do_kOr(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt)
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


void do_kSyscall(CpuState* cpu_state)
{
    Register* regs = cpu_state->gpr_regs;

    regs[0] = syscall(regs[8], regs[0], regs[7]);
}


void do_kClz(CpuState* cpu_state, uint8_t rd, uint8_t rs)
{
    Register* regs = cpu_state->gpr_regs;

    Register count = 0, to_measure = regs[rs];
    size_t reg_size = sizeof(Register) * 8;
    for (size_t i = 0; i < reg_size; i++)
    {
        if ((to_measure << i) >> (reg_size - 1)) {
            count++;
        }
        else 
            break;
    }

    regs[rd] = count;
}


void do_kSlti(CpuState* cpu_state, uint8_t rs, uint8_t rt, int16_t imm)
{
    Register* regs = cpu_state->gpr_regs;

    regs[rs] = (regs[rs] < (Register)imm);
}


void do_kSt(CpuState* cpu_state, uint8_t base, uint8_t rt, int16_t offset)
{
    EXEC_ASSERT((offset & 3) == 0, "align") //check allignment

    Register* regs = cpu_state->gpr_regs;

    cpu_state->memory->data[regs[base] + (Register)offset] = regs[rt]; 
}


void do_kSsat(CpuState* cpu_state, uint8_t rd, uint8_t rs, int16_t imm5)
{
    Register* regs = cpu_state->gpr_regs;
    Register max_val = (Register)pow(2, imm5), min_val = -(pow(2, imm5) - 1); 

    if (regs[rs] > max_val)
    {
        regs[rd] = max_val;
    }
    else if (regs[rs] < min_val)
    {
        regs[rd] = min_val;
    }
    else 
    {
        regs[rd] = regs[rs];
    }
}


//offset = 10-bit number
void do_kLdp(CpuState* cpu_state, uint8_t base, uint8_t rt1, uint8_t rt2, int16_t offset)
{
    EXEC_ASSERT((offset & 3) == 0, "align") //check allignment

    Register* regs = cpu_state->gpr_regs;
    Register addr = regs[base] + (Register)offset;

    regs[rt1] = cpu_state->memory->data[addr];
    regs[rt2] = cpu_state->memory->data[addr + 4];
}


void do_kBeq(CpuState* cpu_state, uint8_t rs, uint8_t rt, int16_t offset)
{
    Register* regs = cpu_state->gpr_regs;
    Register target = ((Register)(offset) << 2);

    if (regs[rs] == regs[rt])
    {
        cpu_state->pc += target;
    }
    else 
    {
        cpu_state->pc += 4;
    }
}


void do_kLd(CpuState* cpu_state, uint8_t base, uint8_t rt, int16_t offset)
{
    EXEC_ASSERT((offset & 3) == 0, "align") //check allignment

    Register* regs = cpu_state->gpr_regs;

    regs[rt] = cpu_state->memory->data[regs[base] + (Register)offset];
}


void do_kJ(CpuState* cpu_state, int32_t index)
{
    cpu_state->pc = ((Register)index << 2) | ((cpu_state->pc >> 28) << 28);
}


void do_kUsat(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t imm5)
{
    Register* regs = cpu_state->gpr_regs;
    Register max_val = pow(2, imm5); 

    if (regs[rs] > max_val)
    {
        regs[rd] = max_val;
    }
    else 
    {
        regs[rd] = regs[rs];
    }
}

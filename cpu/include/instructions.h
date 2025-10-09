#include <unistd.h>
#include <sys/syscall.h>
#include <math.h>

#include "cpu_header.h"

#ifndef INSTRUCTIONS_HEADER
#define INSTRUCTIONS_HEADER

#define TYPE_MASK 0b0111111 //6 bits == 1
#define TYPE_SHIFT 26 
#define GET_TYPE(cmd) (((cmd) >> TYPE_SHIFT) & TYPE_MASK) 

#define OPERAND_MASK 31 //5 bits == 1 
#define ARG_1_SHIFT 21 
#define ARG_2_SHIFT 16 
#define ARG_3_SHIFT 11
#define GET_ARG_1(cmd) (((cmd) >> ARG_1_SHIFT) & OPERAND_MASK)  
#define GET_ARG_2(cmd) (((cmd) >> ARG_2_SHIFT) & OPERAND_MASK)  
#define GET_ARG_3(cmd) (((cmd) >> ARG_3_SHIFT) & OPERAND_MASK)  

#define GET_LAST_11(cmd) ((cmd) & 2047) //11 bits == 1
#define GET_LAST_16(cmd) ((cmd) & 65535) //16 bits == 1  
#define GET_LAST_26(cmd) ((cmd) & 67108863) //26 bits == 1

//for R-type
#define FUNC_MASK 31 //5 bits == 1
#define GET_FUNC(cmd) ((cmd) & FUNC_MASK)

typedef enum
{   
    //R-type (reg-reg)
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
} Opcodes;

void do_kAdd(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt);
void do_kSub(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt);
void do_kOr(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t rt);
void do_kBext(CpuState* cpu_state, uint8_t rd, uint8_t rs1, uint8_t rs2);
void do_kSyscall(CpuState* cpu_state);
void do_kClz(CpuState* cpu_state, uint8_t rd, uint8_t rs);
void do_kSlti(CpuState* cpu_state, uint8_t rs, uint8_t rt, int16_t imm);
void do_kSt(CpuState* cpu_state, uint8_t base, uint8_t rt, int16_t offset);
void do_kSsat(CpuState* cpu_state, uint8_t rd, uint8_t rs, int16_t imm5);
void do_kLdp(CpuState* cpu_state, uint8_t base, uint8_t rt1, uint8_t rt2, int16_t offset);
void do_kBeq(CpuState* cpu_state, uint8_t rs, uint8_t rt, int16_t offset);
void do_kLd(CpuState* cpu_state, uint8_t base, uint8_t rt, int16_t offset);
void do_kJ(CpuState* cpu_state, int32_t index);
void do_kUsat(CpuState* cpu_state, uint8_t rd, uint8_t rs, uint8_t imm5);

#endif 
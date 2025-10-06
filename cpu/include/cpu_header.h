#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef CPU_HEADER
#define CPU_HEADER

#include "../../common/interface.h"
#include "../../utilities/file_work.h"

typedef int32_t Register;
#define kNumRegs 32
#define CMD_SIZE 4 //bytes

typedef enum 
{
    kGood,
    kInputEnd,
} CpuInfo;

typedef struct
{   
    char* data; 
} Memory;

typedef struct
{   
    CpuInfo status;
    Register gpr_regs[kNumRegs];
    Register pc;
    Memory* memory;
} CpuState;

#define TYPE_MASK 63 //6 bits == 1
#define TYPE_SHIFT 26 
#define GET_TYPE(cmd) (((cmd) >> TYPE_SHIFT) & TYPE_MASK) 

#define OPERAND_MASK 31 //5 bits == 1 
#define ARG_1_SHIFT 21 
#define ARG_2_SHIFT 16 
#define ARG_3_SHIFT 11
#define GET_ARG_1(cmd) (((cmd) >> ARG_1_SHIFT) & OPERAND_MASK)  
#define GET_ARG_2(cmd) (((cmd) >> ARG_2_SHIFT) & OPERAND_MASK)  
#define GET_ARG_3(cmd) (((cmd) >> ARG_3_SHIFT) & OPERAND_MASK)  

#define GET_LAST_10(cmd) ((cmd) & 1023) //10 bits == 1
#define GET_LAST_15(cmd) ((cmd) & 32767) //15 bits == 1  
#define GET_LAST_25(cmd) ((cmd) & 33554431) //25 bits == 1

//for R-type
#define FUNC_MASK 31 //5 bits == 1
#define GET_FUNC(cmd) ((cmd) & FUNC_MASK)

typedef enum 
{
    kRtype = 0,
} CmdTypes;

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
    kSlti,
    kSt,
    kSsat,    
    kLdp,
    kBeq,
    kLd,
    kJ,
    kUsat,
} Opcodes;

void fetch(CpuState* cpu_state, BufInfo* input, uint32_t* curr_cmd);
void decode_exec(CpuState* cpu_state, uint32_t curr_cmd);
void write_to_mem(CpuState* cpu_state, size_t pos);
void advance_pc(CpuState* cpu_state);
void except_handler();

#endif

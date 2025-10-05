#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef CPU_HEADER
#define CPU_HEADER

#include "../../common/interface.h"
#include "../../utilities/file_work.h"

#define kNumRegs 32
typedef uint32_t Register;

#define CMD_SIZE 4 //bytes

typedef enum 
{
    kGood,
    kInputEnd,
} CpuInfo;

typedef struct
{   
    uint32_t* data; 
} Memory;

typedef struct
{   
    CpuInfo status;

    Register gpr_regs[kNumRegs];
    Memory* memory;
} CpuState;


#define TYPE_MASK 63 //6 bits == 1
#define TYPE_SHIFT 26 
#define GET_TYPE(cmd) (((cmd) >> TYPE_SHIFT) & TYPE_MASK) 

#define OPERAND_MASK 31 //5 bits == 1 
#define REG_1_SHIFT 21 
#define REG_2_SHIFT 16 
#define REG_3_SHIFT 11
#define GET_REG_1(cmd) (((cmd) >> REG_1_SHIFT) & OPERAND_MASK)  
#define GET_REG_2(cmd) (((cmd) >> REG_2_SHIFT) & OPERAND_MASK)  
#define GET_REG_3(cmd) (((cmd) >> REG_3_SHIFT) & OPERAND_MASK)  

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

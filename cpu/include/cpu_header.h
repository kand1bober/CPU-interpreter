#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../utilities/file_work.h"

#ifndef CPU_HEADER
#define CPU_HEADER


    // #define DEBUG
#ifdef DEBUG
    #define DEB(expr) expr
#else 
    #define DEB(expr) 
#endif


static const char* kCmdBinFilename = "common/cmd_bin.txt";


typedef enum 
{
    kGood,
    kInputEnd,
} CpuInfo;


typedef struct
{   
    char* data; 
    size_t capacity;
} Memory;


#define kNumRegs 32
#define CMD_SIZE 4 
typedef int32_t Register;


typedef struct
{   
    CpuInfo status;
    Register gpr_regs[kNumRegs];
    Register pc;
    Memory memory;
} CpuState;


void fetch(CpuState* cpu_state, BufInfo* input, uint32_t* curr_cmd);
void decode_exec(CpuState* cpu_state, uint32_t curr_cmd);
void write_to_mem(CpuState* cpu_state, Register addr, Register val);
Register read_from_mem(CpuState* cpu_state, Register addr);


#define CPU_DUMP(cpu_state) \
    printf("Registers:\n"\
           "pc: %-2d\n", cpu_state->pc);\
    for (int i = 0; i < kNumRegs; i += 4) \
    { \
        for (int j = 0; j < 4; j++) \
        { \
            printf("x%-2d: %-5d  ", i + j, cpu_state->gpr_regs[i + j]); \
        } \
        printf("\n"); \
    } \
    printf("\n");

#define CMD_DUMP(curr_cmd)\
    printf("=====cmd=====:\n"\
        "type: %d\n"\
        "func(for R-type): %d\n"\
        "arg1: %d\n"\
        "arg2: %d\n"\
        "arg3: %d\n"\
        "last10: %d\n"\
        "last15: %d\n"\
        "last25: %d\n"\
        "=============\n\n",\
        GET_TYPE(curr_cmd),\
        GET_FUNC(curr_cmd),\
        GET_ARG_1(curr_cmd),\
        GET_ARG_2(curr_cmd),\
        GET_ARG_3(curr_cmd),\
        GET_LAST_11(curr_cmd),\
        GET_LAST_16(curr_cmd),\
        GET_LAST_26(curr_cmd));

#define MEM_DUMP \
    { \
        printf("Memory:\n"); \
        size_t reg_size = sizeof(Register); \
        for (int i = 0; (i * reg_size) < cpu_state->memory.capacity; i++) \
        { \
            printf("%-3d: %-4d\n", i, *(Register*)(cpu_state->memory.data + i * reg_size)); \
            printf("\n"); \
        } \
        printf("\n"); \
    } \

#endif

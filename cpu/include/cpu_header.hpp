#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <vector>

#include "../../utilities/file_work.hpp"
#include "dsl.hpp"

#ifndef CPU_HEADER
#define CPU_HEADER


    // #define DEBUG
#ifdef DEBUG
    #define DEB(expr) expr
#else 
    #define DEB(expr) 
#endif


typedef enum 
{
    kGood,
    kStopped,

    kInputEnd,

    kBaseBlockFound,
    kBaseBlockNotFound,

    kTransBlockFound,
    kTransBlockNotFound,
} CpuInfo;


typedef struct
{   
    char* data; 
    size_t capacity;
} Memory;


#define kNumRegs 32
#define CMD_SIZE 4
typedef uint32_t Register;


typedef struct
{   
    CpuInfo status;
    Register gpr_regs[kNumRegs];
    Register pc;
} CpuState;

#endif

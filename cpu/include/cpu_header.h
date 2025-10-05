#include <stdint.h>
#include <stddef.h>

#ifndef CPU_HEADER
#define COU_HEADER

#define kNumRegs 32
typedef uint32_t Register;

#define CMD_SIZE 4 //bytes

typedef struct
{   
    uint8_t data;
} Memory;

typedef struct
{   
    Register gpr_regs[kNumRegs];
    Memory* memory;
} CpuState;

#endif

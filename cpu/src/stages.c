#include "../include/include.h"


void fetch(CpuState* cpu_state, BufInfo* code, uint32_t* curr_cmd)
{
    if ((cpu_state->pc + CMD_SIZE - 1) >= code->sz) 
        cpu_state->status = kInputEnd;

    memcpy(curr_cmd, code->buf + cpu_state->pc, CMD_SIZE);

    cpu_state->status = kGood; 
}


void decode_exec(CpuState* cpu_state, Memory* memory, uint32_t curr_cmd)
{
    DEB(CMD_DUMP(curr_cmd))
    DEB(CPU_DUMP(cpu_state))    
    DEB(MEM_DUMP)

    switch (TYPE(curr_cmd))
    {
        case kHighType:
        {
            switch(FUNC(curr_cmd))
            {
                CMD_CASE(kAdd, cpu_state, ARG_3(curr_cmd), ARG_1(curr_cmd), ARG_2(curr_cmd))

                CMD_CASE(kSub, cpu_state, ARG_3(curr_cmd), ARG_1(curr_cmd), ARG_2(curr_cmd))

                CMD_CASE(kOr, cpu_state, ARG_3(curr_cmd), ARG_1(curr_cmd), ARG_2(curr_cmd))

                CMD_CASE(kBext, cpu_state, ARG_1(curr_cmd), ARG_2(curr_cmd), ARG_3(curr_cmd))

                CMD_CASE(kSyscall, cpu_state)

                CMD_CASE(kClz, cpu_state, ARG_1(curr_cmd), ARG_2(curr_cmd))

                default:{
                    break;
                } 
            }

            break;
        }

        CMD_CASE(kSlti, cpu_state, ARG_1(curr_cmd), ARG_2(curr_cmd), LAST_16(curr_cmd))

        CMD_CASE(kSt, cpu_state, memory, ARG_1(curr_cmd), ARG_2(curr_cmd), LAST_16(curr_cmd))

        CMD_CASE(kSsat, cpu_state, ARG_1(curr_cmd), ARG_2(curr_cmd), ARG_3(curr_cmd))

        CMD_CASE(kLdp, cpu_state, memory, ARG_1(curr_cmd), ARG_2(curr_cmd), ARG_3(curr_cmd), LAST_11(curr_cmd))

        CMD_CASE(kBeq, cpu_state, ARG_1(curr_cmd), ARG_2(curr_cmd), LAST_16(curr_cmd))

        CMD_CASE(kLd, cpu_state, memory, ARG_1(curr_cmd), ARG_2(curr_cmd), LAST_16(curr_cmd))

        CMD_CASE(kJ, cpu_state, LAST_26(curr_cmd))

        CMD_CASE(kUsat, cpu_state, ARG_1(curr_cmd), ARG_2(curr_cmd), ARG_3(curr_cmd))

        default:{
            break;
        }
    }

    DEB(CMD_DUMP(curr_cmd))
    DEB(CPU_DUMP(cpu_state))    
    DEB(MEM_DUMP)
}


void advance_pc(CpuState* cpu_state, uint32_t curr_cmd)
{
    switch (TYPE(curr_cmd))
    {
        case kHighType:
        {
            switch(FUNC(curr_cmd))
            {
                case kAdd:
                case kSub:
                case kOr:
                case kBext:
                case kClz:                     
                case kSyscall:  
                    cpu_state->pc += CMD_SIZE;
                    break;

                default:{
                    break;
                } 
            }

            break;
        }

        case kSlti:
        case kSt:
        case kSsat:
        case kLdp:
        case kLd:
        case kUsat:
        case kBeq:
            cpu_state->pc += CMD_SIZE;
            break;

        case kJ:
            break;

        default:{
            break;
        }
    }
}


void write_to_mem(Memory* memory, Register addr, Register val)
{
    if (addr < 0)
    {
        printf("Error: Wrong addr to write to memory\n");
        exit(0);
    }

    if (memory->capacity < addr + sizeof(Register))
    {   
        memory->capacity = (addr + sizeof(Register));
        memory->data = (char*)realloc(memory->data, memory->capacity);
        if (memory->data == NULL)
        {
            perror("realloc");
            exit(1);
        }
    }

    memcpy(memory->data + addr, &val, sizeof(Register));
}   


Register read_from_mem(Memory* memory, Register addr)
{
    if (addr < 0)
    {
        printf("Error: Wrong addr to read from\n");
        exit(0);
    }

    if (memory->capacity < addr + sizeof(Register))
    {   
        printf("Error: Trying to address unallocated memory region\n");
        exit(0);
    }

    return *(Register*)(memory->data + addr);
}

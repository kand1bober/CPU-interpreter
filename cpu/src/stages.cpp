#include "../include/include.hpp"


void fetch(CpuState* cpu_state, BufInfo* code, uint32_t* curr_cmd)
{
    if ((cpu_state->pc + CMD_SIZE - 1) >= code->sz) 
        cpu_state->status = kInputEnd;

    memcpy(curr_cmd, code->buf + cpu_state->pc, CMD_SIZE);

    cpu_state->status = kGood; 
}


#define OP(num, arg) decoded->operands[num] = arg;
void decode(CpuState* cpu_state, uint32_t curr_cmd, DecodedResult* decoded)
{
    DEB(CMD_DUMP(curr_cmd))
    DEB(CPU_DUMP(cpu_state))    

    Opcode opcode = (Opcode)TYPE(curr_cmd); 
    decoded->opcode = opcode;

    switch (opcode)
    {
        case kHighType:
        {
            Opcode high_type_opcode = (Opcode)FUNC(curr_cmd);
            decoded->opcode = high_type_opcode;

            switch (high_type_opcode)
            {
                DECODE_CASE(kAdd, OP(0, ARG_3(curr_cmd)) 
                                  OP(1, ARG_1(curr_cmd))
                                  OP(2, ARG_2(curr_cmd)))
                
                DECODE_CASE(kSub, OP(0, ARG_3(curr_cmd)) 
                                  OP(1, ARG_1(curr_cmd))
                                  OP(2, ARG_2(curr_cmd)))
                                
                DECODE_CASE(kOr,  OP(0, ARG_3(curr_cmd)) 
                                  OP(1, ARG_1(curr_cmd))
                                  OP(2, ARG_2(curr_cmd)))

                DECODE_CASE(kBext, OP(0, ARG_1(curr_cmd)) 
                                   OP(1, ARG_2(curr_cmd))
                                   OP(2, ARG_3(curr_cmd)))

                DECODE_CASE(kSyscall, ;)

                DECODE_CASE(kClz, OP(0, ARG_1(curr_cmd)) 
                                  OP(1, ARG_2(curr_cmd)))

                default:{
                    break;
                } 
            }

            break;
        }
        
        DECODE_CASE(kSlti, OP(0, ARG_1(curr_cmd))
                           OP(1, ARG_2(curr_cmd))
                           OP(2, LAST_16(curr_cmd)))

        DECODE_CASE(kSt, OP(0, ARG_1(curr_cmd))
                         OP(1, ARG_2(curr_cmd))
                         OP(2, LAST_16(curr_cmd)))

        DECODE_CASE(kSsat, OP(0, ARG_1(curr_cmd))
                           OP(1, ARG_2(curr_cmd))
                           OP(2, ARG_3(curr_cmd)))

        DECODE_CASE(kLdp, OP(0, ARG_1(curr_cmd))
                          OP(1, ARG_2(curr_cmd))
                          OP(2, ARG_3(curr_cmd))
                          OP(3, LAST_11(curr_cmd)))

        DECODE_CASE(kBeq, OP(0, ARG_1(curr_cmd))
                          OP(1, ARG_2(curr_cmd))
                          OP(2, LAST_16(curr_cmd)))

        DECODE_CASE(kLd, OP(0, ARG_1(curr_cmd))
                         OP(1, ARG_2(curr_cmd))
                         OP(2, LAST_16(curr_cmd)))

        DECODE_CASE(kJ, OP(0, LAST_26(curr_cmd)))

        DECODE_CASE(kUsat, OP(0, ARG_1(curr_cmd))
                           OP(1, ARG_2(curr_cmd))
                           OP(2, ARG_3(curr_cmd)))

        default:{
            break;
        }
    }
}
#undef OP


#define OP(num) instr->operands[num]
void execute(CpuState* cpu_state, Memory* memory, DecodedResult* instr_arr, int instr_num)
{
    DecodedResult* instr;
    for (int i = 0; i < instr_num; i++)
    {
        instr = &(instr_arr[i]);
     
        switch (instr->opcode)
        {
            EXECUTE_CASE(kAdd, cpu_state, OP(0), OP(1), OP(2))

            EXECUTE_CASE(kSub, cpu_state, OP(0), OP(1), OP(2))

            EXECUTE_CASE(kOr, cpu_state, OP(0), OP(1), OP(2))

            EXECUTE_CASE(kBext, cpu_state, OP(0), OP(1), OP(2))

            EXECUTE_CASE(kSyscall, cpu_state)

            EXECUTE_CASE(kClz, cpu_state, OP(0), OP(1))

            EXECUTE_CASE(kSlti, cpu_state, OP(0), OP(1), OP(2))

            EXECUTE_CASE(kSt, cpu_state, memory, OP(0), OP(1), OP(2))

            EXECUTE_CASE(kSsat, cpu_state, OP(0), OP(1), OP(2))

            EXECUTE_CASE(kLdp, cpu_state, memory, OP(0), OP(1), OP(2), OP(3))

            EXECUTE_CASE(kBeq, cpu_state, OP(0), OP(1), OP(2))

            EXECUTE_CASE(kLd, cpu_state, memory, OP(0), OP(1), OP(2))

            EXECUTE_CASE(kJ, cpu_state, OP(0))

            EXECUTE_CASE(kUsat, cpu_state, OP(0), OP(1), OP(2))

           default: {break;}
        }

        DEB(CPU_DUMP(cpu_state))    
        DEB(MEM_DUMP)
    }
}
#undef OP


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

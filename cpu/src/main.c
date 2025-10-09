#include "../include/include.h"

int main()
{
    //read instructions
    BufInfo code;
    extern const char* kCmdBinFilename;
    read_file(kCmdBinFilename, &code);

    //start interpreter
    CpuState cpu_state = {.status = kGood, .pc = 0, .memory.capacity = 0};
    uint32_t curr_cmd = 0;

    //------- TEST -------------
    cpu_state.gpr_regs[1] = 17;
    cpu_state.gpr_regs[2] = 12;
    cpu_state.gpr_regs[3] = 0;
    cpu_state.gpr_regs[4] = 0;

    cpu_state.gpr_regs[5] = 0;
    cpu_state.gpr_regs[6] = 0;
    //--------------------------

    while (1)
    {
        fetch(&cpu_state, &code, &curr_cmd);

        if (cpu_state.status == kInputEnd)
            break;

        decode_exec(&cpu_state, curr_cmd);
    }

    free(code.buf);
    free(cpu_state.memory.data);

    return 0;
}


void fetch(CpuState* cpu_state, BufInfo* code, uint32_t* curr_cmd)
{
    if ((cpu_state->pc) >= (code->sz - CMD_SIZE + 1)) 
    {
        cpu_state->status = kInputEnd;
        return;
    }

    memcpy(curr_cmd, code->buf + cpu_state->pc, CMD_SIZE);
    cpu_state->pc += CMD_SIZE;

    cpu_state->status = kGood; 
}


void decode_exec(CpuState* cpu_state, uint32_t curr_cmd)
{
    DEB(CMD_DUMP(curr_cmd))
    DEB(CPU_DUMP(cpu_state))    
    DEB(MEM_DUMP)

    switch (GET_TYPE(curr_cmd))
    {
        case 0:
        {
            switch(GET_FUNC(curr_cmd))
            {
                CMD_CASE(kAdd, cpu_state, GET_ARG_3(curr_cmd), GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd))

                CMD_CASE(kSub, cpu_state, GET_ARG_3(curr_cmd), GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd))

                CMD_CASE(kOr, cpu_state, GET_ARG_3(curr_cmd), GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd))

                CMD_CASE(kBext, cpu_state, GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd), GET_ARG_3(curr_cmd))

                CMD_CASE(kSyscall, cpu_state)

                CMD_CASE(kClz, cpu_state, GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd))

                default:{
                    break;
                } 
            }

            break;
        }

        CMD_CASE(kSlti, cpu_state, GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd), GET_LAST_15(curr_cmd))

        CMD_CASE(kSt, cpu_state, GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd), GET_LAST_15(curr_cmd))

        CMD_CASE(kSsat, cpu_state, GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd), GET_ARG_2(curr_cmd))

        CMD_CASE(kLdp, cpu_state, GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd), GET_ARG_3(curr_cmd), GET_LAST_10(curr_cmd))

        CMD_CASE(kBeq, cpu_state, GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd), GET_LAST_15(curr_cmd))

        CMD_CASE(kLd, cpu_state, GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd), GET_LAST_15(curr_cmd))

        CMD_CASE(kJ, cpu_state, GET_LAST_25(curr_cmd))

        CMD_CASE(kUsat, cpu_state, GET_ARG_1(curr_cmd), GET_ARG_2(curr_cmd), GET_ARG_3(curr_cmd))

        default:{
            break;
        }
    }

    DEB(CMD_DUMP(curr_cmd))
    DEB(CPU_DUMP(cpu_state))    
    DEB(MEM_DUMP)
}


void write_to_mem(CpuState* cpu_state, Register addr, Register val)
{
    if (addr < 0)
    {
        printf("Wronf addr to write to memory\n");
        exit(0);
    }

    if (cpu_state->memory.capacity < addr + sizeof(Register))
    {   
        cpu_state->memory.capacity = (addr + sizeof(Register));
        printf("HUY: %ld\n", cpu_state->memory.capacity);
        cpu_state->memory.data = (char*)realloc(cpu_state->memory.data, cpu_state->memory.capacity);
        if (cpu_state->memory.data == NULL)
        {
            perror("realloc");
            exit(1);
        }
    }

    *(Register*)(cpu_state->memory.data + addr) = val;
}   

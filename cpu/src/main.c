#include "../include/cpu_header.h"
#include "../include/dsl.h"

int main()
{
    BufInfo code;
    read_file(kCmdBinFilename, &code);

    CpuState cpu_state = {.status = kGood, .pc = 0};
    
    uint32_t curr_cmd = 0;
    while (1)
    {
        fetch(&cpu_state, &code, &curr_cmd);

        decode_exec(&cpu_state, curr_cmd);

        if (cpu_state.status == kInputEnd)
            break;
    }

    free(code.buf);

    return 0;
}


void fetch(CpuState* cpu_state, BufInfo* code, uint32_t* curr_cmd)
{
    memcpy(curr_cmd, code->buf + cpu_state->pc * CMD_SIZE, CMD_SIZE);
    cpu_state->pc += CMD_SIZE;

    if ((cpu_state->pc * CMD_SIZE) >= code->sz)
        cpu_state->status = kInputEnd;
    
    cpu_state->status = kGood; 
}


void decode_exec(CpuState* cpu_state, uint32_t curr_cmd)
{
    switch (GET_TYPE(curr_cmd))
    {
        case kRtype:
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
}


void write_to_mem(CpuState* cpu_state, size_t pos)
{

}


void advance_pc(CpuState* cpu_state)
{

}


void except_handler()
{
    //advance_pc; return;
}


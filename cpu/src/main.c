#include <stdio.h>
#include <string.h>

#include "../include/cpu_header.h"
#include "../../common/interface.h"
#include "../../utilities/file_work.h"

int main()
{
    BufInfo input;
    read_file(CMD_BIN_FILENAME, &input);

    CpuState cpu_state;

    uint32_t curr_cmd = 0;
    while (1)
    {
        fetch(&cpu_state, &input, &curr_cmd);

        decode(&cpu_state, &curr_cmd);

        execute(&cpu_state);

        if (cpu_state.status == kInputEnd)
            break;
    }

    free(input.buf);

    return 0;
}


void fetch(CpuState* cpu_state, BufInfo* input, uint32_t* curr_cmd)
{
    memcpy(curr_cmd, input->pos, CMD_SIZE);
    input->pos += CMD_SIZE;

    if (input->pos >= input->sz)
        cpu_state->status = kInputEnd;
    
    cpu_state->status == kGood; 
}


void decode_exec(CpuState* cpu_state, uint32_t curr_cmd)
{
    switch (GET_TYPE(curr_cmd))
    {
        case kRtype:
        {
            switch(GET_FUNC(curr_cmd))
            {
                CMD_CASE(kAdd, GET_REG_3(curr_cmd), GET_REG_1(curr_cmd), GET_REG_2(curr_cmd))

                CMD_CASE(kSub, GET_REG_3(curr_cmd), GET_REG_1(curr_cmd), GET_REG_2(curr_cmd))

                CMD_CASE(kOr, GET_REG_3(curr_cmd), GET_REG_1(curr_cmd), GET_REG_2(curr_cmd))

                CMD_CASE(kBext, GET_REG_1(curr_cmd), GET_REG_2(curr_cmd), GET_REG_3(curr_cmd))

                // CMD_CASE(kSub, )

                // CMD_CASE(kSub, )

                default:
                {
                    break;
                } 
            }

            break;
        }
        case kSlti: 
        {

            break;
        }
        case kSt: 
        {
            break;
        }
        case kSsat: 
        {
            break;
        }
        case kLdp: 
        {
            break;
        }
        case kBeq: 
        {
            break;
        }
        case kLd: 
        {
            break;
        }
        case kJ: 
        {
            break;
        }
        case kUsat: 
        {
            break;
        }
        default:
        {

            break;
        }
    }
}


void execute(CpuState* cpu_state)
{

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


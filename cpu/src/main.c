#include "../include/include.h"

int main(int argc, char* argv[])
{
    //read instructions
    static const char* kCmdBinDirName = "executables/";
    char filename[50];
    if (argc == 2) {
        snprintf(filename, 50, "%s%s", kCmdBinDirName, argv[1]);
    }
    else {
        printf("Wrong number of argumnets provided\n");
        exit(0);
    }

    BufInfo code;
    read_file(filename, &code);

    //init interpreter
    CpuState cpu_state = {.status = kGood, .pc = 0};
    Memory memory = {.capacity = 0};
    uint32_t curr_cmd = 0;
    for (int i = 0; i < kNumRegs; i++)
    {
        cpu_state.gpr_regs[i] = 0;
    }
    BlockArr block_arr = {.arr_sz = 0, .arr = NULL};

    //start interpreter
    while (1)
    {
        fetch(&cpu_state, &code, &curr_cmd);

        process_cmd(&cpu_state, &memory, curr_cmd, &block_arr);
    }

    //close interpreter
    free(code.buf);
    free(memory.data);
    free_block_arr(&block_arr);
    return 0;
}


void process_cmd(CpuState* cpu_state, Memory* memory, uint32_t curr_cmd, BlockArr* block_arr)
{
    static BaseBlock* curr_block = NULL; //currently builded block
    static BaseBlockState block_state = kNotBuilding;
    DecodedResult decoded;
    // DecodedResult* decoded_ptr = &decoded;

    BaseBlock* to_execute = NULL;
    cpu_state->status = find_block(cpu_state->pc, block_arr, &to_execute); 
    if (cpu_state->status == kBaseBlockFound)
    {
        // printf("execute ready block\n");
        execute(cpu_state, memory, to_execute->instr_arr, to_execute->sz);
        cpu_state->pc = to_execute->pc_end;
        return;
    }   

    decode(cpu_state, curr_cmd, &decoded);

    if (decoded.opcode == kJ || decoded.opcode == kBeq)
    {
        // printf("jump op\n");
        switch (block_state)
        {
            case kBuilding: //fall through
            {
                // printf("    add instr to curr block & end block\n");
                add_instr_to_block(curr_block, &decoded);
                end_block(cpu_state->pc, curr_block);
                block_state = kNotBuilding;
            }
            case kNotBuilding:
            {
                // printf("    execute op\n");
                execute(cpu_state, memory, &decoded, 1);
                advance_pc(cpu_state, curr_cmd);
                break;
            }
            default: 
            {
                printf("Error in building base block");
                exit(1);
            }
        }
    }
    else
    {
        // printf("normal op\n");
        switch (block_state)
        {
            case kNotBuilding: //fall through
            {
                // printf("    start block\n");
                curr_block = start_block(cpu_state->pc, block_arr);                            
                block_state = kBuilding;
            }
            case kBuilding:
            {
                // printf("    add instr to curr block\n");
                add_instr_to_block(curr_block, &decoded);
                // printf("    executed\n");
                execute(cpu_state, memory, &decoded, 1);
                advance_pc(cpu_state, curr_cmd);
                break;
            }   
            default: 
            {
                printf("Error in building base block");
                exit(1);
            }
        }
    }
}  

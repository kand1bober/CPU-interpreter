#include "../include/include.hpp"

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
    CpuState cpu_state = {.status = CpuState::kGood, .pc = 0};
    Memory memory = {.capacity = 0};
    uint32_t curr_cmd = 0;
    for (int i = 0; i < kNumRegs; i++)
    {
        cpu_state.gpr_regs[i] = 0;
    }
    BaseBlockTable base_block_table;
    TransBlockTable trans_block_table;

    //start interpreter
    while (1)
    {
        fetch(&cpu_state, &code, &curr_cmd);

        process_cmd(&cpu_state, &memory, curr_cmd, base_block_table, trans_block_table);
    }

    //close interpreter
    free(code.buf);
    free(memory.data);
    return 0;
}


void process_cmd(CpuState* cpu_state, 
                 Memory* memory, 
                 uint32_t curr_cmd, 
                 BaseBlockTable& base_block_table, 
                 TransBlockTable& trans_block_table)
{
    TransBlock* to_translate = NULL;
    cpu_state->status = trans_block_table.lookup_block(cpu_state->pc, &to_translate);
    if (cpu_state->status == CpuState::kTransBlockFound)
    {
        to_translate->execute();
        return;
    }

    static BaseBlock* curr_block = NULL; //currently builded block
    static BaseBlockState block_state = kNotBuilding;
    DecodedResult decoded;

    BaseBlock* to_execute = NULL;
    cpu_state->status = base_block_table.lookup_block(cpu_state->pc, &to_execute); 
    if (cpu_state->status == CpuState::kBaseBlockFound)
    {
        if (to_execute->freq_ > kTrashHold)
        {
            printf("translate base block\n");
            TransBlock* trans_block = trans_block_table.add_block(to_execute->pc_beg_);
            trans_block->translate(cpu_state, memory, *to_execute);
            trans_block->execute();
        }
        else 
        {
            printf("interpret base block\n");
            execute(cpu_state, memory, to_execute->instr_arr_, to_execute->sz_);
            to_execute->freq_++;
            cpu_state->pc = to_execute->pc_end_;
        }
        return;
    }

    decode(cpu_state, curr_cmd, &decoded);

    if (decoded.opcode == kJ || decoded.opcode == kBeq)
    {
        // printf("    jump op\n");
        switch (block_state)
        {
            case kBuilding: //fall through
            {
                printf("    add instr to curr block\n");
                printf("end block\n");
                curr_block->add_instr(decoded);
                curr_block->end_building(cpu_state->pc);
                block_state = kNotBuilding;
            }
            case kNotBuilding:
            {
                std::vector<DecodedResult> decoded_vector{decoded};
                execute(cpu_state, memory, decoded_vector, 1);
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
        // printf("    normal op\n");
        switch (block_state)
        {
            case kNotBuilding: //fall through
            {
                printf("start block\n");
                curr_block = base_block_table.add_block(cpu_state->pc);                            
                block_state = kBuilding;
            }
            case kBuilding:
            {
                printf("    add instr to curr block\n");
                curr_block->add_instr(decoded);
                std::vector<DecodedResult> decoded_vector{decoded};
                execute(cpu_state, memory, decoded_vector, 1);
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

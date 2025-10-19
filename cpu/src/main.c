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

    while (1)
    {
        fetch(&cpu_state, &code, &curr_cmd);

        decode_exec(&cpu_state, &memory, curr_cmd);

        advance_pc(&cpu_state, curr_cmd);
    }

    free(code.buf);
    free(memory.data);

    return 0;
}

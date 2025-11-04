#include "../include/trans_block.hpp"
#include "../include/translate.hpp"

using namespace asmjit;

#define OP(num) operands[num]
void TransBlock::translate(CpuState* cpu_state, 
                           Memory* memory, 
                           const BaseBlock& base_block) 
{
    TransBlock::pc_beg_ = base_block.pc_beg_;
    TransBlock::pc_end_ = base_block.pc_end_;
    
    CodeHolder code;                            // Holds code and relocation information.
    code.init(TransBlock::rt_.environment(),    // Initialize code to match the JIT environment.
              rt_.cpu_features());
    
    x86::Assembler as(&code);               // Create and attach x86::Assembler to code.
    as.mov(x86::edi, cpu_state->gpr_regs);  // rdi = cpu_state->gpr_regs

    Opcode opcode;
    const uint32_t* operands = NULL;
    for (size_t i = 0; i < base_block.sz_; i++)
    {
        opcode = base_block.instr_arr_[i].opcode;
        operands = base_block.instr_arr_[i].operands;
        switch (opcode)
        {
            TRANSLATE_CASE(kAdd, OP(0), OP(1), OP(2))

            TRANSLATE_CASE(kSub, OP(0), OP(1), OP(2))

            TRANSLATE_CASE(kOr, OP(0), OP(1), OP(2))

            TRANSLATE_CASE(kBext, OP(0), OP(1), OP(2))

            TRANSLATE_CASE(kSyscall, 0)

            TRANSLATE_CASE(kClz, OP(0), OP(1))

            TRANSLATE_CASE(kSlti, OP(0), OP(1), OP(2))

            TRANSLATE_CASE(kSt, memory, OP(0), OP(1), OP(2))

            TRANSLATE_CASE(kSsat, OP(0), OP(1), OP(2))

            TRANSLATE_CASE(kLdp, memory, OP(0), OP(1), OP(2), OP(3))

            TRANSLATE_CASE(kBeq, OP(0), OP(1), OP(2))

            TRANSLATE_CASE(kLd, memory, OP(0), OP(1), OP(2))

            TRANSLATE_CASE(kJ, OP(0))

            TRANSLATE_CASE(kUsat, OP(0), OP(1), OP(2))

           default: {break;}
        }
    }   
    
    Error err = rt_.add(&(TransBlock::fn_), &code); // Add the generated code to the runtime.

    if (err != Error::kOk) {  
        printf("Error in generating code with asmjit\n");
        exit(1);
    }
}
#undef OP

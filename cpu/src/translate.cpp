#include "../include/trans_block.hpp"

using namespace asmjit;

void TransBlock::translate(CpuState* cpu_state, 
                           Memory* memory, 
                           const BaseBlock& base_block) 
{
    TransBlock::pc_beg_ = base_block.pc_beg_;
    TransBlock::pc_end_ = base_block.pc_end_;
    
    CodeHolder code;                            // Holds code and relocation information.
    code.init(TransBlock::rt_.environment(),    // Initialize code to match the JIT environment.
              rt_.cpu_features());
    
    // <-- switch case by Opcode
    // rdi = cpu_state->gpr_regs

    x86::Assembler as(&code);          // Create and attach x86::Assembler to code.
    as.mov(x86::eax, 1);               // Move one to eax register.
    as.ret();                          // Return from function.
    // ===== x86::Assembler is no longer needed from here and can be destroyed =====
    
    Error err = rt_.add(&(TransBlock::fn_), &code); // Add the generated code to the runtime.
    

    if (err != Error::kOk) {  
        printf("Error in generating code with asmjit\n"); // Handle a possible error returned by AsmJit.
        exit(1);
    }
    // ===== CodeHolder is no longer needed from here and can be destroyed =====
    
    // All classes use RAII, all resources will be released before `main()` returns,
    // the generated function can be, however, released explicitly if you intend to
    // reuse or keep the runtime alive, which you should in a production-ready code.
    
    // rt.release(fn); //--> in destructor of TransBlock
 }

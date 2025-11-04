#include <asmjit/x86.h>
#include <stdio.h>

#include "cpu_header.hpp"
#include "base_block.hpp"

#ifndef TRANSLATE_BLOCK_HEADER
#define TRANSLATE_BLOCK_HEADER

#define ASMJIT_EMBED
// #define ASMJIT_STATIC
#define ASMJIT_BUILD_DEBUG
// #define ASMJIT_BUILD_RELEASE
#define ASMJIT_NO_ABI_NAMESPACE
// #define ASMJIT_NO_X86
#define ASMJIT_NO_AARCH64   
#define ASMJIT_NO_SHM_OPEN
#define ASMJIT_NO_JIT
#define ASMJIT_NO_LOGGING
#define ASMJIT_NO_TEXT
#define ASMJIT_NO_INTROSPECTION
#define ASMJIT_NO_FOREIGN
#define ASMJIT_NO_BUILDER
#define ASMJIT_NO_COMPILER
#define ASMJIT_NO_UJIT

// Signature of the generated function.
using Func = int (*)(void);

//Translated base block
struct TransBlock
{
private:
    asmjit::JitRuntime rt_; // Runtime specialized for JIT code execution.

public:
    Register pc_beg_;
    Register pc_end_;
    Func fn_; // Holds address to the generated function.

    void translate(CpuState* cpu_state, 
                   Memory* memory, 
                   const BaseBlock& base_block);

    void execute(CpuState* cpu_state, Memory* memory) { 
        // DEB(CPU_DUMP(cpu_state))    
        // DEB(MEM_DUMP)
        // getchar();

        fn_();
        cpu_state->pc = pc_end_;
    }

    TransBlock() :
        pc_beg_(0),
        pc_end_(0)
        {}

    ~TransBlock()
    {
        rt_.release(fn_);
    }
};

struct TransBlockTable
{
private:
    int sz_;
    std::unordered_map<Register, TransBlock*> hash_;

public:
    TransBlockTable() : sz_(0) {}

    TransBlock* add_block(Register curr_pc)
    {
        TransBlock* new_block = new TransBlock();
        hash_[curr_pc] = new_block;

        return new_block;
    }

    CpuState::CpuInfo lookup_block(Register curr_pc, TransBlock** ans)
    {
        auto hit = hash_.find(curr_pc);
        if (hit == hash_.end())
            return CpuState::kTransBlockNotFound;
        else 
        {
            *ans = hit->second;
            return CpuState::kTransBlockFound;
        }
    }

    ~TransBlockTable(){
        for (auto& pair : hash_){
            pair.second;
            delete pair.second;
        }
    }
};

#endif
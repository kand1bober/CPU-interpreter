#include "../include/translate.hpp"
#include "../include/instructions.hpp"

using namespace asmjit::x86;

#define TRANS_ERROR   printf("error in execution\n" \
                             " :file: %s\n" \
                             "  func: %s\n" \
                             "  line: %d\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

#define TRANS_ASSERT(cond, text)    if (!(cond)) \
                                    { \
                                        TRANS_ERROR \
                                        printf("details: %s\n\n", text); \
                                        exit(0);\
                                    }

#define OFF(text)  sizeof(Register) * text

// Extends imm16 to dword register
#define SIGN_EXTEND(reg, imm) \
    as.mov(reg, imm); \
    as.movsx(e##reg, reg);

void translate_kAdd(Assembler& as, uint8_t rd, uint8_t rs, uint8_t rt)
{
    as.mov(edx, dword_ptr(rdi,OFF(rs)));
    as.add(edx, dword_ptr(rdi, OFF(rt)));
    as.mov(dword_ptr(rdi, OFF(rd)), edx);
    as.ret();
}

void translate_kSub(Assembler& as, uint8_t rd, uint8_t rs, uint8_t rt)
{
    as.mov(edx, dword_ptr(rdi, OFF(rs)));
    as.sub(edx, dword_ptr(rdi, OFF(rt)));
    as.mov(dword_ptr(rdi, OFF(rd)), edx);
    as.ret();
}

void translate_kOr(Assembler& as, uint8_t rd, uint8_t rs, uint8_t rt)
{
    as.mov(edx, dword_ptr(rdi, OFF(rs)));
    as.or_(edx, dword_ptr(rdi, OFF(rt)));
    as.mov(dword_ptr(rdi, OFF(rd)), edx);
    as.ret();
}

void translate_kBext(Assembler& as, uint8_t rd, uint8_t rs1, uint8_t rs2)
{
    as.mov(esi, dword_ptr(rdi, OFF(rs1)));
    as.pext(edx, esi, dword_ptr(rdi, OFF(rs2)));
    as.mov(dword_ptr(rdi, OFF(rd)), edx);
    as.ret();    
}

void translate_kSyscall(Assembler& as, uint8_t plug)
{   
    as.mov(rbx, rdi); //save rdi

    as.mov(eax, dword_ptr(rdi, OFF(8)));
    as.mov(edi, dword_ptr(rdi, OFF(8)));
    as.mov(esi, dword_ptr(rdi, OFF(0)));
    as.mov(edx, dword_ptr(rdi, OFF(1)));
    as.mov(r10d, dword_ptr(rdi, OFF(2)));
    as.mov(r8d, dword_ptr(rdi, OFF(3)));
    as.mov(r9d, dword_ptr(rdi, OFF(4)));
    as.syscall();

    as.mov(rdi, rbx); //return rdi
    as.mov(dword_ptr(rdi, OFF(0)), eax); //take return value
    as.ret();
}

void translate_kClz(Assembler& as, uint8_t rd, uint8_t rs)
{
    as.mov(edx, dword_ptr(edi, OFF(rd)));
    as.lzcnt(edx, dword_ptr(edi, OFF(rs)));
    as.mov(dword_ptr(edi, OFF(rd)), edx);
    as.ret();
}

void translate_kSlti(Assembler& as, uint8_t rs, uint8_t rt, int16_t imm)
{
    as.mov(edx, dword_ptr(rdi, OFF(rt)));
    SIGN_EXTEND(bx, imm)
    as.cmp(edx, ebx);
    as.setl(edx);
    as.mov(dword_ptr(rdi, OFF(rs)), edx);
    as.ret();
}

void translate_kSt(Assembler& as, CpuState* cpu_state, Memory* memory, uint8_t base, uint8_t rt, int16_t offset)
{
    TRANS_ASSERT(((offset & 3) == 0), "bad allignment")
    TRANS_ASSERT(((cpu_state->gpr_regs[base] + sign_extend(offset, 16)) <= memory->capacity), 
                 "attempt to access unallocated memory")

    as.mov(rax, memory->data); //beg of guest memory
    as.add(eax, dword_ptr(rdi, base)); //add base
    SIGN_EXTEND(bx, offset)
    as.add(eax, ebx);

    as.mov(ebx, dword_ptr(edi, OFF(rt)));    
    as.mov(dword_ptr(eax), ebx);    

    as.ret();
}

void translate_kSsat(Assembler& as, uint8_t rd, uint8_t rs, int16_t imm5)
{
    asmjit::Label L1 = as.new_label();
    asmjit::Label L2 = as.new_label();
    asmjit::Label L3 = as.new_label();
    asmjit::Label L4 = as.new_label();

    as.mov(eax, 1);
    as.shl(eax, imm5);
    as.mov(ebx, eax);
    as.sub(eax, 1); //max val
    as.neg(ebx); //min val

    as.mov(ecx, dword_ptr(rdi, OFF(rs)));
    as.cmp(ecx, eax);
    as.jg(L1);
    as.cmp(ecx, ebx);
    as.jl(L2);
    as.jmp(L3);

as.bind(L1); //more than max
    as.mov(edx, eax);
    as.jmp(L4);

as.bind(L2); //less than min 
    as.mov(edx, eax);
    as.jmp(L4);

as.bind(L3); //in range
    as.mov(edx, ecx);
    as.jmp(L4);

as.bind(L4); //end
    as.mov(dword_ptr(rdi, OFF(rd)), edx);
    as.ret();
}

void translate_kLdp(Assembler& as, CpuState* cpu_state, Memory* memory, uint8_t base, uint8_t rt1, uint8_t rt2, int16_t offset)
{
    TRANS_ASSERT(((offset & 3) == 0), "bad allignment")
    TRANS_ASSERT(((cpu_state->gpr_regs[base] + sign_extend(offset, 16)) <= memory->capacity), 
                 "attempt to access unallocated memory")

    as.mov(rax, memory->data); //beg of guest memory
    as.add(eax, dword_ptr(rdi, OFF(base))); //add base
    SIGN_EXTEND(bx, offset)
    as.add(eax, ebx);

    as.mov(ebx, dword_ptr(rax)); 
    as.mov(dword_ptr(rdi, OFF(rt1)), ebx); //ld rt1    
    
    as.mov(ebx, dword_ptr(rax, 4)); 
    as.mov(dword_ptr(rdi, OFF(rt1)), ebx); //ld rt2    

    as.ret();
}

void translate_kBeq(Assembler& as, uint8_t rs, uint8_t rt, int16_t offset)
{

    as.ret();
}

void translate_kLd(Assembler& as, CpuState* cpu_state, Memory* memory, uint8_t base, uint8_t rt, int16_t offset)
{
    TRANS_ASSERT(((offset & 3) == 0), "bad allignment")
    TRANS_ASSERT(((cpu_state->gpr_regs[base] + sign_extend(offset, 16)) <= memory->capacity), 
                 "attempt to access unallocated memory")

    as.mov(rax, memory->data); //beg of guest memory
    as.add(eax, dword_ptr(rdi, OFF(base))); //add base
    SIGN_EXTEND(bx, offset)
    as.add(eax, ebx);

    as.mov(ebx, dword_ptr(rax));

    as.mov(dword_ptr(rdi, OFF(rt)), ebx);    
    as.ret();
}

void translate_kJ(Assembler& as, int32_t index)
{

    as.ret();
}

void translate_kUsat(Assembler& as, uint8_t rd, uint8_t rs, uint8_t imm5)
{
    asmjit::Label L1 = as.new_label();
    asmjit::Label L2 = as.new_label();
    asmjit::Label L3 = as.new_label();

    as.mov(eax, 1);
    as.shl(eax, imm5);
    as.sub(eax, 1); //max val

    as.mov(ecx, dword_ptr(rdi, OFF(rs)));
    as.cmp(ecx, eax);
    as.jg(L1);
    as.jmp(L2);

as.bind(L1); //more than max
    as.mov(edx, eax);
    as.jmp(L3);

as.bind(L2); //in range
    as.mov(edx, ecx);
    as.jmp(L3);

as.bind(L3); //end
    as.mov(dword_ptr(rdi, OFF(rd)), edx);
    as.ret();
}

#undef TRANS_ERROR
#undef TRANS_ASSERT
#undef OFF

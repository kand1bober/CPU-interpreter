#include "../include/translate.hpp"

using namespace asmjit::x86;

void translate_kAdd(Assembler& as, uint8_t rd, uint8_t rs, uint8_t rt)
{
    as.mov(edx, dword_ptr(rdi, rs));
    as.add(edx, dword_ptr(rdi, rt));
    as.mov(dword_ptr(rdi, rd), edx);
    as.ret();
}

void translate_kSub(Assembler& as, uint8_t rd, uint8_t rs, uint8_t rt)
{
    as.mov(edx, dword_ptr(rdi, rs));
    as.sub(edx, dword_ptr(rdi, rt));
    as.mov(dword_ptr(rdi, rd), edx);
    as.ret();
}

void translate_kOr(Assembler& as, uint8_t rd, uint8_t rs, uint8_t rt)
{
    as.mov(edx, dword_ptr(rdi, rs));
    as.or_(edx, dword_ptr(rdi, rt));
    as.mov(dword_ptr(rdi, rd), edx);
    as.ret();
}

void translate_kBext(Assembler& as, uint8_t rd, uint8_t rs1, uint8_t rs2)
{
    
}

void translate_kSyscall(Assembler& as, uint8_t plug)
{
    
}

void translate_kClz(Assembler& as, uint8_t rd, uint8_t rs)
{
    
}

void translate_kSlti(Assembler& as, uint8_t rs, uint8_t rt, int16_t imm)
{
    
}

void translate_kSt(Assembler& as, Memory* memory, uint8_t base, uint8_t rt, int16_t offset)
{
    
}

void translate_kSsat(Assembler& as, uint8_t rd, uint8_t rs, int16_t imm5)
{
    
}

void translate_kLdp(Assembler& as, Memory* memory, uint8_t base, uint8_t rt1, uint8_t rt2, int16_t offset)
{
    
}

void translate_kBeq(Assembler& as, uint8_t rs, uint8_t rt, int16_t offset)
{
    
}

void translate_kLd(Assembler& as, Memory* memory, uint8_t base, uint8_t rt, int16_t offset)
{
    
}

void translate_kJ(Assembler& as, int32_t index)
{
    
}

void translate_kUsat(Assembler& as, uint8_t rd, uint8_t rs, uint8_t imm5)
{
    
}


void trans_sign_extend(Register val, int len)
{
    Register mask = 0;

    if (val & (2 << (len - 1))) //older bit == 1
    {
        mask = ((2 << len) - 1) ^ 1;
        val &= mask;
    }

    return;
}

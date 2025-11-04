#include <asmjit/x86.h>

#include "cpu_header.hpp"

#ifndef TRANSLATE_HEADER 
#define TRANSLATE_HEADER

#define TRANSLATE_CASE(instr, ...) \
case(instr): \
{ \
    translate_##instr(as, __VA_ARGS__); \
    break; \
}

void translate_kAdd(asmjit::x86::Assembler& as, uint8_t rd, uint8_t rs, uint8_t rt);

void translate_kSub(asmjit::x86::Assembler& as, uint8_t rd, uint8_t rs, uint8_t rt);

void translate_kOr(asmjit::x86::Assembler& as, uint8_t rd, uint8_t rs, uint8_t rt);

void translate_kBext(asmjit::x86::Assembler& as, uint8_t rd, uint8_t rs1, uint8_t rs2);

void translate_kSyscall(asmjit::x86::Assembler& as, uint8_t plug);

void translate_kClz(asmjit::x86::Assembler& as, uint8_t rd, uint8_t rs);

void translate_kSlti(asmjit::x86::Assembler& as, uint8_t rs, uint8_t rt, int16_t imm);

void translate_kSt(asmjit::x86::Assembler& as, CpuState* cpu_state, Memory* memory, uint8_t base, uint8_t rt, int16_t offset);

void translate_kSsat(asmjit::x86::Assembler& as, uint8_t rd, uint8_t rs, int16_t imm5);

void translate_kLdp(asmjit::x86::Assembler& as, CpuState* cpu_state, Memory* memory, uint8_t base, uint8_t rt1, uint8_t rt2, int16_t offset);

void translate_kBeq(asmjit::x86::Assembler& as, CpuState* cpu_state, uint8_t rs, uint8_t rt, int16_t offset);

void translate_kLd(asmjit::x86::Assembler& as, CpuState* cpu_state, Memory* memory, uint8_t base, uint8_t rt, int16_t offset);

void translate_kJ(asmjit::x86::Assembler& as, int32_t index);

void translate_kUsat(asmjit::x86::Assembler& as, uint8_t rd, uint8_t rs, uint8_t imm5);

#endif
//    Klyx - an operating system kernel.
//    Copyright (C) 2025 Nikolay Shevelko
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, see <https://www.gnu.org/licenses/>.



#pragma once
#include <klyx/shred.h>

// This microcode not by Intel and does not compatable with Intel's microcode and Intel's instruction encoding!

typedef uint8_t abyte_t;
typedef uint16_t aword_t;
typedef uint32_t adword_t;

#define I386EM_REG(reg) regs->reg
#define I386EM_M8(addr) ((uint8_t *)(addr))
#define I386EM_M16(addr) ((uint16_t *)(addr))
#define I386EM_M32(addr) ((uint32_t *)(addr))

#define I386E_FUNC(name)                                                \
    void i386_emu_int_regs_##name(int_regs_t *regs, i386_operand_lexpr_t arg0, i386_operand_rexpr_t arg1) {MC(I386EM_LESET_IR, I386EM_LEGET_IR, I386EM_REGET_IR, I386EM_READ_REG_BY_ID_IR, I386EM_WRITE_REG_BY_ID_IR)} \
    void i386_emu_task_regs_##name(regs_t *regs, i386_operand_lexpr_t arg0, i386_operand_rexpr_t arg1) {MC(I386EM_LESET_TR, I386EM_LEGET_TR, I386EM_REGET_TR, I386EM_READ_REG_BY_ID_TR, I386EM_WRITE_REG_BY_ID_TR)}

#define I386E_FUNC_DECL(name) void i386_emu_int_regs_##name(int_regs_t *regs, i386_operand_lexpr_t arg0, i386_operand_rexpr_t arg1); \
    void i386_emu_task_regs_##name(regs_t *regs, i386_operand_lexpr_t arg0, i386_operand_rexpr_t arg1);

typedef enum i386_reg {
    I386_REG_EAX,
    I386_REG_EBX,
    I386_REG_ECX,
    I386_REG_EDX,
    I386_REG_ESI,
    I386_REG_EDI,
    I386_REG_ESP,
    I386_REG_EBP,
    I386_REG_EIP,
    
    I386_REG_AX,
    I386_REG_BX,
    I386_REG_CX,
    I386_REG_DX,
    I386_REG_SI,
    I386_REG_DI,
    I386_REG_SP,
    I386_REG_BP,
    I386_REG_IP,
    
    I386_REG_AL,
    I386_REG_BL,
    I386_REG_CL,
    I386_REG_DL,
    
    I386_REG_AH,
    I386_REG_BH,
    I386_REG_CH,
    I386_REG_DH,
    
    I386_REG_EFLAGS,
    
    I386_REG_CS,
    I386_REG_DS,
    I386_REG_SS,
    I386_REG_ES,
    I386_REG_FS,
    I386_REG_GS,
} i386_reg_t;

typedef enum i386_operand_type_lexpr {
    I386_LOPERAND_REG,
    I386_LOPERAND_MEM,
    I386_LOPERAND_SIB_RRN,
    I386_LOPERAND_SIB_DRRN,
    I386_LOPERAND_SIB_DRN,
} i386_operand_type_lexpr_t;

typedef enum i386_operand_type_rexpr {
    I386_ROPERAND_REG,
    I386_ROPERAND_MEM,
    I386_ROPERAND_SIB_RRN,
    I386_ROPERAND_SIB_DRRN,
    I386_ROPERAND_SIB_DRN,
    I386_ROPERAND_IMM,
} i386_operand_type_rexpr_t;

// REG - reg1
// MEM - imm1
// SIB_RRN - reg1+reg2*imm1
// SIB_DRRN - imm1+reg1+reg2*imm2
// SIB_DRN - imm1+reg1*imm2
// IMM - imm1

typedef struct i386_operand_lexpr {
    i386_operand_type_lexpr_t type;
    union {
        i386_reg_t reg1;
        i386_reg_t reg2;
        word_t imm1;
        word_t imm2;
    };
} i386_operand_lexpr_t;

typedef struct i386_operand_rexpr {
    i386_operand_type_rexpr_t type;
    union {
        i386_reg_t reg1;
        i386_reg_t reg2;
        word_t imm1;
        word_t imm2;
    };
} i386_operand_rexpr_t;



adword_t I386EM_READ_REG_BY_ID_IR(int_regs_t *regs, int id);

void I386EM_WRITE_REG_BY_ID_IR(int_regs_t *regs, int id, adword_t value);

adword_t I386EM_READ_REG_BY_ID_TR(regs_t *regs, int id);

void I386EM_WRITE_REG_BY_ID_TR(regs_t *regs, int id, adword_t value);

void I386EM_LESET_IR(int_regs_t *regs, i386_operand_lexpr_t *le, adword_t val);

adword_t I386EM_LEGET_IR(int_regs_t *regs, i386_operand_lexpr_t *le);

adword_t I386EM_REGET_IR(int_regs_t *regs, i386_operand_rexpr_t *re);

void I386EM_LESET_TR(regs_t *regs, i386_operand_lexpr_t *le, adword_t val);

adword_t I386EM_LEGET_TR(regs_t *regs, i386_operand_lexpr_t *le);

adword_t I386EM_REGET_TR(regs_t *regs, i386_operand_rexpr_t *re);

// TODO: CALL_FAR, JMP_NEAR, CALL_NEAR, CLI, STI, DIV, IDIV, MUL, IMUL
// TODO: segment-based memory address

I386E_FUNC_DECL(add);
I386E_FUNC_DECL(sub);
I386E_FUNC_DECL(jmp_far);
I386E_FUNC_DECL(push);
I386E_FUNC_DECL(pop);

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



#include <klyx/i386_emulator.h>


adword_t I386EM_READ_REG_BY_ID_IR(int_regs_t *regs, int id) {
    switch (id) {
    case I386_REG_EAX: return I386EM_REG(eax);
    case I386_REG_EBX: return I386EM_REG(ebx);
    case I386_REG_ECX: return I386EM_REG(ecx);
    case I386_REG_EDX: return I386EM_REG(edx);
    case I386_REG_ESI: return I386EM_REG(esi);
    case I386_REG_EDI: return I386EM_REG(edi);
    case I386_REG_ESP: return I386EM_REG(esp);
    case I386_REG_EBP: return I386EM_REG(ebp);
    case I386_REG_EIP: return I386EM_REG(eip);
    case I386_REG_AX:  return I386EM_REG(eax)&0xFFFF;
    case I386_REG_BX:  return I386EM_REG(ebx)&0xFFFF;
    case I386_REG_CX:  return I386EM_REG(ecx)&0xFFFF;
    case I386_REG_DX:  return I386EM_REG(edx)&0xFFFF;
    case I386_REG_SI:  return I386EM_REG(esi)&0xFFFF;
    case I386_REG_DI:  return I386EM_REG(edi)&0xFFFF;
    case I386_REG_SP:  return I386EM_REG(esp)&0xFFFF;
    case I386_REG_BP:  return I386EM_REG(ebp)&0xFFFF;
    case I386_REG_IP:  return I386EM_REG(eip)&0xFFFF;
    case I386_REG_AL:  return I386EM_REG(eax)&0xFF;
    case I386_REG_BL:  return I386EM_REG(ebx)&0xFF;
    case I386_REG_CL:  return I386EM_REG(ecx)&0xFF;
    case I386_REG_DL:  return I386EM_REG(edx)&0xFF;
    case I386_REG_AH:  return (I386EM_REG(eax)&0xFFFF)>>8;
    case I386_REG_BH:  return (I386EM_REG(ebx)&0xFFFF)>>8;
    case I386_REG_CH:  return (I386EM_REG(ecx)&0xFFFF)>>8;
    case I386_REG_DH:  return (I386EM_REG(edx)&0xFFFF)>>8;
    case I386_REG_EFLAGS: return I386EM_REG(eflags);
    case I386_REG_CS: return I386EM_REG(cs);
    case I386_REG_DS: return I386EM_REG(ds);
    case I386_REG_SS: return I386EM_REG(ss);
    case I386_REG_ES: return I386EM_REG(es);
    case I386_REG_FS: return I386EM_REG(fs);
    case I386_REG_GS: return I386EM_REG(gs);
    }
}

void I386EM_WRITE_REG_BY_ID_IR(int_regs_t *regs, int id, adword_t value) {
    switch (id) {
    case I386_REG_EAX: I386EM_REG(eax) = value; break;
    case I386_REG_EBX: I386EM_REG(ebx) = value; break;
    case I386_REG_ECX: I386EM_REG(ecx) = value; break;
    case I386_REG_EDX: I386EM_REG(edx) = value; break;
    case I386_REG_ESI: I386EM_REG(esi) = value; break;
    case I386_REG_EDI: I386EM_REG(edi) = value; break;
    case I386_REG_ESP: I386EM_REG(esp) = value; break;
    case I386_REG_EBP: I386EM_REG(ebp) = value; break;
    case I386_REG_EIP: I386EM_REG(eip) = value; break;
    case I386_REG_AX:  I386EM_REG(eax) = (I386EM_REG(eax)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_BX:  I386EM_REG(ebx) = (I386EM_REG(ebx)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_CX:  I386EM_REG(ecx) = (I386EM_REG(ecx)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_DX:  I386EM_REG(edx) = (I386EM_REG(edx)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_SI:  I386EM_REG(esi) = (I386EM_REG(esi)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_DI:  I386EM_REG(edi) = (I386EM_REG(edi)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_SP:  I386EM_REG(esp) = (I386EM_REG(esp)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_BP:  I386EM_REG(ebp) = (I386EM_REG(ebp)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_IP:  I386EM_REG(eip) = (I386EM_REG(eip)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_AL:  I386EM_REG(eax) = (I386EM_REG(eax)&0xFFFFFF00)|(value&0x0000FF00); break;
    case I386_REG_BL:  I386EM_REG(ebx) = (I386EM_REG(ebx)&0xFFFFFF00)|(value&0x0000FF00); break;
    case I386_REG_CL:  I386EM_REG(ecx) = (I386EM_REG(ecx)&0xFFFFFF00)|(value&0x0000FF00); break;
    case I386_REG_DL:  I386EM_REG(edx) = (I386EM_REG(edx)&0xFFFFFF00)|(value&0x0000FF00); break;
    case I386_REG_AH:  I386EM_REG(eax) = (I386EM_REG(eax)&0xFFFF00FF)|(value&0x000000FF); break;
    case I386_REG_BH:  I386EM_REG(ebx) = (I386EM_REG(ebx)&0xFFFF00FF)|(value&0x000000FF); break;
    case I386_REG_CH:  I386EM_REG(ecx) = (I386EM_REG(ecx)&0xFFFF00FF)|(value&0x000000FF); break;
    case I386_REG_DH:  I386EM_REG(edx) = (I386EM_REG(edx)&0xFFFF00FF)|(value&0x000000FF); break;
    case I386_REG_EFLAGS: I386EM_REG(eflags) = value; break;
    case I386_REG_CS: I386EM_REG(cs) = value; break;
    case I386_REG_DS: I386EM_REG(ds) = value; break;
    case I386_REG_SS: I386EM_REG(ss) = value; break;
    case I386_REG_ES: I386EM_REG(es) = value; break;
    case I386_REG_FS: I386EM_REG(fs) = value; break;
    case I386_REG_GS: I386EM_REG(gs) = value; break;
    }
}

adword_t I386EM_READ_REG_BY_ID_TR(regs_t *regs, int id) {
    switch (id) {
    case I386_REG_EAX: return I386EM_REG(eax);
    case I386_REG_EBX: return I386EM_REG(ebx);
    case I386_REG_ECX: return I386EM_REG(ecx);
    case I386_REG_EDX: return I386EM_REG(edx);
    case I386_REG_ESI: return I386EM_REG(esi);
    case I386_REG_EDI: return I386EM_REG(edi);
    case I386_REG_ESP: return I386EM_REG(esp);
    case I386_REG_EBP: return I386EM_REG(ebp);
    case I386_REG_EIP: return I386EM_REG(eip);
    case I386_REG_AX:  return I386EM_REG(eax)&0xFFFF;
    case I386_REG_BX:  return I386EM_REG(ebx)&0xFFFF;
    case I386_REG_CX:  return I386EM_REG(ecx)&0xFFFF;
    case I386_REG_DX:  return I386EM_REG(edx)&0xFFFF;
    case I386_REG_SI:  return I386EM_REG(esi)&0xFFFF;
    case I386_REG_DI:  return I386EM_REG(edi)&0xFFFF;
    case I386_REG_SP:  return I386EM_REG(esp)&0xFFFF;
    case I386_REG_BP:  return I386EM_REG(ebp)&0xFFFF;
    case I386_REG_IP:  return I386EM_REG(eip)&0xFFFF;
    case I386_REG_AL:  return I386EM_REG(eax)&0xFF;
    case I386_REG_BL:  return I386EM_REG(ebx)&0xFF;
    case I386_REG_CL:  return I386EM_REG(ecx)&0xFF;
    case I386_REG_DL:  return I386EM_REG(edx)&0xFF;
    case I386_REG_AH:  return (I386EM_REG(eax)&0xFFFF)>>8;
    case I386_REG_BH:  return (I386EM_REG(ebx)&0xFFFF)>>8;
    case I386_REG_CH:  return (I386EM_REG(ecx)&0xFFFF)>>8;
    case I386_REG_DH:  return (I386EM_REG(edx)&0xFFFF)>>8;
    case I386_REG_EFLAGS: return I386EM_REG(eflags);
    case I386_REG_CS: return I386EM_REG(cs);
    case I386_REG_DS: return I386EM_REG(ds);
    case I386_REG_SS: return I386EM_REG(ss);
    case I386_REG_ES: return I386EM_REG(es);
    case I386_REG_FS: return I386EM_REG(fs);
    case I386_REG_GS: return I386EM_REG(gs);
    }
}

void I386EM_WRITE_REG_BY_ID_TR(regs_t *regs, int id, adword_t value) {
    switch (id) {
    case I386_REG_EAX: I386EM_REG(eax) = value; break;
    case I386_REG_EBX: I386EM_REG(ebx) = value; break;
    case I386_REG_ECX: I386EM_REG(ecx) = value; break;
    case I386_REG_EDX: I386EM_REG(edx) = value; break;
    case I386_REG_ESI: I386EM_REG(esi) = value; break;
    case I386_REG_EDI: I386EM_REG(edi) = value; break;
    case I386_REG_ESP: I386EM_REG(esp) = value; break;
    case I386_REG_EBP: I386EM_REG(ebp) = value; break;
    case I386_REG_EIP: I386EM_REG(eip) = value; break;
    case I386_REG_AX:  I386EM_REG(eax) = (I386EM_REG(eax)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_BX:  I386EM_REG(ebx) = (I386EM_REG(ebx)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_CX:  I386EM_REG(ecx) = (I386EM_REG(ecx)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_DX:  I386EM_REG(edx) = (I386EM_REG(edx)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_SI:  I386EM_REG(esi) = (I386EM_REG(esi)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_DI:  I386EM_REG(edi) = (I386EM_REG(edi)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_SP:  I386EM_REG(esp) = (I386EM_REG(esp)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_BP:  I386EM_REG(ebp) = (I386EM_REG(ebp)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_IP:  I386EM_REG(eip) = (I386EM_REG(eip)&0xFFFF0000)|(value&0x0000FFFF); break;
    case I386_REG_AL:  I386EM_REG(eax) = (I386EM_REG(eax)&0xFFFFFF00)|(value&0x0000FF00); break;
    case I386_REG_BL:  I386EM_REG(ebx) = (I386EM_REG(ebx)&0xFFFFFF00)|(value&0x0000FF00); break;
    case I386_REG_CL:  I386EM_REG(ecx) = (I386EM_REG(ecx)&0xFFFFFF00)|(value&0x0000FF00); break;
    case I386_REG_DL:  I386EM_REG(edx) = (I386EM_REG(edx)&0xFFFFFF00)|(value&0x0000FF00); break;
    case I386_REG_AH:  I386EM_REG(eax) = (I386EM_REG(eax)&0xFFFF00FF)|(value&0x000000FF); break;
    case I386_REG_BH:  I386EM_REG(ebx) = (I386EM_REG(ebx)&0xFFFF00FF)|(value&0x000000FF); break;
    case I386_REG_CH:  I386EM_REG(ecx) = (I386EM_REG(ecx)&0xFFFF00FF)|(value&0x000000FF); break;
    case I386_REG_DH:  I386EM_REG(edx) = (I386EM_REG(edx)&0xFFFF00FF)|(value&0x000000FF); break;
    case I386_REG_EFLAGS: I386EM_REG(eflags) = value; break;
    case I386_REG_CS: I386EM_REG(cs) = value; break;
    case I386_REG_DS: I386EM_REG(ds) = value; break;
    case I386_REG_SS: I386EM_REG(ss) = value; break;
    case I386_REG_ES: I386EM_REG(es) = value; break;
    case I386_REG_FS: I386EM_REG(fs) = value; break;
    case I386_REG_GS: I386EM_REG(gs) = value; break;
    }
}

void I386EM_LESET_IR(int_regs_t *regs, i386_operand_lexpr_t *le, adword_t val) {
    switch (le->type) {
    case I386_LOPERAND_REG:
        {I386EM_WRITE_REG_BY_ID_IR(regs, le->reg1, val);} break;
    case I386_LOPERAND_MEM:
        {*I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, le->imm1)) = val;} break;
    case I386_LOPERAND_SIB_RRN:
        {*I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, le->reg1)+
                     I386EM_READ_REG_BY_ID_IR(regs, le->reg1)
                     *le->imm1) = val;} break;
    case I386_LOPERAND_SIB_DRRN:
        {*I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, le->reg1)+
                     I386EM_READ_REG_BY_ID_IR(regs, le->reg1)*le->imm2+
                     le->imm1) = val;} break;
    case I386_LOPERAND_SIB_DRN:
        {*I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, le->reg1)*le->imm2+
                     le->imm1) = val;} break;
    }
}

adword_t I386EM_LEGET_IR(int_regs_t *regs, i386_operand_lexpr_t *le) {
    switch (le->type) {
    case I386_LOPERAND_REG:
        {return I386EM_READ_REG_BY_ID_IR(regs, le->reg1);} break;
    case I386_LOPERAND_MEM:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, le->imm1));} break;
    case I386_LOPERAND_SIB_RRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, le->reg1)+
                            I386EM_READ_REG_BY_ID_IR(regs, le->reg1)
                            *le->imm1);} break;
    case I386_LOPERAND_SIB_DRRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, le->reg1)+
                            I386EM_READ_REG_BY_ID_IR(regs, le->reg1)*le->imm2+
                            le->imm1);} break;
    case I386_LOPERAND_SIB_DRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, le->reg1)*le->imm2+
                            le->imm1);} break;
    }
}

adword_t I386EM_REGET_IR(int_regs_t *regs, i386_operand_rexpr_t *re) {
    switch (re->type) {
    case I386_ROPERAND_REG:
        {return I386EM_READ_REG_BY_ID_IR(regs, re->reg1);} break;
    case I386_ROPERAND_MEM:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, re->imm1));} break;
    case I386_ROPERAND_SIB_RRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, re->reg1)+
                            I386EM_READ_REG_BY_ID_IR(regs, re->reg1)
                            *re->imm1);} break;
    case I386_ROPERAND_SIB_DRRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, re->reg1)+
                            I386EM_READ_REG_BY_ID_IR(regs, re->reg1)*re->imm2+
                            re->imm1);} break;
    case I386_ROPERAND_SIB_DRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_IR(regs, re->reg1)*re->imm2+
                            re->imm1);} break;
    case I386_ROPERAND_IMM:
        {return re->imm1;} break;
    }
}

void I386EM_LESET_TR(regs_t *regs, i386_operand_lexpr_t *le, adword_t val) {
    switch (le->type) {
    case I386_LOPERAND_REG:
        {I386EM_WRITE_REG_BY_ID_TR(regs, le->reg1, val);} break;
    case I386_LOPERAND_MEM:
        {*I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, le->imm1)) = val;} break;
    case I386_LOPERAND_SIB_RRN:
        {*I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, le->reg1)+
                     I386EM_READ_REG_BY_ID_TR(regs, le->reg1)
                     *le->imm1) = val;} break;
    case I386_LOPERAND_SIB_DRRN:
        {*I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, le->reg1)+
                     I386EM_READ_REG_BY_ID_TR(regs, le->reg1)*le->imm2+
                     le->imm1) = val;} break;
    case I386_LOPERAND_SIB_DRN:
        {*I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, le->reg1)*le->imm2+
                     le->imm1) = val;} break;
    }
}

adword_t I386EM_LEGET_TR(regs_t *regs, i386_operand_lexpr_t *le) {
    switch (le->type) {
    case I386_LOPERAND_REG:
        {return I386EM_READ_REG_BY_ID_TR(regs, le->reg1);} break;
    case I386_LOPERAND_MEM:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, le->imm1));} break;
    case I386_LOPERAND_SIB_RRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, le->reg1)+
                            I386EM_READ_REG_BY_ID_TR(regs, le->reg1)
                            *le->imm1);} break;
    case I386_LOPERAND_SIB_DRRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, le->reg1)+
                            I386EM_READ_REG_BY_ID_TR(regs, le->reg1)*le->imm2+
                            le->imm1);} break;
    case I386_LOPERAND_SIB_DRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, le->reg1)*le->imm2+
                            le->imm1);} break;
    }
}

adword_t I386EM_REGET_TR(regs_t *regs, i386_operand_rexpr_t *re) {
    switch (re->type) {
    case I386_ROPERAND_REG:
        {return I386EM_READ_REG_BY_ID_TR(regs, re->reg1);} break;
    case I386_ROPERAND_MEM:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, re->imm1));} break;
    case I386_ROPERAND_SIB_RRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, re->reg1)+
                            I386EM_READ_REG_BY_ID_TR(regs, re->reg1)
                            *re->imm1);} break;
    case I386_ROPERAND_SIB_DRRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, re->reg1)+
                            I386EM_READ_REG_BY_ID_TR(regs, re->reg1)*re->imm2+
                            re->imm1);} break;
    case I386_ROPERAND_SIB_DRN:
        {return *I386EM_M32(I386EM_READ_REG_BY_ID_TR(regs, re->reg1)*re->imm2+
                            re->imm1);} break;
    case I386_ROPERAND_IMM:
        {return re->imm1;} break;
    }
}

#undef MC
#define MC(LES, LEG, REG, RR, WR) {LES(regs, &arg0, LEG(regs, &arg0)+REG(regs, &arg1));}

I386E_FUNC(add);

#undef MC
#define MC(LES, LEG, REG, RR, WR) {LES(regs, &arg0, LEG(regs, &arg0)-REG(regs, &arg1));}

I386E_FUNC(sub);

#undef MC
#define MC(LES, LEG, REG, RR, WR) {WR(regs, I386_REG_EIP, REG(regs, &arg1));}

I386E_FUNC(jmp_far);

#undef MC
#define MC(LES, LEG, REG, RR, WR) {WR(regs, I386_REG_ESP, RR(regs, I386_REG_ESP)-4); *I386EM_M32(RR(regs, I386_REG_ESP)) = REG(regs, &arg1);}

I386E_FUNC(push);

#undef MC
#define MC(LES, LEG, REG, RR, WR) {LES(regs, &arg0, *I386EM_M32(RR(regs, I386_REG_ESP))); WR(regs, I386_REG_ESP, RR(regs, I386_REG_ESP)+4);}

I386E_FUNC(pop);

#include <stdio.h>
#include <stdlib.h>

#include "instructions.h"
#include "memory.h"

/*  Instruction splitting (32 bits):
 *
 * ---- --SS SSST TTTT DDDD DHHH HH-- ----
 * ---- --SS SSST TTTT IIII IIII IIII IIII
 * ---- --JJ JJJJ JJJJ JJJJ JJJJ JJJJ JJJJ
 */

#define PART_S(inst) ((inst&0x03e00000) >> 21)
#define PART_T(inst) ((inst&0x001f0000) >> 16)
#define PART_D(inst) ((inst&0x0000f800) >> 11)
#define PART_H(inst)  (inst&0x000007c0)
#define PART_I(inst)  (inst&0x0000ffff)
#define PART_J(inst)  (inst&0x03ffffff)

#define MEM(x) (mem->reg.addr[x])

int identify_instruction(inst_t inst)
{
    if ((inst & 0xfc0007ff) == 0x00000020) return ADD;
    if ((inst & 0xfc000000) == 0x20000000) return ADDI;
    if ((inst & 0xfc000000) == 0x24000000) return ADDIU;
    if ((inst & 0xfc0007ff) == 0x00000021) return ADDU;
    if ((inst & 0xfc0007ff) == 0x00000024) return AND;
    if ((inst & 0xfc000000) == 0x30000000) return ANDI;
    if ((inst & 0xfc000000) == 0x10000000) return BEQ;
    if ((inst & 0xfc1f0000) == 0x04010000) return BGEZ;
    if ((inst & 0xfc1f0000) == 0x04110000) return BGEZAL;
    if ((inst & 0xfc1f0000) == 0x1c000000) return BGTZ;
    if ((inst & 0xfc1f0000) == 0x18000000) return BLEZ;
    if ((inst & 0xfc1f0000) == 0x04000000) return BLTZ;
    if ((inst & 0xfc1f0000) == 0x04100000) return BLTZAL;
    if ((inst & 0xfc000000) == 0x14000000) return BNE;
    if ((inst & 0xfc00ffff) == 0x00000019) return DIV;
    if ((inst & 0xfc00ffff) == 0x0000001b) return DIVU;
    if ((inst & 0xfc000000) == 0x08000000) return J;
    if ((inst & 0xfc000000) == 0x0c000000) return JAL;
    if ((inst & 0xfc1fffff) == 0x00000008) return JR;
    if ((inst & 0xfc000000) == 0x80000000) return LB;
    if ((inst & 0xfc000000) == 0x3c000000) return LUI;
    if ((inst & 0xfc000000) == 0x8c000000) return LW;
    if ((inst & 0xffff07ff) == 0x00000010) return MFHI;
    if ((inst & 0xffff07ff) == 0x00000012) return MFLO;
    if ((inst & 0xfc00ffff) == 0x00000018) return MULT;
    if ((inst & 0xfc00ffff) == 0x00000019) return MULTU;
    if ((inst & 0xffffffff) == 0x00000000) return NOOP;
    if ((inst & 0xfc0007ff) == 0x00000025) return OR;
    if ((inst & 0xfc000000) == 0x34000000) return ORI;
    if ((inst & 0xfc000000) == 0xa0000000) return SB;
    if ((inst & 0xfc00003f) == 0x00000000) return SLL;
    if ((inst & 0xfc00003f) == 0x00000004) return SLLV;
    if ((inst & 0xfc0007ff) == 0x0000002a) return SLT;
    if ((inst & 0xfc000000) == 0x28000000) return SLTI;
    if ((inst & 0xfc000000) == 0x2c000000) return SLTIU;
    if ((inst & 0xfc0007ff) == 0x0000002b) return SLTU;
    if ((inst & 0xfc00003f) == 0x00000003) return SRA;
    if ((inst & 0xfc00003f) == 0x00000002) return SRL;
    if ((inst & 0xfc0007ff) == 0x00000006) return SRLV;
    if ((inst & 0xfc0007ff) == 0x00000022) return SUB;
    if ((inst & 0xfc0007ff) == 0x00000023) return SUBU;
    if ((inst & 0xfc000000) == 0xac000000) return SW;
    if ((inst & 0xfc00003f) == 0x0000000c) return SYSCALL;
    if ((inst & 0xfc00003f) == 0x00000026) return XOR;
    if ((inst & 0xfc000000) == 0x38000000) return XORI;
    return ERR;
}

void add(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);
    MEM(d) = MEM(s) + MEM(t);
}

void addi(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);
    MEM(t) = MEM(s) + i;
}

void addiu(inst_t inst, mem_p mem)
{
    uint32_t s = PART_S(inst);
    uint32_t t = PART_T(inst);
    int i = PART_I(inst);
    MEM(t) = (uint32_t)MEM(s) + i;
}

void addu(inst_t inst, mem_p mem)
{
    uint32_t s = PART_S(inst);
    uint32_t t = PART_T(inst);
    int d = PART_D(inst);
    MEM(d) = (uint32_t)MEM(s) + (uint32_t)MEM(t);
}

void and(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);
    MEM(d) = MEM(s) & MEM(t);
}

void andi(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);
    MEM(t) = MEM(s) & i;
}

void beq(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);

    if (MEM(s) == MEM(t))
        mem->pc += (i-1)*4;
}

void bgez(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int i = PART_I(inst);

    if (MEM(s) >= 0)
        mem->pc += (i-1)*4;
}

void bgezal(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int i = PART_I(inst);

    if (MEM(s) >= 0) {
        mem->reg.name.ra = mem->pc + 8;
        mem->pc += (i-1)*4;
    }
}

void bgtz(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int i = PART_I(inst);

    if (MEM(s) > 0)
        mem->pc += (i-1)*4;
}

void blez(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int i = PART_I(inst);

    if (MEM(s) <= 0)
        mem->pc += (i-1)*4;
}

void bltz(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int i = PART_I(inst);

    if (MEM(s) < 0)
        mem->pc += (i-1)*4;
}

void bltzal(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int i = PART_I(inst);

    if (MEM(s) < 0) {
        mem->reg.name.ra = mem->pc + 8;
        mem->pc += (i-1)*4;
    }
}

void bne(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);

    if (MEM(s) != MEM(t))
        mem->pc += (i-1)*4;
}

void _div(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    mem->lo = MEM(s) / MEM(t);
    mem->hi = MEM(s) % MEM(t);
}

void divu(inst_t inst, mem_p mem)
{
    uint32_t s = PART_S(inst);
    uint32_t t = PART_T(inst);
    mem->lo = (uint32_t)MEM(s) / (uint32_t)MEM(t);
    mem->hi = (uint32_t)MEM(s) % (uint32_t)MEM(t);
}

void j(inst_t inst, mem_p mem)
{
    int i = PART_J(inst);
    mem->pc = ((mem->pc & 0xf0000000) | i*4) - 4;
}

void jal(inst_t inst, mem_p mem)
{
    int i = PART_J(inst);
    mem->reg.name.ra = mem->pc + 8;
    mem->pc = ((mem->pc & 0xf0000000) | i*4) - 4;
}

void jr(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    mem->pc = MEM(s) - 4;
}

void lb(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);
    MEM(t) = MEM(s+i) & 0x000000ff;
}

void lui(inst_t inst, mem_p mem)
{
    int t = PART_T(inst);
    int i = PART_I(inst);
    MEM(t) = i<<16;
}

void lw(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);
    MEM(t) = MEM(s+i);
}

void mfhi(inst_t inst, mem_p mem)
{
    int d = PART_D(inst);
    MEM(d) = mem->hi;
}

void mflo(inst_t inst, mem_p mem)
{
    int d = PART_D(inst);
    MEM(d) = mem->lo;
}

void mult(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    mem->lo = MEM(s) * MEM(t);
}

void multu(inst_t inst, mem_p mem)
{
    uint32_t s = PART_S(inst);
    uint32_t t = PART_T(inst);
    mem->lo = (uint32_t)MEM(s) * (uint32_t)MEM(t);
}

void or(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);
    MEM(d) = MEM(s) | MEM(t);
}

void ori(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);
    MEM(t) = MEM(s) | i;
}

void sb(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);
    MEM(s+i) = (0xff & MEM(t));
}

void sll(inst_t inst, mem_p mem)
{
    int t = PART_T(inst);
    int d = PART_D(inst);
    int h = PART_H(inst);
    MEM(d) = MEM(t) << h;
}

void sllv(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);
    MEM(d) = MEM(t) << MEM(s);
}

void slt(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);

    if (MEM(s) < MEM(t))
        MEM(d) = 1;
    else
        MEM(d) = 0;
}

void slti(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);

    if (MEM(s) < i)
        MEM(t) = 1;
    else
        MEM(t) = 0;
}

void sltiu(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    uint32_t i = PART_I(inst);

    if ((uint32_t)MEM(s) < i)
        MEM(t) = 1;
    else
        MEM(t) = 0;
}

void sltu(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);

    if ((uint32_t)MEM(s) < (uint32_t)MEM(t))
        MEM(d) = 1;
    else
        MEM(d) = 0;
}

void sra(inst_t inst, mem_p mem)
{
    int i;
    int t = PART_T(inst);
    int d = PART_D(inst);
    int h = PART_H(inst);

    MEM(d) = MEM(t) >> h;
    for (i=31 ; i>31-h ; i--)
        MEM(d) |= 0x1 << i;
}

void srl(inst_t inst, mem_p mem)
{
    int t = PART_T(inst);
    int d = PART_D(inst);
    int h = PART_H(inst);
    MEM(d) = MEM(t) >> h;
}

void srlv(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);
    MEM(d) = MEM(t) >> MEM(s);
}

void sub(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);
    MEM(d) = MEM(s) - MEM(t);
}

void subu(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);
    MEM(d) = (uint32_t)MEM(s) - (uint32_t)MEM(t);
}

void sw(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);
    MEM(s+i) = MEM(t);
}

void xor(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int d = PART_D(inst);
    MEM(d) = MEM(s) ^ MEM(t);
}

void xori(inst_t inst, mem_p mem)
{
    int s = PART_S(inst);
    int t = PART_T(inst);
    int i = PART_I(inst);
    MEM(t) = MEM(s) ^ i;
}

void print_str(int addr, mem_p mem)
{
    char c;
    while(c = (MEM(addr++)))
        printf("%c", c);
}

void read_int(mem_p mem)
{
    scanf("%i", mem->reg.name.v);
}

void read_float(mem_p mem)
{
    scanf("%f", (float*)mem->reg.name.v);
}

void read_double(mem_p mem)
{
    scanf("%lf", (double*)mem->reg.name.v);
}

void read_string(int addr, int max_len, mem_p mem)
{
    int i;
    char s[2048];
    scanf("%s", s);

    for (i=0 ; i<max_len ; i++) {
        MEM(addr+i) = s[i];
        if (s[i] == 0)
            break;
    }
}

// TODO
int sbrk(int amount, mem_p mem)
{
    // Not implemented yet because without a running OS, I don't see how one
    // can allocate memory for processes... There aren't even processes !
    return -1;
}

/* Normally thoses would send signals to the OS, but as I don't plan to
 * implement a full MIPS OS, I'll simulate the syscalls.
 */
int syscall(mem_p mem)
{
    switch (mem->reg.name.v[0]) {
        case PRINT_INT:
            printf("%d\n", mem->reg.name.a[0]);
            break;

        case PRINT_FLOAT:
            printf("%f\n", mem->fl.f);
            break;

        case PRINT_DOUBLE:
            printf("%e\n", mem->fl.d);
            break;

        case PRINT_STRING:
            print_str(mem->reg.name.a[0], mem);
            break;

        case READ_INT:
            read_int(mem);
            break;

        case READ_FLOAT:
            read_float(mem);
            break;

        case READ_DOUBLE:
            read_double(mem);
            break;

        case READ_STRING:
            read_string(mem->reg.name.a[0], mem->reg.name.a[1], mem);
            break;

        case SBRK:
            mem->reg.name.v[0] = sbrk(mem->reg.name.a[0], mem);
            break;

        case EXIT:
            return 0;

        default:
            break;
    }
    return 1;
}

int exec_instruction(inst_t inst, mem_p mem)
{
    int continue_running = 1;
    action act = NULL;
    switch (identify_instruction(inst)) {
        case ADD:    act = add;    break;
        case ADDI:   act = addi;   break;
        case ADDIU:  act = addiu;  break;
        case ADDU:   act = addu;   break;
        case AND:    act = and;    break;
        case ANDI:   act = andi;   break;
        case BEQ:    act = beq;    break;
        case BGEZ:   act = bgez;   break;
        case BGEZAL: act = bgezal; break;
        case BGTZ:   act = bgtz;   break;
        case BLEZ:   act = blez;   break;
        case BLTZ:   act = bltz;   break;
        case BLTZAL: act = bltzal; break;
        case BNE:    act = bne;    break;
        case DIV:    act = _div;   break;
        case DIVU:   act = divu;   break;
        case J:      act = j;      break;
        case JAL:    act = jal;    break;
        case JR:     act = jr;     break;
        case LB:     act = lb;     break;
        case LUI:    act = lui;    break;
        case LW:     act = lw;     break;
        case MFHI:   act = mfhi;   break;
        case MFLO:   act = mflo;   break;
        case MULT:   act = mult;   break;
        case MULTU:  act = multu;  break;
        case OR:     act = or;     break;
        case ORI:    act = ori;    break;
        case SB:     act = sb;     break;
        case SLL:    act = sll;    break;
        case SLLV:   act = sllv;   break;
        case SLT:    act = slt;    break;
        case SLTI:   act = slti;   break;
        case SLTIU:  act = sltiu;  break;
        case SLTU:   act = sltu;   break;
        case SRA:    act = sra;    break;
        case SRL:    act = srl;    break;
        case SRLV:   act = srlv;   break;
        case SUB:    act = sub;    break;
        case SUBU:   act = subu;   break;
        case SW:     act = sw;     break;
        case XOR:    act = xor;    break;
        case XORI:   act = xori;   break;

        case SYSCALL:
           continue_running = syscall(mem);
           break;

        case NOOP:
           break;

        case ERR:
        default:
            return 1;
    }
    if (act)
        act(inst, mem);
    mem->pc += 4;
    restore_memory(mem);
    return continue_running;
}

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdint.h>
#include "memory.h"

/* Assembly instructions */
#define   ERR       0  /* Invalid instruction                              */
#define   ADD       1  /* Add (with overflow)                              */
#define   ADDI      2  /* Add immediate (with overflow)                    */
#define   ADDIU     3  /* Add immediate unsigned (no overflow)             */
#define   ADDU      4  /* Add unsigned (no overflow)                       */
#define   AND       5  /* Bitwise and                                      */
#define   ANDI      6  /* Bitwise and immediate                            */
#define   BEQ       7  /* Branch on equal                                  */
#define   BGEZ      8  /* Branch on greater than or equal to zero          */
#define   BGEZAL    9  /* Branch on greater than or equal to zero and link */
#define   BGTZ      10 /* Branch on greater than zero                      */
#define   BLEZ      11 /* Branch on less than or equal to zero             */
#define   BLTZ      12 /* Branch on less than zero                         */
#define   BLTZAL    13 /* Branch on less than zero and link                */
#define   BNE       14 /* Branch on not equal                              */
#define   DIV       15 /* Divide                                           */
#define   DIVU      16 /* Divide unsigned                                  */
#define   J         17 /* Jump                                             */
#define   JAL       18 /* Jump and link                                    */
#define   JR        19 /* Jump register                                    */
#define   LB        20 /* Load byte                                        */
#define   LUI       21 /* Load upper immediate                             */
#define   LW        22 /* Load word                                        */
#define   MFHI      23 /* Move from HI                                     */
#define   MFLO      24 /* Move from LO                                     */
#define   MULT      25 /* Multiply                                         */
#define   MULTU     26 /* Multiply unsigned                                */
#define   NOOP      27 /* No operation                                     */
#define   OR        28 /* Bitwise or                                       */
#define   ORI       29 /* Bitwise or immediate                             */
#define   SB        30 /* Store byte                                       */
#define   SLL       31 /* Shift left logical                               */
#define   SLLV      32 /* Shift left logical variable                      */
#define   SLT       33 /* Set on less than (signed)                        */
#define   SLTI      34 /* Set on less than immediate (signed)              */
#define   SLTIU     35 /* Set on less than immediate (unsigned)            */
#define   SLTU      36 /* Set on less than (unsigned)                      */
#define   SRA       37 /* Shift right arithmetic                           */
#define   SRL       38 /* Shift right logical                              */
#define   SRLV      39 /* Shift right logical variable                     */
#define   SUB       40 /* Substract                                        */
#define   SUBU      41 /* Substract unsigned                               */
#define   SW        42 /* Store word                                       */
#define   SYSCALL   43 /* System call                                      */
#define   XOR       44 /* Bitwise exclusive or                             */
#define   XORI      45 /* Bitwise exclusive or immediate                   */

/* Syscall indentifiers (in $v0) */
#define   PRINT_INT      1  /* $a0  = integer to be printed          */
#define   PRINT_FLOAT    2  /* $fl2 = float to be printed            */
#define   PRINT_DOUBLE   3  /* $fl2 = double to be printed           */
#define   PRINT_STRING   4  /* $a0  = address of string              */
#define   READ_INT       5  /* $v0  = integer returned               */
#define   READ_FLOAT     6  /* $v0  = integer returned               */
#define   READ_DOUBLE    7  /* $v0  = double returned                */
#define   READ_STRING    8  /* $a0  = address of string input buffer */
                            /* $a1  = length of string buffer        */
#define   SBRK           9  /* $a0  = amount                         */
                            /* $v0  = address                        */
#define   EXIT           10

typedef uint32_t inst_t;
typedef void (*action)(inst_t, mem_p);

int exec_instruction(inst_t inst, mem_p mem);

#endif

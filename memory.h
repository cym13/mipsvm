#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define STACKMEM 1024    // *4=4Ko
#define PROGMEM  16384   // *4=64Ko

typedef union {
    struct registers {
        int zero;   /* Value 0                     */
        int at;     /* Assembler temporary         */
        int v[2];   /* Values                      */
        int a[4];   /* Arguments                   */
        int t[8];   /* Temporaries                 */
        int s[8];   /* Saved values                */
        int t2[2];  /* Temporaries                 */
        int k[2];   /* Interrupt/trap handler      */
        int gp;     /* Global pointer              */
        int sp;     /* Stack pointer               */
        int fp;     /* Frame pointer / Saved value */
        int ra;     /* Return address              */
    } name;
    int addr[32];
} registers;

typedef union {
    int    i;
    double d;
    float  f;
} fl_reg;

struct memory {
    int32_t pc;
    float   hi;
    float   lo;
    fl_reg  fl;
    registers reg;
    int32_t data[PROGMEM];
    int32_t stack[STACKMEM];
};

typedef struct memory* mem_p;

mem_p init_memory();
void  restore_memory(mem_p);
#endif

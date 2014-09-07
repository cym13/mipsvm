#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "memory.h"

struct memory* init_memory()
{
    mem_p mem= malloc(sizeof(struct memory));

    mem->pc            = 0;
    mem->reg.name.zero = 0;
    mem->reg.name.gp   = 0;
    mem->reg.name.sp   = 0;
    mem->reg.name.ra   = 0;

    return mem;
}

void restore_memory(mem_p mem) {
    mem->reg.name.zero = 0;
}

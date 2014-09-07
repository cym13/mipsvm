#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "instructions.h"
#include "memory.h"


int main(int argc, char *argv[])
{
    int i, n;
    int retval = 0;
    mem_p mem = NULL;
    FILE src;

    if (argc > 1) {
        for (i=0 ; i<argc ; i++) {
            if ((strcmp(argv[i], "-h")==0) || (strcmp(argv[i], "--help")==0)){
                printf("Usage: miscvm [-h] FILE\n");
                printf("\nArguments:\n");
                printf("\tFILE\tProgram to be launched\n");
                printf("\nOptions:\n");
                printf("\t-h, --help\tPrint this help and exit");

                return 0;
            }

            if (argc > 2) {
                printf("Usage: miscvm [-h] FILE\n");
                return 1;
            }
        }
    }

    else {
        printf("Usage: miscvm [-h] FILE\n");
        return 1;
    }

    mem = init_memory();
    if ((n = fscanf(&src, "%s", (char*)mem->data))) {
        for (i=0 ; i<PROGMEM && i<n ; i++)
            if (exec_instruction((inst_t)mem->data[mem->pc].i, mem) == 0)
                break;
    }
    else {
        printf("Error while reading file");
        retval = 1;
    }

    free(mem);
    return retval;
}

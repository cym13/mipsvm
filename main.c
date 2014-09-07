#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "instructions.h"
#include "memory.h"

global int RUNNING = 1;

int main(int argc, char *argv[])
{
    int i;

    if (argc > 0) {
        for (i=0 ; i<argc ; i++) {
            if ((strcmp(argv[i], "-h")==0) || (strcmp(argv[i], "--help")==0)){
                printf("Usage: miscvm [-h]\n");
                printf("\nOptions:\n");
                printf("\t-h, --help\tPrint this help and exit");

                return 0;
            }
        }
    }

    return 0;
}

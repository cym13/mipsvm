/*
 * LICENSE
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * END_OF_LICENSE
 */

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

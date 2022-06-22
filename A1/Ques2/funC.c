#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include "functions.h"
#include <stdlib.h>
void C(void){
    printf("In C\n");
    //exit(1);
    __asm__("mov $60, %rax\n\t"
            "mov $0, %rdi\n\t"
            "syscall");
}
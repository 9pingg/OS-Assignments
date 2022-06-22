#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include "functions.h"

void B(int64_t);

int main(){
    
    printf("In main\n");
    A();
    printf("In main again\n"); // not printed, which shows that program ends in function C()
    return 0;
}
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include "functions.h"
void A(){
    printf("In A\n");
    int64_t num;
    printf("Enter a Number: ");
    scanf("%ld", &num);
    B(num);
}
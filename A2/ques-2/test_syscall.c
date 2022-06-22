
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#define sys_kernel_2d_memcpy  448
int main(){
	float arr[2][2] = { {1,2},{3,4}};
	int ROWS = 2;
	int COLS = 2;
	float **brr = (float**)malloc(ROWS * sizeof(float *));
	for(int i = 0 ; i < ROWS ; i ++){
		brr[i] = (float*)malloc(COLS * sizeof(float));
	}
	long len_bytes = sizeof(float) * ROWS * COLS;
	long result = syscall(sys_kernel_2d_memcpy, (void *)arr,(void *)brr, len_bytes);
	if(result == -1){
		perror("Error in Syscall");
	}
	for(int i=0;i < 2;i++ ){
		for(int j = 0 ;j<2;j++){
			printf("%.5f\n",brr[i][j]); 
		}
	}
	return 0;
}

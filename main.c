#include <stdio.h>
#include "FatFs/ff.h"

FATFS FatFs;
FIL myFile;

int main(int argc, char* argv[]){
	int ret;
	printf("FatFS Test\n");
	if(f_mount(&FatFs, "", 1) == 0){
		printf("Mount Success\n");
		ret = f_open(&myFile, "Mohanad.txt", FA_READ | FA_WRITE);
		if(ret == 0){
			printf("fopen Success");
		}else{
			printf("fopen failure");
		}
	}else{
		printf("Mount Failure\n");
	}
	
}
#include <stdint.h>
#include <stdio.h>
#include "FatFs/ff.h"

FATFS FatFs;
FIL myFile;
FIL MyFile;

char fxname[200];
char sbuf[200];
int fno;
int packet_counter = 0;

#define BLOCKSIZE 512

#define RND_HALF_BUFFER_SIZE (20*BLOCKSIZE)

int _lastSectorNum = 0;

const int SIZE_1G = 1024*1024;

unsigned int StartNewSigrawFile() {
	uint32_t start_block;
	int retx;
	sprintf(fxname, "sigraw%02d.bin", fno);

	fprintf(stdout,  "StartNewSigrawFile = %s\r\n", fxname);
	//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);

	retx = f_open(&MyFile, fxname, FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
	if (retx == 0) {
		unsigned int bw, ret1, ret2, ret3 = 0;
		ret1 = 0;
		ret1 = f_expand(&MyFile, 2 * SIZE_1G, 1);
		fprintf(stdout,  "expand ret = %d\r\n", ret1);
		//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);
		ret2 = f_write(&MyFile, sbuf, BLOCKSIZE, &bw);
		fprintf(stdout,  "write ret = %d\r\n", ret2);
		//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);
		start_block = _lastSectorNum;
		ret3 = f_close(&MyFile);
		fprintf(stdout,  "close ret = %d\r\n", ret3);
		//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);
		if (ret1 == 0 && ret2 == 0 && ret3 == 0 && bw > 0) {
			fprintf(stdout,  "sector address = %u\r\n", start_block);
			//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);
		} else {
			fprintf(stdout,  "Failed to get sector address bw = %d\r\n", bw);
			//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);
		}
	} else {
		fprintf(stdout,  "sigraw.bin not found or cannot be openned\r\n");
		//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);
		start_block = 0xFFFFFFFF;
	}
	return start_block;
}

void CloseSigrawFile(uint32_t packet_counter) {
	int ret1;
	sprintf(fxname, "sigraw%02d.bin", fno);

	fprintf(stdout,  "CloseSigrawFile = %s\r\n", fxname);
	//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);

	ret1 = f_open(&MyFile, fxname, FA_WRITE);
	if (ret1 == 0) {
		ret1 = f_lseek(&MyFile, packet_counter * RND_HALF_BUFFER_SIZE);
		fprintf(stdout,  "lseek val = %d, ret = %d\r\n", (int)packet_counter, ret1);
		//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);

		ret1 = f_truncate(&MyFile);
		fprintf(stdout,  "truncate ret = %d\r\n", ret1);
		//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);

		ret1 = f_close(&MyFile);
		fprintf(stdout,  "close ret = %d\r\n", ret1);
		//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);
	}else{
		fprintf(stdout,  "%s not found or cannot be opened\r\n", fxname);
		//HAL_UART_Transmit(&huart3, (uint8_t*) sbuf, strlen(sbuf), 0xFFFFF);
	}
}


int main(int argc, char* argv[]){
	int ret;
	printf("FatFS Test\n");
	if(f_mount(&FatFs, "", 1) == 0){
		printf("Mount Success\n");
		
		for(int i = 0; i < 3; i++){
			StartNewSigrawFile();
			packet_counter = 10;
			CloseSigrawFile(packet_counter);
			fno++;
		}
		
	}else{
		printf("Mount Failure\n");
	}
	
}
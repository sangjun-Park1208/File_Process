#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "blkmap.h"

FILE *devicefp;
void ftl_open();
void ftl_write(int lsn, char* sectorbuf);
void ftl_read(int lsn, char* sectorbuf);

int main(){
	char sectorbuf[SECTOR_SIZE], *blockbuf;
	int i, lsn;

	devicefp = fopen("flashmemory", "w+b");	

	if(devicefp == NULL){
		printf("file open error\n");
		exit(1);
	}

	blockbuf = (char *)malloc(BLOCK_SIZE);
	memset(blockbuf, 0xFF, BLOCK_SIZE);

	for(i = 0; i < BLOCKS_PER_DEVICE; i++)
		fwrite(blockbuf, BLOCK_SIZE, 1, devicefp);


	ftl_open();
	ftl_write(172, sectorbuf);
//	ftl_read(65, sectorbuf);
	


	free(blockbuf);
	fclose(devicefp);

	return 0;
}

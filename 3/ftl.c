#define PRINT_FOR_DEBUG

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <time.h>
#include "blkmap.h"

AddrMapTbl addrmaptbl;
extern FILE *devicefp;
int reserved_empty_blk = DATABLKS_PER_DEVICE;

/****************  prototypes ****************/
void ftl_open();
void ftl_write(int lsn, char *sectorbuf);
void ftl_read(int lsn, char *sectorbuf);
void print_block(int pbn);
void print_addrmaptbl();
int dd_read(int ppn, char* pagebuf);
int dd_write(int ppn, char* pagebuf);

void ftl_open(){
	int i;

	for(i = 0; i < DATABLKS_PER_DEVICE; i++)
		addrmaptbl.pbn[i] = DATABLKS_PER_DEVICE - i;


	return;
}

void ftl_write(int lsn, char *sectorbuf){
	int pbn = lsn / PAGES_PER_BLOCK;
	int offset = lsn % PAGES_PER_BLOCK;
	int ppn = addrmaptbl.pbn[pbn] * BLOCK_SIZE + offset;

	char* pagebuf = (char*)malloc(PAGE_SIZE);
	strcpy(pagebuf, sectorbuf);
	
	SpareData* spd;
	spd = (SpareData*)malloc(SPARE_SIZE);
	spd->lsn = lsn;
	memset(spd->dummy, '\0', SPARE_SIZE-4);
	memcpy(pagebuf+SECTOR_SIZE, spd, SPARE_SIZE);

	printf("spd->lsn : %d\n", spd->lsn);
	printf("addrmaptbl.pbn[%d] : %d\n", pbn, addrmaptbl.pbn[pbn]);
	printf("pagebuf : \n%s\n", pagebuf);

	dd_write(ppn, pagebuf);

	char* tmp = (char*)malloc(PAGE_SIZE);
	dd_read(ppn, tmp);
	printf("%s\n", tmp);

#ifdef PRINT_FOR_DEBUG			
//	print_addrmaptbl();
//	print_block(pbn);
#endif
	
	return;
}

void ftl_read(int lsn, char *sectorbuf){
	

#ifdef PRINT_FOR_DEBUG
//	int pbn = lsn / PAGES_PER_BLOCK;
//	print_block(pbn);
//	print_addrmaptbl();
#endif

	return;
}

void print_block(int pbn){
	char *pagebuf;
	SpareData *sdata;
	int i;
	
	pagebuf = (char *)malloc(PAGE_SIZE);
	sdata = (SpareData *)malloc(SPARE_SIZE);

	printf("Physical Block Number: %d\n", pbn);

	for(i = pbn*PAGES_PER_BLOCK; i < (pbn+1)*PAGES_PER_BLOCK; i++){
		dd_read(i, pagebuf);
		memcpy(sdata, pagebuf+SECTOR_SIZE, SPARE_SIZE);
		printf("\t   %5d-[%7d]\n", i, sdata->lsn);
	}

	free(pagebuf);
	free(sdata);

	return;
}

void print_addrmaptbl(void){
	int i;

	printf("Address Mapping Table: \n");
	for(i = 0; i < DATABLKS_PER_DEVICE; i++)
		if(addrmaptbl.pbn[i] >= 0)
			printf("[%d %d]\n", i, addrmaptbl.pbn[i]);
	return;
}

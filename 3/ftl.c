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
int dd_erase(int pbn);

void ftl_open(){
	int i;

	for(i = 0; i < BLOCKS_PER_DEVICE; i++)
		addrmaptbl.pbn[i] = -1;


	return;
}

void ftl_write(int lsn, char *sectorbuf){

	int lbn = lsn / PAGES_PER_BLOCK;
	int offset = lsn % PAGES_PER_BLOCK;
	int pbn = addrmaptbl.pbn[lbn];
	int ppn = pbn * PAGES_PER_BLOCK + offset;

	char* pagebuf = (char*)malloc(PAGE_SIZE);
	strcpy(pagebuf, sectorbuf);

	SpareData* spd;
	spd = (SpareData*)malloc(SPARE_SIZE);
	spd->lsn = lsn;
	memcpy(pagebuf+SECTOR_SIZE, spd, SPARE_SIZE);

	if(pbn == -1){
		pbn = lbn;
		ppn = pbn * PAGES_PER_BLOCK + offset;
		dd_write(ppn, pagebuf);
		addrmaptbl.pbn[lbn] = pbn;
	}
	else{
		char* readbuf = (char*)malloc(PAGE_SIZE);
		dd_read(ppn, readbuf);
		int cmp;
		memcpy(&cmp, readbuf, sizeof(char));
	
		if(cmp == (char)0xFF){ // first write
			dd_write(ppn, pagebuf);
			addrmaptbl.pbn[lbn] = pbn;
		}
		else{ // overwrite
			for(int i=0; i<PAGES_PER_BLOCK; i++){
				if(i == offset){
					continue;
				}
				dd_read(pbn*PAGES_PER_BLOCK+i, readbuf);
				dd_write(reserved_empty_blk*PAGES_PER_BLOCK+i, readbuf);
			}
			memcpy(readbuf, sectorbuf, SECTOR_SIZE);
			memcpy(readbuf+SECTOR_SIZE, spd, SPARE_SIZE);
			dd_write(reserved_empty_blk*PAGES_PER_BLOCK+offset, readbuf);
			addrmaptbl.pbn[lbn] = reserved_empty_blk;
			dd_erase(pbn);
			reserved_empty_blk = pbn;

		}

	}


#ifdef PRINT_FOR_DEBUG			
#endif
	
	return;
}

void ftl_read(int lsn, char *sectorbuf){
	
	int lbn = lsn / PAGES_PER_BLOCK;
	int offset = lsn % PAGES_PER_BLOCK;
	int pbn = addrmaptbl.pbn[lbn];
	int ppn = pbn * PAGES_PER_BLOCK + offset;

	char* pagebuf = (char*)malloc(PAGE_SIZE);
	
	dd_read(ppn, pagebuf);
	memcpy(sectorbuf, pagebuf, SECTOR_SIZE);

	printf("%s\n", sectorbuf);
#ifdef PRINT_FOR_DEBUG

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
	for(i = 0; i < BLOCKS_PER_DEVICE; i++)
		if(addrmaptbl.pbn[i] >= 0)
			printf("[%d %d]\n", i, addrmaptbl.pbn[i]);
	return;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "student.h"

#define FIELDNAME_SIZE 5
#define KEYVAL_SIZE 32
#define FILENAME_SIZE 1024

int readRecord(FILE *fp, STUDENT *s, int rrn);
void unpack(const char *recordbuf, STUDENT *s);
int writeRecord(FILE *fp, const STUDENT *s, int rrn);
void pack(char *recordbuf, const STUDENT *s);
int appendRecord(FILE *fp, char *id, char* name, char *dept, char *addr, char *email);
void searchRecord(FILE *fp, enum FIELD f, char *keyval);
void printRecord(const STUDENT *s);
enum FIELD getFieldID(char *fieldname);

STUDENT* s;
enum FIELD f;
char* filename;
int main(int argc, char *argv[]){
	FILE* fp;
	s = (STUDENT*)malloc(RECORD_SIZE);
	filename = (char*)malloc(FILENAME_SIZE);
	strcpy(filename, argv[2]);
	
	if(!strcmp(argv[1], "-a")){
		if(argc != 8){
			fprintf(stderr, "Usage : %s -a record_file_name \"val1\" \"val2\" \"val3\" \"val4\" \"val5\" \n", argv[0]);
			exit(1);
		}
		if(access(argv[2], F_OK) < 0){
			if((fp = fopen(argv[2], "a+")) == NULL){
				fprintf(stderr, "fopen error in w+ mode\n");
				exit(1);
			}
			int tot_recNum = 0;
			int spare = 0;
			fwrite(&tot_recNum, sizeof(int), 1, fp);
			fwrite(&spare, sizeof(int), 1, fp);
			fclose(fp);
			appendRecord(fp, argv[3], argv[4], argv[5], argv[6], argv[7]);
		}
		else
			appendRecord(fp, argv[3], argv[4], argv[5], argv[6], argv[7]);
		
		
	}

	if(!strcmp(argv[1], "-s")){
		if(argc != 4){
			fprintf(stderr, "Usage : %s -s record_file_name \"field_name=field_value\"\n", argv[0]);
			exit(1);
		}
		char* fieldname = (char*)malloc(FIELDNAME_SIZE);
		char equal;
		char* keyval = (char*)malloc(KEYVAL_SIZE);
		sscanf(argv[3], "%[^=] %c %s", fieldname, &equal, keyval);
		f = getFieldID(fieldname);
		searchRecord(fp, f, keyval);
	}

	exit(0);

}

int appendRecord(FILE *fp, char *id, char* name, char *dept, char *addr, char *email){

	if((fp = fopen(filename, "r")) == NULL){
		fprintf(stderr, "fopen error\n");
		exit(1);
	}
	fseek(fp, 0, SEEK_END);
	int cursor = ftell(fp);
	int rrn = cursor / RECORD_SIZE;
	fclose(fp);

	strcpy(s->id, id);
	strcpy(s->name, name);
	strcpy(s->dept, dept);
	strcpy(s->addr, addr);
	strcpy(s->email, email);
	return writeRecord(fp, s, rrn);

}

void pack(char* recordbuf, const STUDENT* s){
	sprintf(recordbuf, "%s#%s#%s#%s#%s#", s->id, s->name, s->dept, s->addr, s->email);
	return;
}


int writeRecord(FILE *fp, const STUDENT *s, int rrn){
	char recordbuf[RECORD_SIZE];
	int tot_rec;
	pack(recordbuf, s);
	if((fp = fopen(filename, "a")) == NULL){
		fprintf(stderr, "fopen error\n");
		exit(1);
	}
	int readCnt = fwrite(recordbuf, RECORD_SIZE, 1, fp);
	fclose(fp);
	if((fp = fopen(filename, "r+")) == NULL){
		fprintf(stderr, "fopen error\n");
		exit(1);
	}
	fread(&tot_rec, sizeof(int), 1, fp);
	tot_rec++;
	rewind(fp);
	fwrite(&tot_rec, sizeof(int), 1, fp);
	fclose(fp);
	if(readCnt != 1)
		return 0;
	return 1;
}

void searchRecord(FILE *fp, enum FIELD f, char *keyval){
	if((fp = fopen(filename, "r")) == NULL){
		fprintf(stderr, " fopen error\n");
		exit(1);
	}
	int rrn = 0;

	while(1){
		readRecord(fp, s, rrn);
		if(feof(fp))
			break;
		switch(f){
			case ID:
				if(!strcmp(keyval, s->id))
					printRecord(s);
				break;

			case NAME:
				if(!strcmp(keyval, s->name))
					printRecord(s);
				break;

			case DEPT:
				if(!strcmp(keyval, s->dept))
					printRecord(s);
				break;

			case ADDR:
				if(!strcmp(keyval, s->addr))
					printRecord(s);
				break;

			case EMAIL:
				if(!strcmp(keyval, s->email))
					printRecord(s);
				break;
		}
		rrn++;

	}
	fclose(fp);
	return;
}
void unpack(const char *recordbuf, STUDENT *s){
	char p;
	sscanf(recordbuf, "%[^#] %c %[^#] %c %[^#] %c %[^#] %c %[^#] %c", s->id, &p, s->name, &p, s->dept, &p, s->addr, &p, s->email, &p);
	return;
}
int readRecord(FILE* fp, STUDENT* s, int rrn){
	char recordbuf[RECORD_SIZE];
	fseek(fp, 8 + rrn*RECORD_SIZE, SEEK_SET);
	int check = fread(recordbuf, RECORD_SIZE, 1, fp);
	unpack(recordbuf, s);

	if(check != 1)
		return 0;
	return 1;
	
}
enum FIELD getFieldID(char *fieldname){
	if(!strcmp(fieldname, "ID"))
		f = ID;
	else if(!strcmp(fieldname, "NAME"))
		f = NAME;
	else if(!strcmp(fieldname, "DEPT"))
		f = DEPT;
	else if(!strcmp(fieldname, "ADDR"))
		f = ADDR;
	else if(!strcmp(fieldname, "EMAIL"))
		f = EMAIL;
	else
		f= -1;
	return f;
}
void printRecord(const STUDENT *s){
	printf("%s | %s | %s | %s | %s\n", s->id, s->name, s->dept, s->addr, s->email);
}

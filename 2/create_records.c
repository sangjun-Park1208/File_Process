#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

typedef struct Student{
	char name[50]; // 50bytes
	char studentNum[10]; // 10bytes
	char address[140]; // 140bytes
}student; // total 200 bytes

int main(int argc, char* argv[]){
	if(argc != 3){
		fprintf(stderr, "Usage : %s #records <record_file_name>\n", argv[0]);
		exit(1);
	}

	int records;
	char* record_file_name;
	int fd;

	records = atoi(argv[1]);
	strcpy(record_file_name, argv[2]);

	if((fd = open(record_file_name, O_RDWR | O_CREAT | O_TRUNC)) < 0){
		fprintf(stderr, "creat error for %s", record_file_name);
		exit(1);
	}
	
	student* st_list = (student*)malloc(sizeof(student) * records);
	
	for(int i=0; i<records; i++){
		strcpy(st_list[i].name, "sangjun");
		strcpy(st_list[i].studentNum, "20182613");
		strcpy(st_list[i].address, "Seoul-Si Guro-Gu Sindorim-dong");
	}

	for(int i=0; i<records; i++){
		if(write(fd, (char*)&st_list[i], sizeof(student)) < 0){
			fprintf(stderr, "write error\n");
			exit(1);
		}
	}

	close(fd);
	exit(0);		

	
}

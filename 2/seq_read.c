#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>


struct Student{
	char name[50];
	char studentNum[10];
	char address[140];
}student;

int main(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "Usage : %s <fileName>\n", argv[0]);
		exit(1);
	}
	
	struct timeval startTime, endTime;
	gettimeofday(&startTime, NULL);
	char* record_file_name = (char*)malloc(1024);
	int fd;
	int size;
	char* record = (char*)malloc(200);

	strcpy(record_file_name, argv[1]);

	if((fd = open(record_file_name, O_RDONLY)) < 0){
		fprintf(stderr, "open error for %s\n", record_file_name);
		exit(1);
	}

	size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	for(int i=0; i<(size/200); i++){
		if(read(fd, record, 200) < 0){
			fprintf(stderr, "read error\n");
			exit(1);
		}
	}
	gettimeofday(&endTime, NULL);
	printf("%ld usec\n", endTime.tv_usec - startTime.tv_usec);
	exit(0);
}

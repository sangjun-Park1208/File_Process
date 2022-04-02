#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024


int main(int argc, char* argv[]){
	if(argc != 4){
		fprintf(stderr, "Usage : %s [offset] [byteNum] [fileName]\n", argv[0]);
		exit(1);
	}

	int fd;
	int offset = atoi(argv[1]);
	int byteNum = atoi(argv[2]);
	char* fileName = argv[3];
	char buf[BUFFER_SIZE] = {'\0', };

	if((fd = open(fileName, O_RDONLY)) < 0){
		fprintf(stderr, "open error for %s\n", argv[0]);
		exit(1);
	}

	if(byteNum < 0){
		printf("byteNum is negative : %d\n", byteNum);
		if(offset + byteNum < 0){
			lseek(fd, 0, SEEK_SET);
			read(fd, buf, offset);
		}
		else{
			lseek(fd, offset+byteNum-1, SEEK_SET);
			read(fd, buf, 0 - byteNum);		
		}
	}
	else if(byteNum > 0){
		printf("byteNum is non-negative : %d\n", byteNum);
		lseek(fd, offset, SEEK_SET);
		read(fd, buf, byteNum);
	}
	else{
		printf("byteNum is 0\n");

	}
	printf("%s\n", buf);
	close(fd);

	exit(1);

}

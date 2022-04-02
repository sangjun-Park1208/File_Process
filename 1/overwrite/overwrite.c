#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc != 4){
		fprintf(stderr, "Usage : %s [offset] \"data\" [fileName]\n", argv[0]);
		exit(1);
	}

	int fd;
	int offset = atoi(argv[1]);
	char* data = (char*)malloc(strlen(argv[2]));
	sscanf(argv[2], "%s", data);

	char* fileName = (char*)malloc(strlen(argv[3]));
	strcpy(fileName, argv[3]);

	if((fd = open(fileName, O_RDWR)) < 0){
		fprintf(stderr, "open error for %s\n", fileName);
		exit(1);
	}

	lseek(fd, offset, SEEK_SET);
	write(fd, data, strlen(data));

	exit(0);

}

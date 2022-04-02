#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc != 4){
		fprintf(stderr, "Usage : %s [offset] \"data\" [fileName]\n", argv[0]);
		exit(1);
	}

	int offset = atoi(argv[1]);
	char* data = (char*)malloc(strlen(argv[2]));
	strcpy(data, argv[2]);
	char* fileName = (char*)malloc(strlen(argv[3]));
	strcpy(fileName, argv[3]);

	int fd;
	off_t size;

	if((fd = open(fileName, O_RDWR)) < 0){
		fprintf(stderr, "open error for %s\n", argv[0]);
		exit(1);
	}

	size = lseek(fd, 0, SEEK_END);
	char* tmp = (char*)malloc(size - offset);

	if((int)size == offset){
		write(fd, data, strlen(data));
	}
	else{
		lseek(fd, offset, SEEK_SET);
		read(fd, tmp, size - offset);
		lseek(fd, offset, SEEK_SET);
		write(fd, data, strlen(data));
		write(fd, tmp, strlen(tmp));
	}

	exit(1);

}

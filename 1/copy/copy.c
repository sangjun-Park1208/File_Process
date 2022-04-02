#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
	int fd1, fd2;
	int checkByte;
	off_t size, remain;
	char buf[11];

	if(argc != 3){
		fprintf(stderr, "Usage : %s <sourceFile> <targetFile>\n", argv[0]);
		exit(1);
	}

	if((fd1 = open(argv[1], O_RDONLY), 0666) < 0){
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	if((fd2 = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0){
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(1);
	}

	size = lseek(fd1, 0, SEEK_END);
	lseek(fd1, 0, SEEK_SET);
	remain = size % 10;

	while(1){
		if((checkByte = read(fd1, buf, 10)) > 0){
			if(checkByte == remain){
				write(fd2, buf, remain);
				break;
			}
			else{
				write(fd2, buf, 10);
			}
		}
		else{
			fprintf(stderr, "read error\n");
			exit(1);
		}
	}

	close(fd1);
	close(fd2);
	exit(1);
}

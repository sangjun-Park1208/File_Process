#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	int fd1, fd2, fd3;
	off_t size1, size2, size3=0;

	if(argc != 4){
		fprintf(stderr, "Usage : %s [file1] [file2] [file3]\n", argv[0]);
		exit(1);
	}


	if((fd1 = open(argv[1], O_RDONLY)) < 0){
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	if((fd2 = open(argv[2], O_RDONLY)) < 0){
		fprintf(stderr, "open error for %s\n", argv[2]);
		exit(1);
	}

	if((fd3 = open(argv[3], O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0666)) < 0){
		fprintf(stderr, "open error for %s\n", argv[3]);
		exit(1);
	}

	size1 = lseek(fd1, 0, SEEK_END);
	size2 = lseek(fd2, 0, SEEK_END);
	
	char* buf1 = (char*)malloc(size1);
	char* buf2 = (char*)malloc(size2);
	lseek(fd1, 0, SEEK_SET);
	lseek(fd2, 0, SEEK_SET);
	
	if(read(fd1, buf1, size1) < 0){
		fprintf(stderr, "read error in fd1\n");
		exit(1);
	}
	if((size3 += write(fd3, buf1, size1)) < 0){
		fprintf(stderr, "write error in fd3\n");
		exit(1);
	}

	if(read(fd2, buf2, size2) < 0){
		fprintf(stderr, "read error in fd2\n");
		exit(1);
	}

	if((size3 += write(fd3, buf2, size2)) < 0){
		fprintf(stderr, "write error in fd3\n");
		exit(1);
	}

//	if(size3 == (size1 + size2))
//		printf("same size!\n");

	exit(0);

}

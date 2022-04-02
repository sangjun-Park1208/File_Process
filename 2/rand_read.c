#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#define SUFFLE_NUM 1000000

void GenRecordSequence(int* list, int n);
void swap(int* a, int* b);

int main(int argc, char* argv[]){
	int* read_order_list;
	int num_of_records;
	int fd;
	int size;
	char* record = (char*)malloc(200);
	struct timeval startTime, endTime;

	gettimeofday(&startTime, NULL);

	if(argc != 2){
		fprintf(stderr, "Usage : %s <fileName>\n", argv[0]);
		exit(1);
	}

	
	if((fd = open(argv[1], O_RDONLY)) < 0){
		fprintf(stderr, "open error for %s\n", argv[1]);
		exit(1);
	}

	if((size = lseek(fd, 0, SEEK_END)) < 0){
		fprintf(stderr, "first lseek error for %s\n", argv[1]);
		exit(1);
	}

	if(lseek(fd, 0, SEEK_SET) < 0){
		fprintf(stderr, "second lseek error for %s\n", argv[1]);
		exit(1);
	}
	num_of_records = (size / 200);
	read_order_list = (int*)malloc(sizeof(int)*num_of_records);
	
	GenRecordSequence(read_order_list, num_of_records);

	for(int i=0; i<num_of_records; i++){
		lseek(fd, read_order_list[i]*200, SEEK_SET);
		if(read(fd, record, 200) < 0){
			fprintf(stderr, "read error for %dth record\n", i);
			exit(1);
		}
	}

	close(fd);

	gettimeofday(&endTime, NULL);
	printf("%ld usec\n", endTime.tv_usec - startTime.tv_usec);
	exit(0);
}

void GenRecordSequence(int* list, int n){
	int i, j, k;


	srand((unsigned int)time(0));
	for(i=0; i<n; i++){
		list[i] = i;
	}

	for(i=0; i<SUFFLE_NUM; i++){
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}
	return;
}

void swap(int *a, int *b){
	
	int tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;

	return;
}

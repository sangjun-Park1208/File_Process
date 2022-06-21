#ifndef _STUDENT_H_
#define _STUDENT_H_

#define RECORD_SIZE 85
#define HEADER_SIZE 8

enum FIELD {ID=0, NAME, DEPT, ADDR, EMAIL};

typedef struct _STUDENT{
	char id[9];
	char name[11];
	char dept[13];
	char addr[31];
	char email[21];
}STUDENT;

#endif

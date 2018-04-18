#ifndef types_h
#define types_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024
#define MAXKEYLEN 64  
#define MAXVALUELEN 256  

typedef struct EtcFile
{
	char *key[MAXSIZE];
	char *value[MAXSIZE];
};
// #pragma pack(1)
// #pragma pack()

#endif /* types_h */
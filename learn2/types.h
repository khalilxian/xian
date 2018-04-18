#ifndef types_h
#define types_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXSIZE 1024

void ReleaseStringArr(char *arr[], int len);
int Input(char *str, char *dst[], int dst_len, char *ch);
void Output(char *str[], int len, char buf[]);
void Sort(char *str[], int len);
void Delete(char *str, int len);
void GetStringStr(char *src, char *buf, char *num);
char *DealString(char *str[], int len);
char *Trans(char *s);
#endif /* types_h */

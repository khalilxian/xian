#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include "types.h"

int main (int argc,char *argv[])
{
	struct EtcFile efile;
	//const char *filename="/mnt/hgfs/VM_share/ktest.conf";
	int sum;
	int i;
	const char *key="Path";
	char *value;
	
	if(argc<2){
		printf("请输入文件路径\n");
	}
	else{
		sum=ReadEct(argv[1],&efile,MAXSIZE); 
		for(i=0;i<sum;i++)
			printf("%s=%s\n", efile.key[i], efile.value[i]);
		
		
		for(i=0;i<sum;i++){
			if(efile.value[i] != NULL){
				free(efile.value[i]);
				efile.value[i] = NULL;
			}
			 if(efile.key[i] != NULL){
				free(efile.key[i]);
				efile.key[i] = NULL;
			}
		}
	}
	return 0;
}
  

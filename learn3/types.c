#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//删除两端的空格
int DelTrim(char s[])  
{  
    int n;  
    for(n = strlen(s) - 1; n >= 0; n--)  
    {  
        if(s[n]!=' ' && s[n]!='\t' && s[n]!='\n')  
            break;  
        s[n+1] = '\0';  
    }  
    return n;  
}
//待解决问题：遇到/* */时后面的内容均被放弃
int ReadEct(const char* filename,struct EtcFile *efile,int efsize) 
{   
    char buf[MAXSIZE];  
    int flag = 0; 
	int buflen; 
	char keybuf[MAXKEYLEN] = {0}, valuebuf[MAXVALUELEN] = {0};  
	int kvflag=0, klen=0, vlen=0;  
	int i = 0,sum=0;  
    FILE * fp;  
    if ((fp = fopen(filename, "r"))== NULL)  
    {  
        printf("fopen wrong.\n", filename);  
        return -1;  
    }  
  
  
   
    while(fgets(buf, MAXSIZE, fp) != NULL)  
    {  
        DelTrim(buf);  
        //跳过注释
        if (buf[0] != '#' && (buf[0] != '/' || buf[1] != '/')){  
            if (strstr(buf, "/*") != NULL){  
                flag = 1;  
                continue;  
            }  
            else if (strstr(buf, "*/") != NULL){  
                flag = 0;  
                continue;  
            }  
        }  
        if (flag == 1){  
            continue;  
        }  
  
        buflen = strlen(buf);  
        //跳过#/=
        if (buflen <= 1 || buf[0] == '#' || buf[0] == '=' || buf[0] == '/'){  
            continue;  
        }  
        buf[buflen-1] = '\0';  
  
		//清空，继续读取  
		memset(keybuf,0,sizeof(keybuf));
		memset(valuebuf,0,sizeof(valuebuf));
        kvflag=0;
		klen=0;
		vlen=0;
        for (i=0; i<buflen; ++i){  
            if (buf[i] == ' ')  
                continue;  
  
            if (kvflag == 0 && buf[i] != '='){  
                if (klen >= MAXKEYLEN)  
                    break;  
                keybuf[klen++] = buf[i];  
                continue;  
            }  
            else if (buf[i] == '='){  
                kvflag = 1;  
                continue;  
            }  
            
            if (vlen >= MAXVALUELEN || buf[i] == '#')  
                break;  
            valuebuf[vlen++] = buf[i];  
        }  
        if (strcmp(keybuf, "")==0 || strcmp(valuebuf, "")==0)  
            continue;  
        //printf("%s=%s\n", keybuf, valuebuf);  
		efile->key[sum] = (char *)malloc(sizeof(char) * strlen(keybuf) + 1);
		efile->value[sum] = (char *)malloc(sizeof(char) * strlen(valuebuf) + 1);
		strcpy(efile->key[sum], keybuf);
		strcpy(efile->value[sum], valuebuf);
		// printf("%s=%s\n", efile->key[sum], efile->value[sum]);
		sum++;
    }  
	printf("sum=%d\n",sum);
	if(efsize<sum){
		printf("结构体空间不足\n");
		return -1;
	}
    return sum;  
} 

#include "types.h"

//按照分割符号',' 把字符串取出来，放到数组，返回对应的数组,不改变原数组
int Input(char *str, char *dst[], int dst_len, char *ch)
{
	char *ptr;
	int i = 0, j;
	int len = strlen(str) + 1;
	char buf[len];
	memcpy(buf, str, len);
	ptr = buf;

	char *temp = NULL;
	while ((temp = strtok(ptr, ch)) != NULL)
	{
		int sub_len = strlen(temp) + 1;
		dst[i] = malloc(strlen(temp) + 1);
		memcpy(dst[i], temp, sub_len);
		ptr = NULL;
		i++;
		if (dst_len <= i)
		{
			printf("dst_len overflow");
			exit(1);
		}
	}
	return i;
}

void ReleaseStringArr(char *arr[], int len)
{
	for (int i = 0; i < len; i++)
	{
		if (arr[i])
		{
			free(arr[i]);
		}
	}
}

//数组按照分割符号',' 转换为字符串
void Output(char *str[], int len, char buf[])
{
	int i;
	for (i = 0; i < len; i++)
	{
		strcat(buf, str[i]);
		if (i < len - 1)
			strcat(buf, ",");
	}
}

//转换为由小到大排列，返回处理后的字符串
void Sort(char *str[], int len)
{
	int i, j;
	char *tmp;
	char buf1[MAXSIZE];
	char num1[MAXSIZE];
	char buf2[MAXSIZE];
	char num2[MAXSIZE];
	char buf[MAXSIZE];

	for (i = 0; i < len - 1; i++)
	{
		for (j = 0; j < len - 1 - i; j++)
		{
			bzero(buf1, MAXSIZE);
			bzero(buf2, MAXSIZE);
			bzero(num1, MAXSIZE);
			bzero(num2, MAXSIZE);
			GetStringStr(str[j], buf1, num1);
			GetStringStr(str[j + 1], buf2, num2);

			if (strcmp(buf1, buf2) == 1)
			{
				tmp = str[j];
				str[j] = str[j + 1];
				str[j + 1] = tmp;
			}
			else if (strcmp(buf1, buf2) == 0)
			{
				if (atoi(num1) > atoi(num2))
				{
					tmp = str[j];
					str[j] = str[j + 1];
					str[j + 1] = tmp;
				}
			}
		}
	}
}

//处理掉字符串中的空格符号，返回处理后的字符串
void Delete(char *str, int len)
{
	int i = 0, j = 0;
	for (i = 0; i < len; i++)
	{
		if (str[i] != ' ')
			str[j++] = str[i];
	}
	str[j] = '\0';
}

//分别获取字符串中的数字和字符
void GetStringStr(char *src, char *buf, char *num)
{
	char *ptr = src;
	for (; *ptr; ptr++)
	{
		if (isdigit(*ptr))
		{
			strcpy(num, ptr);
			memcpy(buf, src, ptr - src);
			buf[ptr - src] = 0;
			break;
		}
	}
}

//将连续的字符串以“-”处理，并返回对应的字符串
char *DealString(char *str[], int len)
{
	int i;
	int left = -1;
	int right = -1;
	char buf1[MAXSIZE];
	char num1[MAXSIZE];
	char buf2[MAXSIZE];
	char num2[MAXSIZE];
	char buf[MAXSIZE];
	char *dst = malloc(1024);
	bzero(dst, MAXSIZE);
	for (i = 0; i < len; i++)
	{
		if (left == -1)
			left = i;
		bzero(buf1, MAXSIZE);
		bzero(buf2, MAXSIZE);
		bzero(num1, MAXSIZE);
		bzero(num2, MAXSIZE);
		GetStringStr(str[i], buf1, num1);
		if (i < len - 1)
			GetStringStr(str[i + 1], buf2, num2);
		//printf("str1: %s, num: %d\n", buf1, atoi(num1));
		//printf("str2: %s, num: %d\n", buf2, atoi(num2));
		if ((strcmp(buf1, buf2) != 0) || ((atoi(num1) + 1) != atoi(num2)))
		{
			right = i;

			if (left == right)
				sprintf(buf, "%s", str[left]);
			else
				sprintf(buf, "%s-%s", str[left], str[right]);

			strcat(dst, buf);
			if (i != len - 1)
				strcat(dst, ",");
			else
				strcat(dst, "\n");
			left = -1;
			right = -1;
		}
	}
	return dst;
}
//把“-”去掉，返回对应的字符串
char *Trans(char *s)
{
	int i, j, n1, n2;
	int count;
	char *buf1[MAXSIZE], *buf2[MAXSIZE];
	char *a;
	char *buf = malloc(1024);
	char num_buf[255];
	char *left, *right;
	char bbuf1[MAXSIZE];
	char nnum1[MAXSIZE];
	char bbuf2[MAXSIZE];
	char nnum2[MAXSIZE];
	bzero(buf, 1024);
	bzero(buf1, MAXSIZE);
	bzero(buf2, MAXSIZE);
	//先按逗号切割
	//printf("s=%s\n",s);
	n1 = Input(s, buf1, MAXSIZE, ",");
	// for(i=0;i<n1;i++)
	// printf("buf1[%d]=%s\n",i ,buf1[i]);

	for (i = 0; i < n1; i++)
	{
		bzero(bbuf1, MAXSIZE);
		bzero(nnum1, MAXSIZE);
		bzero(bbuf2, MAXSIZE);
		bzero(nnum2, MAXSIZE);
		a = buf1[i];
		n2 = Input(a, buf2, MAXSIZE, "-");

		GetStringStr(buf2[0], bbuf1, nnum1);
		GetStringStr(buf2[n2 - 1], bbuf2, nnum2);

		//printf("num1: %d, num2: %d\n",atoi(nnum1), atoi(nnum2));
		for (j = atoi(nnum1); j <= atoi(nnum2); j++)
		{

			strcat(buf, bbuf1);
			sprintf(num_buf, "%d", j);
			//printf("num_buf=%s\n",num_buf);
			strcat(buf, num_buf);
			if (j < atoi(nnum2))
				strcat(buf, ",");
		}
		if (i < n1 - 1)
			strcat(buf, ",");

		ReleaseStringArr(buf2, n2);
	}
	ReleaseStringArr(buf1, n1);
	return buf;
}
//删除字符串数组中相同的字符串
int DelSame(char *str[], int len)
{
	int i, j, k;
	char *tmp, *temp;
	int sum = len;
	for (i = 0; i < sum - 1; i++)
	{
		tmp = str[i];
		for (j = i + 1; j < sum; j++) {
			if (strcmp(tmp, str[j]) == 0) {

				free(str[j]); // 删除 释放内存
				for (k = j; k < sum - 1; k++) {
					str[k] = str[k + 1];
				}
				str[k] = NULL;
				sum--; // 数组长度-1
			}
		}
	}
	return sum;
}

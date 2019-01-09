#include<stdio.h>
#include"encrypt.h"
#include"decrypt.h"
#include"private_key.h"
#include"public_key.h"
#include"commom.h"
#include"percent.h"
#include"public_function.h"
#include<time.h>
#include<string.h>
#include<stdlib.h>
const Node[PLAIN_NUM];//定义全局变量
void printf_info()
{
	printf("加密请输入 1\n");
	printf("解密请输入 2\n");
	printf("私钥产生请输入 3\n");
	printf("公钥产生请输入 4\n");
	printf("概率请输入 5\n");
}


int main(int argc, char*argv[])
{
	int num;
	char filename[30];
	char number[5];
	unsigned short table =0;
	
	clock_t encode_start,encode_finish;
	float encode_duration;
	printf_info();
	scanf("%d",&num);
	if(num == 1)
	{
		encode_start = clock();
		encrypt_data();
		encode_finish = clock();
		encode_duration = (float)(encode_finish-encode_start)/CLOCKS_PER_SEC;
		printf("\n加密时间为:%f seconds\n",encode_duration);
		return 0;
	}
	if(num == 2)
	{
		return decrypt_data(Node);
	}
	if(num == 3)
	{
		unsigned char **plain = newarr(PLAIN_NUM, PLAIN_LEN);
		srand((unsigned)time(NULL));//通过时间函数设置随机数种子，使得每次运行结果随机。
		while(table<TABLE)
		{
			
			RandPlain(plain);//将随机产生的明文赋给起始节点
			memset(number,0,5);
			memset(filename,0,30);
			sprintf(number,"%02d",table);
			strcat(filename,"privatekey");
			strcat(filename,number);
			strcat(filename,".bin");
			private_key(table,filename,plain);
			deletenewarr(plain, PLAIN_NUM);
			table++;
		}
		return 0;
	}
	if(num == 4)
	{
		return public_key();
	}
	if(num == 5)
	{
		return percent();
	}
	return 0;
}
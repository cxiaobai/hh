
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include"commom.h"
#include"public_function.h"

int percent()
{
	int count = 0;
	unsigned char f1[PLAIN_LEN];
	unsigned char f2[PLAIN_LEN];
	unsigned char f3[CIPHER_LEN];
	FILE *fp1 = NULL;
	fp1 = fopen("all_plain_text.bin","rb");
	FILE *fp2 = NULL;
	FILE *fp3 = NULL; 
	if(fp1 == NULL)
	{
		printf("open file for read error");
		exit(0);
	}
	fp2 = fopen("de_cipher.bin","rb");
	if(fp2 == NULL)
	{
		printf("open file for read error");
		exit(0); 
	}
	
	fp3 = fopen("cipher.bin","rb");
	if(fp3 == NULL)
	{
		printf("open file for read error");
		exit(0); 
	}
	int j;
	for(int i=0;i<SQ;i++)
	{
		fread(f1,sizeof(f1),1,fp1);
		//明文 
		for( j=0;j<PLAIN_LEN;j++)
		{
			printf("%02x",f1[j]);
		} 
		printf(" ");
		//密文 
		fread(f3,sizeof(f3),1,fp3);
		for(j=0;j<CIPHER_LEN;j++)
		{
			printf("%02x",f3[j]);
		} 
		printf(" ");
		
		//解密的结果 
		fread(f2,sizeof(f2),1,fp2);
		for( j=0;j<PLAIN_LEN;j++)
		{
			printf("%02x",f2[j]);
		} 
		printf(" ");
		printf("\n");
		if(plaincompare(f1,f2)==0)
		{
			count++;
		}
	}
	printf("%d\n",count);
	printf("正确率为：%f",count/((float)SQ));
	return 0;
}

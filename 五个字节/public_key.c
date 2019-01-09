
#include<stdio.h>
#include"commom.h"
//产生PK_LEN长度的公钥
int public_key()
{
//	unsigned short public_key;
//	scanf("%u",&public_key);
	unsigned char pk[PK_LEN] = {0x74,0x65,0x73,0x74,0x35};//保存公钥
	FILE *fp;
	fp = fopen("public_key.bin","wb");
	if(fp == NULL)
	{
		printf("open file for read error\n");
	}
	
//	pk[0] = public_key/256; 
//	pk[1] = public_key%256;
	fwrite(pk,PK_LEN,1,fp);
	fclose(fp);
	printf("the public key is:");
	for(int zijie=0;zijie<PK_LEN;zijie++)
	{
		printf("%02x",pk[zijie]);
	}
	printf("\n");
	return 0;
}
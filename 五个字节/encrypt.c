
#include<stdio.h>
#include<assert.h>
#include<crtdbg.h>
#include<malloc.h>
#include<string.h>
#include"md5.h"
#include"commom.h"
#include"public_function.h"
int encrypt_data()
{
	unsigned char pk[1024];
	unsigned char Encode[CIPHER_LEN];
	FILE *fp,*fp1;
	fp = fopen("public_key.bin","rb");
	if(fp == NULL)
	{
		printf("open file for read error\n");
	}
	fread(pk,PK_LEN,1,fp);
	fclose(fp);
	fp = fopen("all_plain_text.bin","wb");
	if(fp == NULL)
	{
		printf("open file for read error\n");
	}
	fp1 = fopen("cipher.bin","wb");
	if(fp1 == NULL)
	{
		printf("open file for write error\n");
	}
//	printf("公钥为：");
//	for(int zijie=0;zijie<PK_LEN;zijie++)
//	{
//		printf("%02x",pk[zijie]);
//	}
//	printf("\n");
	unsigned char encrypt_plain[PLAIN_LEN];
	unsigned long long plain_text;

	//把明文空间中五字节的明文加密，并将加密后的值写入文件中
	for(plain_text =0;plain_text<SQ;plain_text++)
	{
		memset(encrypt_plain,0,sizeof(encrypt_plain));
		encrypt_plain[4] = (unsigned char)(plain_text/4294967296);
		encrypt_plain[3] = (unsigned char)((plain_text -(encrypt_plain[4]*4294967296))/16777216);
		encrypt_plain[2] = (unsigned char)((plain_text -(encrypt_plain[4]*4294967296)-(encrypt_plain[3]*16777216))/65536);
		encrypt_plain[1] = (unsigned char)((plain_text -(encrypt_plain[4]*4294967296)-(encrypt_plain[3]*16777216)-(encrypt_plain[2]*65536))/256);
		encrypt_plain[0] = (unsigned char)((plain_text -(encrypt_plain[4]*4294967296)-(encrypt_plain[3]*16777216)-(encrypt_plain[2]*65536)-(encrypt_plain[1]*256))&255);
		printf("%02x%02x%02x%02x%02x\n",encrypt_plain[0],encrypt_plain[1],encrypt_plain[2],encrypt_plain[3],encrypt_plain[4]);
		//将加密前的明文写入all_plain_text.bin文件中
		fwrite(encrypt_plain,sizeof(encrypt_plain),1,fp);
		memcpy(Encode,MD5(strconnect(encrypt_plain,pk),PLAIN_LEN+PK_LEN),CIPHER_LEN);
//		for(int tt=0;tt<10;tt++)
//		{
//			printf("%02x",Encode[tt]);
//		}
//		printf("\n");
		//将加密后的密文写入cipher.bin文件中
		fwrite(Encode,sizeof(Encode),1,fp1);
	}
	fclose(fp);
	fclose(fp1);
	
/*加密测试
	unsigned char test[]={0xa6,0x00,0x05,0x7f,0xa6 };
	memcpy(Encode,MD5(strconnect(test,pk),PLAIN_LEN+PK_LEN),CIPHER_LEN);
	for(int tt=0;tt<CIPHER_LEN;tt++)
	{
		printf("%02x",Encode[tt]);
	}
*/	return 0;
}
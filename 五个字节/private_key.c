
#include"commom.h"
#include"md5.h"
#include"public_function.h"
#include<windows.h>
#include<stdio.h>
#include <assert.h>
//unsigned char plain[PLAIN_NUM][PLAIN_LEN];//存储随机明文
size_t table = 0;

void RandPlain(unsigned char **plain)
{  
	unsigned int i,j,xiabiao;
	for(i=0;i<PLAIN_NUM;i++)
	{
		for(j = 0;j< PLAIN_LEN;j++)
		{
			xiabiao = rand()&255;
			plain[i][j] = xiabiao;
		} 
	}
}

 
//写入文件
int write_file(struct node Node[],char filename[])
{
	FILE *fp = NULL;
	fp = fopen(filename,"wb");
	if(fp == NULL)
	{
		printf("open file for write error\n");
		return -1;
	}
	//int i=0;
	//printf("写入中.....\n");
	//i++;
	fwrite(Node,sizeof(struct node),PLAIN_NUM,fp);
	fclose(fp);
	return 0;
}

//按照尾节点从小到大排序
void quick_sort(struct node Node[],int p,int q)
{
	int i;
	int j;
	struct node jizhunzhi;
	i = p;
	j = q;
	if(p>=q)
	{
		return;
	}
	memcpy(jizhunzhi.start,Node[p].start,PLAIN_LEN);
	memcpy(jizhunzhi.stop,Node[p].stop,CIPHER_LEN);
	while(i<j)
	{
		while((i<j)&&(compare(jizhunzhi.stop,Node[j].stop)<=0))
		{
			j--;
		}
		memcpy(Node[i].start,Node[j].start,PLAIN_LEN);
		memcpy(Node[i].stop,Node[j].stop,CIPHER_LEN);
		while((i<j)&&(compare(jizhunzhi.stop,Node[i].stop)>=0))
		{
			i++;
		}
		memcpy(Node[j].start,Node[i].start,PLAIN_LEN);
		memcpy(Node[j].stop,Node[i].stop,CIPHER_LEN);
	}
	memcpy(Node[i].start,jizhunzhi.start,PLAIN_LEN);
	memcpy(Node[i].stop,jizhunzhi.stop,CIPHER_LEN);
	quick_sort(Node,p,i-1);
	quick_sort(Node,i+1,q);
}


int private_key(unsigned short tab,char filename[], unsigned char **plain)
{
	int zijie;
	unsigned char pk[PK_LEN];
	struct node *Node_Private = (struct node*)malloc(sizeof(struct node)*PLAIN_NUM);
	FILE *fp;
	fp = fopen("public_key.bin","rb");
	if(fp == NULL)
	{
		printf("open file for read error\n");
	}
	fread(pk,PK_LEN,1,fp);
	fclose(fp);
	printf("the public key is:");
	for(zijie=0;zijie<PK_LEN;zijie++)
	{
		printf("%02x",pk[zijie]);
	}
	printf("\n");
	int i,j;
	for(i=0;i<PLAIN_NUM;i++)
	{
		memcpy(Node_Private[i].start,plain[i],PLAIN_LEN);//将随机产生的明文赋给起始节点 
	}
	unsigned char t[CIPHER_LEN];
//	unsigned char te[2]={0x9c,0x3a};
	for(j=0;j<PLAIN_NUM;j++) //链的条数 
	{
		printf("产生第%d条链...\n",j);
		memcpy(t,MD5(strconnect(plain[j],pk),PLAIN_LEN+PK_LEN),CIPHER_LEN);
	
	//	memcpy(t,MD5(strconnect(te,pk),PLAIN_LEN+PK_LEN),CIPHER_LEN);
		for(i=0;i<CHAIN_LENGTH;i++) 
		{	
/*			for(int tt=0;tt<CIPHER_LEN;tt++)
			{
				printf("%02x",t[tt]);
			}
			printf(" ");
*/			memcpy(t,SK_EncryptBlock(t,pk,table,i),CIPHER_LEN);
		}
/*		for(int tt=0;tt<CIPHER_LEN;tt++)
		{
			printf("%02x",t[tt]);
		}
		printf(" ");
*/		memcpy(Node_Private[j].stop,t,CIPHER_LEN);

	}
	
	quick_sort(Node_Private,0,PLAIN_NUM-1);
	
	if(write_file(Node_Private,filename)<0)//写入文件  
	{
		return -1;
	}
	printf("第%d个私钥表产生完成。\n",tab);
/*	for (i = 0;i<PLAIN_NUM;i++)
	{
		for (j = 0;j<PLAIN_LEN;j++)
			printf("%02x", Node_Private[i].start[j]);
		printf(" ");
		for (j = 0;j<CIPHER_LEN;j++)
			printf("%02x", Node_Private[i].stop[j]);
		printf("\n");
	}
*/
	free(Node_Private);
	return 1;
}
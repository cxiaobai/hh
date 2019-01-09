#include"commom.h"
#include"public_function.h"
#include<string.h>
#include<windows.h>
#include<stdio.h>
unsigned char m[PLAIN_LEN];
unsigned int chenggong = 0;
unsigned int flase = 0;
unsigned int failure = 0;
unsigned char fail[PLAIN_LEN] = {0}; 
unsigned char result[PLAIN_LEN] = {0};

int read_file(char filename[],struct node*Node)
{
	FILE *fp = NULL;
	fp = fopen(filename,"rb");
	if(fp == NULL)
	{
		printf("open file for read error\n");
		return -1;
	}
	fread(Node,sizeof(struct node),PLAIN_NUM,fp);
	fclose(fp);
	return 0;
}


unsigned char* decode(unsigned char* utemp,unsigned int table, struct node*Node)
{
	int j,k,zijie,chazhao;
	unsigned char pk[PK_LEN];
	FILE *fp;
	fp = fopen("public_key.bin","rb");
	if(fp == NULL)
	{
		printf("open file for read error\n");
	}
	fread(pk,PK_LEN,1,fp);
	fclose(fp);
	unsigned char z[CIPHER_LEN];
	unsigned char x[CIPHER_LEN];
	unsigned char m1[CIPHER_LEN];//临时变量
	unsigned char y[CIPHER_LEN];
	unsigned char cipher[CIPHER_LEN];
	memcpy(y,utemp,CIPHER_LEN);
	memcpy(cipher,utemp,CIPHER_LEN);
	for (int tt = 0;tt<CIPHER_LEN;tt++)
	{
		printf("%02x", utemp[tt]);
	}
	printf(" ");
//	unsigned char tem[2]={0x9c,0x3a};
	for(j=0;j<=CHAIN_LENGTH;j++)//计算的次数
	{
		
		//查看是否可以匹配到尾节点 
		chazhao = binarySearch(Node,0,PLAIN_NUM-1,y);
//		printf("%d ",chazhao);
		if(chazhao<0)//没有找到，计算一次 
		{
			
			if(j == CHAIN_LENGTH)
			{
				printf("3.解密失败\n");
				failure++;
				//return_value = -1;
				return fail;
			}
			else
			{
				memcpy(y,utemp,CIPHER_LEN);
				for(k=CHAIN_LENGTH-j-1;k<CHAIN_LENGTH;k++)//迭代计算
				{
					memcpy(y,SK_EncryptBlock(y,pk,table,k),CIPHER_LEN);
				}
			}	
		}
		else
		{
			memcpy(z,MD5(strconnect(Node[chazhao].start,pk),PLAIN_LEN+PK_LEN),CIPHER_LEN);
	//		memcpy(z,MD5(strconnect(tem,pk),PLAIN_LEN+PK_LEN),CIPHER_LEN);
			if(compare(z,cipher)==0)
			{
				printf("明文值为:");
				for(zijie=0;zijie<PLAIN_LEN;zijie++)
				{
					m[zijie] = Node[chazhao].start[zijie];
					printf("%02x",Node[chazhao].start[zijie]);
				}
				printf("\n");
				return m;
			}
			//重新从该头结点产生链
			for(k = 0; k<CHAIN_LENGTH-j-1;k++)
			{
				memcpy(z,SK_EncryptBlock(z,pk,table,k),CIPHER_LEN);
			}
			memcpy(m1,z,CIPHER_LEN);
			memcpy(x,SK_EncryptBlock(z,pk,table,CHAIN_LENGTH-j-1),CIPHER_LEN);
			if(compare(x,cipher)==0)
			{
				unsigned int t;
				t = (m1[0]<<40)+(m1[1]<<32)+(m1[2]<<24)+(m1[3]<<16)+(m1[4]<<8)+m1[5];
				t = (t>>(48-(CHAIN_LENGTH-j-1)%48)|t<<(CHAIN_LENGTH-j-1)%48);
				t = t^(CHAIN_LENGTH-j-1);
				
				m1[5]=t>>40;
			    m1[4]=(t-(m1[5]<<40))>>32;
				m1[3] = (t - (m1[5] << 40) - (m1[4] << 32)) >> 24;
				m1[2]= (t - (m1[5] << 40) - (m1[4] << 32)-(m1[3]<<24)) >> 16;
				m1[1]= (t - (m1[5] << 40) - (m1[4] << 32) - (m1[3] << 24)-(m1[2]<<16)) >> 8;
				m1[0]=t&255;

				memcpy(m,m1,PLAIN_LEN);
				printf("明文值为:");
				for(zijie=0;zijie<PLAIN_LEN;zijie++)
				{
					printf("%02x",m[zijie]);
				}
				printf("\n");
				chenggong ++;
				//return_value = 1;
				return m;
			}
			else
			{
				if(j == CHAIN_LENGTH)
				{
					printf("解密失败2\n");
					//return_value = -1;
					flase ++;
					return fail; 
				}
				else
				{
					memcpy(y,utemp,CIPHER_LEN);//缺少的语句
					for(k=CHAIN_LENGTH-j-1;k<CHAIN_LENGTH;k++)//迭代计算
					{
						memcpy(y,SK_EncryptBlock(y,pk,table,k),CIPHER_LEN);
					}
				}
				
			}
			   
		} 	
	}
	free(Node);

}
int decrypt_data(struct node *Node)
{
	unsigned int tab = 0;
	FILE *fp,*fp1;
//	unsigned char Cutemp[CIPHER_LEN] = {0x42,0xa3,0x85,0x4c,0x84,0x5c };
	unsigned char Cutemp[CIPHER_LEN];
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	fp = fopen("cipher.bin","rb");
	if(fp == NULL)
	{
		printf("open file for read error\n");
	}

	fp1 = fopen("de_cipher.bin","wb");
	if(fp1 == NULL)
	{
		printf("open file for write error\n");
	}
/*解密测试
	hFind = FindFirstFile("privatekey*.bin", &FindFileData);
	if(read_file(FindFileData.cFileName,Node)<0)//从文件中读取结构体数据
	{
		printf("私钥载入失败");
	}
	for (int i = 0;i<PLAIN_NUM;i++)
	{
		for (int j = 0;j<PLAIN_LEN;j++)
			printf("%02x", Node[i].start[j]);
		printf(" ");
		for (int j = 0;j<CIPHER_LEN;j++)
			printf("%02x", Node[i].stop[j]);
		printf("\n");
	}
	decode(Cutemp,tab++,Node);
*/
	for(unsigned int i=0;i<SQ;i++)
	{	
		fread(Cutemp,sizeof(Cutemp),1,fp);
	//	for(int tt=0;tt<4;tt++)
	//	{
	//		printf("%02x",Cutemp[tt]);
	//	}
		hFind = FindFirstFile("privatekey*.bin", &FindFileData);
		if(read_file(FindFileData.cFileName,Node)<0)//从文件中读取结构体数据
		{
			printf("私钥载入失败");
			break;
		}
	//	decode(Cutemp,tab++);
		memcpy(m,decode(Cutemp,tab++,Node),PLAIN_LEN);
		if(plaincompare(m,result) !=0)//如果解密成功
		{
			tab = 0; 
			FindClose(hFind);
			fwrite(m,PLAIN_LEN,1,fp1);
			continue;
		}
		if(plaincompare(m,result)==0)//如果解密失败
		{ 
			while(FindNextFile(hFind, &FindFileData))
			{
				if(read_file(FindFileData.cFileName,Node)<0)//从文件中读取结构体数据
				{
					printf("私钥载入失败");
					break;
				}

				memcpy(m,decode(Cutemp,tab++,Node),PLAIN_LEN);
				if(plaincompare(m,result) !=0)
				{
					fwrite(m,PLAIN_LEN,1,fp1);
					tab = 0; 
					FindClose(hFind);
					break;
				}
			}
			if((plaincompare(m,result) ==0)&&(tab == TABLE))
			{
				fwrite(result,PLAIN_LEN,1,fp1);
		//		printf("所有表检索结束,解密失败\n");
			}
			FindClose(hFind);
			tab = 0;
			continue;
		}
	}	
	printf("成功的个数为：");
	printf("\n%d",chenggong);
	printf("\n%f",chenggong/((float)SQ)); 
	printf("假警的个数为：");
	printf("\n%d",flase);
	printf("\n%f",flase/((float)SQ)); 
	printf("失败的个数为：");
	printf("\n%d",failure);
	printf("\n%f",failure/((float)SQ)); 
	free(Node);
	fclose(fp);
	fclose(fp1);
	return 0;
}
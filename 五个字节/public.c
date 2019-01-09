
#include"md5.h"
#include"commom.h"
#include<assert.h>
#include<malloc.h>
#include<string.h>
#include<stdio.h>
#include<crtdbg.h>
#include<stdlib.h>
unsigned char rs[CIPHER_LEN];
unsigned char temp[PK_LEN];
unsigned char string [PLAIN_LEN+PK_LEN];//保存明文加公钥
unsigned char Encrypt[CIPHER_LEN];
//MD5加密截取CIPHER_LEN个字节
unsigned char* MD5(unsigned char * encrypt,unsigned int len)
{
	MD5_CTX md5;  
    MD5Init(&md5);           
    int i;  
    unsigned char *decrypt = (unsigned char*)malloc(sizeof(unsigned char)*16); 
    assert(_CrtCheckMemory());
    MD5Update(&md5,encrypt,len);  
    MD5Final(&md5,decrypt);  
	memset(rs,0,CIPHER_LEN);
	for(i= 0;i<CIPHER_LEN;i++)  
    {  
		rs[i] = decrypt[i];
//		printf("%02x",rs[i]);
    }
    free(decrypt);
    decrypt = NULL;
    assert(_CrtCheckMemory());
	return rs;
}

//明文+公钥 
unsigned char * strconnect(unsigned char * str1, unsigned char *str2)
{
	if(str1 == NULL || str2 == NULL)
	{
		assert(str1);
		assert(str2);
		return NULL;
	}
	unsigned char *p;
	p = string;
	assert(_CrtCheckMemory());
	memcpy(p,str1,PLAIN_LEN);
	memcpy(p+PLAIN_LEN,str2,PK_LEN);
	assert(_CrtCheckMemory()); 
//	for(int zijie=0;zijie<PLAIN_LEN+PK_LEN;zijie++)
//	{
//		printf("%02x",string[zijie]);
//	}
	return string;
}

//密文字节左循环移col%密文字节*8位，异或col+公钥 
unsigned char * SK_strconnect(unsigned char * str1, unsigned char *str2,unsigned int col)
{
	if(str1 == NULL || str2 == NULL)
	{
		assert(str1);
		assert(str2);
		return NULL;
	}
	unsigned char *p = string;
	assert(_CrtCheckMemory());
	unsigned int mk;
	mk = (str1[0]<<40)+(str1[1]<<32)+(str1[2]<<24)+(str1[3]<<16)+(str1[4]<<8)+str1[5];
	//左循环移位
	mk = (mk>>(48-col%48)|mk<<(col%48));
	mk = mk^col;
   
	str1[5]=mk>>40;
    str1[4]=(mk-(str1[5]<<40))>>32;
	str1[3]=(mk-(str1[5]<<40)-(str1[4]<<32))>>24;
	str1[2]=(mk-(str1[5]<<40)-(str1[4]<<32)-(str1[3]<<24))>>16;
	str1[1]=(mk-(str1[5]<<40)-(str1[4]<<32)-(str1[3]<<24)-(str1[2]<<16))>>8;
	str1[0]=mk&255;
	
	unsigned char *q=str1;
	//截取PLAIN_LEN字节 
	memcpy(p,q,PLAIN_LEN);
	memcpy(p+PLAIN_LEN,str2,PK_LEN);
	assert(_CrtCheckMemory());
	return string;
}

int compare(unsigned char *a,unsigned char *b)//密文比较 
{
	if((a==NULL)||(b==NULL))
	{
		printf("error:%s\n",strerror(errno));
		//exit(1);
	}
	int i;
	for(i=0;*a==*b;a++,b++,i++)
	{
		if(i==CIPHER_LEN-1)
		{
			return 0;
		}
	}
	return *a-*b;
}

//二分查找
int binarySearch(struct node *Node,int low,int high,unsigned char *target)
{	
	if((Node == NULL)||(target==NULL))
	{
		printf("error:%s\n",strerror(errno));
	}
	while(low<=high)
	{
		int mid = (low+high)/2;
		if(compare(Node[mid].stop,target)>0)
		{
			high = mid-1;
		}
		else if(compare(Node[mid].stop,target)<0)
		{
			low = mid+1;
		}
		else
		{
			return mid;
			
		}
	}
	return -1;
}  

unsigned char* SK_EncryptBlock(unsigned char*p_hex,unsigned char*k_hex,unsigned int tab,unsigned int col)
{
	if((p_hex==NULL)||(k_hex==NULL))
	{
		assert(p_hex);
		assert(k_hex);
		return NULL; 
	}
//	memcpy(string,SK_strconnect(p_hex,k_hex,col),PLAIN_LEN+PK_LEN);
	memcpy(Encrypt,MD5(SK_strconnect(p_hex,k_hex,col),PLAIN_LEN+PK_LEN),CIPHER_LEN);
	return Encrypt;
}

int plaincompare(unsigned char *a,unsigned char *b)//明文比较 
{
	if((a==NULL)||(b==NULL))
	{
		printf("error:%s\n",strerror(errno));
		//exit(1);
	}
	int i;
	for(i=0;*a==*b;a++,b++,i++)
	{
		if(i==PLAIN_LEN-1)
		{
			return 0;
		}
	}
	return *a-*b;
}

unsigned char**newarr(unsigned long rows,unsigned int cols)
{
	unsigned char **p = (unsigned char**)malloc(sizeof(unsigned char*)*rows);
 	if(p == NULL)
	{
		exit(1);
	}
	unsigned int i;
	for(i=0;i<rows;i++)
	{
		*(p+i) = (unsigned char*)malloc(sizeof(unsigned char)*cols);
		if(p[i] == NULL)
		{
			exit(1);
		}
	} 
	return p;
}

void deletenewarr(unsigned char ** p,unsigned int rows)
{
	unsigned int i;
	for(i = 0;i<rows;i++)
	{
		free(p[i]);
	}
	free(p);
}

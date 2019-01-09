#ifndef PUBLIC_FUNCTION_H
#define PUBLIC_FUNCTION_H
unsigned char* MD5(unsigned char * encrypt,unsigned int len);
unsigned char * SK_strconnect(unsigned char * str1, unsigned char *str2,unsigned int col);
unsigned char * strconnect(unsigned char * str1, unsigned char *str2);
int compare(unsigned char *a,unsigned char *b);//密文比较 

unsigned char* SK_EncryptBlock(unsigned char*p_hex,unsigned char*k_hex,unsigned int tab,unsigned int col);
int binarySearch(struct node *Node,int low,int high,unsigned char *target);
int plaincompare(unsigned char *a,unsigned char *b);//明文比较 

unsigned char**newarr(unsigned long rows, unsigned int cols);
void deletenewarr(unsigned char ** p,unsigned int rows);
#endif
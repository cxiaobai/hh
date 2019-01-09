#ifndef COMMOM_H
#define COMMOM_H
#define CIPHER_LEN 6  //密文长度
#define PLAIN_LEN 5	//明文长度
#define PLAIN_NUM 159793022 //链的条数
#define PK_LEN 5  //公钥长度
#define CHAIN_LENGTH 10321 //链长
#define SQ 6
#define TABLE 1//私钥表的个数
//定义表的结构            
struct node
{
	unsigned char start[PLAIN_LEN];//首节点
	unsigned char stop[CIPHER_LEN];//尾节点
};
#endif


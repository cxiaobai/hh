#ifndef COMMOM_H
#define COMMOM_H
#define CIPHER_LEN 6  //���ĳ���
#define PLAIN_LEN 5	//���ĳ���
#define PLAIN_NUM 159793022 //��������
#define PK_LEN 5  //��Կ����
#define CHAIN_LENGTH 10321 //����
#define SQ 6
#define TABLE 1//˽Կ��ĸ���
//�����Ľṹ            
struct node
{
	unsigned char start[PLAIN_LEN];//�׽ڵ�
	unsigned char stop[CIPHER_LEN];//β�ڵ�
};
#endif


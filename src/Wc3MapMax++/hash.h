#pragma once

//*	
//*	OBJ_Hash		KeyWordNum:2954	WordLength:4	HashValue:14-14739		Object ID
//* W3O_Hash		KeyWordNum:1166	WordLength:4	HashValue:150-8364		W3O Field
//* SLK_Hash		KeyWordNum:450	WordLength:2-18	HashValue:9-1393		SLK Field
//* TXT_Hash		KeyWordNum:89	WordLength:3-19	HashValue:3-163			TXT Field
//* ACode_Hash		KeyWordNum:76	WordLength:4	HashValue:10-255		Ability Code
//* HMQ_Hash		KeyWordNum:85	WordLength:9-32	HashValue:9-261			Mpq File Name

//��:OBJ_HashΪobject ID��hash����,����ԭʼobj������Ϊ2954,���ַ����ȹ̶�Ϊ4,��ĳ�㷨�õ�����ЩobjID��hashֵ��ΧΪ14-14739�����Զ���obj���õ�hashֵ���ܲ��ڸ÷�Χ��

//#define OBJ_HASH_MAX	14740
#define OBJ_HASH_MAX	0x4400	//16384+1024
#define W3O_HASH_MAX	8365
#define SLK_HASH_MAX	1394
#define TXT_HASH_MAX	164
#define ACODE_HASH_MAX	256

unsigned int OBJHash(register const char *str);
unsigned int W3OHash(register const char *str);
unsigned int SLKHash(register const char *str);
unsigned int TXTHash(register const char *str);
unsigned int ACodeHash(register const char *str);

void AddBufferFile(char* fileName,char* buffer,int size,bool removeBuffer);

unsigned int HMQHash(register const char *str);
void HMQHashInit(HANDLE hMPQ);

extern char** hmqHashString;	//hashValue����ӦhashString
extern char*  hmqFileType;		//hashValue����Ӧ�ļ�����
extern char*  hmqReadFile;		//hashValue����Ӧ�ļ��Ƿ񱻶�ȡ��
extern int    hmqHashSize;
extern int    hmqHashFactor;

extern char** bufferFile;		//only for func AddBufferFile
extern char** bufferFileName;
extern int*   bufferFileSize;
extern bool*  bufferNeedRemove;
extern int    bufferFileCount;
extern int    bufferFinishCount;

//�ļ�����
#define FILE_TYPE_NORMAL		0	//��ͨ�ļ���ֱ��ѹ����ͼ�ļ���
#define FILE_TYPE_REMOVE_ALWAYS	1	//�����ļ����������
#define FILE_TYPE_FOR_WE_ONLY	2	//����WE��ʹ�õ��ļ����Ӳ����趨�Ƿ���
#define FILE_TYPE_OPEN_FOR_USE	3	//3��4�����Ż�����Ҫʹ�õ��ļ�
#define FILE_TYPE_SLK_FILES		4	//
#define SetFileType(sFile,type) hmqFileType[HMQHash(sFile)] = type
#define GetFileType(sFile) hmqFileType[HMQHash(sFile)]
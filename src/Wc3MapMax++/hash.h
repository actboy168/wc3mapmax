#pragma once

//*	
//*	OBJ_Hash		KeyWordNum:2954	WordLength:4	HashValue:14-14739		Object ID
//* W3O_Hash		KeyWordNum:1166	WordLength:4	HashValue:150-8364		W3O Field
//* SLK_Hash		KeyWordNum:450	WordLength:2-18	HashValue:9-1393		SLK Field
//* TXT_Hash		KeyWordNum:89	WordLength:3-19	HashValue:3-163			TXT Field
//* ACode_Hash		KeyWordNum:76	WordLength:4	HashValue:10-255		Ability Code
//* HMQ_Hash		KeyWordNum:85	WordLength:9-32	HashValue:9-261			Mpq File Name

//例:OBJ_Hash为object ID的hash函数,所有原始obj的数量为2954,其字符长度固定为4,由某算法得到的这些objID的hash值范围为14-14739，但自定义obj所得的hash值可能不在该范围。

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

extern char** hmqHashString;	//hashValue所对应hashString
extern char*  hmqFileType;		//hashValue所对应文件类型
extern char*  hmqReadFile;		//hashValue所对应文件是否被读取过
extern int    hmqHashSize;
extern int    hmqHashFactor;

extern char** bufferFile;		//only for func AddBufferFile
extern char** bufferFileName;
extern int*   bufferFileSize;
extern bool*  bufferNeedRemove;
extern int    bufferFileCount;
extern int    bufferFinishCount;

//文件类型
#define FILE_TYPE_NORMAL		0	//普通文件，直接压到地图文件中
#define FILE_TYPE_REMOVE_ALWAYS	1	//无用文件，不予理会
#define FILE_TYPE_FOR_WE_ONLY	2	//仅在WE中使用的文件，视参数设定是否保留
#define FILE_TYPE_OPEN_FOR_USE	3	//3和4是在优化中需要使用的文件
#define FILE_TYPE_SLK_FILES		4	//
#define SetFileType(sFile,type) hmqFileType[HMQHash(sFile)] = type
#define GetFileType(sFile) hmqFileType[HMQHash(sFile)]
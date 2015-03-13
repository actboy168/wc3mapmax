#pragma once


extern char*  g_Cache[100];			                                    //�ڴ�������
extern char*  g_CachePos;		                                    //��ǰ����ָ��λ��
extern char*  g_CacheEnd;		                                    //��ǰ�������λ��
extern int    g_CacheNum;			                                    //�ѷ�����������
extern HANDLE g_hMPQ;				                                        //��ȡĿ���ͼ�ļ�
extern char   szAppDir[MAX_PATH];			                                        //������·��
extern char*  mapHeader;			                                        //��ͼ�ļ�ͷ512Byte
extern char** wtsString;			                                        //WTS�ַ�����
extern int    wtsStringNum;			                                    //WTS�ַ�����
extern char** newWtsStr;		                                    //��WTS�ַ����飨ĳЩWTS�޷���ת������Ҫ������WTS�ļ���
extern int    newWtsStrNum;			                                    //��WTS�ַ�����
extern int    wtsFileSize;			                                      //��WTS�ַ���ԭ�������ļ�ʱ������ʹ���ļ����ӵ�������
extern int    maxSlkFileSize;		                          //2M��дSLK��TXT�ļ�ʱ�������ڴ��С
extern char*  jFilePath;			                                        //J�ļ�·��
extern char*  jFileBuffer;			                                      //J�ļ�buffer
extern int    jFileSize;				                                      //J�ļ���С
extern char   gameVer;				                                      //��Ϸ�汾 1:TFT	0:ORC
extern char   mapTileset;			                                      //��ͼ������
extern char   gameDataType;			                                    //��Ϸ�����趨 -1:��սʹ��Melee,�Ƕ�սʹ��Custom	1:Custom	2:Melee
extern int    MapFileNum;		                                        //��ͼ�ļ�����
extern int    MapFileNumFound;		                                    //ʵ���������ļ�����
extern char** MapFileList;

//Config����
extern char   onlyForCompress;			                                  //ֻѹ���ļ����������Ż�
extern char   szListFilePath[MAX_PATH];			                                    //�ⲿlistfile·��
extern char   keepListFile;			                                    //�Ƿ��ڵ�ͼ�б���listfile�ļ�
extern char   convDooToJScript;		                                  //�Ƿ�ʹ��doo�Ż�
extern char   digitsNumForDooConv;	                                  //doo�Ż�ת�������С����ȷλ��
extern char   szMarkStringForDooConv[MAX_PATH];	                                //doo�Ż���־��
extern char   searchObjectLevel;		                                  //SLK�Ż����������ȼ�
extern char   szUnusedObjectName[MAX_PATH];	                                  //����Object��־��
extern char   searchObjectInDoo;	                                  //�Ƿ���.doo�ļ�������Obj(��ʼ���ƻ��װ����)
extern char   searchObjectInJScript;	                                //�Ƿ���.j�ļ�������Obj
extern char   useRandomCreep;	                                      //�������Obj����
extern char   useRandomBuild;	                                     
extern char   useRandomItem;	                                       
extern char   useMarket;	                                           
//char keepSpecialNullStringInTxt=1;	                        //ĳЩ�ı���"Tip="��Ҫ���������ɾ���Ļ�����ʾtool tip miss	
extern char   szTxtNullMark[MAX_PATH];	                                          //txt�ı��и��ַ���������Ϊ������ַ�ʹ��
extern char   blpForOpt;
extern char   jForOpt;
extern char   blpMipNum;
extern char   mdxOptLevel;
extern char   removeWEOnlyFiles;		                                  //ɾ������WE��ʹ�õ��ļ�
extern char   wtsForOptimizer;		                                    //�Ƿ��Ż�WTS�ļ�
//int  mapBlockSize;			//��ͼBlockSize��С
extern char   wavAutoBlockSize;			                                    //�Դ���wav�ļ��ĵ�ͼ�Զ��������ʵ�BlockSize
extern bool*  objectUsed;			                                      //ָ����ʹ�õ�Object��		[OBJ_HASH_MAX]
extern bool*  fieldNoEmpty;			                                    //SLK�в���Ϊ�յ�field�	[SLK_HASH_MAX]
extern char   objectsForOptimizer[7];                                //SLK�Ż����ã�0:���Ż� 1:�Ż� 2:�Ż�����
extern int    hashTableSize;			                                    //��ͼHashTable��С
extern char*  usedObjectCfgStr;
extern char*  Anei_Str;
extern char*  objectSearStr[6];
extern char*  extraString[5][4];

extern char txtOptInit;


extern char* txtFieldIsValue;	
extern char* txtFieldNoEmpty;	
extern char* txtFieldIsList;	
extern char* abiDataField[10];
extern int objectId[OBJ_HASH_MAX];
extern int objectLevel[OBJ_HASH_MAX];
extern int abilDataSearch_Field[7];
extern unsigned char* abilDataSearch_Data;

void SlkOptByFile(char* slkPath);
void SlkOptByBuffer(char* slkPath,char* buffer,int size);
void TxtOptByFile(char* txtPath,char txtDataVer);
void TxtOptByBuffer(char* txtPath,char* buffer,int size,char txtDataVer);

char* Malloc(int size);
char* Calloc(int size);

char* i2a(int i);
char* i2a(int i, char* a);
int a2i(char* a);
unsigned int a2ui(char* a);

char* xCopyStr(char*& dst,char* src);

char* StrAddTemp(char* A,char* B);
BOOL ListMapFile(char* mapName);
int ClearJScript(char*& fileBuffer, int fileSize);

int WriteCFG();
int ReadCFG();

int RunOptimizer(char* mapName,char* destMapName);
void ExInit();

void WriteInt(int fp,int i);

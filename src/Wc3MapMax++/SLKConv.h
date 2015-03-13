#pragma once


extern char*  g_Cache[100];			                                    //内存区块组
extern char*  g_CachePos;		                                    //当前区块指针位置
extern char*  g_CacheEnd;		                                    //当前区块结束位置
extern int    g_CacheNum;			                                    //已分配区块数量
extern HANDLE g_hMPQ;				                                        //读取目标地图文件
extern char   szAppDir[MAX_PATH];			                                        //程序工作路径
extern char*  mapHeader;			                                        //地图文件头512Byte
extern char** wtsString;			                                        //WTS字符数组
extern int    wtsStringNum;			                                    //WTS字符数量
extern char** newWtsStr;		                                    //新WTS字符数组（某些WTS无法被转换，需要保留在WTS文件）
extern int    newWtsStrNum;			                                    //新WTS字符数量
extern int    wtsFileSize;			                                      //将WTS字符还原到其他文件时，可能使该文件增加的最大体积
extern int    maxSlkFileSize;		                          //2M，写SLK或TXT文件时最大分配内存大小
extern char*  jFilePath;			                                        //J文件路径
extern char*  jFileBuffer;			                                      //J文件buffer
extern int    jFileSize;				                                      //J文件大小
extern char   gameVer;				                                      //游戏版本 1:TFT	0:ORC
extern char   mapTileset;			                                      //地图主地形
extern char   gameDataType;			                                    //游戏数据设定 -1:对战使用Melee,非对战使用Custom	1:Custom	2:Melee
extern int    MapFileNum;		                                        //地图文件数量
extern int    MapFileNumFound;		                                    //实际搜索到文件数量
extern char** MapFileList;

//Config设置
extern char   onlyForCompress;			                                  //只压缩文件而不进行优化
extern char   szListFilePath[MAX_PATH];			                                    //外部listfile路径
extern char   keepListFile;			                                    //是否在地图中保留listfile文件
extern char   convDooToJScript;		                                  //是否使用doo优化
extern char   digitsNumForDooConv;	                                  //doo优化转化后代码小数精确位数
extern char   szMarkStringForDooConv[MAX_PATH];	                                //doo优化标志符
extern char   searchObjectLevel;		                                  //SLK优化智能搜索等级
extern char   szUnusedObjectName[MAX_PATH];	                                  //无用Object标志符
extern char   searchObjectInDoo;	                                  //是否在.doo文件中搜索Obj(初始可破坏物、装饰物)
extern char   searchObjectInJScript;	                                //是否在.j文件中搜索Obj
extern char   useRandomCreep;	                                      //各种随机Obj设置
extern char   useRandomBuild;	                                     
extern char   useRandomItem;	                                       
extern char   useMarket;	                                           
//char keepSpecialNullStringInTxt=1;	                        //某些文本如"Tip="需要保留，如果删除的话会显示tool tip miss	
extern char   szTxtNullMark[MAX_PATH];	                                          //txt文本中该字符串将被作为替代空字符使用
extern char   blpForOpt;
extern char   jForOpt;
extern char   blpMipNum;
extern char   mdxOptLevel;
extern char   removeWEOnlyFiles;		                                  //删除仅在WE中使用的文件
extern char   wtsForOptimizer;		                                    //是否优化WTS文件
//int  mapBlockSize;			//地图BlockSize大小
extern char   wavAutoBlockSize;			                                    //对存在wav文件的地图自动检测最合适的BlockSize
extern bool*  objectUsed;			                                      //指定被使用的Object，		[OBJ_HASH_MAX]
extern bool*  fieldNoEmpty;			                                    //SLK中不能为空的field项，	[SLK_HASH_MAX]
extern char   objectsForOptimizer[7];                                //SLK优化设置：0:不优化 1:优化 2:优化并简化
extern int    hashTableSize;			                                    //地图HashTable大小
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

#include "stdafx.h"

unsigned int OBJHash(register const char *str)
{
  static char* objHashString[OBJ_HASH_MAX] = {};
  static unsigned short objHashArray[] =
  {
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,   226,  1029,
     2595,  1835,    30,     0, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,    50,   895,
     3577,  6146,  1035,  3972,     5,     0,    10,   298,  2978,    25,
      615,  5261,  3792,     0,     5,  3420,  1755,     0,   360,   135,
       25,  4053,    85,  1791,     0, 14740, 14740,  3185,   445,   225,
     3980,   489,     4,    54,  2510,   914,  1505,   374,  1154,  3859,
     3425,  1750,  1635,   685,    15,    45,     0,  3568,  3775,  1610,
     2599,   435,   370,  1365,  3955,    19,   953,   614,  2239,  1015,
      157,   155,  1257,     4,  3080,  2794,  2148,  3929,   165,   540,
      665,  2320,  3894,  2935,    75,  3262,  2552,  1221, 14740,   360,
      300,  1726, 14740,  3742,   475,     0,  3513, 14740,  3689, 14740,
    14740,  3749, 14740, 14740, 14740, 14740, 14740, 14740,    63,   994,
     3935,  1670,   173,   134,  3844,  1624,  3059,   105,  1210,  4058,
     2900,  3138,  3899,  2618,    70,  2780,  1485,   709,  3379,  3717,
     2383,     0,    15,   715, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740, 14740,
    14740, 14740, 14740, 14740, 14740, 14740, 14740
  };

  if (str==NULL)
    return 0;
  register unsigned int i = (objHashArray[(unsigned char)str[3]+25] + objHashArray[(unsigned char)str[2]] + objHashArray[(unsigned char)str[1]+71] + objHashArray[(unsigned char)str[0]+3]) & 0x3FFF;
  for(;;)
  {
    if (objHashString[i] == NULL)	//如果该位置为空则记录此Obj并返回该位置
    {
      objHashString[i]=(char*)malloc(5);	//不做释放，整个程序中使用
      objHashString[i][0]=str[0];
      objHashString[i][1]=str[1];
      objHashString[i][2]=str[2];
      objHashString[i][3]=str[3];
      objHashString[i][4]=NULL;
      return i;
    }
    else if (str[0]==objHashString[i][0] && str[1]==objHashString[i][1] && str[2]==objHashString[i][2] && str[3]==objHashString[i][3])
      return i;	//如果该位置所记录字符为此Obj则返回该位置
    i++;			//否则则检查下一位置
  }
}

unsigned int W3OHash(register const char *str)
{	//高效的静态Hash函数，不进行数据检测
  static unsigned short w3oHashArray[] =
  {
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 1920, 1630, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 1335,  355,    4,   27,
    1850,    5,    0,   10,   15, 1855, 1625,  780, 1705,   90,
    1097,  335, 6495, 2020, 1642,  120,  767, 6495,  882, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,  285,    2,
    1610,   40,  585, 1000,   20,  107,  110,    9,   89, 1915,
    1180, 1770, 1720, 1400,  802,  525,   55,  350,  215,  155,
    1340,  775,   82, 1387, 1512, 1432, 1430,  224, 6495,   60,
     227, 1402, 1112,  109,  354,   30,    2, 1862,  207, 1782,
     560, 1895,  650, 1037,  955, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495,
    6495, 6495, 6495, 6495, 6495, 6495, 6495, 6495
  };
  register unsigned int v = w3oHashArray[(unsigned char)str[3]+22] + w3oHashArray[(unsigned char)str[2]+5] + w3oHashArray[(unsigned char)str[1]+1] + w3oHashArray[(unsigned char)str[0]+1];
  return (v<W3O_HASH_MAX)?v:0;
}

unsigned int SLKHash(register const char *str)
{	//高效的静态Hash函数，不进行数据检测
  static unsigned short slkHashArray[] =
  {
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394,   15, 1394, 1394, 1394, 1394, 1394, 1394,   80,  115,
      60,  205,  160,  215,  125,   95,   40,   15, 1394, 1394,
    1394, 1394, 1394, 1394, 1394,  440,   90,   85,  215,  205,
     470,  310,  240,  320, 1394, 1394,   50,   75,   15,   55,
     250,   85,   85,  170,  225,   55,    5,   35,   85,   25,
     185, 1394, 1394, 1394, 1394, 1394, 1394,   45,  360,   35,
      15,    5,  300,  155,  490,   70, 1394,  295,    5,   15,
      40,   20,   10,   25,   15,    0,  115,    0,  205,   10,
     100,   40, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394, 1394,
    1394, 1394, 1394, 1394, 1394, 1394
  };
  register unsigned int len = 0;
  for(;str[len]>34;len++)
    ;
  register unsigned int hval = len;

  switch (hval)
  {
  default:
    hval += slkHashArray[(unsigned char)str[4]];
    /*FALLTHROUGH*/
  case 4:
  case 3:
    hval += slkHashArray[(unsigned char)str[2]];
    /*FALLTHROUGH*/
  case 2:
    hval += slkHashArray[(unsigned char)str[1]];
    /*FALLTHROUGH*/
  case 1:
    hval += slkHashArray[(unsigned char)str[0]];
    break;
  }
  hval += slkHashArray[(unsigned char)str[len - 1]];
  return (hval<SLK_HASH_MAX)?hval:0;
}

unsigned int TXTHash(register const char *str)
{	//高效的静态Hash函数，不进行数据检测
  static unsigned char txtHashArray[] =
  {
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164,  15,
     75,  65,  60,  55,  90,  85,  80, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164,   5,  20,  30,
     10,   5,  35,  20,  25,  35, 164,  20,  35,   0,
      0,  40,  15,  10,  25,   0,   0,  60,  50, 164,
      0,  10, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164, 164, 164, 164, 164,
    164, 164, 164, 164, 164, 164
  };
  if (str==NULL)
    return 0;
  register unsigned int len = 0;
  for(;str[len]>34;len++)
    ;
  register unsigned int hval = len;

  switch (hval)
  {
  default:
    hval += txtHashArray[(unsigned char)str[10]|0x20];
    /*FALLTHROUGH*/
  case 10:
  case 9:
  case 8:
  case 7:
  case 6:
  case 5:
    hval += txtHashArray[(unsigned char)str[4]|0x20];
    /*FALLTHROUGH*/
  case 4:
  case 3:
    hval += txtHashArray[(unsigned char)str[2]|0x20];
    break;
  case 2:
  case 1:
    ;
  }
  hval += txtHashArray[(unsigned char)str[len - 1]|0x20];
  return hval<TXT_HASH_MAX?hval:0;
}

unsigned int ACodeHash(register const char *str)
{	//高效的静态Hash函数，进行数据检测
  static const char* aCodeHashString[] =
  {
    0,0,0,0,0,0,0,0,0,0,
    "ANpa",0,0,0,0,
    "ANba",0,0,0,0,
    "ANto",0,0,0,0,
    "ANbs",0,
    "ANsu",0,0,
    "Apts",0,
    "Acoa",0,0,0,0,
    "Astn",0,0,
    "Asta",0,
    "Aspx",0,0,
    "Aspb",0,
    "Assp",0,0,
    "ANcr",0,
    "Aphx",0,0,
    "ANsg",0,
    "Abur",0,0,
    "Acha",0,
    "ANsy",0,0,
    "Aspd",0,
    "Asod",0,0,
    "Acpf",0,
    "ANin",0,0,
    "ANdo",0,
    "ANvc",0,0,
    "ANeg",0,
    "ANdp",0,0,
    "Abtl",0,
    "AUcb",0,0,
    "ANsl",0,
    "Acoh",0,
    "AOhx","ANef",0,
    "ANsw",0,0,
    "Aapl",0,
    "AHpx",0,
    "AUin","Aetf",0,
    "AOsf",0,0,
    "AIsb",0,
    "Agyd",0,0,
    "ANsq",0,
    "Aloa",0,0,
    "Achl",0,
    "Avng",0,0,
    "ANrg",0,
    "Aave",0,0,
    "AImo",0,
    "ANfy",0,0,
    "AIbl",0,
    "AEst",0,0,
    "Abrf",0,0,0,0,
    "ANdc",0,
    "Aply",0,0,
    "AIsh",0,
    "Acoi",0,0,
    "AIpm",0,
    "Aeye",0,0,
    "Ahwd",0,
    "AUls",0,0,
    "ANrc",0,
    "Aent",0,0,
    "AIfs",0,
    "AOwd",0,0,
    "AIgl",0,
    "AEfn",0,0,
    "ANwm",0,
    "Aexh",0,0,
    "Adec",0,0,0,0,
    "AEsv",0,0,0,0,
    "Amil",0,0,0,0,
    "AIrd",0,0,0,0,
    "ANlm",0,0,0,0,
    "Arav",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    "AEme",0,0,0,0,0,0,0,0,0,
    "Arai",0,0,0,0,
    "AHwe"
  };
  static unsigned short aCodeHashArray[] =
  {
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260,  85,
    260, 260,  80,  65, 260, 260, 260, 260,   5,  42,
    260, 260, 260, 260, 260,  37, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260,  35,   5,  25,
     70,  45,  90,  45,  30,  65, 125, 260, 110,  65,
     90,   2,   0,   7,  90,  20,  15,  32,   2,  85,
     72,  22,  37, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260, 260, 260, 260,
    260, 260, 260, 260, 260, 260, 260
  };

  if (str==NULL)
    return 0;
  register unsigned int v = aCodeHashArray[(unsigned char)str[3]+1] + aCodeHashArray[(unsigned char)str[2]] + aCodeHashArray[(unsigned char)str[1]];
  if (v<ACODE_HASH_MAX && aCodeHashString[v]!=NULL && str[0] == aCodeHashString[v][0] && str[1] == aCodeHashString[v][1] && str[2] == aCodeHashString[v][2] && str[3] == aCodeHashString[v][3])
    return v;
  return 0;
}

char** hmqHashString;	//hashValue所对应hashString
char* hmqFileType;		//hashValue所对应文件类型
char* hmqReadFile;		//hashValue所对应文件是否被读取过
int hmqHashSize;
int hmqHashFactor;

char** bufferFile;		//only for func AddBufferFile
char** bufferFileName;
int* bufferFileSize;
bool* bufferNeedRemove;
int bufferFileCount;
int bufferFinishCount;



unsigned int HMQHash(register const char *str)
{
  static unsigned char hmqHashArray[] =
  {
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262,   0, 262, 262, 262, 262,  40, 262, 262, 262,
    262,   0, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262,   0,  20, 115,
     12,  40,   0,  35,  10,  30, 262,  10,   0,  85,
      5,  25,   0, 105,  15,  90,   5,  45, 262,  55,
      0,  15, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262, 262, 262, 262, 262,
    262, 262, 262, 262, 262, 262
  };
  register unsigned int len = strlen(str);

  switch (len)
  {
  default:
    len += hmqHashArray[(unsigned char)(str[15]>64 && str[15]<91?(str[15]|0x20):str[15])];
  case 15:
  case 14:
    len += hmqHashArray[(unsigned char)(str[13]>64 && str[13]<91?(str[13]|0x20):str[13])];
  case 13:
  case 12:
  case 11:
    len += hmqHashArray[(unsigned char)(str[10]>64 && str[10]<91?(str[10]|0x20):str[10])];
  case 10:
    len += hmqHashArray[(unsigned char)(str[9]>64 && str[9]<91?(str[9]|0x20):str[9])];
  case 9:
    break;
  case 8:
  case 7:
  case 6:
  case 5:
  case 4:
  case 3:
  case 2:
  case 1:
    ;
  }
  register unsigned int i = len & hmqHashFactor;
  for(;;)
  {
    if (hmqHashString[i] == NULL)
    {
      hmqHashString[i]=(char*)Malloc(len+1);
      for(int x=0;;x++)
      {
        if ((hmqHashString[i][x]=str[x])==NULL)
          return i;
      }			
    }
    else
      for(int x=0;(str[x]|0x20)==(hmqHashString[i][x]|0x20);x++)
      {
        if (str[x]==NULL)
          return i;
      }		
      i++;
  }
}

void HMQHashInit(HANDLE hMPQ)
{
  hmqHashSize=128;
  for(int size=(SFileGetFileInfo(hMPQ,SFILE_INFO_NUM_FILES)+54)<<1; hmqHashSize<size; hmqHashSize<<=1)
    ;
  hmqHashFactor=hmqHashSize-1;
  hmqHashSize<<=1;
  hmqHashString=(char**)Calloc(hmqHashSize*28);
  hmqFileType=(char*)(hmqHashString+hmqHashSize);
  hmqReadFile=(char*)(hmqFileType+hmqHashSize);

  bufferFile=(char**)(hmqReadFile+hmqHashSize);
  bufferFileName=(char**)(bufferFile+hmqHashSize);
  bufferFileSize=(int*)(bufferFileName+hmqHashSize);
  bufferNeedRemove=(bool*)(bufferFileSize+hmqHashSize);
  bufferFileCount=0;
  bufferFinishCount=0;

  SetFileType("(attributes)",1);
  SetFileType("war3map.imp",2);
  SetFileType("war3campaign.imp",2);
  SetFileType("war3map.w3c",2);
  SetFileType("war3map.w3s",2);
  SetFileType("war3map.w3r",2);
  SetFileType("war3map.wct",2);
  SetFileType("war3map.wtg",2);
  SetFileType("war3mapunits.doo",2);
  SetFileType("(listfile)",3);
  SetFileType("war3map.wts",3);
  SetFileType("war3campaign.wts",3);
  SetFileType("war3map.w3i",3);
  SetFileType("war3map.doo",3);
  SetFileType("war3map.w3u",3);
  SetFileType("war3map.w3t",3);
  SetFileType("war3map.w3b",3);
  SetFileType("war3map.w3d",3);
  SetFileType("war3map.w3a",3);
  SetFileType("war3map.w3h",3);
  SetFileType("war3map.w3q",3);
  SetFileType("war3campaign.w3u",3);
  SetFileType("war3campaign.w3t",3);
  SetFileType("war3campaign.w3b",3);
  SetFileType("war3campaign.w3d",3);
  SetFileType("war3campaign.w3a",3);
  SetFileType("war3campaign.w3h",3);
  SetFileType("war3campaign.w3q",3);
  SetFileType("war3map.j",3);
  SetFileType("scripts\\war3map.j",3);
  SetFileType("war3mapskin.txt",3);
  SetFileType("war3mapmisc.txt",3);

  if (OPT_UNIT)
  {
    SetFileType("units\\campaignunitfunc.txt",4);
    SetFileType("units\\campaignunitstrings.txt",4);
    SetFileType("units\\humanunitfunc.txt",4);
    SetFileType("units\\humanunitstrings.txt",4);
    SetFileType("units\\neutralunitfunc.txt",4);
    SetFileType("units\\neutralunitstrings.txt",4);
    SetFileType("units\\nightelfunitfunc.txt",4);
    SetFileType("units\\nightelfunitstrings.txt",4);
    SetFileType("units\\orcunitfunc.txt",4);
    SetFileType("units\\orcunitstrings.txt",4);
    SetFileType("units\\undeadunitfunc.txt",4);
    SetFileType("units\\undeadunitstrings.txt",4);
    SetFileType("units\\unitabilities.slk",4);
    SetFileType("units\\unitbalance.slk",4);
    SetFileType("units\\unitdata.slk",4);
    SetFileType("units\\unitweapons.slk",4);
    SetFileType("units\\unitui.slk",4);
  }
  if (OPT_ITEM)
  {
    SetFileType("units\\itemdata.slk",4);
    SetFileType("units\\itemfunc.txt",4);
    SetFileType("units\\itemstrings.txt",4);
  }
  if (OPT_DEST)
    SetFileType("units\\destructabledata.slk",4);
  if (OPT_DOOD)
    SetFileType("doodads\\doodads.slk",4);
  if (OPT_ABIL)
  {
    SetFileType("units\\abilitydata.slk",4);
    SetFileType("units\\abilitybuffdata.slk",4);
    SetFileType("units\\campaignabilityfunc.txt",4);
    SetFileType("units\\campaignabilitystrings.txt",4);
    SetFileType("units\\commonabilityfunc.txt",4);
    SetFileType("units\\commonabilitystrings.txt",4);
    SetFileType("units\\humanabilityfunc.txt",4);
    SetFileType("units\\humanabilitystrings.txt",4);
    SetFileType("units\\itemabilityfunc.txt",4);
    SetFileType("units\\itemabilitystrings.txt",4);
    SetFileType("units\\neutralabilityfunc.txt",4);
    SetFileType("units\\neutralabilitystrings.txt",4);
    SetFileType("units\\nightelfabilityfunc.txt",4);
    SetFileType("units\\nightelfabilitystrings.txt",4);
    SetFileType("units\\orcabilityfunc.txt",4);
    SetFileType("units\\orcabilitystrings.txt",4);
    SetFileType("units\\undeadabilityfunc.txt",4);
    SetFileType("units\\undeadabilitystrings.txt",4);
  }
  if (OPT_UPGR)
  {
    SetFileType("units\\campaignupgradefunc.txt",4);
    SetFileType("units\\campaignupgradestrings.txt",4);
    SetFileType("units\\humanupgradefunc.txt",4);
    SetFileType("units\\humanupgradestrings.txt",4);
    SetFileType("units\\neutralupgradefunc.txt",4);
    SetFileType("units\\neutralupgradestrings.txt",4);
    SetFileType("units\\nightelfupgradefunc.txt",4);
    SetFileType("units\\nightelfupgradestrings.txt",4);
    SetFileType("units\\orcupgradefunc.txt",4);
    SetFileType("units\\orcupgradestrings.txt",4);
    SetFileType("units\\undeadupgradefunc.txt",4);
    SetFileType("units\\undeadupgradestrings.txt",4);
    SetFileType("units\\upgradedata.slk",4);
  }
}

void AddBufferFile(char* fileName,char* buffer,int size,bool removeBuffer)
{
  bufferFileName[bufferFileCount] = Malloc(strlen(fileName)+1);
  strcpy(bufferFileName[bufferFileCount],fileName);
  bufferFile[bufferFileCount] = buffer;
  bufferFileSize[bufferFileCount] = size;
  bufferNeedRemove[bufferFileCount] = removeBuffer;
  bufferFileCount++;
}
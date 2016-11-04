#include "StdAfx.h"




#define GLOBAL_CACHE_SIZE	0x800000	                          //8M，每次分配的内存区块大小

char*  g_Cache[100];			                                    //内存区块组
char*  g_CachePos=NULL;		                                    //当前区块指针位置
char*  g_CacheEnd=NULL;		                                    //当前区块结束位置
int    g_CacheNum=0;			                                    //已分配区块数量
HANDLE g_hMPQ;				                                        //读取目标地图文件
char   szAppDir[MAX_PATH];			                              //程序工作路径
char*  mapHeader;			                                        //地图文件头512Byte
char** wtsString;			                                        //WTS字符数组
int    wtsStringNum;			                                    //WTS字符数量
char** newWtsStr=NULL;		                                    //新WTS字符数组（某些WTS无法被转换，需要保留在WTS文件）
int    newWtsStrNum;			                                    //新WTS字符数量
int    wtsFileSize;			                                      //将WTS字符还原到其他文件时，可能使该文件增加的最大体积
int    maxSlkFileSize = 0x200000;		                          //2M，写SLK或TXT文件时最大分配内存大小
char*  jFilePath;			                                        //J文件路径
char*  jFileBuffer;			                                      //J文件buffer
int    jFileSize;				                                      //J文件大小
char   gameVer=1;				                                      //游戏版本 1:TFT	0:ORC
char   mapTileset;			                                      //地图主地形
char   gameDataType;			                                    //游戏数据设定 -1:对战使用Melee,非对战使用Custom	1:Custom	2:Melee
int    MapFileNum;		                                        //地图文件数量
int    MapFileNumFound;		                                    //实际搜索到文件数量
char** MapFileList;

//Config设置
char   onlyForCompress;			                                  //只压缩文件而不进行优化
char   szListFilePath[MAX_PATH];			                        //外部listfile路径
char   keepListFile;			                                    //是否在地图中保留listfile文件
char   convDooToJScript;		                                  //是否使用doo优化
char   digitsNumForDooConv;	                                  //doo优化转化后代码小数精确位数
char   szMarkStringForDooConv[MAX_PATH];	                    //doo优化标志符
char   searchObjectLevel;		                                  //SLK优化智能搜索等级
char   szUnusedObjectName[MAX_PATH];	                        //无用Object标志符
char   searchObjectInDoo=1;	                                  //是否在.doo文件中搜索Obj(初始可破坏物、装饰物)
char   searchObjectInJScript;	                                //是否在.j文件中搜索Obj
char   useRandomCreep;	                                      //各种随机Obj设置
char   useRandomBuild;	                                     
char   useRandomItem;	                                       
char   useMarket;	                                           
//char keepSpecialNullStringInTxt=1;	                        //某些文本如"Tip="需要保留，如果删除的话会显示tool tip miss	
char   szTxtNullMark[MAX_PATH];	                              //txt文本中该字符串将被作为替代空字符使用
char   blpForOpt;
char   jForOpt;
char   blpMipNum;
char   mdxOptLevel;
char   removeWEOnlyFiles;		                                  //删除仅在WE中使用的文件
char   wtsForOptimizer;		                                    //是否优化WTS文件
                                                              //int  mapBlockSize;			//地图BlockSize大小
char   wavAutoBlockSize;			                                    //对存在wav文件的地图自动检测最合适的BlockSize
bool*  objectUsed;			                                      //指定被使用的Object，		[OBJ_HASH_MAX]
bool*  fieldNoEmpty;			                                    //SLK中不能为空的field项，	[SLK_HASH_MAX]
char   objectsForOptimizer[7];                                //SLK优化设置：0:不优化 1:优化 2:优化并简化
int    hashTableSize;			                                    //地图HashTable大小
char*  usedObjectCfgStr="";
char*  Anei_Str="";
char*  objectSearStr[6]={"","","","","",""};
char*  extraString[5][4]={
	"HERO","","Hamg,Hblm,Hmkg,Hpal,Obla,Ofar,Oshd,Otch,Edem,Ekee,Emoo,Ewar,Ucrl,Udea,Udre,Ulic,Npbm,Nbrn,Nngs,Nplh,Nbst,Nalc,Ntin,Nfir","",
	"TALT","","halt,oalt,eate,uaod","",
	"TWN1","","htow,ogre,etol,unpl","",
	"TWN2","","hkee,ostr,etoa,unp1",".15,0",
	"TWN3","","hcas,ofrt,etoe,unp2",".3,0"
};


inline char* Malloc(int size)		//内存块分配
{
	if (g_CachePos+size > g_CacheEnd)
	{
		g_Cache[g_CacheNum]=(char*)malloc(GLOBAL_CACHE_SIZE);
		g_CachePos=g_Cache[g_CacheNum];
		g_CacheEnd=g_CachePos+GLOBAL_CACHE_SIZE;
		g_CacheNum++;
	}
	g_CachePos+=size;
	return g_CachePos-size;
}

inline char* Calloc(int size)		//内存块分配
{
	if (g_CachePos+size > g_CacheEnd)
	{
		g_Cache[g_CacheNum]=(char*)malloc(GLOBAL_CACHE_SIZE);
		g_CachePos=g_Cache[g_CacheNum];
		g_CacheEnd=g_CachePos+GLOBAL_CACHE_SIZE;
		g_CacheNum++;
	}
	memset(g_CachePos,0,size);
	g_CachePos+=size;
	return g_CachePos-size;
}

void FlushGlobalCache()			//清除所有分配的内存区块
{
	for(;g_CacheNum>0;g_CacheNum--)
		free(g_Cache[g_CacheNum]);
	g_CachePos=NULL;
	g_CacheEnd=NULL;
}

#include "hash.h"

char* txtFieldIsValue;			//[TXT_HASH_MAX];
char* txtFieldNoEmpty;			//[TXT_HASH_MAX];
char* txtFieldIsList;			//[TXT_HASH_MAX];
char* abiDataField[10]={NULL,"DataA","DataB","DataC","DataD","DataE","DataF","DataG","DataH","DataI"};
int objectId[OBJ_HASH_MAX];			//Origin Object
int objectLevel[OBJ_HASH_MAX];
int abilDataSearch_Field[7];
unsigned char* abilDataSearch_Data;

#define memcat(dst,src,size) memcpy(dst,src,size),dst+=size
#define strcat(dst,src) for(char* _s=src;*_s!=NULL;*(dst++)=*(_s++))
#define chrcat(str,chr) *str++=chr
#define NewWtsStr(s) newWtsStr[++newWtsStrNum]=s

bool slkOptField[SLK_HASH_MAX];
void InitUsedField(char* s)
{
	memset(slkOptField,0,SLK_HASH_MAX);
	for(;;)
	{
		for(;*s<35;s++)
		{
			if (*s==NULL)
				return;
		}
		slkOptField[SLKHash(s)]=1;
		for(;*s>34;s++)
			;
	}
}

void InitSlkOptField(char* slkPath)
{
	char* slkName=slkPath;
	for(int i=0;slkPath[i]!=NULL;i++)
	{
		if (slkPath[i]=='\\')
			slkName=slkPath+i+1;
	}
	if (stricmp(slkName,"AbilityBuffData.slk")==0)
		InitUsedField("alias");
	else if (stricmp(slkName,"AbilityBuffMetaData.slk")==0)
		InitUsedField("ID	field	slk	index");
	else if (stricmp(slkName,"AbilityData.slk")==0)
		InitUsedField("alias	code	checkDep	levels	reqLevel	levelSkip	priority	targs1	Cast1	Dur1	HeroDur1	Cool1	Cost1	Area1	Rng1	DataA1	DataB1	DataC1	DataD1	DataE1	DataF1	DataG1	DataH1	DataI1	UnitID1	BuffID1	EfctID1	targs2	Cast2	Dur2	HeroDur2	Cool2	Cost2	Area2	Rng2	DataA2	DataB2	DataC2	DataD2	DataE2	DataF2	DataG2	DataH2	DataI2	UnitID2	BuffID2	EfctID2	targs3	Cast3	Dur3	HeroDur3	Cool3	Cost3	Area3	Rng3	DataA3	DataB3	DataC3	DataD3	DataE3	DataF3	DataG3	DataH3	DataI3	UnitID3	BuffID3	EfctID3	targs4	Cast4	Dur4	HeroDur4	Cool4	Cost4	Area4	Rng4	DataA4	DataB4	DataC4	DataD4	DataE4	DataF4	DataG4	DataH4	DataI4	UnitID4	BuffID4	EfctID4");
	else if (stricmp(slkName,"AbilityMetaData.slk")==0)
		InitUsedField("ID	field	slk	index	repeat	data");
	else if (stricmp(slkName,"DestructableData.slk")==0)
		InitUsedField("DestructableID	tilesetSpecific	file	lightweight	fatLOS	texID	texFile	Name	walkable	cliffHeight	targType	armor	numVar	HP	occH	flyH	fixedRot	minScale	maxScale	maxPitch	maxRoll	radius	fogRadius	fogVis	pathTex	pathTexDeath	deathSnd	shadow	colorR	colorG	colorB	showInMM	useMMColor	MMRed	MMGreen	MMBlue	selectable	selcircsize	portraitmodel");
	else if (stricmp(slkName,"DestructableMetaData.slk")==0)
		InitUsedField("ID	field	slk	index");
	else if (stricmp(slkName,"Doodads.slk")==0)
		InitUsedField("doodID	tilesetSpecific	file	soundLoop	defScale	minScale	maxScale	visRadius	walkable	numVar	floats	shadow	showInFog	animInFog	fixedRot	pathTex	showInMM	useMMColor	MMRed	MMGreen	MMBlue	vertR01	vertG01	vertB01	vertR02	vertG02	vertB02	vertR03	vertG03	vertB03	vertR04	vertG04	vertB04	vertR05	vertG05	vertB05	vertR06	vertG06	vertB06	vertR07	vertG07	vertB07	vertR08	vertG08	vertB08	vertR09	vertG09	vertB09	vertR10	vertG10	vertB10");
	else if (stricmp(slkName,"DoodadMetaData.slk")==0)
		InitUsedField("ID	field	slk	index	repeat");
	else if (stricmp(slkName,"ItemData.slk")==0)
		InitUsedField("itemID	class	Level	oldLevel	abilList	cooldownID	ignoreCD	uses	prio	usable	perishable	droppable	pawnable	sellable	pickRandom	powerup	drop	stockMax	stockRegen	stockStart	goldcost	lumbercost	HP	morph	armor	file	scale	colorR	colorG	colorB");
	else if (stricmp(slkName,"UnitAbilities.slk")==0)
		InitUsedField("unitAbilID	auto	abilList	heroAbilList");
	else if (stricmp(slkName,"UnitBalance.slk")==0)
		InitUsedField("unitBalanceID	level	type	goldcost	lumbercost	goldRep	lumberRep	fmade	fused	bountydice	bountysides	bountyplus	lumberbountydice	lumberbountysides	lumberbountyplus	stockMax	stockRegen	stockStart	HP	regenHP	regenType	manaN	mana0	regenMana	def	defUp	defType	spd	minSpd	maxSpd	bldtm	reptm	sight	nsight	STR	INT	AGI	STRplus	INTplus	AGIplus	Primary	upgrades	tilesets	nbrandom	isbldg	preventPlace	requirePlace	repulse	repulseParam	repulseGroup	repulsePrio	collision");
	else if (stricmp(slkName,"UnitData.slk")==0)
		InitUsedField("unitID	race	prio	deathType	death	canSleep	cargoSize	movetp	moveHeight	moveFloor	turnRate	propWin	orientInterp	formation	targType	pathTex	fatLOS	points	buffType	buffRadius	nameCount	canFlee	requireWaterRadius	isBuildOn	canBuildOn");
	else if (stricmp(slkName,"UnitUI.slk")==0)
		InitUsedField("unitUIID	file	fileVerFlags	unitSound	tilesetSpecific	name	nbmmIcon	hideHeroBar	hideHeroMinimap	hideHeroDeathMsg	hideOnMinimap	blend	scale	scaleBull	maxPitch	maxRoll	elevPts	elevRad	fogRad	walk	run	selZ	weap1	weap2	teamColor	customTeamColor	armor	modelScale	red	green	blue	uberSplat	unitShadow	buildingShadow	shadowW	shadowH	shadowX	shadowY	shadowOnWater	selCircOnWater	occH");
	else if (stricmp(slkName,"UnitWeapons.slk")==0)
		InitUsedField("unitWeapID	weapsOn	acquire	minRange	castpt	castbsw	launchX	launchY	launchZ	launchSwimZ	impactZ	impactSwimZ	weapType1	targs1	showUI1	rangeN1	RngBuff1	atkType1	weapTp1	cool1	dice1	sides1	dmgplus1	dmgUp1	dmgpt1	backSw1	Farea1	Harea1	Qarea1	Hfact1	Qfact1	splashTargs1	targCount1	damageLoss1	spillDist1	spillRadius1	weapType2	targs2	showUI2	rangeN2	RngBuff2	atkType2	weapTp2	cool2	dice2	sides2	dmgplus2	dmgUp2	dmgpt2	backSw2	Farea2	Harea2	Qarea2	Hfact2	Qfact2	splashTargs2	targCount2	damageLoss2	spillDist2	spillRadius2");
	else if (stricmp(slkName,"UnitMetaData.slk")==0)
		InitUsedField("ID	field	slk	index");
	else if (stricmp(slkName,"UpgradeData.slk")==0)
		InitUsedField("upgradeid	class	used	global	maxlevel	inherit	goldbase	goldmod	lumberbase	lumbermod	timebase	timemod	effect1	base1	mod1	code1	effect2	base2	mod2	code2	effect3	base3	mod3	code3	effect4	base4	mod4	code4");
	else if (stricmp(slkName,"UpgradeMetaData.slk")==0)
		InitUsedField("ID	field	slk	index	repeat	appendIndex");
	else
		memset(slkOptField,1,SLK_HASH_MAX);
}

int a2i(char* a)
{
	register int i=1,x=0;
	if (*a == '-')
		i=-1,a++;
	for(;;)
	{
		if (*a>'9' || *a<'0')
			return x*i;
		else
			x=(*(a++) & 0xf) + x*10;
	}
}

unsigned int a2ui(char* a)
{
	register unsigned int x=0;
	for(;;)
	{
		if (*a>'9' || *a<'0')
			return x;
		else
			x=(*(a++) & 0xf) + x*10;
	}
}

char* i2a(int i,char* a)
{
	char* b=a;
	if (i<0)
	{
		i=-i;
		*b='-';
		b++;
	}
	register int j=0;
	register int k[10];
	for(;;)
	{
		k[j]=i-i/10*10;
		i=i/10;
		if (i==0)
			break;
		j++;
	}
	i=j;
	for(;j>=0;j--)
	{
		b[i-j] = '0' | k[j];
	}
	b[i+1]=NULL;
	return a;
}

char* i2a(int i)
{
	char* a=Malloc(12);
	char* b=a;
	if (i<0)
	{
		i=-i;
		*b='-';
		b++;
	}
	register int j=0;
	register int k[10];
	for(;;)
	{
		k[j]=i-i/10*10;
		i=i/10;
		if (i==0)
			break;
		j++;
	}
	i=j;
	for(;j>=0;j--)
	{
		b[i-j] = '0' | k[j];
	}
	b[i+1]=NULL;
	return a;
}

inline char* f2a(double f,char* a,char n,bool canBeInt)
{
	register char i,j;
	if (f<0)
	{
		f=-f;
		*a='-';
		i=1;
	}
	else
		i=0;

	register int z=1;
	for(j=n;j>0;j--)
		z*=10;
	f+=0.5/z;
	register int x=(int)f;
	f=(f-x)*z;
	register int y=(int)f;

	if (x==0)
	{
		if (y==0)
			*a='0',i=1;
	}
	else
	{
		register char k[20];
		for(;;)
		{
			k[j]=x-x/10*10;
			x/=10;
			if (x==0)
				break;
			j++;
		}
		i=i+j;
		for(;j>=0;j--)
			a[i-j] = '0' | k[j];
		i++;
	}

	if (y!=0)
	{
		a[i++]='.';
		for(;y-y/10*10==0;y/=10)
			n--;
		for(j=n-1;j>=0;j--)
		{
			a[i+j] = '0' | (y-y/10*10);
			y/=10;
		}
		i+=n;
	}
	else if (!canBeInt)
		a[i++]='.';
	a[i]=NULL;
	return a;
}

inline char* f2a(double f,char n,bool canBeInt)
{
	char* a=Malloc(20);
	register char i,j;
	if (f<0)
	{
		f=-f;
		*a='-';
		i=1;
	}
	else
		i=0;

	register int z=1;
	for(j=n;j>0;j--)
		z*=10;
	f+=0.5/z;
	register int x=(int)f;
	f=(f-x)*z;
	register int y=(int)f;

	if (x==0)
	{
		if (y==0)
			*a='0',i=1;
	}
	else
	{
		register char k[20];
		for(;;)
		{
			k[j]=x-x/10*10;
			x/=10;
			if (x==0)
				break;
			j++;
		}
		i=i+j;
		for(;j>=0;j--)
			a[i-j] = '0' | k[j];
		i++;
	}

	if (y!=0)
	{
		a[i++]='.';
		for(;y-y/10*10==0;y/=10)
			n--;
		for(j=n-1;j>=0;j--)
		{
			a[i+j] = '0' | (y-y/10*10);
			y/=10;
		}
		i+=n;
	}
	else if (!canBeInt)
		a[i++]='.';
	a[i]=NULL;
	return a;
}

char* ValueString(char* s)	//数值型字符简化，如0.340 => .34
{
	if (s==NULL)
		return NULL;
	char* t=s;
	for (;*t!='.';t++)
	{
		if (*t==NULL)
			return s;
	}
	char* v=t+1;
	for (;*v=='0';v++)
		;
	if (*v==NULL)
	{
		*t=NULL;
		return s;
	}
	for (;*v!=NULL;v++)
		;
	for (v--;*v=='0';v--)
		;
	v[1]=NULL;
	if (s[0]=='0' && s[1]=='.')
		return ++s;
	else
		return s;
}

void StringToArray(char* s, char** t, int n, bool isValue)	//将txt文本数据s按等级分配到数组t
{	//s不能是常量,并会被变动,且生成的数组t是基于s的
	int i=0;
	for(int i=0;i<n && t[i]!=NULL;i++)	//如果已有记录则为追加
		;
	for(;i<n;i++)
	{
		char* d=s;
		bool e=false;
		if (*s=='"')
		{
			bool b=false;
			for(;;)
			{
				s++;
				if (*s==',')
					b=true;
				else if (*s=='"')
				{
					if (s[1]==NULL || (s[1]==',' && s[2]==NULL))
						e=true;
					break;
				}
				else if (*s==NULL)
				{
					e=true;
					break;
				}
			}
			if (!b)
			{
				d++;
				if (*s=='"')
				{
					*s=NULL;
					s+=2;
				}
			}
			else
			{
				if (*s=='"')
				{
					*(++s)=NULL;
					s++;
				}
			}
			t[i]=d;
		}
		else
		{
			for(;;s++)
			{
				if (*s==',')
				{
					*(s++)=NULL;
					if (*s==NULL)
						e=true;
					break;
				}
				else if (*s==NULL)
				{
					e=true;
					break;
				}
			}
			if (isValue)
			{
				t[i]=ValueString(d);
				if (t[i][0]=='0' && t[i][1]==NULL)
					t[i]="";
			}
			else
				t[i]=d;
		}
		if (e)
			break;
	}
}

void WriteInt(int fp,int i)
{
	register char s[12];
	register char c[10];
	register int j=0,k=0;

	if (i<0)
	{
		i=-i;
		*s='-';
		j=1;
	}

	for(;;)
	{
		c[k]=i-i/10*10;
		i=i/10;
		if (i==0)
			break;
		k++;
	}
	i=j+k;
	for(;k>=0;k--)
	{
		s[i-k] = '0' | c[k];
	}
	write(fp,s,i+1);
}

inline void WriteFloat(int fp,double f,char n,bool canBeInt)
{
	register char s[40];
	register char i,j;
	if (f<0)
	{
		f=-f;
		*s='-';
		i=1;
	}
	else
		i=0;

	register int z=1;
	for(j=n;j>0;j--)
		z*=10;
	f+=0.5/z;
	register int x=(int)f;
	f=(f-x)*z;
	register int y=(int)f;

	if (x==0)
	{
		if (y==0)
			*s='0',i=1;
	}
	else
	{
		register char k[20];
		for(;;)
		{
			k[j]=x-x/10*10;
			x/=10;
			if (x==0)
				break;
			j++;
		}
		i=i+j;
		for(;j>=0;j--)
			s[i-j] = '0' | k[j];
		i++;
	}

	if (y!=0)
	{
		s[i++]='.';
		for(;y-y/10*10==0;y/=10)
			n--;
		for(j=n-1;j>=0;j--)
		{
			s[i+j] = '0' | (y-y/10*10);
			y/=10;
		}
		i+=n;
	}
	else if (!canBeInt)
		s[i++]='.';
	write(fp,s,i);
}

inline void WriteIntToBuffer(char*& bufferPos,int i)
{
	register char s[12];
	register char c[10];
	register int j=0,k=0;

	if (i<0)
	{
		i=-i;
		*s='-';
		j=1;
	}

	for(;;)
	{
		c[k]=i-i/10*10;
		i=i/10;
		if (i==0)
			break;
		k++;
	}
	i=j+k;
	for(;k>=0;k--)
	{
		s[i-k] = '0' | c[k];
	}
	i++;
	memcpy(bufferPos,s,i);
	bufferPos+=i;
}

inline void WriteFloatToBuffer(char*& bufferPos,double f,char n,bool canBeInt)
{
	register char s[40];
	register char i,j;
	if (f<0)
	{
		f=-f;
		*s='-';
		i=1;
	}
	else
		i=0;

	register int z=1;
	for(j=n;j>0;j--)
		z*=10;
	f+=0.5/z;
	register int x=(int)f;
	f=(f-x)*z;
	register int y=(int)f;

	if (x==0)
	{
		if (y==0)
			s[i++]='0';
	}
	else
	{
		register char k[20];
		for(;;)
		{
			k[j]=x-x/10*10;
			x/=10;
			if (x==0)
				break;
			j++;
		}
		i=i+j;
		for(;j>=0;j--)
			s[i-j] = '0' | k[j];
		i++;
	}

	if (y!=0)
	{
		s[i++]='.';
		for(;y-y/10*10==0;y/=10)
			n--;
		for(j=n-1;j>=0;j--)
		{
			s[i+j] = '0' | (y-y/10*10);
			y/=10;
		}
		i+=n;
	}
	else if (!canBeInt)
		s[i++]='.';
	memcpy(bufferPos,s,i);
	bufferPos+=i;
}

char* xCopyStr(char*& dst,char* src)	//字符串复制
{
	if (dst!=NULL && dst!="")
		free(dst);
	if (src==NULL || *src==NULL)
	{
		dst="";
		return dst;
	}
	int i=0;
	for(;src[i]!=NULL;i++)
		;
	dst=(char*)malloc(i+1);
	i=0;
	for(;src[i]!=NULL;i++)
		dst[i]=src[i];
	dst[i]=NULL;
	return dst;
}

inline char* StrAddTemp(char* A,char* B)	//字符串叠加，仅作临时使用
{
	static char g_tempStr[500]={};
	char s[500];
	int i = 0,j=0;
	for(;A[i]!=NULL;i++)
		s[i]=A[i];
	for(;B[j]!=NULL;j++)
		s[i+j]=B[j];
	i+=j;
	g_tempStr[i]=NULL;
	for(;i>0;)
		g_tempStr[--i]=s[i];
	return g_tempStr; 
}


void ExInit()	//每次优化前运行
{
	memset(objectId,0,OBJ_HASH_MAX*4);
	wtsForOptimizer=1;			//总是优化WTS
    newWtsStrNum=0;
	//mapBlockSize=0;
	objectUsed=(bool*)Calloc(OBJ_HASH_MAX);
	char* tempStr = usedObjectCfgStr;
	for(;;)			//read usedObj cfg
	{
		if (*tempStr=='/')
		{
			for(;*tempStr!='\r' && *tempStr!='\n';tempStr++)
				;
			for(;*tempStr=='\r' || *tempStr=='\n';tempStr++)
				;
		}
		else if (*tempStr==NULL)
			break;
		else
		{
			for(;*tempStr<48;tempStr++)
				;
			objectUsed[OBJHash(tempStr)]=true;
			tempStr+=4;
			for(;*tempStr>47;tempStr++)
				;
		}
	}
}

void ReadWTS()
{
	HANDLE hFile;
	if (!SFileOpenFileEx(g_hMPQ,"war3map.wts",NULL,&hFile))
	{
		wtsForOptimizer=0;
		wtsFileSize=0;
		wtsString=(char**)Calloc(4);
		wtsStringNum=0;
		return;
	}
	
	int WTSNum=0;
	char* tempFile;
	char* tempStr="";
	wtsFileSize = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
	char* fileStr;
	if (wtsForOptimizer==0)
		fileStr=(char*)malloc(wtsFileSize+2);
	else
		fileStr=Malloc(wtsFileSize+2);
	SFileReadFile(hFile,fileStr,wtsFileSize,NULL,NULL);
	SFileCloseFile(hFile);
	tempFile=fileStr;
	tempFile[wtsFileSize]='/';
	tempFile[wtsFileSize+1]=NULL;
	int i=wtsFileSize-1;
	for (char beg=0,end=0;;i--)
	{
		if (tempFile[i]=='}' && tempFile[i-1]!='\\')
			beg=0,end=1;
		else if (tempFile[i]=='{' && tempFile[i-1]!='\\')
			beg=1;
		else if (tempFile[i]=='S' && (i == 0 || i == 3 || tempFile[i-1]=='\n') && beg==1 && end==1)
		{
			wtsStringNum=a2i(tempFile+i+7);
			wtsString=(char**)Calloc((wtsStringNum+1)*4);
            newWtsStr=(char**)Malloc((wtsStringNum+1)*4);
			break;
		}
	}
	if (tempFile[0]==0xEF && tempFile[1]==0xBB && tempFile[2]==0xBF)	//UTF-8文件头
		tempFile+=3;
		
	wtsFileSize=0;	//重新计算有用的字符数量
	int wtsV_a,wtsV_b;
	for(;;)
	{
		if (*tempFile=='S')		//读取数据
		{
			tempStr=tempFile;
			tempFile+=7;
			WTSNum=0;											//atoi
			for(;;)
			{
				if (*tempFile>'9' || *tempFile<'0')
					break;
				else
				{
					WTSNum = WTSNum*10 + (*tempFile & 0xf);
					tempFile++;
				}
			}													//atoi end
			wtsV_a=tempFile-tempStr+1>11?tempFile-tempStr+1:11;	//TRIGSTR_xxx
			for(;*tempFile!='{';tempFile++)		//strchr for '{'
				;
			for(tempFile++;*tempFile=='\n' || *tempFile=='\r';tempFile++)	//起始的换行符都会被忽略
				;
			if (*tempFile=='}')		//全是换行则忽略
				continue;
			tempStr=tempFile;
			for(;;tempFile++)		//strchr for '}'
			{
                if(*tempFile=='}')
				{
					if(tempFile[-2]=='\r' && tempFile[-1]=='\n')
					{
						tempFile[-2]=NULL;
						wtsV_b=tempFile-tempStr-2;
					}
					else if(tempFile[-1]=='\n' || tempFile[-1]=='\r')
					{
						tempFile[-1]=NULL;
						wtsV_b=tempFile-tempStr-1;
					}
					else
					{
						*tempFile=NULL;
						wtsV_b=tempFile-tempStr;
					}
					break;
				}
			}
			wtsString[WTSNum]=tempStr;	//为方便起见,直接指向指针,WTS文本不清除掉
			if (wtsV_b>wtsV_a)
				wtsFileSize+=wtsV_b-wtsV_a;
		}
		else if (*tempFile=='/')
		{
			if (tempFile[1]==NULL)
				break;
			else
				for(;*tempFile!='\n' && *tempFile!='\r';tempFile++)
					;
		}
		tempFile++;
	}

	if (wtsForOptimizer==0)
	{
		wtsFileSize=0;
		tempFile=fileStr;
		for (int i=0;i<wtsStringNum;)
		{
			i++;
			if (wtsString[i]!=NULL)
			{
				memcat(tempFile,"STRING ",7);
				WriteIntToBuffer(tempFile,i);
				memcat(tempFile,"\r\n{\r\n",5);
				strcat(tempFile,wtsString[i]);
				memcat(tempFile,"\r\n}\r\n",5);
			}
		}
		memset(wtsString,NULL,(wtsStringNum+1)*4);
		AddBufferFile("war3map.wts",fileStr,tempFile-fileStr,1);
	}
}

void ConvW3I()
{
	HANDLE hFile;
	SFileOpenFileEx(g_hMPQ,"war3map.w3i",NULL,&hFile);
	int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
	char* fileStr=(char*)malloc(size+1);
	SFileReadFile(hFile,fileStr,size,NULL,NULL);
	SFileCloseFile(hFile);
	if (wtsForOptimizer==0)
		return AddBufferFile("war3map.w3i",fileStr,size,1);
	char* tempFile=fileStr;
	char* tempStr=tempFile;
	char* destStr,* destStrPos;
	int i;
	if (wtsForOptimizer)
	{
		destStr=(char*)malloc(size+wtsFileSize);
		destStrPos = destStr;
	}

	gameVer = *tempStr>18?1:0;	//tft=1 roc=0;
	tempStr+=12;
	for(int n=0;n<4;n++)
	{
		if (*tempStr=='T' && memcmp(tempStr+1,"RIGSTR_",7)==0 && (tempStr+=8) && (i=a2i(tempStr)) && i<=wtsStringNum && wtsString[i]!=NULL)
		{	//find TRIGSTR_ => tempStr+=8,i=a2i(tempStr), Cond:i<=wtsStringNum && wtsString[i]!=NULL
			memcat(destStrPos,tempFile,tempStr-8-tempFile);
			strcat(destStrPos,wtsString[i]);
			for(;*tempStr!=NULL;tempStr++)
				;
			tempFile=tempStr++;
		}
		else
		{
			for(;*tempStr!=NULL;tempStr++)
				;
			tempStr++;
		}
	}
	tempStr+=56;
	char isMelee = ((*tempStr & 4)==4);
	tempStr+=4;
	mapTileset = *tempStr;

	tempStr+=5;
	for(int n=3+gameVer;n>0;n--)	//TFT 4Str,	ROC 3Str
	{
		if (*tempStr=='T' && memcmp(tempStr+1,"RIGSTR_",7)==0 && (tempStr+=8) && (i=a2i(tempStr)) && i<=wtsStringNum && wtsString[i]!=NULL)
		{
			memcat(destStrPos,tempFile,tempStr-8-tempFile);
			strcat(destStrPos,wtsString[i]);
			for(;*tempStr!=NULL;tempStr++)
				;
			tempFile=tempStr++;
		}
		else
		{
			for(;*tempStr!=NULL;tempStr++)
				;
			tempStr++;
		}
	}
	gameDataType = *tempStr;
	if (gameDataType!=1 && gameDataType!=2)
		gameDataType = 1+isMelee;
	tempStr+=4;

	if (wtsForOptimizer==0)
	{
		AddBufferFile("war3map.w3i",fileStr,size,1);
		return;
	}
	for(int n=3+gameVer;n>0;n--)	//TFT 4Str,	ROC 3Str
	{
		if (*tempStr=='T' && memcmp(tempStr+1,"RIGSTR_",7)==0 && (tempStr+=8) && (i=a2i(tempStr)) && i<=wtsStringNum && wtsString[i]!=NULL)
		{
			memcat(destStrPos,tempFile,tempStr-8-tempFile);
			strcat(destStrPos,wtsString[i]);
			for(;*tempStr!=NULL;tempStr++)
				;
			tempFile=tempStr++;
		}
		else
		{
			for(;*tempStr!=NULL;tempStr++)
				;
			tempStr++;
		}
	}
	if (gameVer>0)
	{
		tempStr+=24;
		if (*tempStr=='T' && memcmp(tempStr+1,"RIGSTR_",7)==0 && (tempStr+=8) && (i=a2i(tempStr)) && i<=wtsStringNum && wtsString[i]!=NULL)
		{
			memcat(destStrPos,tempFile,tempStr-8-tempFile);
			strcat(destStrPos,wtsString[i]);
			for(;*tempStr!=NULL;tempStr++)
				;
			tempFile=tempStr++;
		}
		else
		{
			for(;*tempStr!=NULL;tempStr++)
				;
			tempStr++;
		}
		tempStr+=5;
	}
	int num=(int&)*tempStr;
	tempStr+=4;
	for(;num>0;num--)
	{
		tempStr+=16;
		if (*tempStr=='T' && memcmp(tempStr+1,"RIGSTR_",7)==0 && (tempStr+=8) && (i=a2i(tempStr)) && i<=wtsStringNum && wtsString[i]!=NULL)
		{
			memcat(destStrPos,tempFile,tempStr-8-tempFile);
			strcat(destStrPos,wtsString[i]);
			for(;*tempStr!=NULL;tempStr++)
				;
			tempFile=tempStr++;
		}
		else
		{
			for(;*tempStr!=NULL;tempStr++)
				;
			tempStr++;
		}
		tempStr+=16;
	}
	num=(int&)*tempStr;
	tempStr+=4;
	for(;num>0;num--)
	{
		tempStr+=8;
		if (*tempStr=='T' && memcmp(tempStr+1,"RIGSTR_",7)==0 && (tempStr+=8) && (i=a2i(tempStr)) && i<=wtsStringNum && wtsString[i]!=NULL)
		{
			memcat(destStrPos,tempFile,tempStr-8-tempFile);
			strcat(destStrPos,wtsString[i]);
			for(;*tempStr!=NULL;tempStr++)
				;
			tempFile=tempStr++;
		}
		else
		{
			for(;*tempStr!=NULL;tempStr++)
				;
			tempStr++;
		}
	}
	memcat(destStrPos,tempFile,fileStr+size-tempFile);
	AddBufferFile("war3map.w3i",destStr,destStrPos-destStr,1);
	free(fileStr);
	return;
}

int ConvWar3MapSkin()
{
	HANDLE hFile;
	if (!SFileOpenFileEx(g_hMPQ,"war3mapSkin.txt",NULL,&hFile))
		return ERR_FILE_NOT_FOUND;
	int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
	char* fileStr=(char*)malloc(size+1);		//函数结束时释放
	SFileReadFile(hFile,fileStr,size,NULL,NULL);
	SFileCloseFile(hFile);
	char* tempFile=fileStr;
	char* tempStr=tempFile;
	char* endMark=fileStr+size;
	*endMark='T';
	char* destStr = (char*)malloc(size+wtsFileSize);
	char* destStrPos = destStr;
    int i;
	for(;;)
	{
		switch (*tempStr)
		{
		case '\r':
			*tempStr='\n';
		case '\n':
			tempStr++;
			if (*tempStr=='\n' || *tempStr=='\r')
			{
				int x=tempStr-tempFile;
				memcpy(destStrPos,tempFile,x);
				destStrPos+=x;
				for (tempStr++; *tempStr=='\n' || *tempStr=='\r'; tempStr++)
					;
				tempFile=tempStr;
			}
			break;
		case 'T':
			if (memcmp(tempStr+1,"RIGSTR_",7)==0)
            {
				i = a2i(tempStr+8);
				if (i<=wtsStringNum && wtsString[i]!=NULL)
				{
					memcat(destStrPos,tempFile,tempStr-tempFile);
                    for(char* s=wtsString[i];*s!=NULL;)
                    {
                        if (*s=='\r' || *s=='\n')
                        {
                            if (*s=='\r' && s[1]=='\n')
                                s+=2;
                            else
                                s++;
                            *(destStrPos++)='|', *(destStrPos++)='n';
                        }
                        else
                            *(destStrPos++)=*(s++);
                    }
                    for(tempStr+=8;*tempStr>='0' && *tempStr<='9';tempStr++)
                        ;
					tempFile=tempStr;
                }
			}
			else if (tempStr>=endMark)
			{
				memcat(destStrPos,tempFile,endMark-tempFile);
				AddBufferFile("war3mapSkin.txt",destStr,destStrPos-destStr,1);
				free(fileStr);
				return 0;
			}
			else
				tempStr++;
			break;
		default:
			tempStr++;
		}
	}
    return 0;
}

int ClearJScript(char*& fileBuffer, int fileSize)
{
	fileBuffer[fileSize]='\n';
	fileBuffer[fileSize+1]=NULL;

	char* tempStr=fileBuffer;
	char* destStr=fileBuffer;
	char* destStrPos=destStr;
	if (*tempStr=='\xef' && tempStr[1]=='\xbb' && tempStr[2]=='\xbf')
		tempStr+=3;
	for(;;)
	{
		for(;*tempStr==' '||*tempStr=='\t';tempStr++)
			;
		if (*tempStr!='/' && *tempStr!='\n' && *tempStr!='\r')
			break;
		for(;*tempStr!='\n' && *tempStr!='\r';tempStr++)
			;
		for(;*tempStr=='\n' || *tempStr=='\r';tempStr++)
			;
	}

	for(;;)
	{
		switch(*tempStr)
		{
		case ' ':
		case '\t':
			switch(tempStr[-1])
			{
			case '(':
			case ')':
			case ',':
			case '=':
			case '+':
			case '-':
			case '*':
			case '/':
			case '<':
			case '>':
			case '[':
			case ']':
				for(;*tempStr==' ' || *tempStr=='\t';tempStr++)
					;
				break;
			default:
				for(;*tempStr==' ' || *tempStr=='\t';tempStr++)
					;
				switch(*tempStr)
				{
				case ',':
				case '=':
				case '+':
				case '-':
				case '*':
				case '/':
				case '<':
				case '>':
				case '!':
				case ')':
				case '(':
				case ']':
				case '[':
				case '\n':
				case '\r':
					break;
				default:
					*destStrPos++=' ';
				}
			}
			break;
		case '/':
			if (*(++tempStr)!='/')
			{
				*destStrPos++='/';
				break;
			}
			for(;(*tempStr!='\n'&&*tempStr!='\r');tempStr++)
				;				
		case '\r':
		case '\n':
			for(;*tempStr=='\n'||*tempStr=='\r';tempStr++)
				;
			for(;*tempStr==' '||*tempStr=='\t';tempStr++)
				;
			if (*tempStr==NULL)
				return destStrPos-destStr;
			else if (destStrPos[-1]!='\n')
				*destStrPos++='\n';
			break;
		case '.':
			if (tempStr[-1]=='0' && (tempStr[-2]>'9'||tempStr[-2]<'0'))
				destStrPos[-1]='.';
			else
				*destStrPos++='.';
			for(tempStr++;*tempStr<='9'&&*tempStr>='0';*destStrPos++=*tempStr++)
				;
			for(;destStrPos[-1]=='0';destStrPos--)
				;
			if (destStrPos[-1]=='.' && (destStrPos[-2]>'9'||destStrPos[-2]<'0'))
				*destStrPos++='0';
			break;
		case '\"':
			for(*destStrPos++=*tempStr++;;*destStrPos++=*tempStr++)
			{
				if (*tempStr=='\\')
					*destStrPos++=*tempStr++;
				else if (*tempStr=='\"')
					break;
			}
			*destStrPos++=*tempStr++;
			break;
		case '\'':
			for(*destStrPos++=*tempStr++;*tempStr!='\'';*destStrPos++=*tempStr++)
				;
			*destStrPos++=*tempStr++;
			break;
		default:
			*destStrPos++=*tempStr++;
		}
	}
}

BOOL ConvJScriptBase()
{
	jFilePath=NULL;
	jFileSize=0;
	HANDLE jFile;
	if (SFileOpenFileEx(g_hMPQ,"war3map.j",NULL,&jFile))
		jFilePath="war3map.j";
	else if (SFileOpenFileEx(g_hMPQ,"scripts\\war3map.j",NULL,&jFile))
		jFilePath="scripts\\war3map.j";
	else
		return FALSE;		//缺少J文件，无效地图

  if (jForOpt)
  {
    CWc3MapOptimizerHepler helper(jFile, &jFileBuffer, (size_t*)&jFileSize, TEXT("124"));

    if (jFileBuffer == NULL || jFileSize == 0)
    {
        SFileCloseFile(jFile);
        return FALSE;
    }
  }
  else
  {
    jFileSize   = SFileGetFileInfo(jFile,SFILE_INFO_SIZE);
    jFileBuffer = (char*)malloc(jFileSize+8);
    SFileReadFile(jFile, jFileBuffer, jFileSize, NULL, NULL);
  }

  SFileCloseFile(jFile);

  jFileSize = ClearJScript(jFileBuffer,jFileSize);

  return TRUE;
}

#define FuncCondBef(c) (c==' '||c==','||c=='('||c=='='||c=='\t')
#define FuncCondAft(c) (c=='('||c==' '||c=='\t')
bool CheckFuncNoCondBef(char*& chkStr,char* funcName,char* obj)
{
	register int i=0;
	for(;funcName[i]!=NULL;i++)
		if (chkStr[i]!=funcName[i])
			return false;
	if (FuncCondAft(chkStr[i]))
	{
		chkStr+=i+1;
		for(;;)
		{
			if (*obj==NULL)
				break;
			else
			{
				for(;*obj<48;obj++)
					;
				objectUsed[OBJHash(obj)]=true;
				obj+=4;
				for(;*obj>47;obj++)
					;
			}
		}
		return true;
	}
	return false;
}

void SearchObjectInJScript()
{
	jFileBuffer[jFileSize]='/';
	jFileBuffer[jFileSize+1]=NULL;
	char* tempStr = jFileBuffer;
	for(;;)
	{
		if (*tempStr>62)
		{
			switch(*tempStr)
			{
			case 'B':
				if (memcmp(tempStr+1,"lightGoldMineForPlayer",22)==0 && FuncCondBef(tempStr[-1]))
				{
					tempStr+=23;
					if (CheckFuncNoCondBef(tempStr,"","ugol") || CheckFuncNoCondBef(tempStr,"BJ","ugol"))
						continue;
				}
				break;
			case 'C':
				if (memcmp(tempStr+1,"reateBlightedGoldmine",21)==0 && FuncCondBef(tempStr[-1]) && FuncCondAft(tempStr[22]))
				{
					objectUsed[OBJHash("ugol")]=true;
					tempStr+=23;
					continue;
				}
				else if(memcmp(tempStr+1,"hangeElevatorWalls",18)==0 && FuncCondBef(tempStr[-1]) && FuncCondAft(tempStr[19]))
				{
					objectUsed[OBJHash("DTep")]=true;
					tempStr+=20;
					continue;
				}
				break;
			case 'M':
				if (memcmp(tempStr+1,"elee",4)==0 && FuncCondBef(tempStr[-1]))
				{
					tempStr+=5;
					if (memcmp(tempStr,"StartingUnits",13)==0)
					{
						tempStr+=13;
						if (CheckFuncNoCondBef(tempStr,"",			"hpea htow Hamg Hmkg Hpal Hblm stwp Amic ogre opeo Obla Ofar Otch Oshd unpl ugho uaco ugol Udea Udre Ulic Ucrl etol ewsp Edem Ekee Emoo Ewar nshe") || 
							CheckFuncNoCondBef(tempStr,"ForPlayer",	"hpea htow Hamg Hmkg Hpal Hblm stwp Amic ogre opeo Obla Ofar Otch Oshd unpl ugho uaco ugol Udea Udre Ulic Ucrl etol ewsp Edem Ekee Emoo Ewar") ||
							CheckFuncNoCondBef(tempStr,"Human",		"hpea htow Hamg Hmkg Hpal Hblm stwp Amic") ||
							CheckFuncNoCondBef(tempStr,"Orc",		"ogre opeo Obla Ofar Otch Oshd stwp") ||
							CheckFuncNoCondBef(tempStr,"Undead",	"unpl ugho uaco ugol Udea Udre Ulic Ucrl stwp") ||
							CheckFuncNoCondBef(tempStr,"NightElf",	"etol ewsp Edem Ekee Emoo Ewar stwp") ||
							CheckFuncNoCondBef(tempStr,"UnknownRace","nshe"))
							continue;
					}
					else if (memcmp(tempStr,"Grant",5)==0)
					{
						tempStr+=5;
						if (CheckFuncNoCondBef(tempStr,"HeroItems","stwp") || CheckFuncNoCondBef(tempStr,"MeleeGrantItemsToHero","stwp") || CheckFuncNoCondBef(tempStr,"MeleeGrantItemsToHiredHero","stwp") || CheckFuncNoCondBef(tempStr,"MeleeGrantItemsToTrainedHero","stwp"))
							continue;
					}
					else if (CheckFuncNoCondBef(tempStr,"RandomHeroLoc","stwp"))
						continue;
				}
				break;
			//case 'N':	NearbyElevatorExists	NearbyElevatorExistsEnum	'DTrf'  'DTrx'
			}
			for(;*tempStr>62;tempStr++)
				;
		}
		else if (*tempStr=='/')
		{
			if (tempStr[1]=='/')
			{
				for(tempStr+=2;*tempStr!='\n' && *tempStr!='\r';tempStr++)
					;
			}
			else if (tempStr[1]==NULL)
				return;
			tempStr++;
		}
		else if (*tempStr=='\"')
		{
			for(tempStr++;*tempStr!='\"';tempStr++)
				 if (*tempStr=='\\')
					 tempStr++;
			tempStr++;
		}
		else if (*tempStr=='\'')
		{
			tempStr++;
			if (tempStr[4]=='\'')
				objectUsed[OBJHash(tempStr)]=true;
            
            for(;*tempStr!='\'';tempStr++)
					;
            tempStr++;
		}
		else if (*tempStr>='0' && *tempStr<='9')
		{
			int id = *tempStr & 0xf;
			if (*tempStr=='0')
			{
				tempStr++;
				if (*tempStr=='x' || *tempStr=='X')
				{
					for(tempStr++;(*tempStr>='0' && *tempStr<='9') || (*tempStr>='a' && *tempStr<='f') || (*tempStr>='A' && *tempStr<='F');tempStr++)
						id = (id<<4) + (*tempStr & 0xf);
				}
				else
				{
					for(tempStr++;*tempStr>='0' && *tempStr<='7';tempStr++)
						id = (id<<3) + (*tempStr & 0x7);
				}
			}
			else
			{
				for(tempStr++;*tempStr>='0' && *tempStr<='9';tempStr++)
					id = id*10 + (*tempStr & 0xf);
			}

			if (id>='A000')
			{
				char s[5];
				s[4] = NULL;
				s[3] = id - id/256*256,id/=256;
				s[2] = id - id/256*256,id/=256;
				s[1] = id - id/256*256,id/=256;
				s[0] = id - id/256*256;
				objectUsed[OBJHash(s)]=true;
			}
		}
		else
			tempStr++;
	}
}

int ConvDoo()
{
	HANDLE dFile;
	int dFileSize;
	char* dBuffer;
	char digNum=digitsNumForDooConv;
	char* markStr=szMarkStringForDooConv;
	if (!SFileOpenFileEx(g_hMPQ,"war3map.doo",NULL,&dFile))
		return 0;
	dFileSize=SFileGetFileInfo(dFile,SFILE_INFO_SIZE);
	dBuffer=(char*)malloc(dFileSize);
	SFileReadFile(dFile,dBuffer,dFileSize,NULL,NULL);
	SFileCloseFile(dFile);
	char * tempFile=dBuffer;

	if (convDooToJScript==0)
	{
		tempFile+=4;					//可破坏物/装饰物
		bool isVerTFT=((int&)*tempFile==8);
		tempFile+=8;
		int num=(int&)*tempFile;
		tempFile+=4;
		for(;num>0;num--)
		{
 		   objectUsed[OBJHash(tempFile)]=true;
			tempFile+=38;
			if (isVerTFT)
			{
				tempFile+=4;
				int itemDrops=(int&)*tempFile;
				tempFile+=4;
				if (itemDrops>0)
					for(;itemDrops>0;itemDrops--)
						tempFile+=8*((int&)*tempFile)+4;
			}
			tempFile+=4;
		}

		tempFile+=4;					//悬崖/地形装饰物
		num=(int&)*tempFile;
		tempFile+=4;
		for(;num>0;num--)
		{
			objectUsed[OBJHash(tempFile)]=true;
			tempFile+=16;
		}

		AddBufferFile("war3map.doo",dBuffer,dFileSize,1);
		return 0;
	}

	char* jStr[100];
	jStr[0]=jFileBuffer;
	int jStrLen[100];
	int strNum=0;
	for(int markLen=strlen(markStr);;strNum++)
	{
		jStr[strNum+1]=strstr(jStr[strNum],markStr);
Doo_L1:
		if (jStr[strNum+1]==NULL)
		{
			jStrLen[strNum]=jStr[0]+jFileSize-jStr[strNum];
			break;
		}
		else
		{
			char* c=jStr[strNum+1];
			for(;;)
			{
				c--;
				if (*c=='\t' || *c==' ')
					;
				else if (*c=='\n'|| *c=='\r')
					break;
				else
				{
					jStr[strNum+1]=strstr(jStr[strNum+1]+markLen,markStr);
					goto Doo_L1;
				}
			}

			for(c=jStr[strNum+1]+markLen;;)
			{
				if (*c=='\t' || *c==' ')
					;
				else if (*c=='\n'|| *c=='\r')
					break;
				else
				{
					jStr[strNum+1]=strstr(jStr[strNum+1]+markLen,markStr);
					goto Doo_L1;
				}
				c++;
			}

			jStrLen[strNum]=jStr[strNum+1]-jStr[strNum];
			jStr[strNum+1]+=markLen;			
		}
	}

	if (strNum==0)
	{
		strNum++;
		jStr[1]=strstr(jFileBuffer,"function main takes nothing returns nothing");
		jStr[1]=strstr(jStr[1],"endfunction");
		jStrLen[0]=jStr[1]-jStr[0];
		jStrLen[1]=jFileBuffer+jFileSize-jStr[1];
	}

	tempFile=dBuffer;
	char * tempStr;
	char* destDStr = (char*)malloc(dFileSize);
	char* destDStrPos = destDStr;
	memcpy(destDStrPos,tempFile,16);
	destDStrPos+=16;
	tempFile+=4;
	int version=(int&)*tempFile;
	tempFile+=8;
	int dooNum=(int&)*tempFile;
	tempFile+=4;
	int jNum=0;

	int* destID=(int*)malloc(dooNum*4*8);
	int* var=destID+dooNum;
	float* pos_x=(float*)(var+dooNum);
	float* pos_y=pos_x+dooNum;
	float* pos_z=pos_y+dooNum;
	float* face=pos_z+dooNum;
	float* scale=face+dooNum;
	char* life=(char*)(scale+dooNum);

	char flag;
	int itemTable, itemDrops;

  int n = dooNum;
	for(; jNum < n ;)
	{
		tempStr=tempFile;
		destID[jNum]=(int&)*tempFile;
		objectUsed[OBJHash(tempFile)]=true;
		tempFile+=4;
		var[jNum]=(int&)*tempFile;
		tempFile+=4;
		pos_x[jNum]=(float&)*tempFile;
		tempFile+=4;
		pos_y[jNum]=(float&)*tempFile;
		tempFile+=4;
		pos_z[jNum]=(float&)*tempFile;
		tempFile+=4;
    face[jNum]=(float&)*tempFile;
    face[jNum]= face[jNum]*180.0f/3.14159f;
		tempFile+=4;
		scale[jNum]=(float&)*tempFile;
		tempFile+=12;
		flag=(char&)*tempFile;
		tempFile+=1;
		life[jNum]=(char&)*tempFile;
		tempFile+=1;
		if (version==8)
		{
			itemTable=(int&)*tempFile;
			tempFile+=4;
			itemDrops=(int&)*tempFile;
			tempFile+=4;
			if (itemDrops>0)
				for(int i=itemDrops;i>0;i--)
				{
					int set=(int&)*tempFile;
					tempFile+=(8*set + 4);
				}
		}
		else
		{
			itemTable=-1;
			itemDrops=0;
		}
		tempFile+=4;
		if ((life[jNum]==100 || life[jNum]==0) && itemTable==-1 && itemDrops==0)
			jNum++;
		else
		{
			n--;
			memcpy(destDStrPos,tempStr,tempFile-tempStr);
			destDStrPos+=tempFile-tempStr;
		}
	}

	int x=dBuffer+dFileSize-tempFile;
	memcpy(destDStrPos,tempFile,x);
	destDStrPos+=x;

	tempFile+=4;
	int num=(int&)*tempFile;		//悬崖/地形装饰物
	tempFile+=4;
	for(;num>0;num--)
	{
		objectUsed[OBJHash(tempFile)]=true;
		tempFile+=16;
	}

	if (jNum == 0)
	{
		free(destID);
		free(destDStr);
		AddBufferFile("war3map.doo",dBuffer,dFileSize,1);
		return 0;
	}
	else
	{
		dooNum -= jNum;
		memcpy(destDStr+12,(void*)&dooNum,4);
	}

	char* destJStr = (char*)malloc(jFileSize+(45+(8+digNum)*5)*jNum);
	char* destJStrPos = destJStr;
	for(int i=0;i<strNum;i++)
	{
		memcpy(destJStrPos,jStr[i],jStrLen[i]);
		destJStrPos+=jStrLen[i];
		for(int j=jNum*i/strNum,k=jNum*(i+1)/strNum; j<k; j++)
		{
			if (life[j]==0)
			{
				memcpy(destJStrPos,"call CreateDeadDestructableZ('",30);
				destJStrPos+=30;
			}
			else
			{
				memcpy(destJStrPos,"call CreateDestructableZ('",26);
				destJStrPos+=26;
			}
			memcpy(destJStrPos,(void*)&destID[j],4);
			destJStrPos+=4;
			memcpy(destJStrPos,"',",2);
			destJStrPos+=2;
			WriteFloatToBuffer(destJStrPos,pos_x[j],digNum,true);
			memcpy(destJStrPos,",",1);
			destJStrPos+=1;
			WriteFloatToBuffer(destJStrPos,pos_y[j],digNum,true);
			memcpy(destJStrPos,",",1);
			destJStrPos+=1;
			WriteFloatToBuffer(destJStrPos,pos_z[j],digNum,true);
			memcpy(destJStrPos,",",1);
			destJStrPos+=1;
			WriteFloatToBuffer(destJStrPos,face[j],digNum,true);
			memcpy(destJStrPos,",",1);
			destJStrPos+=1;
			WriteFloatToBuffer(destJStrPos,scale[j],digNum,true);
			memcpy(destJStrPos,",",1);
			destJStrPos+=1;
			WriteIntToBuffer(destJStrPos,var[j]);
			memcpy(destJStrPos,")\n",2);
			destJStrPos+=2;
		}
	}
	memcpy(destJStrPos,jStr[strNum],jStrLen[strNum]);
	destJStrPos+=jStrLen[strNum];
	free(jFileBuffer);
	jFileBuffer=destJStr;
	jFileSize=destJStrPos-destJStr;
	free(destID);
	AddBufferFile("war3map.doo",destDStr,destDStrPos-destDStr,1);
	free(dBuffer);
	return 0;
}

int ConvJScript()
{
	char* tempFile=jFileBuffer;
	if (*tempFile=='\xef' && tempFile[1]=='\xbb' && tempFile[2]=='\xbf')
		tempFile+=3;
    char* lineStr=tempFile;
    char* nextLineStr=tempFile;
    jFileBuffer[jFileSize]='\n';
	jFileBuffer[jFileSize+1]=NULL;
    jFileBuffer[jFileSize+2]='\n';
	char* destStr=(char*)malloc(jFileSize+wtsFileSize+8);
	char* destStrPos=destStr;
    int lineLen;
	for(;;)
	{
        lineStr=nextLineStr;
        if (*lineStr==NULL)
        {
			*destStrPos--=NULL;
			free(jFileBuffer);
			AddBufferFile(jFilePath,destStr,destStrPos-destStr,1);
			return 0;
        }
        for(;*nextLineStr!='\n' && *nextLineStr!='\r';nextLineStr++)
            ;
        *(nextLineStr++)=NULL;
        for(;*nextLineStr=='\n' || *nextLineStr=='\r';nextLineStr++)
            ;
        for(;*lineStr==' '|| *lineStr=='\t';lineStr++)
            ;
        if (*lineStr=='/')
            continue;

        lineLen = nextLineStr-lineStr;
        for(;*lineStr!=NULL;)
        {
            if (*lineStr=='"' && (memcmp(lineStr+1,"TRIGSTR_",8)==0))
            {
                *(destStrPos++) = *(lineStr++);
                int i = a2ui(lineStr+8);
                if (i<=wtsStringNum && wtsString[i]!=NULL)
                {
                    lineLen+=strlen(wtsString[i]);
                    if (lineLen<1020)   //Jass行字符不能超过1024
                    {
                        for(char* str=wtsString[i];*str!=NULL;)
                        {
                            if (*str=='\\' || *str=='\"')
                                *(destStrPos++)='\\';
                            else if (*str=='\r' || *str=='\n')
                            {
                                if (*str=='\r' && str[1]=='\n')
                                    str+=2;
                                else
                                    str++;
                                *(destStrPos++)='\\';
                                *(destStrPos++)='n';
                                continue;
                            }
                            *(destStrPos++) = *(str++);
                        }
                    }
                    else
                    {
                        NewWtsStr(wtsString[i]);
                        memcat(destStrPos,"TRIGSTR_",8);
                        WriteIntToBuffer(destStrPos,newWtsStrNum);
                    }
                    for(lineStr+=8;*lineStr!='"';lineStr++)
                        ;
                }
            }
            *(destStrPos++) = *(lineStr++);
		}
		*(destStrPos++) = '\n';
	}
}

int ConvWar3MapMisc()
{
	HANDLE hFile;
	char* fileStr;
	char* tempStr;
	char* lineStr;

	int size=0;
	if (SFileOpenFileEx(g_hMPQ,"war3mapmisc.txt",NULL,&hFile))
	{
		size=SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
		fileStr=(char*)malloc(size+4);
		SFileReadFile(hFile,fileStr,size,NULL,NULL);
		SFileCloseFile(hFile);
	}
	else
		fileStr=(char*)calloc(4,1);
	fileStr[size]='\n';
	fileStr[size+1]='[';
	fileStr[size+2]='\n';
	fileStr[size+3]=NULL;
	tempStr=fileStr;
	if (tempStr[0]==-17 && tempStr[1]==-69 && tempStr[2]==-65)	//WTS UTF-8文件头
		tempStr+=3;

	char* destStr=(char*)malloc(size+1024);
	char* destStrPos=destStr;
	char exSet[5]={};
	char mode=0;
	char fd[3]={};
	char* fdString[3]={"Name=","DependencyOr=","XPFactor="};
	for(;;)
	{
		lineStr=tempStr;	//循环读取行字符串
		for(;*tempStr!='\r' && *tempStr!='\n';tempStr++)
			;

		for(*(tempStr++)=NULL;*tempStr=='\r' || *tempStr=='\n';tempStr++)
			;

		if (*lineStr == '[')
		{
			if (mode>0)
			{
				for(int i=0;i<3;i++)
				{
					if (fd[i]==0)
					{
						strcat(destStrPos,fdString[i]);
						strcat(destStrPos,extraString[mode-1][i+1]);
						chrcat(destStrPos,'\n');
					}
				}
			}
			if (*(lineStr+2)==NULL)	//文件结束标志
				goto Misc_End;
			mode=0;
			switch ((int&)*(lineStr+1))
			{
			case 'OREH':
				mode=1;
				exSet[0]=1;
				fd[0]=0,fd[1]=0,fd[2]=1;
				break;
			case 'TLAT':
				mode=2;
				exSet[1]=1;
				fd[0]=0,fd[1]=0,fd[2]=1;
				break;
			case '1NWT':
				mode=3;
				exSet[2]=1;
				fd[0]=0,fd[1]=0,fd[2]=1;
				break;
			case '2NWT':
				mode=4;
				exSet[3]=1;
				fd[0]=0,fd[1]=0,fd[2]=0;
				break;
			case '3NWT':
				mode=5;
				exSet[4]=1;
				fd[0]=0,fd[1]=0,fd[2]=0;
				break;
			}
		}
		else if (mode>0)
		{
			for(int n=0;n<3;n++)
			{
				int i=0;
				for(;lineStr[i]==fdString[n][i];i++)
					;
				if (fdString[n][i]==NULL)
				{
					fd[n]=1;
					break;
				}
			}
		}
		strcat(destStrPos,lineStr);
		*(destStrPos++)='\n';
	}
Misc_End:
	if (exSet[0]==0)
	{
		memcat(destStrPos,"[HERO]\nName=",12);
		strcat(destStrPos,extraString[0][1]);
		memcat(destStrPos,"\nDependencyOr=Hamg,Hblm,Hmkg,Hpal,Obla,Ofar,Oshd,Otch,Edem,Ekee,Emoo,Ewar,Ucrl,Udea,Udre,Ulic,Npbm,Nbrn,Nngs,Nplh,Nbst,Nalc,Ntin,Nfir\n",134);
	}
	if (exSet[1]==0)
	{
		memcat(destStrPos,"[TALT]\nName=",12);
		strcat(destStrPos,extraString[1][1]);
		memcat(destStrPos,"\nDependencyOr=halt,oalt,eate,uaod\n",34);
	}
	if (exSet[2]==0)
	{
		memcat(destStrPos,"[TWN1]\nName=",12);
		strcat(destStrPos,extraString[2][1]);
		memcat(destStrPos,"\nDependencyOr=htow,ogre,etol,unpl\n",34);
	}
	if (exSet[3]==0)
	{
		memcat(destStrPos,"[TWN2]\nName=",12);
		strcat(destStrPos,extraString[3][1]);
		memcat(destStrPos,"\nDependencyOr=hkee,ostr,etoa,unp1\nXPFactor=.15,0\n",49);
	}
	if (exSet[4]==0)
	{
		memcat(destStrPos,"[TWN3]\nName=",12);
		strcat(destStrPos,extraString[4][1]);
		memcat(destStrPos,"\nDependencyOr=hcas,ofrt,etoe,unp2\nXPFactor=.3,0\n",48);
	}

	free(fileStr);
	AddBufferFile("war3mapMisc.txt",destStr,destStrPos-destStr,1);
	return 0;
}

void WriteNewWts()
{
	if (newWtsStrNum==0)
		return;
	HANDLE hFile;
	SFileOpenFileEx(g_hMPQ,"war3map.wts",NULL,&hFile);
	int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
	SFileCloseFile(hFile);
    char* fileStr = Malloc(size);
    char* tempStr=fileStr;
    for (int i=0;i<newWtsStrNum;)
    {
        i++;
        memcat(tempStr,"STRING ",7);
        WriteIntToBuffer(tempStr,i);
        memcat(tempStr,"\r\n{\r\n",5);
        strcat(tempStr,newWtsStr[i]);
        memcat(tempStr,"\r\n}\r\n",5);
    }
    AddBufferFile("war3map.wts",fileStr,tempStr-fileStr,0);
}

extern void SearchInObjectList(char*);
extern void ExWriteStr(char*&,char*,bool);
struct ObjectData
{
	char w3oType;
	char optimizer;

	int* slkFieldId;//[SLK_HASH_MAX];						//SLK相关
	char** slkField;//[MAX_SLK_FIELD_NUM+21];
	int slkFieldCount;//=0;
	int objectCount;
	int orgObjectCount;
	int usedObjectCount;
	int* usedObjectId;
	int* usedObjectLevel;
	char*** slkData;
	int* slkFieldPoint;//[10];//={};
	bool* slkFieldNoEmpty;
	int* slkObjLevel;

	int* txtFieldId;//[TXT_HASH_MAX];						//TXT相关
	char** txtField;//[MAX_TXT_FIELD_NUM];
	char* txtFieldArray;//[MAX_TXT_FIELD_NUM];
	char* txtFieldIndex;//[MAX_TXT_FIELD_NUM];
	int txtFieldCount;
	char*** txtData;

	int* w3oFieldId;//[W3O_HASH_MAX];//={};					//W3O相关
	char** w3oField;//[W3O_FIELD_NUM];//={};
	int w3oFieldCount;//=0;
	char** w3oFieldFile;//[W3O_FIELD_NUM];//={};
	char* w3oFieldIndex;//[W3O_FIELD_NUM];//={};
	int* w3o2slkFieldId;//[W3O_FIELD_NUM];				//W3O转化SLK TXT
	#define w3o2txtFieldId w3o2slkFieldId

	int w3oObjectNum;							//物件数量
	int w3oOObjectNum;							//原始物件数量
	int w3oCObjectNum;							//自定义物件数量
	int* w3oOrgObject;							//引用物件
	int* w3oObject;								//W3O文件中记录的物件	w3oObjectNum
	int* w3oObjEditNum;							//每个物件的修改项数量	w3oObjectNum
	int* w3oObjLevel;							//该物件等级

	int** w3oDataField;							//修改项的Field项		w3oObjectNum & w3oObjectEditNum
	int** w3oDataType;							//修改项类型
	int** w3oDataLevel;							//修改项等级
	int** w3oDataIndex;
	char*** w3oData;							//修改项数据

	bool* objectSearched;
	int slkFieldForSearch[10];
	int slkFieldCountForSearch;
	int txtFieldForSearch[20];
	int txtFieldCountForSearch;

	void Init(char forOpt)
	{
		optimizer=forOpt;
		slkFieldId=(int*)Calloc(SLK_HASH_MAX*4);
		slkField=(char**)Calloc((MAX_SLK_FIELD_NUM+21)*4);
		slkFieldCount=0;
		objectCount=0;
		orgObjectCount=0;
		usedObjectCount=0;
		slkFieldPoint=(int*)Calloc(10*4);//={};
		slkFieldNoEmpty=(bool*)Calloc(255);

		txtFieldId=(int*)Calloc(TXT_HASH_MAX*4);
		txtField=(char**)Calloc(MAX_TXT_FIELD_NUM*4);
		txtFieldArray=(char*)Calloc(MAX_TXT_FIELD_NUM);
		txtFieldIndex=(char*)Calloc(MAX_TXT_FIELD_NUM);
		txtFieldCount=0;
		txtData=NULL;

		w3oFieldId=(int*)Calloc(W3O_HASH_MAX*4);

		slkFieldCountForSearch=0;
		txtFieldCountForSearch=0;
	}

	int WriteW3O(char* w3oPath, char haveExtraField)
	{
		if (w3oObjectNum==0)
			return 0;
		HANDLE hFile;
		SFileOpenFileEx(g_hMPQ,w3oPath,NULL,&hFile);
		char* destStr=Malloc(SFileGetFileInfo(hFile,SFILE_INFO_SIZE)+wtsFileSize);
		char* destStrPos=destStr;
		SFileCloseFile(hFile);
		int v=2;
		memcat(destStrPos,(void*)&v,4);
		memcat(destStrPos,(void*)&w3oOObjectNum,4);
		for(int i=1;i<=w3oOObjectNum;i++)
		{
			memcat(destStrPos,(void*)(w3oObject+i),4);
			memcat(destStrPos,"\0\0\0\0",4);				//0000
			memcat(destStrPos,(void*)(w3oObjEditNum+i),4);
			for(int j=1;j<=w3oObjEditNum[i];j++)
			{
				memcat(destStrPos,(void*)(w3oDataField[i]+j),4);
				memcat(destStrPos,(void*)(w3oDataType[i]+j),4);
				if (haveExtraField)
				{
					memcat(destStrPos,(void*)(w3oDataLevel[i]+j),4);
					memcat(destStrPos,(void*)(w3oDataIndex[i]+j),4);
				}
				if (w3oDataType[i][j]==3)
				{
					/*if (memcmp(w3oData[i][j],"TRIGSTR_",8)==0)
					{
						int n=a2ui(w3oData[i][j]+8);
						w3oData[i][j]=newWtsStr[n];
						newWtsStrNum--;
					}*/
					switch (w3oDataField[i][j])
					{
					case '1bua':
					case 'tura':
					case '1uua':
					case 'butu':
					case '1bug':
						if (optimizer)
							ExWriteStr(destStrPos,w3oData[i][j],false);
						else
							strcat(destStrPos,w3oData[i][j]);
						break;
					default:
						strcat(destStrPos,w3oData[i][j]);
					}
					*(destStrPos++)=NULL;
				}
				else
					memcat(destStrPos,w3oData[i][j],4);
				memcat(destStrPos,"\0\0\0\0",4);			//0000
			}
		}
		memcat(destStrPos,(void*)&w3oCObjectNum,4);			//自定义Obj
		for(int i=w3oOObjectNum+1;i<=w3oObjectNum;i++)
		{
			memcat(destStrPos,(void*)(w3oOrgObject+i),4);
			memcat(destStrPos,(void*)(w3oObject+i),4);
			memcat(destStrPos,(void*)(w3oObjEditNum+i),4);
			for(int j=1;j<=w3oObjEditNum[i];j++)
			{
				memcat(destStrPos,(void*)(w3oDataField[i]+j),4);
				memcat(destStrPos,(void*)(w3oDataType[i]+j),4);
				if (haveExtraField)
				{
					memcat(destStrPos,(void*)(w3oDataLevel[i]+j),4);
					memcat(destStrPos,(void*)(w3oDataIndex[i]+j),4);
				}
				if (w3oDataType[i][j]==3)
				{
					//if (memcmp(w3oData[i][j],"TRIGSTR_",8)==0)
					//{
					//	int n=a2ui(w3oData[i][j]+8);
					//	w3oData[i][j]=newWtsStr[n];
					//	newWtsStrNum--;
					//}
					strcat(destStrPos,w3oData[i][j]);
					*(destStrPos++)=NULL;
				}
				else
					memcat(destStrPos,w3oData[i][j],4);
				memcat(destStrPos,"\0\0\0\0",4);			//0000
			}
		}

		g_CachePos=destStrPos;
		AddBufferFile(w3oPath,destStr,destStrPos-destStr,0);
		return 0;
	}

	int ReadW3O(char* w3oPath,char haveExtraField)
	{
		w3oType=w3oPath[10];	//war3map.w3_
		MPQHANDLE hFile;
		if (!SFileOpenFileEx(g_hMPQ,w3oPath,NULL,&hFile))
			return ERR_FILE_NOT_FOUND;
		int size=SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
		char* fileStr=Malloc(size+10);
		SFileReadFile(hFile,fileStr,size,NULL,NULL);
		SFileCloseFile(hFile);
		memset(fileStr+size,0,10);
		char* tempFile=fileStr;

		//if ((int &)*tempFile!=2)	//head=02
		//return -1;
		tempFile+=4;
		w3oOObjectNum=(int &)*tempFile;	//ObjectNum
		tempFile+=4;
		for(int i=w3oOObjectNum;i>0;i--)
		{
			tempFile+=8;
			int objEditNum=(int &)*tempFile;
			tempFile+=4;
			for(;objEditNum>0;objEditNum--)
			{
				tempFile+=4;
				int type=(int &)*tempFile;
				tempFile+=4;
				if (haveExtraField>0)
					tempFile+=8;
				if (type==3)
				{
					for(;*tempFile!=NULL;tempFile++)
						;
					tempFile++;
				}
				else
					tempFile+=4;
				tempFile+=4;
			}
		}
		w3oCObjectNum=(int &)*tempFile;
		w3oObjectNum=w3oOObjectNum+w3oCObjectNum;
		
		w3oObject = (int*)Malloc((w3oObjectNum+1)*4*9);		//分配内存空间供以下变量使用
		w3oOrgObject = w3oObject+w3oObjectNum+1;
		w3oObjEditNum = w3oOrgObject+w3oObjectNum+1;
		w3oObjLevel = w3oObjEditNum+w3oObjectNum+1;
		w3oDataField = (int**)(w3oObjLevel+w3oObjectNum+1);
		w3oDataType = w3oDataField+w3oObjectNum+1;
		w3oDataLevel = w3oDataType+w3oObjectNum+1;
		w3oDataIndex = w3oDataLevel+w3oObjectNum+1;
		w3oData = (char***)(w3oDataIndex+w3oObjectNum+1);
//set [0]=>NULL
		tempFile=fileStr+4;
		int objCnt=0;
		int max=0;
		for (int x=0;x<2;x++)
		{
			max+=(int &)*tempFile;
			tempFile+=4;
			for(;objCnt<max;)
			{
				objCnt++;
				if (haveExtraField)
					w3oObjLevel[objCnt]=objectLevel[OBJHash(tempFile)];		//origin object level
				if (objCnt>w3oOObjectNum)	//custom object
				{
					w3oOrgObject[objCnt]=(int &)*tempFile;
					tempFile+=4;
					w3oObject[objCnt]=(int &)*tempFile;
					tempFile+=4;
				}
				else		//origin object
				{
					w3oObject[objCnt]=(int &)*tempFile;
					w3oOrgObject[objCnt]=NULL;
					tempFile+=8;
				}
				w3oObjEditNum[objCnt]=(int &)*tempFile;
				tempFile+=4;
				w3oDataField[objCnt]=(int*)Calloc((w3oObjEditNum[objCnt]+1)*5*4);
				w3oDataType[objCnt]=w3oDataField[objCnt]+w3oObjEditNum[objCnt]+1;
				w3oDataLevel[objCnt]=w3oDataType[objCnt]+w3oObjEditNum[objCnt]+1;
				w3oDataIndex[objCnt]=w3oDataLevel[objCnt]+w3oObjEditNum[objCnt]+1;
				w3oData[objCnt]=(char**)w3oDataIndex[objCnt]+w3oObjEditNum[objCnt]+1;

				for(int i=0;i<w3oObjEditNum[objCnt];)
				{
					i++;
					w3oDataField[objCnt][i]=(int &)*tempFile;
					tempFile+=4;
					w3oDataType[objCnt][i]=(int &)*tempFile;
					tempFile+=4;
					if (haveExtraField)
					{
						w3oDataLevel[objCnt][i]=(int &)*tempFile;
						tempFile+=4;
						w3oDataIndex[objCnt][i]=(int &)*tempFile;
						tempFile+=4;
					}
					w3oData[objCnt][i]=tempFile;
					if (w3oDataType[objCnt][i]==3)	//以Null开头的str在ConvW3O中只有""会被保留
					{
						if (*tempFile=='T' && memcmp(tempFile+1,"RIGSTR_",7)==0)
						{
							int n=a2ui(tempFile+8);
							if (n>wtsStringNum || wtsString[n]==NULL)
								w3oData[objCnt][i]="";
							else
								w3oData[objCnt][i]=wtsString[n];
						}
						if (*szTxtNullMark!=NULL && strcmp(tempFile,szTxtNullMark)==0)
							w3oData[objCnt][i]="";

						for(;*tempFile!=NULL;tempFile++)	//search for NULL
							;
						tempFile++;

//						if (*txtNullMark!=NULL && strcmp(tempFile,txtNullMark)==0)
//							w3oData[objCnt][i]="";
//						else if (*tempFile=='T' && memcmp(tempFile+1,"RIGSTR_",7)==0)
//						{
//							int n=a2ui(tempFile+8);
//							if (n<=wtsStringNum)
//							{
//								if (wtsString[n]==NULL || (*txtNullMark!=NULL && strcmp(wtsString[n],txtNullMark)==0))
//									w3oData[objCnt][i]="";
//                                else if (optimizer)  //optimizer=1则字符最终会转到txt文件中，需要进行转换
//								{
//                                    bool a=false, b=false;
//                                    if (wtsString[n][0]=='"')
//										goto ReadW3O_L2;
//                                    for(char* s=wtsString[n];;s++)
//                                    {
//                                        if (*s=='"')
//                                            a=true;
//                                        else if (*s==',')
//                                            b=true;
//										else if (*s=='<')
//										{
//											int x=0;
//											for(s++;;s++)
//											{
//												if (*s==',')
//												{
//													x++;
//													if (x==2)
//													{
//														s+=2;
//														break;
//													}
//												}
//												else if (*s=='>')
//													break;
//												else if (*s==NULL)
//													goto ReadW3O_L1;
//											}
//										}
//                                        else if (*s=='\r')
//										{
//											if  (s[1]=='\n')
//												*(s++)='|',*s='n';
//											else
//												goto ReadW3O_L2;
//										}
//										else if (*s=='\n')
//											goto ReadW3O_L2;
//                                        else if (*s==NULL)
//                                            break;
//                                    }
//ReadW3O_L1:
//                                    if (a && b)
//                                    {
//ReadW3O_L2:
//                                        NewWtsStr(wtsString[n]);
//										tempFile+=8;
//										char* ss = tempFile;
//										for(;*tempFile!=NULL;tempFile++)
//											;
//                                        i2a(newWtsStrNum,ss);
//                                    }
//                                    else
//                                        w3oData[objCnt][i]=wtsString[n];
//                                }
//                                else
//                                    w3oData[objCnt][i]=wtsString[n];
//                            }
//						}
//						for(;*tempFile!=NULL;tempFile++)	//search for NULL
//							;
//						tempFile++;
					}
					else
						tempFile+=4;
					tempFile+=4;		//结束标志,无用数据
					if (haveExtraField && (w3oDataField[objCnt][i]=='vela' || w3oDataField[objCnt][i]=='lvlg' || w3oDataField[objCnt][i]=='ravd'))	//'abcd'=>"dcba"
						w3oObjLevel[objCnt]=(int&)*w3oData[objCnt][i];
				}
			}
		}
		if (optimizer==0)
		{
			if (wtsForOptimizer)
				WriteW3O(w3oPath,haveExtraField);
			else
				AddBufferFile(w3oPath,fileStr,size,0);
		}
		return 0;
	}

	BOOL ReadSLK(char* slkPath, int index)
	{
		MPQHANDLE hFile;
		int size;
		char* fileStr;
		if (SFileOpenFileEx(g_hMPQ,slkPath,NULL,&hFile))
		{
			size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
			fileStr=Malloc(size+1);
			SFileReadFile(hFile,fileStr,size,NULL,NULL);
			SFileCloseFile(hFile);
		}
		else
		{
			int fp;
			if (gameDataType==1)
				fp=open(StrAddTemp("Input\\Custom_V1\\",slkPath),_O_BINARY);
			if (gameDataType!=1 || fp<=0)
				fp=open(StrAddTemp("Input\\",slkPath),_O_BINARY);
			if (fp<=0)
				return FALSE;
			size=filelength(fp);
			fileStr=Malloc(size+1);
			read(fp,fileStr,size);
			close(fp);
		}
		fileStr[size]='\n';

		char*** tempData;
		char* tempStr=fileStr;
		unsigned int slkMaxX,slkMaxY;
		unsigned int X[128]={};
		unsigned int x=0,y=0;
		int filePoint;

		InitSlkOptField(slkPath);

		for(;*tempStr!='\r' && *tempStr!='\n';tempStr++)
			;
		for(;*tempStr=='\r' || *tempStr=='\n';tempStr++)
			;
		for(;;)
		{
			if (*tempStr=='C')
			{
				for(tempStr++;;)
				{
					switch(*(++tempStr))
					{
					case 'K':
						tempData[y-1][x]=tempStr+1;
						goto readslk_L1;
					case 'X':
						tempStr++;
						x=0;
						for(;;)
						{
							if (*tempStr>'9' || *tempStr<'0')
								break;
							else
								x=(*(tempStr++) & 0xf)+x*10;
						}
						break;
					case 'Y':
						tempStr++;
						y=0;
						for(;;)
						{
							if (*tempStr>'9' || *tempStr<'0')
								break;
							else
								y=(*(tempStr++) & 0xf)+y*10;
						}
						break;
					}
				}
			}
			else if (*tempStr=='B')
			{
				slkMaxX=a2ui((char*)strchr(tempStr,'X')+1);
				slkMaxY=a2ui((char*)strchr(tempStr,'Y')+1);
				if (index==0)			//主slk文件
				{
					objectCount=0;
					*slkFieldPoint=0;
					slkFieldCount=0;
					slkData=(char***)Calloc((slkMaxY+w3oCObjectNum)*4);
					*slkData=(char**)Calloc((slkMaxY+w3oCObjectNum)*(MAX_SLK_FIELD_NUM+21)*4);
					for(int i=1;i<((int)slkMaxY+w3oCObjectNum);i++)
						slkData[i]=slkData[i-1]+MAX_SLK_FIELD_NUM+21;
				}
				
				//if (g_CachePos+slkMaxY*4*(slkMaxX+2) > g_CacheEnd)
				//	tempData=(char***)Calloc(slkMaxY*4*(slkMaxX+2));
				//else
				//	tempData=(char***)g_CachePos,memset(g_CachePos,0,slkMaxY*4*(slkMaxX+2));
				//*tempData=(char**)(tempData+slkMaxY);
				tempData=(char***)Calloc(slkMaxY*4);
				*tempData=(char**)Calloc(slkMaxY*(slkMaxX+1)*4);
				for(int i=1;i<slkMaxY;i++)
					tempData[i]=tempData[i-1]+slkMaxX+1;
			}
			else if (*tempStr=='E' || tempStr[1]!=';')
				break;
readslk_L1:
			for(;*tempStr!='\r' && *tempStr!='\n';tempStr++)
				;
			if (tempStr[-1]=='"')
				tempStr[-1]=NULL;
			else
				tempStr[0]=NULL;
			if (tempStr[1]=='\n')
				tempStr+=2;
			else
				tempStr++;
		}

		filePoint=slkFieldPoint[index];
		for(int i=1,j=0;i<=slkMaxX;i++)	//Field
		{
			if (tempData[0][i]==NULL)
				continue;
			int h = SLKHash(tempData[0][i]+1);
			if (slkOptField[h])
			{
				slkFieldCount++;
				X[slkFieldCount-filePoint]=i;
				slkField[slkFieldCount]=tempData[0][i]+1;
				slkFieldId[h]=slkFieldCount;
				if (fieldNoEmpty[h])
					slkFieldNoEmpty[slkFieldCount]=true;
			}
			/*slkField[i+filePoint]=tempData[0][i]+1;
			tempInt=SLKHash(slkField[i+filePoint]);
			slkFieldId[tempInt]=i+filePoint;
			if (fieldNoEmpty[tempInt])
				slkFieldNoEmpty[i+filePoint]=true;*/
		}
		slkMaxX=slkFieldCount-filePoint;
		slkFieldPoint[index+1]=slkFieldCount;

		int oid;
		for(int y=1;y<slkMaxY;y++)
		{
			if (tempData[y][1]==NULL)
				continue;
			if (index==0)			//主slk文件
			{
				if (tempData[y][1]==NULL)
					continue;
				oid=(++objectCount);
				objectId[OBJHash(tempData[y][1]+1)]=objectCount;
				slkData[oid][1]=tempData[y][1];
			}
			else
			{
				if (tempData[y][1]==NULL)
					continue;
				oid=objectId[OBJHash(tempData[y][1]+1)];
				if (oid>objectCount || oid<1)
					continue;
			}
			for(int x=2;x<=slkMaxX;x++)
			{
				char* str = tempData[y][X[x]];
				if (str==NULL)
					continue;
				if (*str=='0')
					str++;
				else if ((*str=='-') && (str[1]=='0'))
					*(++str)='-';
				else if (! slkFieldNoEmpty[x+filePoint] && (*str==NULL || /*(str[1]=='_' && str[2]==NULL) ||*/ (str[1]=='-' && str[2]==NULL) || (str[1]==' ' && str[2]=='-' && str[3]==' ' && str[4]==NULL)))
					str = NULL;		//清空多余数据
				slkData[oid][x+filePoint]=str;
			}
		}

    return TRUE;
	}

	int ReadMetaSLK(char* slkPath)
	{
		MPQHANDLE slkFile;
		int size;
		char* fileStr;
		if (SFileOpenFileEx(g_hMPQ,slkPath,NULL,&slkFile))
		{
			size = SFileGetFileInfo(slkFile,SFILE_INFO_SIZE);
			fileStr=Malloc(size+1);
			SFileReadFile(slkFile,fileStr,size,NULL,NULL);
			SFileCloseFile(slkFile);
		}
		else
		{
			int fp=open(StrAddTemp("Input\\",slkPath),_O_BINARY);
			if (fp<=0)
				return ERR_FILE_NOT_FOUND;
			size=filelength(fp);
			fileStr=Malloc(size+1);
			read(fp,fileStr,size);
			close(fp);
		}
		fileStr[size]='\n';
		int slkLevelFieldId=MAX_SLK_FIELD_NUM;
		int i_ID=0, i_field=0, i_slk=0, i_index=0, i_repeat=0, i_data=0;
		char * tempData[32]={};
		char * tempFile;
		char * lineStr="";
		int x=0,y=0;
		tempFile=fileStr;

		for(;;)
		{
			lineStr=tempFile;									//读取行字符串
			for(;*tempFile!='\r' && *tempFile!='\n';tempFile++)
				;
			if (tempFile[-1]=='"')
				tempFile[-1]=NULL;
			else
				tempFile[0]=NULL;
			if (tempFile[1]=='\n')
				tempFile+=2;
			else
				tempFile++;

			if (*lineStr=='C')		//读取Field数据
			{
				for(lineStr+=2;*lineStr!='X';lineStr++)
					;
				x=a2i(++lineStr);
				for(lineStr+=2;*lineStr!='K';lineStr++)
					;
				lineStr+=2;
				if (strcmp(lineStr,"ID")==0)
					i_ID=x;
				else if (strcmp(lineStr,"field")==0)
					i_field=x;
				else if (strcmp(lineStr,"slk")==0)
					i_slk=x;
				else if (strcmp(lineStr,"index")==0)
					i_index=x;
				else if (strcmp(lineStr,"repeat")==0)
					i_repeat=x;
				else if (strcmp(lineStr,"data")==0)
					i_data=x;

				if (x==1)
					if (y==0)
						y++;
					else
					{
						w3oFieldCount++;
						tempData[1]=lineStr;
						break;
					}
			}
			else if (*lineStr=='B')
			{
				int x = a2ui((char*)memchr(lineStr,'Y',30)+1);
				w3oFieldCount=0;
				w3oField=(char**)Calloc(x*4);
				w3oFieldFile=(char**)Calloc(x*4);
				w3oFieldIndex=(char*)Calloc(x);
				w3o2slkFieldId=(int*)Calloc(x*4);
			}
		}

		for(;;)
		{
			lineStr=tempFile;									//读取行字符串
			for(;*tempFile!='\r' && *tempFile!='\n';tempFile++)
				;
			if (tempFile[-1]=='"')
				tempFile[-1]=NULL;
			else
				tempFile[0]=NULL;
			if (tempFile[1]=='\n')
				tempFile+=2;
			else
				tempFile++;

			if (*lineStr=='C')		//读取Data数据
			{
				for(lineStr+=2;*lineStr!='X';lineStr++)
					;
				lineStr++;
				x=0;
				for(;;)
				{
					if (*lineStr>'9' || *lineStr<'0')
						break;
					else
					{
						x=(*lineStr & 0xf)+x*10;
						lineStr++;
					}
				}

				for(;*lineStr!='K';lineStr++)
					;
				lineStr++;
				if (*lineStr=='"' || *lineStr=='0')
					lineStr++;
				if (x==1)	//读取数据:Id项
				{
					w3oFieldId[W3OHash(tempData[i_ID])]=w3oFieldCount;		//上一行数据结算
					w3oField[w3oFieldCount]=tempData[i_ID];
					w3oFieldFile[w3oFieldCount]=tempData[i_slk];
					if (strcmp(tempData[i_slk],"Profile")==0)
					{
						if (txtFieldId[TXTHash(tempData[i_field])]==0)
						{
							txtFieldCount++;
							txtFieldId[TXTHash(tempData[i_field])]=txtFieldCount;
							txtField[txtFieldCount]=tempData[i_field];
							if (tempData[i_repeat]!=NULL && tempData[i_repeat][0]<='9' && tempData[i_repeat][0]>='1')
								txtFieldArray[txtFieldCount]=1;
						}
						w3o2txtFieldId[w3oFieldCount]=txtFieldId[TXTHash(tempData[i_field])];
						if (tempData[i_index]!=NULL && tempData[i_index][0]!='0')	//index:only -1,0,1
						{
							w3oFieldIndex[w3oFieldCount]=a2i(tempData[i_index]);
							txtFieldIndex[w3o2txtFieldId[w3oFieldCount]]=w3oFieldIndex[w3oFieldCount];
						}
					}
					else
					{
						if (tempData[i_data]!=NULL && tempData[i_data][0]<='9' && tempData[i_data][0]>='1')
							tempData[i_field]=abiDataField[tempData[i_data][0]-'0'];
						if ((tempData[i_repeat]!=NULL && tempData[i_repeat][0]<='9' && tempData[i_repeat][0]>='1') && slkFieldId[SLKHash(tempData[i_field])]==0)
						{
							slkFieldId[SLKHash(tempData[i_field])]=++slkLevelFieldId;
							slkField[slkLevelFieldId]=tempData[i_field];
						}
						w3o2slkFieldId[w3oFieldCount]=slkFieldId[SLKHash(tempData[i_field])];
					}

					w3oFieldCount++;
					memset(tempData+1,0,31*4);
				}
				else if (*lineStr==NULL || (*lineStr=='_' && lineStr[1]==NULL) || (*lineStr=='-' && lineStr[1]==NULL) || (*lineStr==' ' && lineStr[1]=='-' && lineStr[2]==' ' && lineStr[3]==NULL))
					lineStr = NULL;		//清空多余数据.
				tempData[x]=lineStr;
				continue;
			}
			else if (lineStr[0]=='E')	//结束符号
			{
				if (strcmp(slkPath,"Units\\UpgradeMetaData.slk")==0)	//官方文件中缺少Requirescount项
				{
					txtFieldCount++;
					txtFieldId[TXTHash("Requirescount")]=txtFieldCount;
					txtField[txtFieldCount]="Requirescount";
				}

				w3oFieldId[W3OHash(tempData[i_ID])]=w3oFieldCount;		//上一行数据结算
				w3oField[w3oFieldCount]=tempData[i_ID];
				w3oFieldFile[w3oFieldCount]=tempData[i_slk];
				if (strcmp(tempData[i_slk],"Profile")==0)
				{
					if (txtFieldId[TXTHash(tempData[i_field])]==0)
					{
						txtFieldCount++;
						txtFieldId[TXTHash(tempData[i_field])]=txtFieldCount;
						txtField[txtFieldCount]=tempData[i_field];
						if (tempData[i_repeat]!=NULL && tempData[i_repeat][0]<='9' && tempData[i_repeat][0]>='1')
							txtFieldArray[txtFieldCount]=1;
					}
					w3o2txtFieldId[w3oFieldCount]=txtFieldId[TXTHash(tempData[i_field])];
					if (tempData[i_index]!=NULL && tempData[i_index][0]!='0')
					{
						w3oFieldIndex[w3oFieldCount]=a2i(tempData[i_index]);
						txtFieldIndex[w3o2txtFieldId[w3oFieldCount]]=w3oFieldIndex[w3oFieldCount];
					}
				}
				else
				{
					if (tempData[i_data]!=NULL && tempData[i_data][0]<='9' && tempData[i_data][0]>='1')
						tempData[i_field]=abiDataField[tempData[i_data][0]-'0'];
					if ((tempData[i_repeat]!=NULL && tempData[i_repeat][0]<='9' && tempData[i_repeat][0]>='1') && slkFieldId[SLKHash(tempData[i_field])]==0)
					{
						slkFieldId[SLKHash(tempData[i_field])]=++slkLevelFieldId;
						slkField[slkLevelFieldId]=tempData[i_field];
					}
					w3o2slkFieldId[w3oFieldCount]=slkFieldId[SLKHash(tempData[i_field])];
				}
				return 0;
			}
			else if (lineStr[1]!=';')	//格式错误
				return ERR_FILE_INVALID;
		}
	}

	int ReadTXT(char* txtPath, bool writeNullFile)
	{
		if (txtData==NULL)
		{
			txtData=(char***)Calloc((objectCount+w3oCObjectNum+1)*4);
			*txtData=(char**)Calloc((objectCount+w3oCObjectNum+1)*(txtFieldCount+1)*4);
			objectSearched=(bool*)Calloc(objectCount+w3oCObjectNum+1);
			for(int i=1;i<=(objectCount+w3oCObjectNum);i++)
				txtData[i]=txtData[i-1]+txtFieldCount+1;
		}

		MPQHANDLE txtFile;
		int size;
		char* fileStr;
		if (SFileOpenFileEx(g_hMPQ,txtPath,NULL,&txtFile))
		{
			size = SFileGetFileInfo(txtFile,SFILE_INFO_SIZE);
			fileStr=Malloc(size+4);
			SFileReadFile(txtFile,fileStr,size,NULL,NULL);
			SFileCloseFile(txtFile);
		}
		else
		{
			int fp;
			if (gameDataType==1)
				fp=open(StrAddTemp("Input\\Custom_V1\\",txtPath),_O_BINARY);
			if (gameDataType!=1 || fp<=0)
				fp=open(StrAddTemp("Input\\",txtPath),_O_BINARY);
			if (fp<=0)
				return ERR_FILE_NOT_FOUND;
			size=filelength(fp);
			fileStr=Malloc(size+4);
			read(fp,fileStr,size);
			close(fp);
		}
		fileStr[size]='\n';
		fileStr[size+1]='[';
		fileStr[size+2]='\n';
		fileStr[size+3]=NULL;
		char* tempFile=fileStr;
		char* lineStr;
		char* litStr;
		unsigned int obj,fld;
		int lv;
		if (tempFile[0]==-17 && tempFile[1]==-69 && tempFile[2]==-65)	//WTS UTF-8文件头
			tempFile+=3;

		for(;;)
		{
			lineStr=tempFile;									//读取行字符串
			for(;*tempFile!='\r' && *tempFile!='\n';tempFile++)
				;
			*(tempFile++)=NULL;
			for(;*tempFile=='\r' || *tempFile=='\n';tempFile++)
				;

			if (*lineStr=='[')					//读取Object对象
			{
				if (lineStr[1]==NULL && lineStr[2]==NULL)		//FuncEnd
					break;
				lineStr[5]=NULL;
					obj=OBJHash(++lineStr);
				lv=objectLevel[obj];
				obj=objectId[obj];
				char* s;
				if (obj>objectCount || obj==NULL || (s=slkData[obj][1])==NULL || s[1]!=lineStr[0] || s[2]!=lineStr[1] || s[3]!=lineStr[2] || s[4]!=lineStr[3])
				{
					for(;*tempFile!='[' || (tempFile[-1]!='\n' && tempFile[-1]!='\r');tempFile++)
						;
				}
			}
			else if (*lineStr=='/')	//注释
				;
			else if (*lineStr=='E' && lineStr[1]=='d')	//For Editor only
				;
			else
			{
				litStr=lineStr;							//读取行字符串
				for(;*lineStr!='=' && *lineStr!=NULL;lineStr++)	//search for '='
					;
				if (*lineStr==NULL)								//if not found,continue loop
					continue;
				*(lineStr++)=NULL;		//lineStr=data
				if (w3oType=='q' && litStr[0]=='R' && (litStr[8] | 32)!='c')	//appendIndex	//Requires or Requiresamount	//no Requirescount
				{
					if (strcmp(litStr+8,"amount")==0)
					{
						fld=txtFieldId[TXTHash("Requiresamount")];
						litStr+=14;
					}
					else
					{
						fld=txtFieldId[TXTHash("Requires")];
						litStr+=8;
					}
				}
				else
					fld=txtFieldId[TXTHash(litStr)];
				char isValue=txtFieldIsValue[TXTHash(txtField[fld])];
				if (*lineStr==NULL)
					;
				else if (txtFieldIndex[fld]>0)				//Index
				{
					if (txtData[obj][fld]==NULL)
						txtData[obj][fld]=Calloc(8);
					StringToArray(lineStr,(char**)txtData[obj][fld],2, (isValue!=FALSE));
				}
				else if (txtFieldArray[fld]>0 && lv>1)		// Level
				{
					if (w3oType=='q' && txtFieldIndex[fld]<0)	//仅Requires/Requiresamount符合	//appendIndex
					{
						if (txtData[obj][fld]==NULL)
							txtData[obj][fld]=Calloc(lv*4);
						char* s = ((char**)txtData[obj][fld])[a2ui(litStr)];
						if (s==NULL)
							((char**)txtData[obj][fld])[a2ui(litStr)]=lineStr;
						else
						{
							char* s2 = s;
							s = Malloc(strlen(s)+strlen(lineStr)+2);
							((char**)txtData[obj][fld])[a2ui(litStr)] = s;
							strcat(s,s2);
							if (s[-1]!=',')
								*(s++)=',';
							strcat(s,lineStr);
							*s=NULL;
						}
					}
					else
					{
						if (txtData[obj][fld]==NULL)
							txtData[obj][fld]=Calloc(lv*4);
						StringToArray(lineStr,(char**)txtData[obj][fld],lv, (isValue != FALSE));
					}
				}
				else
				{
					if (txtData[obj][fld]==NULL)
						txtData[obj][fld]=lineStr;
					else
					{
						char* s = Malloc(strlen(txtData[obj][fld])+strlen(lineStr)+2);
						char* s2 = txtData[obj][fld];
						txtData[obj][fld] = s;
						strcat(s,s2);
						if (s[-1]!=',')
							*(s++)=',';
						strcat(s,lineStr);
						*s=NULL;
					}

					if (txtFieldIndex[fld]==0)		//为-1,则无视','
					{
						if (*lineStr=='"')
						{
							bool b=false;
							for(;;)
							{
								lineStr++;
								if (*lineStr==',')
									b=true;
								else if (*lineStr=='"' || *lineStr==NULL)
									break;
							}
							if (!b)
							{
								txtData[obj][fld]++;
								*lineStr=NULL;
							}
							else
							{
								if (*lineStr=='"')
									lineStr[1]=NULL;
							}
						}
						else
						{
							for(;*lineStr!=NULL && *lineStr!=',';lineStr++)
								;
							*lineStr=NULL;
							if (*txtData[obj][fld]==NULL)
								txtData[obj][fld]=NULL;
							else if (isValue)
								txtData[obj][fld]=ValueString(txtData[obj][fld]);
						}
					}
				}
			}
		}
		if (writeNullFile && optimizer>0)
			AddBufferFile(txtPath,"",0,0);
		return 0;
	}

	struct ExW3oData
	{
		int field;
		char type;
		char index;
		char** data;
		int level;
		void Init(int f,char t,char i,char** d,int l)
		{
			field=f,type=t,index=i,data=d,level=l;
		}
	};
	bool NeedConvWts(char* s,bool exSet)
	{
		if (s==NULL || *s==NULL)
			return false;
		else if (*s=='"')
			return true;
		bool a=false,b=false;
		for(;;s++)
		{
			if (*s=='"')
				a=true;
			else if (*s==',')
				b=true;
			else if (*s=='<' && exSet)
			{
				int x=0;
				for(s++;;s++)
				{
					if (*s==',')
					{
						x++;
						if (x==2)
						{
							s+=2;
							break;
						}
					}
					else if (*s=='>')
						break;
					else if (*s==NULL)
						return false;
				}
			}
			else if (*s=='\r')
			{
				if  (s[1]=='\n')
					*(s++)='|',*s='n';
				else
					return true;
			}
			else if (*s=='\n')
				return true;
			else if (*s==NULL)
				return false;
		}
	}
	int ConvW3O(char* w3oPath, char haveExtraField)
	{
		ExW3oData* exData;
		int exDataCnt=0;
		char maxSlkLevel=0;
		if (w3oType=='a')
		{
			maxSlkLevel=4;
			exData=(ExW3oData*)Malloc(sizeof(ExW3oData)*20);
		}
		else if (w3oType=='d')
			maxSlkLevel=10;

		orgObjectCount=objectCount;
		int woCnt=1;
		char* tempStr=slkData[objectCount][1];
		slkData[objectCount][1]=Calloc((w3oCObjectNum+1)*6);
		strcpy(slkData[objectCount][1],tempStr);
		slkObjLevel=(int*)Malloc((objectCount+w3oCObjectNum+1)*4);
		memset(slkObjLevel,0xff,(objectCount+w3oCObjectNum+1)*4);
		for(int w3oObj=w3oOObjectNum;w3oObj<w3oObjectNum;)			//自定义Object项引用数据填充
		{
			exDataCnt=0;
			w3oObj++;
			objectCount++;
			int oid=objectCount;
			slkData[oid][1]=slkData[oid-1][1]+6;
			*slkData[oid][1]='\"';
			memcpy(slkData[oid][1]+1,(char*)&w3oObject[w3oObj],4);
			int org_oid=objectId[OBJHash((char*)&w3oOrgObject[w3oObj])];
			int clv=w3oObjLevel[w3oObj];
			for(int i=2;i<=slkFieldCount;i++)
				slkData[oid][i]=slkData[org_oid][i];
			for(int i=1;i<=txtFieldCount;i++)
			{
				if (txtFieldIndex[i]>0 && txtData[org_oid][i]!=NULL)
				{
					txtData[oid][i]=Calloc(8);
					((char**)txtData[oid][i])[0]=((char**)txtData[org_oid][i])[0];
					((char**)txtData[oid][i])[1]=((char**)txtData[org_oid][i])[1];
				}
				else if (txtFieldArray[i]>0 && txtData[org_oid][i]!=NULL)
				{
					int olv=objectLevel[OBJHash(slkData[org_oid][1]+1)];
					if (clv>1)
					{
						txtData[oid][i]=Calloc(clv*4);
						if (olv>1)
						{
							int n=(clv<olv?clv:olv)-1;
							for(;n>=0;n--)
								((char**)txtData[oid][i])[n]=((char**)txtData[org_oid][i])[n];
						}
						else
							((char**)txtData[oid][i])[0]=txtData[org_oid][i];
					}
					else
					{
						if (olv>1)
							txtData[oid][i]=((char**)txtData[org_oid][i])[0];
						else
							txtData[oid][i]=txtData[org_oid][i];
					}
				}
				else
					txtData[oid][i]=txtData[org_oid][i];
			}
			if (haveExtraField)
				slkObjLevel[oid]=clv;
			int oeCnt=0;							//自定义Object项W3O数据提取
			//bool useWTS=false;
			for(int fldCnt=0;fldCnt<w3oObjEditNum[w3oObj];)
			{
				fldCnt++;
				int fid=w3oFieldId[W3OHash((char*)&w3oDataField[w3oObj][fldCnt])];

				if (w3oFieldFile[fid]==NULL || (w3oDataLevel[w3oObj][fldCnt]>clv && w3oDataLevel[w3oObj][fldCnt]>1))
					continue;

				if (w3oData[w3oObj][fldCnt]==NULL)
					tempStr=NULL;
				else
				{
					switch (w3oDataType[w3oObj][fldCnt])
					{
					case 0:
						tempStr=i2a((int&)*w3oData[w3oObj][fldCnt]);
						break;
					case 1:
					case 2:
						tempStr=f2a((float&)*w3oData[w3oObj][fldCnt],4,true);
						break;
					case 3:
						tempStr=w3oData[w3oObj][fldCnt];
						if (*tempStr==NULL && tempStr!="")
							tempStr=NULL;
						//else if (*tempStr=='T' && memcmp(tempStr+1,"RIGSTR_",7)==0)
							//tempStr=NULL,useWTS=true;
					}
				}

				if (strcmp(w3oFieldFile[fid],"Profile")==0)	//txt
				{
					if (tempStr==NULL && *szTxtNullMark==NULL && txtFieldNoEmpty[TXTHash(txtField[w3o2txtFieldId[w3oFieldId[W3OHash((char*)&w3oDataField[w3oObj][fldCnt])]]])]>0)
						tempStr="";
					//else if (w3oDataType[w3oObj][fldCnt]==3 && tempStr!=NULL && w3oFieldIndex[w3oFieldId[W3OHash((char*)(w3oDataField[w3oObj]+fldCnt))]]>=0 && strchr(tempStr,',')!=NULL)
					//{
					//	int len=strlen(tempStr);
					//	tempStr[len]='\"';
					//	tempStr[len+1]=NULL;
					//	tempStr--;
					//	*tempStr='\"';
					//}
					else if (tempStr!=NULL && *tempStr=='0' && tempStr[1]==NULL && w3oDataType[w3oObj][fldCnt]<3)
						tempStr=NULL;
					if (txtFieldIndex[w3o2txtFieldId[fid]]>0)
					{
						if (txtData[oid][w3o2txtFieldId[fid]]==NULL)
							txtData[oid][w3o2txtFieldId[fid]]=Calloc(8);
						((char**)txtData[oid][w3o2txtFieldId[fid]])[w3oFieldIndex[fid]]=tempStr;
					}
					else if (w3oDataLevel[w3oObj][fldCnt]>0 && clv>1)
					{
						if (txtData[oid][w3o2txtFieldId[fid]]==NULL)
							txtData[oid][w3o2txtFieldId[fid]]=Calloc(clv*4);
						((char**)txtData[oid][w3o2txtFieldId[fid]])[w3oDataLevel[w3oObj][fldCnt]-1]=tempStr;
					}
					else
						txtData[oid][w3o2txtFieldId[fid]]=tempStr;
					bool exSet;
					switch (w3oDataField[w3oObj][fldCnt])
					{
					case '1bua':
					case 'tura':
					case '1uua':
					case 'butu':
					case '1bug':
						exSet=true;
						break;
					default:
						exSet=false;
					}
					if (NeedConvWts(tempStr,exSet))
						goto KeepW3OData;
				}
				else		//slk
				{
					if (w3oDataLevel[w3oObj][fldCnt]>maxSlkLevel)
					{
						if (slkData[oid][w3o2slkFieldId[fid]]==NULL)
						{
							slkData[oid][w3o2slkFieldId[fid]]=Calloc(clv*4);
							exData[exDataCnt++].Init(w3oDataField[w3oObj][fldCnt],w3oDataType[w3oObj][fldCnt],w3oDataIndex[w3oObj][fldCnt],(char**)slkData[oid][w3o2slkFieldId[fid]],clv);
						}
						((char**)slkData[oid][w3o2slkFieldId[fid]])[w3oDataLevel[w3oObj][fldCnt]-1]=w3oData[w3oObj][fldCnt];	//直接保存w3oData,写w3o时再修正
					}
					else
					{
						if (w3oDataType[w3oObj][fldCnt]==3 && tempStr!=NULL)
						{
                            if (w3oType != 'd')
                            {
                                if (*tempStr=='.' && w3o2slkFieldId[fid]==slkFieldId[SLKHash("file")])
                                    tempStr="\"_";
                                else
                                    *(--tempStr)='\"';
                            }
						}
						else if (tempStr!=NULL && *tempStr=='0' && tempStr[1]==NULL && w3oDataType[w3oObj][fldCnt]<3)
							tempStr=NULL;
						int slk_fid;
						if (w3oDataLevel[w3oObj][fldCnt]>0)
						{
							char* s;
							int dlv = w3oDataLevel[w3oObj][fldCnt];
							if (w3oType=='d' && dlv<10)
							{
								s=Malloc(3);
								s[0]='0';
								s[1]=dlv | '0';
								s[2]=NULL;
							}
							else
								s=i2a(dlv);
							slk_fid=slkFieldId[SLKHash(StrAddTemp(slkField[w3o2slkFieldId[fid]],s))];
							if (w3oDataLevel[w3oObj][fldCnt]==maxSlkLevel && clv>maxSlkLevel)
							{
								if (slkData[oid][w3o2slkFieldId[fid]]==NULL)
								{
									slkData[oid][w3o2slkFieldId[fid]]=Calloc(clv*4);
									exData[exDataCnt++].Init(w3oDataField[w3oObj][fldCnt],w3oDataType[w3oObj][fldCnt],w3oDataIndex[w3oObj][fldCnt],(char**)slkData[oid][w3o2slkFieldId[fid]],clv);
								}
								((char**)slkData[oid][w3o2slkFieldId[fid]])[maxSlkLevel-1]=w3oData[w3oObj][fldCnt];
								for(int i=maxSlkLevel;i<clv;i++)
								{
									if (((char**)slkData[oid][w3o2slkFieldId[fid]])[i]==NULL)
										((char**)slkData[oid][w3o2slkFieldId[fid]])[i]=w3oData[w3oObj][fldCnt];
								}
							}
						}
						else
							slk_fid=w3o2slkFieldId[fid];
						slkData[oid][slk_fid]=tempStr;
					}
					/*if (useWTS)
					{
						useWTS=false;
						if (w3oDataLevel[w3oObj][fldCnt]<=maxSlkLevel)
							goto KeepW3OData;
					}*/
				}
				continue;
				
KeepW3OData:
				if (w3oObjEditNum[w3oOObjectNum+woCnt]<w3oObjEditNum[w3oObj]+1-fldCnt)
				{
					w3oObjEditNum[w3oOObjectNum+woCnt]=w3oObjEditNum[w3oObj]+1-fldCnt;
					w3oDataField[w3oOObjectNum+woCnt]=(int*)Calloc((w3oObjEditNum[w3oObj]+2-fldCnt)*5*4);
					w3oDataType[w3oOObjectNum+woCnt]=w3oDataField[w3oOObjectNum+woCnt]+w3oObjEditNum[w3oObj]+2-fldCnt;
					w3oDataLevel[w3oOObjectNum+woCnt]=w3oDataType[w3oOObjectNum+woCnt]+w3oObjEditNum[w3oObj]+2-fldCnt;
					w3oDataIndex[w3oOObjectNum+woCnt]=w3oDataLevel[w3oOObjectNum+woCnt]+w3oObjEditNum[w3oObj]+2-fldCnt;
					w3oData[w3oOObjectNum+woCnt]=(char**)w3oDataIndex[w3oOObjectNum+woCnt]+w3oObjEditNum[w3oObj]+2-fldCnt;
				}
				oeCnt++;
				w3oDataField[w3oOObjectNum+woCnt][oeCnt]=w3oDataField[w3oObj][fldCnt];
				w3oDataType[w3oOObjectNum+woCnt][oeCnt]=w3oDataType[w3oObj][fldCnt];
				w3oDataLevel[w3oOObjectNum+woCnt][oeCnt]=w3oDataLevel[w3oObj][fldCnt];
				w3oDataIndex[w3oOObjectNum+woCnt][oeCnt]=w3oDataIndex[w3oObj][fldCnt];
				w3oData[w3oOObjectNum+woCnt][oeCnt]=w3oData[w3oObj][fldCnt];
			}
			if (clv>maxSlkLevel && w3oType=='a')
			{
				for(int cnt=0;cnt<exDataCnt;cnt++)
				{
					char** data=exData[cnt].data;
					for(int i=4;i<clv;i++)
					{
						if (data[i]!=NULL && data[i]!=data[3])
						{
							if (oeCnt>=w3oObjEditNum[w3oOObjectNum+woCnt])
							{
								char* s = Calloc((oeCnt+clv)*2*5*4);
								memcpy(s,w3oDataField[w3oOObjectNum+woCnt],w3oObjEditNum[w3oOObjectNum+woCnt]*4+4);
								memcpy(s+(oeCnt+clv)*2*4,w3oDataType[w3oOObjectNum+woCnt],w3oObjEditNum[w3oOObjectNum+woCnt]*4+4);
								memcpy(s+(oeCnt+clv)*2*4*2,w3oDataLevel[w3oOObjectNum+woCnt],w3oObjEditNum[w3oOObjectNum+woCnt]*4+4);
								memcpy(s+(oeCnt+clv)*2*4*3,w3oDataIndex[w3oOObjectNum+woCnt],w3oObjEditNum[w3oOObjectNum+woCnt]*4+4);
								memcpy(s+(oeCnt+clv)*2*4*4,w3oData[w3oOObjectNum+woCnt],w3oObjEditNum[w3oOObjectNum+woCnt]*4+4);

								w3oDataField[w3oOObjectNum+woCnt]=(int*)s;
								w3oDataType[w3oOObjectNum+woCnt]=w3oDataField[w3oOObjectNum+woCnt]+(oeCnt+clv)*2;
								w3oDataLevel[w3oOObjectNum+woCnt]=w3oDataType[w3oOObjectNum+woCnt]+(oeCnt+clv)*2;
								w3oDataIndex[w3oOObjectNum+woCnt]=w3oDataLevel[w3oOObjectNum+woCnt]+(oeCnt+clv)*2;
								w3oData[w3oOObjectNum+woCnt]=(char**)w3oDataIndex[w3oOObjectNum+woCnt]+(oeCnt+clv)*2;
								w3oObjEditNum[w3oOObjectNum+woCnt]=(oeCnt+clv)*2-1;
							}
							oeCnt++;
							w3oDataField[w3oOObjectNum+woCnt][oeCnt]=exData[cnt].field;
							w3oDataType[w3oOObjectNum+woCnt][oeCnt]=exData[cnt].type;
							w3oDataLevel[w3oOObjectNum+woCnt][oeCnt]=i+1;
							w3oDataIndex[w3oOObjectNum+woCnt][oeCnt]=exData[cnt].index;
							w3oData[w3oOObjectNum+woCnt][oeCnt]=data[i];
						}
						else if (data[i]==NULL)
							data[i]=data[3];

						if (data[i]==NULL)
							continue;
						switch (exData[cnt].type)
						{
						case 0:
							data[i]=i2a((int&)*data[i]);
							break;
						case 1:
						case 2:
							data[i]=f2a((float&)*data[i],4,true);
							break;
						case 3:
							if (data[i][0]==NULL && data[i]!="")
								data[i]=NULL;
						}
					}
				}
			}
			if (oeCnt>0)
			{
				w3oObject[w3oOObjectNum+woCnt]=w3oObject[w3oObj];
				w3oOrgObject[w3oOObjectNum+woCnt]=w3oOrgObject[w3oObj];
				w3oObjEditNum[w3oOObjectNum+woCnt]=oeCnt;
				woCnt++;
			}
		}

		w3oCObjectNum=woCnt-1;					//原始Object项W3O数据提取
		woCnt=1;
		for(int w3oObj=0;w3oObj<w3oOObjectNum;)
		{
			exDataCnt=0;
			w3oObj++;
			int oHashId=OBJHash((char*)&w3oObject[w3oObj]);
			int oid=objectId[oHashId];
			int lv=w3oObjLevel[w3oObj];
			int olv=objectLevel[oHashId];
			objectUsed[oHashId]=true;
			int oeCnt=0;
			if (haveExtraField)
			{
				slkObjLevel[oid]=lv;
				if (lv>olv)
					for(int i=1;i<=txtFieldCount;i++)
					{
						if (txtFieldArray[i]>0 && txtData[oid][i]!=NULL)
						{
							char* s=txtData[oid][i];
							txtData[oid][i]=Calloc(lv*4);
							if (olv<2)
								((char**)txtData[oid][i])[0]=s;
							else
								for(int n=0;n<olv;n++)
									((char**)txtData[oid][i])[n]=((char**)s)[n];
						}
					}
				else if (lv<2 && olv>1)
					for(int i=1;i<=txtFieldCount;i++)
					{
						if (txtFieldArray[i]>0 && txtData[oid][i]!=NULL)
							txtData[oid][i]=((char**)txtData[oid][i])[0];
					}
			}
			//bool useWTS=false;
			for(int fldCnt=0;fldCnt<w3oObjEditNum[w3oObj];)
			{
				fldCnt++;
				int fid=w3oFieldId[W3OHash((char*)&w3oDataField[w3oObj][fldCnt])];
				if (w3oFieldFile[fid]==NULL || (w3oDataLevel[w3oObj][fldCnt]>lv  && w3oDataLevel[w3oObj][fldCnt]>1))
					continue;
				if (w3oData[w3oObj][fldCnt]==NULL)
					tempStr=NULL;
				else
				{
					switch (w3oDataType[w3oObj][fldCnt])
					{
					case 0:
						tempStr=i2a((int&)*(w3oData[w3oObj][fldCnt]));
						break;
					case 1:
					case 2:
						tempStr=f2a((float&)*(w3oData[w3oObj][fldCnt]),4,true);
						break;
					case 3:
						tempStr=w3oData[w3oObj][fldCnt];
						if (*tempStr==NULL && tempStr!="")
							tempStr=NULL;
						/*else if (*tempStr=='T' && memcmp(tempStr+1,"RIGSTR_",7)==0)
							tempStr=NULL,useWTS=true;*/
					}
				}

				if (strcmp(w3oFieldFile[fid],"Profile")==0)		//txt
				{
					if (tempStr==NULL && *szTxtNullMark==NULL && txtFieldNoEmpty[TXTHash(txtField[w3o2txtFieldId[w3oFieldId[W3OHash((char*)&w3oDataField[w3oObj][fldCnt])]]])]>0)
						tempStr="";
					//else if (w3oDataType[w3oObj][fldCnt]==3 && tempStr!=NULL && w3oFieldIndex[w3oFieldId[W3OHash((char*)(w3oDataField[w3oObj]+fldCnt))]]>=0 && strchr(tempStr,',')!=NULL)
					//{
					//	//int len=strlen(tempStr);
					//	//tempStr[len]='\"';
					//	//tempStr[len+1]=NULL;
					//	//*(--tempStr)='\"';
					//}
					else if (tempStr!=NULL && *tempStr=='0' && tempStr[1]==NULL && w3oDataType[w3oObj][fldCnt]<3)
						tempStr=NULL;

					if (txtFieldIndex[w3o2txtFieldId[fid]]>0)
					{
						if (txtData[oid][w3o2txtFieldId[fid]]==NULL)
							txtData[oid][w3o2txtFieldId[fid]]=Calloc(8);
						((char**)txtData[oid][w3o2txtFieldId[fid]])[w3oFieldIndex[fid]]=tempStr;
					}
					else if (w3oDataLevel[w3oObj][fldCnt]>0 && lv>1)
					{
						if (txtData[oid][w3o2txtFieldId[fid]]==NULL)
							txtData[oid][w3o2txtFieldId[fid]]=Calloc(lv*4);
						((char**)txtData[oid][w3o2txtFieldId[fid]])[w3oDataLevel[w3oObj][fldCnt]-1]=tempStr;
					}
					else
						txtData[oid][w3o2txtFieldId[fid]]=tempStr;
					bool exSet;
					switch (w3oDataField[w3oObj][fldCnt])
					{
					case '1bua':
					case 'tura':
					case '1uua':
					case 'butu':
					case '1bug':
						exSet=true;
						break;
					default:
						exSet=false;
					}
					if (NeedConvWts(tempStr,exSet))
						goto KeepW3OData2;
				}
				else												//slk
				{
					if (w3oDataLevel[w3oObj][fldCnt]>maxSlkLevel)
					{
						if (slkData[oid][w3o2slkFieldId[fid]]==NULL)
						{
							slkData[oid][w3o2slkFieldId[fid]]=Calloc(lv*4);
							exData[exDataCnt++].Init(w3oDataField[w3oObj][fldCnt],w3oDataType[w3oObj][fldCnt],w3oDataIndex[w3oObj][fldCnt],(char**)slkData[oid][w3o2slkFieldId[fid]],lv);
						}
						((char**)slkData[oid][w3o2slkFieldId[fid]])[w3oDataLevel[w3oObj][fldCnt]-1]=w3oData[w3oObj][fldCnt];
					}
					else
					{
						if (w3oDataType[w3oObj][fldCnt]==3 && tempStr!=NULL)
                        {
                            if (w3oType != 'd')
                            {
                                if (*tempStr=='.' && w3o2slkFieldId[fid]==slkFieldId[SLKHash("file")])
                                    tempStr="\"_";
                                else
                                    *(--tempStr)='\"';
                            }
						}
						else if (tempStr!=NULL && *tempStr=='0' && tempStr[1]==NULL && w3oDataType[w3oObj][fldCnt]<3)
							tempStr=NULL;
						int slk_fid;
						if (w3oDataLevel[w3oObj][fldCnt]>0)
						{
							char* s;
							int dlv = w3oDataLevel[w3oObj][fldCnt];
							if (w3oType=='d' && dlv<10)
							{
								s=Malloc(3);
								s[0]='0';
								s[1]=dlv | '0';
								s[2]=NULL;
							}
							else
								s=i2a(dlv);
							slk_fid=slkFieldId[SLKHash(StrAddTemp(slkField[w3o2slkFieldId[fid]],s))];
							if (w3oDataLevel[w3oObj][fldCnt]==maxSlkLevel && lv>maxSlkLevel)
							{
								if (slkData[oid][w3o2slkFieldId[fid]]==NULL)
								{
									slkData[oid][w3o2slkFieldId[fid]]=Calloc(lv*4);
									exData[exDataCnt++].Init(w3oDataField[w3oObj][fldCnt],w3oDataType[w3oObj][fldCnt],w3oDataIndex[w3oObj][fldCnt],(char**)slkData[oid][w3o2slkFieldId[fid]],lv);
								}
								((char**)slkData[oid][w3o2slkFieldId[fid]])[maxSlkLevel-1]=w3oData[w3oObj][fldCnt];
								for(int i=maxSlkLevel;i<lv;i++)
								{
									if (((char**)slkData[oid][w3o2slkFieldId[fid]])[i]==NULL)
										((char**)slkData[oid][w3o2slkFieldId[fid]])[i]=w3oData[w3oObj][fldCnt];
								}
							}
						}
						else
							slk_fid=w3o2slkFieldId[fid];
						slkData[oid][slk_fid]=tempStr;
					}
					/*if (useWTS)
					{
						useWTS=false;
						if (w3oDataLevel[w3oObj][fldCnt]<=maxSlkLevel)
						goto KeepW3OData2;
					}*/
				}
				continue;

KeepW3OData2:
				if (w3oObjEditNum[woCnt]<w3oObjEditNum[w3oObj]+1-fldCnt)
				{
					w3oObjEditNum[woCnt]=w3oObjEditNum[w3oObj]+1-fldCnt;
					w3oDataField[woCnt]=(int*)Calloc((w3oObjEditNum[w3oObj]+2-fldCnt)*5*4);
					w3oDataType[woCnt]=w3oDataField[woCnt]+w3oObjEditNum[w3oObj]+2-fldCnt;
					w3oDataLevel[woCnt]=w3oDataType[woCnt]+w3oObjEditNum[w3oObj]+2-fldCnt;
					w3oDataIndex[woCnt]=w3oDataLevel[woCnt]+w3oObjEditNum[w3oObj]+2-fldCnt;
					w3oData[woCnt]=(char**)w3oDataIndex[woCnt]+w3oObjEditNum[w3oObj]+2-fldCnt;
				}
				oeCnt++;
				w3oDataField[woCnt][oeCnt]=w3oDataField[w3oObj][fldCnt];
				w3oDataType[woCnt][oeCnt]=w3oDataType[w3oObj][fldCnt];
				w3oDataLevel[woCnt][oeCnt]=w3oDataLevel[w3oObj][fldCnt];
				w3oDataIndex[woCnt][oeCnt]=w3oDataIndex[w3oObj][fldCnt];
				w3oData[woCnt][oeCnt]=w3oData[w3oObj][fldCnt];
			}
			if (lv>maxSlkLevel && w3oType=='a')
			{
				for(int cnt=0;cnt<exDataCnt;cnt++)
				{
					char** data=exData[cnt].data;
					for(int i=4;i<lv;i++)
					{
						if (data[i]!=NULL && data[i]!=data[3])
						{
							if (oeCnt>=w3oObjEditNum[woCnt])
							{
								char* s = Calloc((oeCnt+lv)*2*5*4);
								memcpy(s,w3oDataField[woCnt],w3oObjEditNum[woCnt]*4+4);
								memcpy(s+(oeCnt+lv)*2*4,w3oDataType[woCnt],w3oObjEditNum[woCnt]*4+4);
								memcpy(s+(oeCnt+lv)*2*4*2,w3oDataLevel[woCnt],w3oObjEditNum[woCnt]*4+4);
								memcpy(s+(oeCnt+lv)*2*4*3,w3oDataIndex[woCnt],w3oObjEditNum[woCnt]*4+4);
								memcpy(s+(oeCnt+lv)*2*4*4,w3oData[woCnt],w3oObjEditNum[woCnt]*4+4);

								w3oDataField[woCnt]=(int*)s;
								w3oDataType[woCnt]=w3oDataField[woCnt]+(oeCnt+lv)*2;
								w3oDataLevel[woCnt]=w3oDataType[woCnt]+(oeCnt+lv)*2;
								w3oDataIndex[woCnt]=w3oDataLevel[woCnt]+(oeCnt+lv)*2;
								w3oData[woCnt]=(char**)w3oDataIndex[woCnt]+(oeCnt+lv)*2;
								w3oObjEditNum[woCnt]=(oeCnt+lv)*2-1;
							}
							oeCnt++;
							w3oDataField[woCnt][oeCnt]=exData[cnt].field;
							w3oDataType[woCnt][oeCnt]=exData[cnt].type;
							w3oDataLevel[woCnt][oeCnt]=i+1;
							w3oDataIndex[woCnt][oeCnt]=exData[cnt].index;
							w3oData[woCnt][oeCnt]=data[i];
						}
						else if (data[i]==NULL)
							data[i]=data[3];

						switch (exData[cnt].type)
						{
						case 0:
              if (NULL != data[i])
                data[i] = i2a((int&)*data[i]);
              else
                data[i] = i2a(0);
							break;
						case 1:
            case 2:
              if (NULL != data[i])
                data[i]=f2a((float&)*data[i], 4, true);
              else
                data[i] = f2a(0.f, 4, true);
							break;
						case 3:
							if (data[i]!=NULL && data[i][0]==NULL && data[i]!="")
								data[i]=NULL;
						}
					}
				}
			}
			if (oeCnt>0)
			{
				w3oObject[woCnt]=w3oObject[w3oObj];
				w3oObjEditNum[woCnt]=oeCnt;
				woCnt++;
			}
		}
		woCnt--;

		for(int i=0;i<w3oCObjectNum;)
		{
			i++;
			if (w3oObjEditNum[woCnt+i]<w3oObjEditNum[w3oOObjectNum+i])
			{
				w3oDataField[woCnt+i]=(int*)Calloc((w3oObjEditNum[w3oOObjectNum+i]+1)*5*4);
				w3oDataType[woCnt+i]=w3oDataField[woCnt+i]+w3oObjEditNum[w3oOObjectNum+i]+1;
				w3oDataLevel[woCnt+i]=w3oDataType[woCnt+i]+w3oObjEditNum[w3oOObjectNum+i]+1;
				w3oDataIndex[woCnt+i]=w3oDataLevel[woCnt+i]+w3oObjEditNum[w3oOObjectNum+i]+1;
				w3oData[woCnt+i]=(char**)w3oDataIndex[woCnt+i]+w3oObjEditNum[w3oOObjectNum+i]+1;
			}
			w3oObject[woCnt+i]=w3oObject[w3oOObjectNum+i];
			w3oOrgObject[woCnt+i]=w3oOrgObject[w3oOObjectNum+i];
			w3oObjEditNum[woCnt+i]=w3oObjEditNum[w3oOObjectNum+i];
			for(int j=0;j<w3oObjEditNum[woCnt+i];)
			{
				j++;
				w3oDataField[woCnt+i][j]=w3oDataField[w3oOObjectNum+i][j];
				w3oDataType[woCnt+i][j]=w3oDataType[w3oOObjectNum+i][j];
				w3oDataLevel[woCnt+i][j]=w3oDataLevel[w3oOObjectNum+i][j];
				w3oDataIndex[woCnt+i][j]=w3oDataIndex[w3oOObjectNum+i][j];
				w3oData[woCnt+i][j]=w3oData[w3oOObjectNum+i][j];
			}
		}
		w3oObjectNum=woCnt+w3oCObjectNum;
		w3oOObjectNum=w3oObjectNum;		//原始项+自定义项全作为原始项保存
		w3oCObjectNum=0;

		if (optimizer>0 && w3oObjectNum>0)
			WriteW3O(w3oPath,haveExtraField);
		if (w3oType!='a')
			return 0;
		for(int i=objectCount;i>0;i--)
		{
			if (slkObjLevel[i]==-1)
				slkObjLevel[i]=objectLevel[OBJHash(slkData[i][1]+1)];
			if (slkObjLevel[i]==0)
				slkObjLevel[i]=1;
			for(int j=MAX_SLK_FIELD_NUM+1;j<MAX_SLK_FIELD_NUM+21;j++)
			{
				if (slkData[i][j]==NULL)
					slkData[i][j]=Calloc(slkObjLevel[i]*4);
				for(int k=(slkObjLevel[i]<4?slkObjLevel[i]:4);k>0;k--)
					((char**)slkData[i][j])[k-1]=slkData[i][slkFieldId[SLKHash(StrAddTemp(slkField[j],i2a(k)))]];
			}
		}
		return 0;
	}

	int WriteSLK(char* slkFile, int index)
	{
		if (optimizer==0)
			return 0;
		int fieldPoint=slkFieldPoint[index];
		int fieldCount=slkFieldPoint[index+1]-slkFieldPoint[index];

		if (index==0)
		{
			usedObjectCount=0;
			usedObjectId=(int*)Calloc((objectCount+1)*4);
			usedObjectLevel=(int*)Calloc((objectCount+1)*4);
			for(int i=1;i<=orgObjectCount;i++)
			{
				int ohid=OBJHash(slkData[i][1]+1);
				if (objectUsed[ohid] || optimizer==1)
				{
					usedObjectCount++;
					usedObjectId[usedObjectCount]=i;
					usedObjectLevel[usedObjectCount]=(slkObjLevel[i]!=-1?slkObjLevel[i]:objectLevel[OBJHash(slkData[i][1]+1)]);
				}
			}
			for(int i=orgObjectCount+1;i<=objectCount;i++)
			{
				usedObjectCount++;
				usedObjectId[usedObjectCount]=i;
				usedObjectLevel[usedObjectCount]=slkObjLevel[i];
			}
		}

		char* destStr=Malloc(maxSlkFileSize);
		char* destStrPos=destStr;

		memcat(destStrPos,"ID;PWXL;N;E\n",12);
		memcat(destStrPos,"B;X",3);
		WriteIntToBuffer(destStrPos,fieldCount);
		memcat(destStrPos,";Y",2);
		WriteIntToBuffer(destStrPos,usedObjectCount+1);
		memcat(destStrPos,";D0\n",4);

		memcat(destStrPos,"C;X1;Y1;K\"",10);						//Field项
		strcat(destStrPos,slkField[1+fieldPoint]);
		chrcat(destStrPos,'\"');
		for(int i=2;i<=fieldCount;i++)
		{
			memcat(destStrPos,"\nC;X",4);
			WriteIntToBuffer(destStrPos,i);
			memcat(destStrPos,";K\"",3);
			strcat(destStrPos,slkField[i+fieldPoint]);
			chrcat(destStrPos,'\"');
		}

		for(int i=1;i<=usedObjectCount;i++)
		{
			memcat(destStrPos,"\nC;X1;Y",7);
			WriteIntToBuffer(destStrPos,i+1);
			memcat(destStrPos,";K",2);
			memcat(destStrPos,slkData[usedObjectId[i]][1],5);
			chrcat(destStrPos,'\"');
			for(int j=2;j<=fieldCount;j++)
			{
				if (slkFieldNoEmpty[j+fieldPoint] && slkData[usedObjectId[i]][j+fieldPoint]==NULL)
					slkData[usedObjectId[i]][j+fieldPoint]="\"_";

				if (slkData[usedObjectId[i]][j+fieldPoint]!=NULL)
				{
					memcat(destStrPos,"\nC;X",4);
					WriteIntToBuffer(destStrPos,j);
					memcat(destStrPos,";K",2);
					strcat(destStrPos,slkData[usedObjectId[i]][j+fieldPoint]);
					if (slkData[usedObjectId[i]][j+fieldPoint][0]=='"')
						chrcat(destStrPos,'\"');
				}
			}
		}
		memcat(destStrPos,"\nE",2);
		g_CachePos = destStrPos;
		AddBufferFile(slkFile,destStr,destStrPos-destStr,0);
		return 0;
	}

	int WriteTXT(char* txtFile)
	{
		if (optimizer==0)
			return 0;
		int* txtFieldLen=(int*)malloc((txtFieldCount+1)*4);
		for(int i=1;i<=txtFieldCount;i++)
		{
			if (txtField[i]!=NULL)
				txtFieldLen[i]=strlen(txtField[i]);
		}
		char* destStr=Malloc(maxSlkFileSize);
		char* destStrPos=destStr;

		//memcat(destStrPos,"\xef\xbb\xbf",3);
		for(int i=1;i<=usedObjectCount;i++)
		{
			chrcat(destStrPos,'[');
			memcat(destStrPos,slkData[usedObjectId[i]][1]+1,4);
			memcat(destStrPos,"]\n",2);
			for(int j=1;j<=txtFieldCount;j++)
			{
				if (txtData[usedObjectId[i]][j]!=NULL)
				{
					if (txtFieldIndex[j]>0)		//Index
					{
						if (((char**)txtData[usedObjectId[i]][j])[0]!=NULL || ((char**)txtData[usedObjectId[i]][j])[1]!=NULL)
						{
							memcat(destStrPos,txtField[j],txtFieldLen[j]);
							chrcat(destStrPos,'=');
							if (((char**)txtData[usedObjectId[i]][j])[0]!=NULL)
								strcat(destStrPos,((char**)txtData[usedObjectId[i]][j])[0]);
							if (((char**)txtData[usedObjectId[i]][j])[1]!=NULL)
							{
								chrcat(destStrPos,',');
								strcat(destStrPos,((char**)txtData[usedObjectId[i]][j])[1]);
							}
							chrcat(destStrPos,'\n');
						}
					}
					else if (txtFieldArray[j]>0 && usedObjectLevel[i]>1)	//Level>1
					{
						int lv=usedObjectLevel[i];
						for(;((char**)txtData[usedObjectId[i]][j])[lv-1]==NULL;lv--)
							;
						if (lv!=0)
						{
							if (w3oType=='q' && txtFieldIndex[j]<0)	//Upgrade的Requires/Requiresamount项	//appendIndex
							{
								for(int n=0;n<lv;n++)
								{
									if (((char**)txtData[usedObjectId[i]][j])[n]!=NULL)
									{
										memcat(destStrPos,txtField[j],txtFieldLen[j]);
										if (n>0)
											WriteIntToBuffer(destStrPos,n);
										chrcat(destStrPos,'=');
										strcat(destStrPos,((char**)txtData[usedObjectId[i]][j])[n]);
										chrcat(destStrPos,'\n');
									}
								}
							}
							else
							{
								memcat(destStrPos,txtField[j],txtFieldLen[j]);
								chrcat(destStrPos,'=');
								if (((char**)txtData[usedObjectId[i]][j])[0]!=NULL)
									ExWriteStr(destStrPos,((char**)txtData[usedObjectId[i]][j])[0],true);
									//strcat(destStrPos,((char**)txtData[usedObjectId[i]][j])[0]);
								for(int n=1;n<lv;n++)
								{
									chrcat(destStrPos,',');
									if (((char**)txtData[usedObjectId[i]][j])[n]!=NULL)
										ExWriteStr(destStrPos,((char**)txtData[usedObjectId[i]][j])[n],true);
										//strcat(destStrPos,((char**)txtData[usedObjectId[i]][j])[n]);
								}
								chrcat(destStrPos,'\n');
							}
						}
					}
					else
					{
						memcat(destStrPos,txtField[j],txtFieldLen[j]);
						chrcat(destStrPos,'=');
						if (txtData[usedObjectId[i]][j]=="")
							chrcat(destStrPos,',');
						else if (txtFieldIndex[j]<0)
						{
							for(char* s=txtData[usedObjectId[i]][j];;)
							{
								if (*s=='\r')
								{
									*(destStrPos++)='|', *(destStrPos++)='n';
									if (*(++s)=='\n')
										s++;
								}
								else if (*s=='\n')
									*(destStrPos++)='|', *(destStrPos++)='n', s++;
								else if (*s==NULL)
									break;
								else
									*(destStrPos++) = *(s++);
							}
							//strcat(destStrPos,txtData[usedObjectId[i]][j]);
						}
						else
							ExWriteStr(destStrPos,txtData[usedObjectId[i]][j],true);
							//strcat(destStrPos,txtData[usedObjectId[i]][j]);
						chrcat(destStrPos,'\n');
					}
				}
			}
			if (destStrPos[-2]==']' && destStrPos[-7]=='[')		//数据为空，则取消该项
				destStrPos-=7;
		}

		if (odAbil.optimizer>0)
		{
			memcat(destStrPos,"[Anei]\n",7);
			strcat(destStrPos,Anei_Str);
		}

		free(txtFieldLen);
		g_CachePos=destStrPos;
		AddBufferFile(txtFile,destStr,destStrPos-destStr,0);
		return 0;
	}

	void RecordCustomObjHash()
	{
		for(int i=orgObjectCount;i<objectCount;)
			objectId[OBJHash(slkData[++i][1]+1)]=i;
	}

	void AddAbilDataForSearch(char* dataField,char* objectList)
	{
		int i=0;
		for(;i<7;i++)
		{
			if (abilDataSearch_Field[i]==slkFieldId[SLKHash(dataField)])
				break;
		}
		for(;;)
		{
			for(;*objectList<48;objectList++)
			{
				if (*objectList==NULL)
					return;
			}
			int oid = objectId[OBJHash(objectList)];
			if (oid ==0)
				abilDataSearch_Data[ACodeHash(objectList)] |= (0x80>>i);
			else
			{
				abilDataSearch_Data[ACodeHash(slkData[oid][2]+1)] |= (0x80>>i);
				char s = abilDataSearch_Data[ACodeHash(slkData[oid][2]+1)];
				s=s;
			}
			objectList+=4;
			for(;*objectList>47;objectList++)
				;
		}
	}

	void AddAbilDataForSearchAll()
	{
		bool linkedSkill=false;
		bool createdDestructable=false;
		bool upgradeTech=false;
		bool summonedUnit=false;
		bool morphedUnit=false;
		bool allowedUnit=false;
		bool techLimitedUnit=false;
		char* tempStr=objectSearStr[5];
		char* litStr;
		abilDataSearch_Field[0]=slkFieldId[SLKHash("UnitID")];
		abilDataSearch_Field[1]=slkFieldId[SLKHash("DataA")];
		abilDataSearch_Field[2]=slkFieldId[SLKHash("DataB")];
		abilDataSearch_Field[3]=slkFieldId[SLKHash("DataC")];
		abilDataSearch_Field[4]=slkFieldId[SLKHash("DataD")];
		abilDataSearch_Field[5]=slkFieldId[SLKHash("DataE")];
		abilDataSearch_Field[6]=slkFieldId[SLKHash("DataF")];
		abilDataSearch_Data=(unsigned char*)Calloc(objectCount+1);
		for(;;)
		{
			for(;*tempStr<48 && *tempStr!=NULL;tempStr++)
				;
			if (*tempStr==NULL)
				break;
			litStr=tempStr;
			for(;*tempStr!='=';tempStr++)
				;
			*(tempStr++)=NULL;
			if (((*(tempStr++))&0xf) <= searchObjectLevel)
			{
				if (strcmp(litStr,"LinkedSkill")==0)
					linkedSkill=true;
				else if (strcmp(litStr,"CreatedDestructable")==0)
					createdDestructable=true;
				else if (strcmp(litStr,"UpgradeTech")==0)
					upgradeTech=true;
				else if (strcmp(litStr,"SummonedUnit")==0)
					summonedUnit=true;
				else if (strcmp(litStr,"MorphedUnit")==0)
					morphedUnit=true;
				else if (strcmp(litStr,"AllowedUnit")==0)
					allowedUnit=true;
				else if (strcmp(litStr,"TechLimitedUnit")==0)
					techLimitedUnit=true;
			}
			tempStr[-2]='=';
		}

		if (linkedSkill)
		{
			AddAbilDataForSearch("UnitID","AIdf,AIll,AIsb");//球类关联技能
			AddAbilDataForSearch("DataA","Aspb");			//魔法书关联技能
			AddAbilDataForSearch("DataC","ANeg");			//工程升级关联技能
			AddAbilDataForSearch("DataD","ANeg");			//工程升级关联技能
			AddAbilDataForSearch("DataE","ANeg");			//工程升级关联技能
			AddAbilDataForSearch("DataF","ANeg");			//工程升级关联技能
		}
		if (createdDestructable)
			AddAbilDataForSearch("UnitID","ANvc");			//火山爆发可破坏物
		if (upgradeTech)
			AddAbilDataForSearch("UnitID","AIgf,AIgu");		//防御浮雕升级科技
		if (summonedUnit)
		{
			AddAbilDataForSearch("DataA","ANrc");			//混乱之雨召唤单位
			AddAbilDataForSearch("UnitID","AHwe,AEst,ANsg,ANsq,ANsw,ANwm,AOsw,AOwd,Anwm,ACwe,AHpx,ACtn,ANlm");			//召唤水元素
			AddAbilDataForSearch("UnitID","AUin,ANin,SNin,AIin");		//地狱火召唤单位
			AddAbilDataForSearch("UnitID","AOsf,ACsf,ACs9,AIsh");		//野兽幽魂召唤单位
			AddAbilDataForSearch("UnitID","AEfn,ACfr");					//召唤树人召唤单位
			AddAbilDataForSearch("UnitID","Aapl,Aap1,Aap2,Aap3,Aap4");	//疾病云雾召唤单位
			AddAbilDataForSearch("UnitID","Agyd");						//创建尸体尸体类型
			AddAbilDataForSearch("DataC","Arai,ACrd,AUcb,AIrd,Avng");	//复活死尸召唤单位
			AddAbilDataForSearch("DataD","Arai,ACrd,AUcb,AIrd,Avng");	//复活死尸召唤单位
			AddAbilDataForSearch("DataB","Asod,Assp,Aspd,Aspy,Aspt");	//蜘蛛等分裂裂变单位
			AddAbilDataForSearch("UnitID","Asta");						//静止陷阱召唤单位
			AddAbilDataForSearch("DataC","AIfd,AIff,AIfr,AIfu,AIfh,AIfs,AIir,AIuw");	//召唤物品召唤单位
			AddAbilDataForSearch("DataD","AIfd,AIff,AIfr,AIfu,AIfh,AIfs,AIir,AIuw");	//召唤物品召唤单位
			AddAbilDataForSearch("UnitID","ANdc,SNdc");					//黑暗转换变换单位
			AddAbilDataForSearch("UnitID","AUls");						//蝗虫群召唤单位
			AddAbilDataForSearch("UnitID","ANba,ANbs");					//黑暗之箭召唤单位
			AddAbilDataForSearch("UnitID","ANto");						//龙卷风召唤单位
			AddAbilDataForSearch("UnitID","AEsv");						//复仇之魂召唤单位
			AddAbilDataForSearch("UnitID","Aexh");						//挖掘尸体尸体类型
			AddAbilDataForSearch("UnitID","Ahwd,AChw,Apts,Aeye,AIhw,AIsw");	//治疗/岗哨守卫/疾病云雾召唤单位
			AddAbilDataForSearch("UnitID","AImo");						//诱捕守卫召唤单位
			AddAbilDataForSearch("UnitID","AIpm,ANpa");					//地雷放置/寄生虫召唤单位
			AddAbilDataForSearch("UnitID","ANdo");						//末日审判召唤单位
			AddAbilDataForSearch("DataB","ANsy,ANs1,ANs2,ANs3");		//口袋工厂生产地精
			AddAbilDataForSearch("UnitID","ANsy,ANs1,ANs2,ANs3");		//口袋工厂召唤工厂
			AddAbilDataForSearch("UnitID","ANfy");						//工厂生产地精
			AddAbilDataForSearch("DataA","ANdp");						//黑暗之门召唤单位
			AddAbilDataForSearch("UnitID","AIbl,AIbg,AIbt");			//物品建造技能
			AddAbilDataForSearch("DataA","ANef");						//火土风暴召唤单位
		}
		if (morphedUnit)
		{
			AddAbilDataForSearch("UnitID","AEme,AEIl,AEvi,Abrf,Arav,Amrf,Astn,Aspx,Aave,Abur,Abu2,Abu3,Aetf,Acpf,Aphx,Asb1,Asb2,Asb3,ANcr,ANrg,ANg1,ANg2,ANg3");		//变身变化单位
			AddAbilDataForSearch("UnitID","Acha,Sca1,Sca2,Sca3,Sca4,Sca5,Sca6,Sbsk,Srtt");		//混沌技能转变单位
			AddAbilDataForSearch("DataB","Aply,ACpy,AOhx,AChx");		//变形术 变化类型地
			AddAbilDataForSearch("DataC","Aply,ACpy,AOhx,AChx");		//变形术 变化类型空
			AddAbilDataForSearch("DataD","Aply,ACpy,AOhx,AChx");		//变形术 变化类型两栖
			AddAbilDataForSearch("DataE","Aply,ACpy,AOhx,AChx");		//变形术 变化类型水
			AddAbilDataForSearch("UnitID","Aent");						//缠绕金矿金矿变化单位
			AddAbilDataForSearch("DataB","Amil");						//战斗号召变化单位
			AddAbilDataForSearch("UnitID","Acoa,Acoh,Aco2,Aco3");		//骑乘角鹰兽合成单位类型
			AddAbilDataForSearch("DataA","Adec");						//角鹰兽分离单位类型
			AddAbilDataForSearch("DataB","Adec");						//角鹰兽分离单位类型
		}
		if (allowedUnit)
		{
			AddAbilDataForSearch("DataA","AEme,AEIl,AEvi,Abrf,Arav,Amrf,Astn,Aspx,Aave,Abur,Abu2,Abu3,Aetf,Acpf,Aphx,Asb1,Asb2,Asb3,ANcr,ANrg,ANg1,ANg2,ANg3");	//变身允许单位类型
			AddAbilDataForSearch("UnitID","Abtl,Sbtl");					//地洞战备状态允许单位
			AddAbilDataForSearch("UnitID","Aloa,Sloa,Slo2,Slo3");		//装载允许目标单位
			AddAbilDataForSearch("DataA","Amil");						//战斗号召允许单位
			AddAbilDataForSearch("UnitID","ANsl");						//灵魂保存目标单位
			AddAbilDataForSearch("UnitID","Achl");						//地洞装载允许目标单位
			AddAbilDataForSearch("DataA","Acoa,Acoh,Aco2,Aco3");		//骑乘角鹰兽指定单位类型
		}
		if (techLimitedUnit)
			AddAbilDataForSearch("UnitID","Arai,ACrd,AIrd,Avng");		//复活死尸科技限制单位
	}

	void AddObjDataForSearch(char* fldlist)
	{
		char* litStr;
		for(;;)
		{
			for(;*fldlist<48;fldlist++)
			{
				if (*fldlist==NULL)
					return;
			}
			litStr=fldlist;
			for(;*fldlist!='=';fldlist++)
				;
			*(fldlist++)=NULL;
			if (((*(fldlist++))&0xf) <= searchObjectLevel)
			{
				int fid = slkFieldId[SLKHash(litStr)];
				if (slkField[fid]!=NULL && strcmp(slkField[fid],litStr)==0)
					slkFieldForSearch[slkFieldCountForSearch++]=fid;
				else
				{
					fid = txtFieldId[TXTHash(litStr)];
					if (strcmp(txtField[fid],litStr)==0)
						txtFieldForSearch[txtFieldCountForSearch++]=fid;
				}
			}
			fldlist[-2]='=';
		}
	}

	void SearchForAbilData(int oid)
	{
		if (slkData[oid][2]==NULL)
			return;
		int code = ACodeHash(slkData[oid][2]+1);
		if (abilDataSearch_Data[code]==NULL)
			return;
		for(int i=0;i<7;i++)
		{
			if ((abilDataSearch_Data[code]<<i) > 0x7f)
			{
				for(int lv=slkObjLevel[oid];lv>0;)
				{
					lv--;
					SearchInObjectList(((char**)slkData[oid][abilDataSearch_Field[i]])[lv]);
				}
			}
		}
	}

	void SearchInObjectId(int oid)
	{
		if (objectSearched[oid])
			return;
		objectSearched[oid]=true;
		for(int i=slkFieldCountForSearch;i>0;)
		{
			i--;
			if (slkFieldForSearch[i]>MAX_SLK_FIELD_NUM)
				for(int lv=slkObjLevel[oid];lv>0;)
				{
					lv--;
					SearchInObjectList(((char**)slkData[oid][slkFieldForSearch[i]])[lv]);
				}
			else
				SearchInObjectList(slkData[oid][slkFieldForSearch[i]]);
		}
		for(int i=txtFieldCountForSearch;i>0;)
		{
			i--;
			if (txtFieldArray[i]>0)
				for(int lv=slkObjLevel[oid];lv>0;)
				{
					lv--;
					SearchInObjectList(((char**)txtData[oid][slkFieldForSearch[i]])[lv]);
				}
			else
				SearchInObjectList(txtData[oid][txtFieldForSearch[i]]);
		}

		if (w3oType=='a')
			SearchForAbilData(oid);
	}

	void SearchInObjectData()
	{
		for(int i=1;i<=orgObjectCount;i++)
		{
			if (objectSearched[i])
				continue;
			int ohid=OBJHash(slkData[i][1]+1);
			if (objectUsed[ohid] || optimizer==1)
				SearchInObjectId(i);
		}
		for(int i=orgObjectCount+1;i<=objectCount;i++)
		{
			if (!objectSearched[i])
				SearchInObjectId(i);
		}
	}

}odUnit,odItem,odDest,odDood,odAbil,odBuff,odUpgr;
void SearchInObjectList(char* objList)
{
	if (objList==NULL)
		return;
	else if (*objList=='"')
		objList++;
	int hid,oid;
	for(;;)
	{
		for(;*objList==',';objList++)
			;
		hid = OBJHash(objList);
		oid = objectId[hid];
		if (oid>0)
		{
			objectUsed[hid]=true;
			if (oid<= odUnit.objectCount && memcmp(odUnit.slkData[oid][1]+1,objList,4)==0)
				odUnit.SearchInObjectId(oid);
			else if (oid<= odAbil.objectCount && memcmp(odAbil.slkData[oid][1]+1,objList,4)==0)
				odAbil.SearchInObjectId(oid);
			else if (oid<= odItem.objectCount && memcmp(odItem.slkData[oid][1]+1,objList,4)==0)
				odItem.SearchInObjectId(oid);
			else if (oid<= odUpgr.objectCount && memcmp(odUpgr.slkData[oid][1]+1,objList,4)==0)
				odUpgr.SearchInObjectId(oid);
		}
		for(;*objList!=',' && *objList!=NULL;objList++)
			;
		if (*objList==NULL)
			break;
	}
}

void SearchDataFieldForObject()
{
	int cnt;
	for(cnt = odAbil.objectCount;cnt>0;cnt--)
	{
		if (odAbil.slkData[cnt][2]==NULL)
			;
		else if (strcmp(odAbil.slkData[cnt][2]+1,"ANvc")==0)
			objectUsed[OBJHash("Volc")]=true;
		else if (strcmp(odAbil.slkData[cnt][2]+1,"Asac")==0 || strcmp(odAbil.slkData[cnt][2]+1,"Alam")==0)
			objectUsed[OBJHash("ushd")]=true;
	}

	if (searchObjectLevel==0)
		return;
	odUnit.AddObjDataForSearch(objectSearStr[1]);
	odItem.AddObjDataForSearch(objectSearStr[2]);
	odAbil.AddObjDataForSearch(objectSearStr[3]);
	odUpgr.AddObjDataForSearch(objectSearStr[4]);
	odAbil.AddAbilDataForSearchAll();

	odUnit.SearchInObjectData();
	odItem.SearchInObjectData();
	odAbil.SearchInObjectData();
	odUpgr.SearchInObjectData();
}

void SearchUnusedObjectInTxt(ObjectData od,char* mark)
{
	if (od.optimizer==0)
		return;
	int xid = TXTHash(mark);
	int fid = od.txtFieldId[TXTHash(mark)];
	for(int cnt = od.objectCount;cnt>0;cnt--)
	{
		if (od.txtData[cnt][fid]!=NULL && strcmp(od.txtData[cnt][fid],szUnusedObjectName)==0)
			objectUsed[OBJHash(od.slkData[cnt][1])]=false;
	}
}

void SearchUnusedObject()
{
	if (szUnusedObjectName==NULL || *szUnusedObjectName==NULL)
		return;
	SearchUnusedObjectInTxt(odUnit,"Name");
	SearchUnusedObjectInTxt(odItem,"Name");
	SearchUnusedObjectInTxt(odAbil,"Name");
	SearchUnusedObjectInTxt(odBuff,"Bufftip");

	if (odDest.optimizer>0)
	{
		int fid = odDest.slkFieldId[SLKHash("Name")];
		for(int cnt = odDest.orgObjectCount;cnt>0;cnt--)
		{
			if (odDest.slkData[cnt][fid]!=NULL && strcmp(odDest.slkData[cnt][fid],szUnusedObjectName)==0)
				objectUsed[OBJHash(odDest.slkData[cnt][1])]=false;
		}
	}
	if (odUpgr.optimizer>0)
	{
		int fid = odUpgr.txtFieldId[TXTHash("Name")];
		for(int cnt = odUpgr.orgObjectCount;cnt>0;cnt--)
		{
			if (odUpgr.slkObjLevel[cnt]>1)
			{
				if (odUpgr.txtData[cnt][fid]!=NULL && ((char**)odUpgr.txtData[cnt][fid])[0]!=NULL && strcmp(((char**)odUpgr.txtData[cnt][fid])[0],szUnusedObjectName)==0)
					objectUsed[OBJHash(odUpgr.slkData[cnt][1])]=false;
			}
			else
			{
				if (odUpgr.txtData[cnt][fid]!=NULL && strcmp(odUpgr.txtData[cnt][fid],szUnusedObjectName)==0)
					objectUsed[OBJHash(odUpgr.slkData[cnt][1])]=false;
			}
		}
	}
}

void SearchUsedRandomObject()
{
	int f_race =		odUnit.slkFieldId[SLKHash("race")];
	int f_isbldg =		odUnit.slkFieldId[SLKHash("isbldg")];
	int f_tilesets =	odUnit.slkFieldId[SLKHash("tilesets")];
	int f_nbrandom =	odUnit.slkFieldId[SLKHash("nbrandom")];
	int f_pickRandom =	odItem.slkFieldId[SLKHash("pickRandom")];
	int f_sellable =	odItem.slkFieldId[SLKHash("sellable")];

	int cnt;
	if (useRandomCreep)
	{
		for(cnt = odUnit.orgObjectCount;cnt>0;cnt--)
		{
			if ((odUnit.slkData[cnt][f_isbldg]==NULL || odUnit.slkData[cnt][f_isbldg][0]!='1') && odUnit.slkData[cnt][f_race]!=NULL && strcmp(odUnit.slkData[cnt][f_race]+1,"creeps")==0)
			{
				if (odUnit.slkData[cnt][f_tilesets]!=NULL)
				{
					char* s = odUnit.slkData[cnt][f_tilesets]+1;
					for(;*s!='*' && *s!=mapTileset && *s!=NULL;s++)
						;
					if (*s!=NULL)
						objectUsed[OBJHash(odUnit.slkData[cnt][1]+1)]=true;
				}
			}
		}
	}

	if (useRandomBuild)
	{
		for(cnt = odUnit.objectCount;cnt>0;cnt--)
		{
			if ((odUnit.slkData[cnt][f_nbrandom]!=NULL && odUnit.slkData[cnt][f_nbrandom][0]=='1') && odUnit.slkData[cnt][f_isbldg]!=NULL && odUnit.slkData[cnt][f_isbldg][0]=='1' && odUnit.slkData[cnt][f_race]!=NULL && strcmp(odUnit.slkData[cnt][f_race]+1,"creeps")==0)
			{
				if (odUnit.slkData[cnt][f_tilesets]!=NULL)
				{
					char* s = odUnit.slkData[cnt][f_tilesets]+1;
					for(;*s!='*' && *s!=mapTileset && *s!=NULL;s++)
						;
					if (*s!=NULL)
						objectUsed[OBJHash(odUnit.slkData[cnt][1]+1)]=true;
				}
			}
		}
	}

	if (useRandomItem)
	{
		for(cnt = odItem.objectCount;cnt>0;cnt--)
		{
			if (odItem.slkData[cnt][f_pickRandom]!=NULL && odItem.slkData[cnt][f_pickRandom][0]=='1')
				objectUsed[OBJHash(odItem.slkData[cnt][1]+1)]=true;
		}
	}

	if (useMarket)
	{
		for(cnt = odItem.objectCount;cnt>0;cnt--)
		{
			if (odItem.slkData[cnt][f_sellable]!=NULL && odItem.slkData[cnt][f_pickRandom]!=NULL && odItem.slkData[cnt][f_sellable][0]=='1' && odItem.slkData[cnt][f_pickRandom][0]=='1')
				objectUsed[OBJHash(odItem.slkData[cnt][1]+1)]=true;
		}
	}
}
#define AddMapFileList(fileName) MapFileList[MapFileNumFound++]=hmqHashString[HMQHash(fileName)]
char* blpFileOptList;
extern void ReadMapFile(char*);
void SearchFilesInMDX(char* buffer)
{
	char* s = buffer;
	if (*s!='M' || s[1]!='D' || s[2]!='L' || s[3]!='X')
		return;
	s+=4;
	if (*s=='V' && s[1]=='E' && s[2]=='R' && s[3]=='S')
		s+=(int&)*(s+4)+8;
	if (*s=='M' && s[1]=='O' && s[2]=='D' && s[3]=='L')
		s+=(int&)*(s+4)+8;
	if (*s=='S' && s[1]=='E' && s[2]=='Q' && s[3]=='S')
		s+=(int&)*(s+4)+8;
	if (*s=='G' && s[1]=='L' && s[2]=='B' && s[3]=='S')
		s+=(int&)*(s+4)+8;
	if (*s=='M' && s[1]=='T' && s[2]=='L' && s[3]=='S')
		s+=(int&)*(s+4)+8;
	if (*s!='T' || s[1]!='E' || s[2]!='X' || s[3]!='S')
		return;
	s+=4;
	int i = ((int&)*s)/0x10c;
	s+=4;

	for(;i>0;i--)
	{
		ReadMapFile(s+4);
		s+=0x10c;
	}
}
void SearchLoadingMDX(char* fileName)
{
	if (fileName==NULL || *fileName==NULL)
		return;
	HANDLE hFile;
	if (!SFileOpenFileEx(g_hMPQ,fileName,NULL,&hFile))
		return;

	int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
	char* buffer = (char*)malloc(size);
	SFileReadFile(hFile,buffer,size,NULL,NULL);
	SFileCloseFile(hFile);
	SearchFilesInMDX(buffer);
	int fileId=HMQHash(fileName);
	if (hmqReadFile[fileId]==0)
	{
		hmqReadFile[fileId]=1;
		AddMapFileList(fileName);
		AddBufferFile(fileName,buffer,size,1);
	}
	else
		free(buffer);
}

void SearchOptBlp(char* fileName)
{
	HANDLE hFile;
	if (!SFileOpenFileEx(g_hMPQ,fileName,NULL,&hFile))
		return;
	int fileId=HMQHash(fileName);
	blpFileOptList[fileId]=blpMipNum;
	if (hmqReadFile[fileId]==0)
	{
		hmqReadFile[fileId]=1;
		AddMapFileList(fileName);
		int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
		char* buffer=(char*)malloc(size);
		SFileReadFile(hFile,buffer,size,NULL,NULL);
		AddBufferFile(fileName,buffer,size,1);
	}
	SFileCloseFile(hFile);

	char* s2=fileName;
	for(;*fileName!=NULL;fileName++)
	{
		if (*fileName=='\\')
			s2=fileName+1;
	}
	s2=StrAddTemp("ReplaceableTextures\\CommandButtonsDisabled\\DIS",s2);
	if (!SFileOpenFileEx(g_hMPQ,s2,NULL,&hFile))
		return;
	fileId=HMQHash(s2);
	blpFileOptList[fileId]=blpMipNum;
	if (hmqReadFile[fileId]==0)
	{
		hmqReadFile[fileId]=1;
		AddMapFileList(s2);
		int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
		char* buffer=(char*)malloc(size);
		SFileReadFile(hFile,buffer,size,NULL,NULL);
		AddBufferFile(s2,buffer,size,1);
	}
	SFileCloseFile(hFile);
}

void SearchFilesInW3I(char* buffer)
{
	if (*buffer<25)	//tft=25,roc=18
		return;
	char* s = buffer;
	s+=12;
	for(int n=0;n<4;n++)
	{
		for(;*s!=NULL;s++)
			;
		s++;
	}
	s+=65;
	SearchLoadingMDX(s);
	for(int n=0;n<4;n++)
	{
		for(;*s!=NULL;s++)
			;
		s++;
	}
	s+=4;
	SearchLoadingMDX(s);
}

void SearchFilesInW3O(char* buffer,bool haveExtraField)
{
	buffer+=4;
	for (int x=0;x<2;x++)
	{
		int num=(int &)*buffer;
		buffer+=4;
		for(;num>0;num--)
		{
			buffer+=8;
			int edNum=(int &)*buffer;
			buffer+=4;
			for(;edNum>0;edNum--)
			{
				int field=(int &)*buffer;
				buffer+=4;
				if ((int &)*buffer!=3)
				{
					buffer+=(haveExtraField?20:12);
					continue;
				}

				buffer+=(haveExtraField?12:4);
				char* s = buffer;

				for(;*buffer!=NULL;buffer++)
				{
					if (*buffer==',')
					{

                        if (buffer[-4]=='.')
						{
							if ((buffer[-1]|32) == 'p')
							{
								if (field=='fxtb')
									ReadMapFile(s);
								else
									SearchOptBlp(s);
							}
							else if (field=='ldmu')
							{
								buffer[-4]=NULL;
								ReadMapFile(StrAddTemp(s,".mdx"));
								ReadMapFile(StrAddTemp(s,"_V1.mdx"));
							}
							else if (field=='lifb' || field=='lifd')
							{
								buffer[-4]=NULL;
								ReadMapFile(StrAddTemp(s,".mdx"));
								ReadMapFile(StrAddTemp(s,"0.mdx"));
								ReadMapFile(StrAddTemp(s,"1.mdx"));
								ReadMapFile(StrAddTemp(s,"2.mdx"));
								ReadMapFile(StrAddTemp(s,"3.mdx"));
								ReadMapFile(StrAddTemp(s,"4.mdx"));
								ReadMapFile(StrAddTemp(s,"5.mdx"));
								ReadMapFile(StrAddTemp(s,"6.mdx"));
								ReadMapFile(StrAddTemp(s,"7.mdx"));
								ReadMapFile(StrAddTemp(s,"8.mdx"));
								ReadMapFile(StrAddTemp(s,"9.mdx"));
							}
							else
							{
								*buffer=NULL;
								if ((buffer[-1]|32) == 'l')
									buffer[-1]='x';
								ReadMapFile(s);
							}
						}
						s=buffer+1;
					}
				}

				if (buffer[-4]=='.')
				{
					if ((buffer[-1]|32) == 'p')
					{
						if (field=='fxtb')
							ReadMapFile(s);
						else
							SearchOptBlp(s);
					}
					else if (field=='ldmu')
					{
						buffer[-4]=NULL;
						ReadMapFile(StrAddTemp(s,".mdx"));
						ReadMapFile(StrAddTemp(s,"_V1.mdx"));
					}
					else if (field=='lifb' || field=='lifd')
					{
						buffer[-4]=NULL;
						ReadMapFile(StrAddTemp(s,".mdx"));
						ReadMapFile(StrAddTemp(s,"0.mdx"));
						ReadMapFile(StrAddTemp(s,"1.mdx"));
						ReadMapFile(StrAddTemp(s,"2.mdx"));
						ReadMapFile(StrAddTemp(s,"3.mdx"));
						ReadMapFile(StrAddTemp(s,"4.mdx"));
						ReadMapFile(StrAddTemp(s,"5.mdx"));
						ReadMapFile(StrAddTemp(s,"6.mdx"));
						ReadMapFile(StrAddTemp(s,"7.mdx"));
						ReadMapFile(StrAddTemp(s,"8.mdx"));
						ReadMapFile(StrAddTemp(s,"9.mdx"));
					}
					else
					{
						if ((buffer[-1]|32) == 'l')
							buffer[-1]='x';
						ReadMapFile(s);
					}
				}
				buffer+=5;
			}
		}
	}
}

void SearchFilesInTXT(char* buffer)
{
	char* str;
	for(;*buffer!=NULL;buffer++)
	{
		if (*buffer=='=')
		{
			str = ++buffer;
			for(;;)		//.mdl .mdx .mp3 .wav .blp .tga
			{
				switch (*buffer)
				{
				case '.':
					switch (buffer[4])
					{
					case ',':
						buffer[4]=NULL;
                        
						if ((buffer[3]|32) == 'p')
							SearchOptBlp(str);
						else
						{
							if ((buffer[3]|32) == 'l')
								buffer[3]='x';
							ReadMapFile(str);
						}
						buffer+=5;
						str=buffer;
						break;
					case '\n':
					case '\r':
                        buffer[4]=NULL;
                        if ((buffer[3]|32) == 'p')
							SearchOptBlp(str);
						else
						{
							if ((buffer[3]|32) == 'l')
								buffer[3]='x';
							ReadMapFile(str);
						}
						buffer+=5;
						goto SearchFilesInTXT_LoopEnd;
                    case NULL:
                        if ((buffer[3]|32) == 'p')
							SearchOptBlp(str);
						else
						{
							if ((buffer[3]|32) == 'l')
								buffer[3]='x';
							ReadMapFile(str);
						}
						return;
					default:
						buffer++;
					}
					break;
				case '\n':
				case '\r':
					goto SearchFilesInTXT_LoopEnd;
				case NULL:
					return;
				default:
					buffer++;
				}

			}
		}
SearchFilesInTXT_LoopEnd:
		;
	}
}

void SearchFilesInSLK(char* fileName, char* buffer)
{
	unsigned char exSet = 0,fileX=0;
	if (stricmp(fileName,"Units\\UnitUI.slk")==0)
		exSet=1;
	else if (stricmp(fileName,"Units\\DestructableData.slk")==0 || stricmp(fileName,"Doodads\\Doodads.slk")==0)
		exSet=2;

	char* str;
	for(; *buffer!='C' || (buffer[-1]!='\n' && buffer[-1]!='\r'); buffer++)	//Search for C; start
			;

	if (exSet>0)
	{
		for(;;buffer++)
		{
			if (*buffer=='\"')
			{
				if (memcmp(buffer+1,"file\"",5)==0)
				{
					char* s = buffer-4;
					for(;*s!='X';s--)
						;
					fileX=a2ui(s+1);
					buffer+=6;
					break;
				}
				for(;buffer[1]!='\n' && buffer[1]!='\r'; buffer++)
					;
			}
		}
	}

	for(;;buffer++)
	{
		if (*buffer=='\"')
		{
			for(str=++buffer; *buffer!='\"' || (buffer[1]!='\n' && buffer[1]!='\r'); buffer++)
				;
			if (exSet)
			{
				char* x=str-4;
				for(;*x!='X';x--)
					;
				if (a2ui(x+1)==fileX)
				{
					if (buffer[-4]=='.')
						buffer[-4]=NULL;
					else
						*buffer=NULL;
					if (exSet==1)
					{
						ReadMapFile(StrAddTemp(str,".mdx"));
						ReadMapFile(StrAddTemp(str,"_V1.mdx"));
					}
					else if (exSet==2)
					{
						ReadMapFile(StrAddTemp(str,".mdx"));
						ReadMapFile(StrAddTemp(str,"0.mdx"));
						ReadMapFile(StrAddTemp(str,"1.mdx"));
						ReadMapFile(StrAddTemp(str,"2.mdx"));
						ReadMapFile(StrAddTemp(str,"3.mdx"));
						ReadMapFile(StrAddTemp(str,"4.mdx"));
						ReadMapFile(StrAddTemp(str,"5.mdx"));
						ReadMapFile(StrAddTemp(str,"6.mdx"));
						ReadMapFile(StrAddTemp(str,"7.mdx"));
						ReadMapFile(StrAddTemp(str,"8.mdx"));
						ReadMapFile(StrAddTemp(str,"9.mdx"));
					}
					continue;
				}
			}
			if (buffer[-4]=='.')	//.mdl .mdx .blp .tga
			{
				*buffer=NULL;
				if ((buffer[-1]|32) == 'l')	//仅.mdl是有用的
					buffer[-1]='x';
				ReadMapFile(str);
			}
		}
		else if (*buffer=='E')
			break;
	}
}

void SearchFilesInJ(char* buffer)
{
	char* str;
	for(;*buffer!=NULL;buffer++)
	{
		if (*buffer=='\"')
		{
			str = ++buffer;
			int i=0;
			for(;*buffer!='\"';)
			{
				if (*buffer=='\\')
					buffer++;
				str[i++] = *(buffer++);
			}
			str[i]=NULL;
			if (str[i-4]=='.')
			{
				if ((str[i-1]|32)=='l')
					str[i-1]='x';
				ReadMapFile(str);
			}
		}
		else if (*buffer=='/')
		{
			if (buffer[1]=='/')
			{
				for (buffer += 2; *buffer != '\0' && *buffer != '\n' && *buffer != '\r'; buffer++)
					;
			}
		}
	}
}

void SearchFilesInImp()
{
	HANDLE hFile;
	if (SFileOpenFileEx(g_hMPQ,"war3map.imp",NULL,&hFile)==NULL)
		return;
	int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
	char* buffer = (char*)malloc(size);
	SFileReadFile(hFile,buffer,size,NULL,NULL);

	char* bfPos = buffer;
	bfPos+=4;
	int cnt = (int&)*bfPos;
	bfPos+=4;
	for(;cnt>0;cnt--)
	{
		if (*(bfPos++)>9)	//10 or 13
			ReadMapFile(bfPos);
		else				//5 or 8
			ReadMapFile(StrAddTemp("war3mapImported\\",bfPos));
		for(;*bfPos!=NULL;bfPos++)
			;
		bfPos++;
	}
	SFileCloseFile(hFile);
	free(buffer);
}

void SearchFilesInBuffer(char* fileName)
{
	int len = strlen(fileName);
	if (len>3)
		fileName[len-4]|=32,fileName[len-3]|=32,fileName[len-2]|=32,fileName[len-1]|=32;	//小写化
	if ((fileName[len-3])=='w' && (fileName[len-2])=='3' && fileName[len-4]=='.')
	{
		switch (fileName[len-1])
		{
		case 'i':
		case 'u':
		case 't':
		case 'b':
		case 'h':
		case 'd':
		case 'a':
		case 'q':
			break;
		default:
			return;
		}
	}
	else if (fileName[len-3]=='m' && fileName[len-2]=='d' && fileName[len-1]=='x' && fileName[len-4]=='.')
		;
	else if (fileName[len-3]=='t' && fileName[len-2]=='x' && fileName[len-1]=='t' && fileName[len-4]=='.')
		;
	else if (fileName[len-3]=='s' && fileName[len-2]=='l' && fileName[len-1]=='k' && fileName[len-4]=='.')
		;
	else if (fileName[len-1]=='j' && fileName[len-2]=='.')
		;
	else
	{
		if (fileName[len-3]=='b' && fileName[len-2]=='l' && fileName[len-1]=='p' && fileName[len-4]=='.')
		{
			if (memicmp(fileName,"ReplaceableTextures\\",20)==0 && (memicmp(fileName+20,"CommandButtons",14)==0 || memicmp(fileName+20,"PassiveButtons",14)==0))
				blpFileOptList[HMQHash(fileName)]=blpMipNum;

			char* s=fileName;
			for(;*fileName!=NULL;fileName++)
			{
				if (*fileName=='\\')
					s=fileName+1;
			}
			s=StrAddTemp("ReplaceableTextures\\CommandButtonsDisabled\\DIS",s);
			HANDLE hFile;
			if (!SFileOpenFileEx(g_hMPQ,s,NULL,&hFile))
				return;
			int fileId=HMQHash(s);
			if (hmqReadFile[fileId]==0)
			{
				blpFileOptList[fileId]=blpMipNum;//blp opt
				hmqReadFile[fileId]=1;
				AddMapFileList(s);
				int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
				char* str=(char*)malloc(size);
				SFileReadFile(hFile,str,size,NULL,NULL);
				AddBufferFile(s,str,size,1);
			}
			SFileCloseFile(hFile);
		}
		return;
	}

	HANDLE hFile;	
    SFileOpenFileEx(g_hMPQ,fileName,NULL,&hFile);
	int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
	char* buffer = (char*)malloc(size+1);
	SFileReadFile(hFile,buffer,size,NULL,NULL);
	SFileCloseFile(hFile);
	buffer[size]=NULL;

	switch (fileName[len-1])
	{
	case 'x':	//mdx
		SearchFilesInMDX(buffer);
		{
			int len=strlen(fileName);
			char* s=(char*)malloc(len+10);
			memcpy(s,fileName,len-4), memcpy(s+len-4,"_portrait.mdx",14);
			if (!SFileOpenFileEx(g_hMPQ,s,NULL,&hFile))
				break;
			int fileId=HMQHash(s);
			if (hmqReadFile[fileId]==0)
			{
				size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
				hmqReadFile[fileId]=1;
				AddMapFileList(s);
				char* str;
				str=(char*)malloc(size);
				SFileReadFile(hFile,str,size,NULL,NULL);
				AddBufferFile(s,str,size,1);
			}
			SFileCloseFile(hFile);
			SearchFilesInMDX(s);
			free(s);
		}
		break;
	case 't':	//txt & w3t
		if (fileName[len-2]=='x')
        {
            SearchFilesInTXT(buffer);
        }
		else
			SearchFilesInW3O(buffer,0);
		break;
	case 'k':
		SearchFilesInSLK(fileName,buffer);
		break;
	case 'j':
		SearchFilesInJ(buffer);
		break;
	case 'i':
		SearchFilesInW3I(buffer);
		break;
	case 'u':
	case 'b':
	case 'h':
		SearchFilesInW3O(buffer,0);
		break;
	case 'd':
	case 'a':
	case 'q':
		SearchFilesInW3O(buffer,1);
		break;
	}

	free(buffer);
}

void ReadMapFile(char* fileName)
{
	if (fileName==NULL || *fileName==NULL)
		return;

	HANDLE hFile;
	if (!SFileOpenFileEx(g_hMPQ,fileName,NULL,&hFile))
		return;
	int fileId=HMQHash(fileName);
	if (hmqReadFile[fileId]==0)
	{
		int size=SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
		hmqReadFile[fileId]=1;
		AddMapFileList(fileName);
		fileName=hmqHashString[HMQHash(fileName)];	//fileName可能是临时字符串
		char* str;
		switch(hmqFileType[fileId])
		{
		case FILE_TYPE_NORMAL:
			str=(char*)malloc(size);
			SFileReadFile(hFile,str,size,NULL,NULL);
			AddBufferFile(fileName,str,size,1);
			SearchFilesInBuffer(fileName);
			break;
		case FILE_TYPE_REMOVE_ALWAYS:
			break;
		case FILE_TYPE_FOR_WE_ONLY:
			if (removeWEOnlyFiles==0)
			{
				str=(char*)malloc(size);
				SFileReadFile(hFile,str,size,NULL,NULL);
				AddBufferFile(fileName,str,size,1);
			}
			break;
		case FILE_TYPE_OPEN_FOR_USE:
		case FILE_TYPE_SLK_FILES:
			SearchFilesInBuffer(fileName);
			break;
		}
    }
    SFileCloseFile(hFile);
}

void ReadListFile(char* listFile)
{
	char* tempStr=listFile;
	for(;;)
	{
		tempStr=listFile;
		for(;*listFile!='\r' && *listFile!='\n';listFile++)
		{
			if (*listFile==NULL)
			{
				ReadMapFile(tempStr);
				return;
			}
		}
		for(*(listFile++)=NULL;*listFile=='\r' || *listFile=='\n';listFile++)
			;
		ReadMapFile(tempStr);
		if (MapFileNumFound==MapFileNum)
			return;
	}
}

BOOL ListMapFile(char* mapName)
{
  chdir(szAppDir);

	for(;bufferFileCount>0;bufferFileCount--)
		free(bufferFile[bufferFileCount]);
	FlushGlobalCache();

	if (mapName==NULL || *mapName==NULL)
		return FALSE;
	SFileOpenArchive(mapName,0,0,&g_hMPQ);
	if ((int)g_hMPQ<=0)
		return FALSE;

	int hashTableSize = ((MPQARCHIVE*)g_hMPQ)->MpqHeader.dwHashTableSize;
	int blockTableSize = ((MPQARCHIVE*)g_hMPQ)->MpqHeader.dwBlockTableSize;
	HASHTABLEENTRY* hashTable = ((MPQARCHIVE*)g_hMPQ)->lpHashTable;
	MapFileNum=0;
	MapFileNumFound=0;
	for (int i=0; i<hashTableSize; i++)
	{
		if (hashTable[i].dwBlockTableIndex<blockTableSize)
			MapFileNum++;
	}
	MapFileList=(char**)Calloc(MapFileNum*4);

	HMQHashInit(g_hMPQ);
	blpFileOptList = Calloc(hmqHashSize);
	HANDLE hFile;
	if (SFileOpenFileEx(g_hMPQ,"(attributes)",NULL,&hFile))	//正常listfile可能没包含的2个文件
	{
		if (hmqReadFile[HMQHash("(attributes)")]==0)
		{
			hmqReadFile[HMQHash("(attributes)")]=1;
			AddMapFileList("(attributes)");
		}
		SFileCloseFile(hFile);
	}
	if (SFileOpenFileEx(g_hMPQ,"(listfile)",NULL,&hFile))
	{
		if (hmqReadFile[HMQHash("(listfile)")]==0)
		{
			hmqReadFile[HMQHash("(listfile)")]=1;
			AddMapFileList("(listfile)");
		}
		SFileCloseFile(hFile);
	}

	int listFP = open(szListFilePath,O_BINARY);
	if (listFP > 0)
	{
		int size = filelength(listFP);
		char* listFile = (char*)malloc(size+1);
		read(listFP,listFile,size);
		listFile[size]=NULL;
		ReadListFile(listFile);
		close(listFP);
		free(listFile);
	}
	if (MapFileNumFound<MapFileNum && SFileOpenFileEx(g_hMPQ,"(listfile)",NULL,&hFile))
	{
		int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
		char* listFile = (char*)malloc(size+1);
		SFileReadFile(hFile,listFile,size,NULL,NULL);
		listFile[size]=NULL;
		ReadListFile(listFile);
		SFileCloseFile(hFile);
		free(listFile);
	}
	if (MapFileNumFound<MapFileNum)
	{
		SearchFilesInImp();
		int listFP = open("listfile.txt",O_BINARY);
		if (listFP > 0)
		{
			int size = filelength(listFP);
			char* listFile = (char*)malloc(size+1);
			read(listFP,listFile,size);
			listFile[size]=NULL;
			ReadListFile(listFile);
			close(listFP);
			free(listFile);
		}
	}
	SFileCloseArchive(g_hMPQ);

	for (int i=1; i<MapFileNumFound; i++)
	{
		char* temp = MapFileList[i];
		int j=i;
		for (; j>0 && stricmp(temp,MapFileList[j-1])<0; j--)
			MapFileList[j]=MapFileList[j-1];
		MapFileList[j]=temp;
	}

	int fp = creat("(listfile)",S_IWRITE);
	for(int i=0;i<MapFileNumFound;i++)
	{
		if (MapFileList[i]!=NULL)
		{
			write(fp,MapFileList[i],strlen(MapFileList[i]));
			write(fp,"\n",1);
		}
	}
	close(fp);

  return TRUE;
}

void SetHashTable()
{
	HANDLE hFile;
	hashTableSize=bufferFileCount+3;	//war3mapmisc.txt & war3map.j & war3map.w3i
	if (keepListFile==1)
		hashTableSize++;
	if (SFileOpenFileEx(g_hMPQ,"war3map.wts",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }
	if (SFileOpenFileEx(g_hMPQ,"war3map.doo",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }
	if (SFileOpenFileEx(g_hMPQ,"war3mapskin.txt",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }

	if (OPT_UNIT>0)
		hashTableSize+=18;
	else if (SFileOpenFileEx(g_hMPQ,"war3map.w3u",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }
	if (OPT_ITEM>0)
		hashTableSize+=4;
	else if (SFileOpenFileEx(g_hMPQ,"war3map.w3t",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }
	if (OPT_DEST>0)
		hashTableSize+=2;
	else if (SFileOpenFileEx(g_hMPQ,"war3map.w3b",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }
	if (OPT_DOOD>0)
		hashTableSize+=2;
	else if (SFileOpenFileEx(g_hMPQ,"war3map.w3d",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }

	if (OPT_ABIL>0)
		hashTableSize+=18;
	if (SFileOpenFileEx(g_hMPQ,"war3map.w3a",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }
	if (SFileOpenFileEx(g_hMPQ,"war3map.w3h",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }
	if (OPT_UPGR>0)
		hashTableSize+=14;
	else if (SFileOpenFileEx(g_hMPQ,"war3map.w3q",NULL,&hFile))
    {
		hashTableSize++;
        SFileCloseFile(hFile);
    }
}

void InitOriginLevel()
{
	char* str="ARrk10 BPca10 BObo10 BRrk10 LRrk10 NObo10 NRic10 NRrk10 CRrk10 DRrk10 GRrk10 ZOsh10 ZRrk10 ZWcl10 ORrk10 ORrs10 IRgc10 IRrs10 BPtw10 ZPtw10 IRcy9 JOgr9 BRrs8 BRsp8 CRrs8 NRwr8 IRrk8 ZRrs8 ZRsp8 OPop8 APtv7 NPth7 KOdr7 LOss6 ZPms6 ZZgr6 ORrr6 LOhb5 LOrh5 ZOrp5 ZWsw5 ZPfw5 ORmk5 OOob5 ANbr4 Arsg4 Aamk4 Arsq4 ANsb4 ANhw4 Arsw4 ANhx4 ANcf4 Acdb4 Acdh4 AOr24 AOs24 AOw24 APms4 CPms4 LPrs4 LOrb4 LOsk4 YOgr4 DRst4 DOlc4 GRst4 GOlc4 ZOss4 IWie4 IRic4 ZRbs4 ZRbd4 ORfk4 ZPvp4 DOsv4 AHbz3 AHab3 AHwe3 ANsg3 ANsq3 ANsw3 AOcr3 AOmi3 AOwk3 AHbn3 AHfs3 AHdr3 AUcb3 AUim3 AUts3 ANba3 ANdr3 ANsi3 AUdc3 AUdp3 AUau3 AEev3 AEim3 AEmb3 AUsl3 AUav3 AUcs3 AOcl3 AOfs3 AOsf3 AEer3 AEfn3 AEah3 AUdr3 AUfa3 AUfu3 AUfn3 AHbh3 AHtb3 AHtc3 ANfl3 ANfa3 ANms3 AHad3 AHds3 AHhb3 ANbf3 ANdb3 ANdh3 ANht3 ANca3 ANrf3 AHfa3 AEst3 AEar3 AOae3 AOsh3 AOws3 AOhw3 AOhx3 AOsw3 AEbl3 AEfk3 AEsh3 ANab3 ANcr3 ANhs3 ANeg3 ANcs3 ANc13 ANc23 ANc33 ANsy3 ANs13 ANs23 ANs33 ANic3 ANia3 ANso3 ANlm3 ANfb3 ANdp3 ANrc3 ANr33 ANmo3 AEpa3 ANwm3 AHca3 ANcl3 ANbl3 ACs83 ACs73 ANsh3 Rhme3 Rhra3 Rhar3 Rhac3 Rhla3 Rome3 Rora3 Roar3 Rosp3 Rume3 Rura3 Ruar3 Rucr3 Resm3 Resw3 Rema3 Rerh3 Rgfo3 Rnat3 Rnam3 AOob3 AOsr3 AWfl3 ASbr3 BOct3 BRrp3 BRgs3 BSar3 CSbr3 LPfp3 LOsh3 LOpg3 LOsm3 NOtb3 NSrb3 VSvb3 XOmr3 YOec3 DOch3 DSp03 DSp93 GSp03 GSp93 ZSas3 ZSs13 ZSrb3 ZPsh3 ZPlp3 IOob3 IOpr3 ZOls3 AOnt3 DOsw3 ISrb3 Arsp2 AOls2 Acef2 AOr32 Arai2 AIgu2 Rhpt2 Rhst2 Rhlh2 Rowd2 Rost2 Rowt2 Rune2 Ruba2 Redt2 Redc2 Rnsw2 AObo2 ASbc2 BOtt2 BRcr2 BRfs2 BSrc2 CObo2 CPbs2 CRfs2 CSbc2 LPcw2 LOh12 LOo12 LOic2 LOlp2 LOtr2 LSeb2 NObc2 NRfs2 DOtp2 DOjp2 ZObz2 ZOst2 ZOsb2 ZOob2 ZOrt2 ZOtb2 ZPru2 ZWca2 ZZdt2 LOwf2 ZOtr2 OOgr2 OOsk2 OOst2 OOsd2 ";
	for(;*str!=NULL;)
	{
		objectLevel[OBJHash(str)]=a2i(str+4);
		for(str+=5;*str!=' ';str++)
			;
		str++;
	}
}

#include "TxtSlkOpt.h"
void WriteFilesToMap(char* mapName)
{
	HANDLE destMPQ=MpqOpenArchiveForUpdate(mapName, MOAU_CREATE_ALWAYS, hashTableSize);
	if (destMPQ == (HANDLE)-1)
	{
		MessageBox(NULL, TEXT("打开地图失败!"), TEXT("Wc3MapMax++"), 0);
		return;
	}
	MpqCloseUpdatedArchive(destMPQ,NULL);
	int fp=open(mapName,O_BINARY|O_RDWR);
	int size=filelength(fp);
	char* s=(char*)malloc(size);
	read(fp,s,size);
	lseek(fp,0,SEEK_SET);
	write(fp,mapHeader,512);
	write(fp,s,size);
	close(fp);
	free(s);

	int maxBlockSize=0,maxWavSize=0;
	int blockSize=512,wavSize=512;	
	for(int i=0;i<bufferFileCount;i++)
	{
		int len = strlen(bufferFileName[i]);
		if ((bufferFileName[i][len-1] | 32)=='v' && (bufferFileName[i][len-2] | 32)=='a' && (bufferFileName[i][len-3] | 32)=='w' && bufferFileName[i][len-4]=='.')
		{
			for(;bufferFileSize[i]>wavSize;maxWavSize++,wavSize<<=1)
				;
		}
		else
		{
			for(;bufferFileSize[i]>blockSize;maxBlockSize++,blockSize<<=1)
				;
		}
	}

	destMPQ=MpqOpenArchiveForUpdate(mapName, MOAU_OPEN_ALWAYS, 0);
	(*(MPQARCHIVE*)destMPQ).MpqHeader.wBlockSize=maxBlockSize;
	for(int i=0;i<bufferFileCount;i++)
	{
		int len = strlen(bufferFileName[i]);

		if ((bufferFileName[i][len-1] | 32) == 'v' 
      && (bufferFileName[i][len-2] | 32) == 'a' 
      && (bufferFileName[i][len-3] | 32) == 'w' 
      && (bufferFileName[i][len-4] == '.'))
    {
      if (bufferFileSize[i] > blockSize)
        MpqAddWaveFromBuffer(destMPQ, bufferFile[i], bufferFileSize[i], bufferFileName[i], MAFA_COMPRESS, MAWA_QUALITY_HIGH);
    }
		else if ((bufferFileName[i][len-1] | 32) == 'p' 
      && (bufferFileName[i][len-2] | 32) == 'l' 
      && (bufferFileName[i][len-3] | 32) == 'b'
      && (bufferFileName[i][len-4] == '.'))
    {
      if (blpForOpt > 0)
        bufferFileSize[i] = BlpOpt(bufferFile[i], bufferFileSize[i], blpFileOptList[HMQHash(bufferFileName[i])]);
    }
		else  if ((bufferFileName[i][len-1] | 32) == 'x' 
      && (bufferFileName[i][len-2] | 32) == 'd' 
      && (bufferFileName[i][len-3] | 32) == 'm' 
      && (bufferFileName[i][len-4] == '.'))
    {
      if (mdxOptLevel > 0)
        MdxOpt(bufferFile[i],bufferFileSize[i]);
    }

		MpqAddFileFromBufferEx(destMPQ, bufferFile[i], bufferFileSize[i], bufferFileName[i], MAFA_COMPRESS, MAFA_COMPRESS_DEFLATE, 9);
	}

	if (keepListFile)
	{
		char* listFile = Malloc(0x10000);	//64k
		char* tempStr = listFile;
		for(int i=0;i<bufferFileCount;i++)
		{
			for(char* s=bufferFileName[i];*s!=NULL;tempStr++,s++)
				*tempStr=*s;
			*(tempStr++)='\n';
		}
		AddBufferFile("(listfile)",listFile,tempStr-listFile,0);
		MpqAddFileFromBufferEx(destMPQ, listFile, tempStr-listFile, "(listfile)", MAFA_COMPRESS, MAFA_COMPRESS_DEFLATE, 9);
	}
	MpqCompactArchive(destMPQ);
	int mapSize=SFileGetFileInfo(destMPQ,SFILE_INFO_SIZE);
	MpqCloseUpdatedArchive(destMPQ,NULL);

	if (wavAutoBlockSize && maxWavSize>3)	//3 -> 4k
	{
		if (maxWavSize>maxBlockSize)
			maxWavSize=maxBlockSize;
		int len = strlen(mapName);
		char* tmpName=Malloc(len+1);
		memcpy(tmpName,mapName,len-3);
		memcpy(tmpName+len-3,"tmp",4);
		for(;maxWavSize>3;)
		{
			maxWavSize--,wavSize>>=1;
			HANDLE hMPQ=MpqOpenArchiveForUpdate(tmpName, MOAU_CREATE_ALWAYS, hashTableSize);
			MpqCloseUpdatedArchive(hMPQ,NULL);
			fp=open(tmpName,O_BINARY|O_RDWR);
			int size=filelength(fp);
			char* s=(char*)malloc(size);
			read(fp,s,size);
			lseek(fp,0,SEEK_SET);
			write(fp,mapHeader,512);
			write(fp,s,size);
			close(fp);
			free(s);
			hMPQ=MpqOpenArchiveForUpdate(tmpName, MOAU_OPEN_ALWAYS, 0);
			(*(MPQARCHIVE*)hMPQ).MpqHeader.wBlockSize=maxWavSize;

			for(int i=0;i<bufferFileCount;i++)
			{
				int len = strlen(bufferFileName[i]);
				if (bufferFileSize[i]>wavSize && (bufferFileName[i][len-1] | 32)=='v' && (bufferFileName[i][len-2] | 32)=='a' && (bufferFileName[i][len-3] | 32)=='w' && bufferFileName[i][len-4]=='.')
					MpqAddWaveFromBuffer(hMPQ, bufferFile[i], bufferFileSize[i], bufferFileName[i], MAFA_COMPRESS, MAWA_QUALITY_HIGH);
				else
					MpqAddFileFromBufferEx(hMPQ, bufferFile[i], bufferFileSize[i], bufferFileName[i], MAFA_COMPRESS, MAFA_COMPRESS_DEFLATE, 9);
			}
			MpqCompactArchive(hMPQ);
			int newSize=SFileGetFileInfo(hMPQ,SFILE_INFO_SIZE);
			MpqCloseUpdatedArchive(hMPQ,NULL);
			if (newSize<mapSize)
			{
				mapSize=newSize;
				remove(mapName);
				rename(tmpName,mapName);
			}
			else
				remove(tmpName);
		}
	}
	for(int i=0;i<bufferFileCount;i++)
	{
		if (bufferNeedRemove[i])
			free(bufferFile[i]);
	}
}

void WriteExDataNum(char*& bufferPos,char* idStr,char* fdStr,bool pMark)
{
	int oid=objectId[OBJHash(idStr)];
	ObjectData* od=(ObjectData*)Malloc(4);
	if (oid<=odAbil.objectCount && odAbil.slkData[oid][1]!=NULL && objectId[OBJHash(odAbil.slkData[oid][1]+1)]==oid)
		*od=odAbil;
	else if (oid<=odUnit.objectCount && odUnit.slkData[oid][1]!=NULL && objectId[OBJHash(odUnit.slkData[oid][1]+1)]==oid)
		*od=odUnit;
	else if (oid<=odItem.objectCount && odItem.slkData[oid][1]!=NULL && objectId[OBJHash(odItem.slkData[oid][1]+1)]==oid)
		*od=odItem;
	else if (oid<=odUpgr.objectCount && odUpgr.slkData[oid][1]!=NULL && objectId[OBJHash(odUpgr.slkData[oid][1]+1)]==oid)
		*od=odUpgr;
	else
	{
		*(bufferPos++)='0';
		return;
	}
	char* s;
	int fid = od->slkFieldId[SLKHash(fdStr)];

	if (od->slkField[fid]!=NULL && strcmp(od->slkField[fid],fdStr)==0)
		s = od->slkData[oid][fid];
	else
	{
		char* fd2=fdStr;
		int i=0;
		for(;*fdStr > '9';)
			fdStr++,i++;
		int x=0;
		for(; *fdStr>='0' && *fdStr<='9'; fdStr++)
			x=x*10+(*fdStr & 0xf);
		if (x>od->slkObjLevel[oid])
		{
			*(bufferPos++)='0';
			return;
		}
		fd2[i]=NULL;
		fid = od->slkFieldId[SLKHash(fd2)];

		if (od->slkField[fid]!=NULL && strcmp(od->slkField[fid],fd2)==0)
			s = ((char**)od->slkData[oid][fid])[x-1];
		else
		{
			*(bufferPos++)='0';
			return;
		}
	}
	if (s==NULL)
	{
		*(bufferPos++)='0';
		return;
	}

	if (*s == '-')
		*(bufferPos++)=*(s++);
	bool zero=(*s=='0');
	for(;*s>='0'&&*s<='9';)
		*(bufferPos++)=*(s++);

	if (!pMark)
		return;

	if (*s=='.')
	{
		if (s[1]>='0'&&s[1]<='9')
		{
			if (zero)
				bufferPos[-1]=s[1];
			else
				*(bufferPos++)=s[1];
		}
		else
		{
			if (!zero)
			{
				*(bufferPos++)='0';
				*(bufferPos++)='0';
			}
			return;
		}
		if (s[2]>='0'&&s[2]<='9')
		{
			if (zero && bufferPos[-1]=='0')
				bufferPos[-1]=s[2];
			else
				*(bufferPos++)=s[2];
		}
		else if (!zero || bufferPos[-1]!='0')
			*(bufferPos++)='0';
	}
	else
	{
		*(bufferPos++)='0';
		*(bufferPos++)='0';
	}
}

void ExWriteStr(char*& bufferPos,char* str,bool useQuote)
{
	char* src = str;
	char* dst = str;
	if (useQuote)
	{
		useQuote=false;
		for(;*src!=NULL;)
		{
			if (*src=='<')
			{
				int x=0;
				for(src++;;src++)
				{
					if (*src==',')
					{
						if (x==1)
						{
							if (*(++src)==NULL)
								break;
							src++;
							break;
						}
						x++;
					}
					else if (*src=='>' || *src==NULL)
						break;
				}
			}
			else if (*src==',')
			{
				useQuote=true;
				break;
			}
			else
				src++;
		}
	}
	if (useQuote)
		*bufferPos++='\"';
	src = str;
	if (*src=='\"')
		src++;

	for(;;)
	{
		if (*src=='<')
		{
			char* idStr = 0;
			char* fdStr = 0;
			bool pMark=false;
			int x=0;
			for(idStr=(++src);;src++)
			{
				if (*src==',')
				{
					*src=NULL;
					if (x==0)
						x++,fdStr=src+1;
					else
					{
						if (*(++src)=='%')
							pMark=true;
						else if (*src==NULL)
							return;
						src++;
						if (*src==NULL)
							return;
						src++;
						break;
					}
				}
				else if (*src=='>')
				{
					*(src++)=NULL;
					break;
				}
				else if (*src==NULL)
					return;
			}
			if (idStr && fdStr)
				WriteExDataNum(bufferPos,idStr,fdStr,pMark);
		}
		else if (*src=='\r')
		{
			*(bufferPos++)='|', *(bufferPos++)='n';
			if (*(++src)=='\n')
				src++;
		}
		else if (*src=='\n')
			*(bufferPos++)='|', *(bufferPos++)='n', src++;
		else if (*src==NULL)
			break;
		else
			*(bufferPos++) = *(src++);
	}
	if (*bufferPos=='\"')
		bufferPos--;
	if (useQuote)
		*(bufferPos++) = '\"';
}

int RunOptimizer(char* mapName,char* destMapName)
{
  chdir(szAppDir);

	if (mapName==NULL || *mapName==NULL)
		return ERR_FILE_NOT_FOUND;
	if (!SFileOpenArchive(mapName,0,0,&g_hMPQ))	//原地图文件
		return ERR_FILE_NOT_FOUND;
	int fp = open(mapName,O_BINARY);		//读取地图文件头
	mapHeader=Malloc(512);
	read(fp,mapHeader,512);
	close(fp);
	if (onlyForCompress)
	{
		hashTableSize=MapFileNumFound+keepListFile;
		for(int i=0;i<MapFileNumFound;i++)
		{
			if (keepListFile && stricmp(MapFileList[i],"(listfile)")==0)
				MapFileList[i]=MapFileList[--MapFileNumFound];
			HANDLE hFile;
			SFileOpenFileEx(g_hMPQ,MapFileList[i],0,&hFile);
			int size = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
			char* buffer = Malloc(size+8);
			SFileReadFile(hFile,buffer,size,NULL,NULL);
			AddBufferFile(MapFileList[i],buffer,size,0);
			SFileCloseFile(hFile);
		}
		WriteFilesToMap(destMapName);
        FlushGlobalCache();
        SFileCloseArchive(g_hMPQ);
		return OPTIMIZER_FINISH;
	}

	HANDLE hFile;
	if ((!SFileOpenFileEx(g_hMPQ,"war3map.j",NULL,&hFile)) && (!SFileOpenFileEx(g_hMPQ,"scripts\\war3map.j",NULL,&hFile)))
	{
		MpqCloseUpdatedArchive(g_hMPQ,NULL);
		for(;bufferFileCount>0;bufferFileCount--)
			free(bufferFile[bufferFileCount]);
        FlushGlobalCache();
        SFileCloseArchive(g_hMPQ);
		return ERR_JSCRIPT_LOST;
    }
    SFileCloseFile(hFile);

	if (SFileOpenFileEx(g_hMPQ,"war3map.w3u",NULL,&hFile))
		SFileCloseFile(hFile);
	else
		OPT_UNIT=0;
	if (SFileOpenFileEx(g_hMPQ,"war3map.w3t",NULL,&hFile))
		SFileCloseFile(hFile);
	else
		OPT_ITEM=0;
	if (SFileOpenFileEx(g_hMPQ,"war3map.w3b",NULL,&hFile))
		SFileCloseFile(hFile);
	else
		OPT_DEST=0;
	if (SFileOpenFileEx(g_hMPQ,"war3map.w3d",NULL,&hFile))
		SFileCloseFile(hFile);
	else
        OPT_DOOD=0;
    if (SFileOpenFileEx(g_hMPQ,"war3map.w3a",NULL,&hFile))
    {
        SFileCloseFile(hFile);
    }
    else if (SFileOpenFileEx(g_hMPQ,"war3map.w3h",NULL,&hFile))
    {
        SFileCloseFile(hFile);
    }
    else
    {
        OPT_ABIL=0;
        OPT_BUFF=0;
    }

	if (SFileOpenFileEx(g_hMPQ,"war3map.w3q",NULL,&hFile))
		SFileCloseFile(hFile);
	else
		OPT_UPGR=0;

	SetHashTable();
	InitOriginLevel();
	ReadWTS();
	ConvW3I();
	ConvWar3MapSkin();
	if (!ConvJScriptBase())
    {
        SFileCloseArchive(g_hMPQ);
        return ERR_JSCRIPT_LOST;
    }
	if (searchObjectInJScript)
		SearchObjectInJScript();
	ConvDoo();
	ConvJScript();
	ConvWar3MapMisc();

	odUnit.Init(OPT_UNIT);
	odItem.Init(OPT_ITEM);
	odDest.Init(OPT_DEST);
	odDood.Init(OPT_DOOD);
	odAbil.Init(OPT_ABIL);
	odBuff.Init(OPT_BUFF);
	odUpgr.Init(OPT_UPGR);

	odUnit.ReadW3O("war3map.w3u",CNV_UNIT);
	odUnit.ReadSLK("Units\\UnitData.slk",0);
	odUnit.ReadSLK("Units\\UnitAbilities.slk",1);
	odUnit.ReadSLK("Units\\UnitBalance.slk",2);
	odUnit.ReadSLK("Units\\UnitUI.slk",3);
	odUnit.ReadSLK("Units\\UnitWeapons.slk",4);
	odUnit.ReadMetaSLK("Units\\UnitMetaData.slk");

	odItem.ReadW3O("war3map.w3t",CNV_ITEM);
	odItem.ReadSLK("Units\\ItemData.slk",0);
	odItem.ReadMetaSLK("Units\\UnitMetaData.slk");

	odDest.ReadW3O("war3map.w3b",CNV_DEST);
	odDest.ReadSLK("Units\\DestructableData.slk",0);
	odDest.ReadMetaSLK("Units\\DestructableMetaData.slk");

	odDood.ReadW3O("war3map.w3d",CNV_DOOD);
	odDood.ReadSLK("Doodads\\Doodads.slk",0);
	odDood.ReadMetaSLK("Doodads\\DoodadMetaData.slk");

	odAbil.ReadW3O("war3map.w3a",CNV_ABIL);
	odAbil.ReadSLK("Units\\AbilityData.slk",0);
	odAbil.ReadMetaSLK("Units\\AbilityMetaData.slk");

	odBuff.ReadW3O("war3map.w3h",CNV_BUFF);
	odBuff.ReadSLK("Units\\AbilityBuffData.slk",0);
	odBuff.ReadMetaSLK("Units\\AbilityBuffMetaData.slk");

	odUpgr.ReadW3O("war3map.w3q",CNV_UPGR);
	odUpgr.ReadSLK("Units\\UpgradeData.slk",0);
	odUpgr.ReadMetaSLK("Units\\UpgradeMetaData.slk");

	//txt需要按Blz读图的顺序读取
	//MiscData.txt
	//MiscGame.txt
	//CommandFunc.txt
	//CommandStrings.txt
	odUnit.ReadTXT("Units\\HumanUnitStrings.txt",true);
	odUnit.ReadTXT("Units\\NeutralUnitStrings.txt",true);
	odUnit.ReadTXT("Units\\NightElfUnitStrings.txt",true);
	odUnit.ReadTXT("Units\\OrcUnitStrings.txt",true);
	odUnit.ReadTXT("Units\\UndeadUnitStrings.txt",true);
	odUnit.ReadTXT("Units\\CampaignUnitStrings.txt",true);
	//UnitGlobalStrings.txt
	odUnit.ReadTXT("Units\\HumanUnitFunc.txt",false);
	odUnit.ReadTXT("Units\\NeutralUnitFunc.txt",true);
	odUnit.ReadTXT("Units\\NightElfUnitFunc.txt",true);
	odUnit.ReadTXT("Units\\OrcUnitFunc.txt",true);
	odUnit.ReadTXT("Units\\UndeadUnitFunc.txt",true);
	odUnit.ReadTXT("Units\\CampaignUnitFunc.txt",true);

	odItem.ReadTXT("Units\\ItemStrings.txt",true);
	odItem.ReadTXT("Units\\ItemFunc.txt",false);

	odAbil.ReadTXT("Units\\HumanAbilityFunc.txt",false);
	odBuff.ReadTXT("Units\\HumanAbilityFunc.txt",false);
	odAbil.ReadTXT("Units\\NeutralAbilityFunc.txt",false);
	odBuff.ReadTXT("Units\\NeutralAbilityFunc.txt",false);
	odAbil.ReadTXT("Units\\NightElfAbilityFunc.txt",true);
	odBuff.ReadTXT("Units\\NightElfAbilityFunc.txt",false);
	odAbil.ReadTXT("Units\\OrcAbilityFunc.txt",true);
	odBuff.ReadTXT("Units\\OrcAbilityFunc.txt",false);
	odAbil.ReadTXT("Units\\UndeadAbilityFunc.txt",true);
	odBuff.ReadTXT("Units\\UndeadAbilityFunc.txt",false);
	odAbil.ReadTXT("Units\\CommonAbilityFunc.txt",true);
	odBuff.ReadTXT("Units\\CommonAbilityFunc.txt",false);
	odAbil.ReadTXT("Units\\ItemAbilityFunc.txt",true);
	odBuff.ReadTXT("Units\\ItemAbilityFunc.txt",false);
	odAbil.ReadTXT("Units\\CampaignAbilityFunc.txt",true);
	odBuff.ReadTXT("Units\\CampaignAbilityFunc.txt",false);
	odAbil.ReadTXT("Units\\HumanAbilityStrings.txt",true);
	odBuff.ReadTXT("Units\\HumanAbilityStrings.txt",false);
	odAbil.ReadTXT("Units\\NeutralAbilityStrings.txt",true);
	odBuff.ReadTXT("Units\\NeutralAbilityStrings.txt",false);
	odAbil.ReadTXT("Units\\NightElfAbilityStrings.txt",true);
	odBuff.ReadTXT("Units\\NightElfAbilityStrings.txt",false);
	odAbil.ReadTXT("Units\\OrcAbilityStrings.txt",true);
	odBuff.ReadTXT("Units\\OrcAbilityStrings.txt",false);
	odAbil.ReadTXT("Units\\UndeadAbilityStrings.txt",true);
	odBuff.ReadTXT("Units\\UndeadAbilityStrings.txt",false);
	odAbil.ReadTXT("Units\\CommonAbilityStrings.txt",true);
	odBuff.ReadTXT("Units\\CommonAbilityStrings.txt",false);
	odAbil.ReadTXT("Units\\ItemAbilityStrings.txt",true);
	odBuff.ReadTXT("Units\\ItemAbilityStrings.txt",false);
	odAbil.ReadTXT("Units\\CampaignAbilityStrings.txt",true);
	odBuff.ReadTXT("Units\\CampaignAbilityStrings.txt",false);

	odUpgr.ReadTXT("Units\\HumanUpgradeFunc.txt",false);
	odUpgr.ReadTXT("Units\\OrcUpgradeFunc.txt",true);
	odUpgr.ReadTXT("Units\\UndeadUpgradeFunc.txt",true);
	odUpgr.ReadTXT("Units\\NightElfUpgradeFunc.txt",true);
	odUpgr.ReadTXT("Units\\CampaignUpgradeFunc.txt",true);
	odUpgr.ReadTXT("Units\\NeutralUpgradeFunc.txt",true);
	odUpgr.ReadTXT("Units\\HumanUpgradeStrings.txt",true);
	odUpgr.ReadTXT("Units\\OrcUpgradeStrings.txt",true);
	odUpgr.ReadTXT("Units\\UndeadUpgradeStrings.txt",true);
	odUpgr.ReadTXT("Units\\NightElfUpgradeStrings.txt",true);
	odUpgr.ReadTXT("Units\\CampaignUpgradeStrings.txt",true);
	odUpgr.ReadTXT("Units\\NeutralUpgradeStrings.txt",true);

	odUnit.ConvW3O("war3map.w3u",CNV_UNIT);
	odItem.ConvW3O("war3map.w3t",CNV_ITEM);
	odDest.ConvW3O("war3map.w3b",CNV_DEST);
	odDood.ConvW3O("war3map.w3d",CNV_DOOD);
	odAbil.ConvW3O("war3map.w3a",CNV_ABIL);
	odBuff.ConvW3O("war3map.w3h",CNV_BUFF);
	odUpgr.ConvW3O("war3map.w3q",CNV_UPGR);

	odUnit.RecordCustomObjHash();
	odItem.RecordCustomObjHash();
	odAbil.RecordCustomObjHash();
	odUpgr.RecordCustomObjHash();
	if (OPT_UNIT==2 || OPT_ITEM==2 || OPT_DEST==2 || OPT_ABIL==2 || OPT_BUFF==2 || OPT_UPGR==2)
	{
		SearchUsedRandomObject();
		SearchDataFieldForObject();
		SearchUnusedObject();
	}

	odUnit.WriteSLK("Units\\UnitData.slk",0);
	odUnit.WriteSLK("Units\\UnitAbilities.slk",1);
	odUnit.WriteSLK("Units\\UnitBalance.slk",2);
	odUnit.WriteSLK("Units\\UnitUI.slk",3);
	odUnit.WriteSLK("Units\\UnitWeapons.slk",4);
	odUnit.WriteTXT("Units\\HumanUnitFunc.txt");
	odItem.WriteSLK("Units\\ItemData.slk",0);
	odItem.WriteTXT("Units\\ItemFunc.txt");
	odDest.WriteSLK("Units\\DestructableData.slk",0);
	odDood.WriteSLK("Doodads\\Doodads.slk",0);
	odAbil.WriteSLK("Units\\AbilityData.slk",0);
	odAbil.WriteTXT("Units\\HumanAbilityFunc.txt");
	odBuff.WriteSLK("Units\\AbilityBuffData.slk",0);
	odBuff.WriteTXT("Units\\NeutralAbilityFunc.txt");
	odUpgr.WriteSLK("Units\\UpgradeData.slk",0);
	odUpgr.WriteTXT("Units\\HumanUpgradeFunc.txt");
    WriteNewWts();
	SFileCloseArchive(g_hMPQ);
	WriteFilesToMap(destMapName);
	FlushGlobalCache();
	return OPTIMIZER_FINISH;
}
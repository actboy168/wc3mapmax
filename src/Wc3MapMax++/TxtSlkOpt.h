//*	TOP_Hash		KeyWordNum:39	WordLength:4	HashValue:1-136		Txt Optimizer
#define TOP_HASH_MAX 137
inline static unsigned int TOPHash(register const char *str)
{	//高效的静态Hash函数，不进行数据检测
	static const int topHashString[] =
	{
		0,
		'3scu',0,0,0,0,
		'2scu',0,0,0,0,
		'1scu',0,0,0,0,
		'3pso',0,0,0,0,
		'2pso',0,0,0,0,
		'1pso',0,0,0,0,
		'3twh',0,0,0,0,
		'2twh',0,0,0,0,
		'ttrh',0,0,0,0,
		'tawh',0,0,0,0,0,
		'4pso',0,0,0,
		'3wso',0,0,0,0,
		'2wso','3vln',0,0,0,
		'1wso','2vln',0,0,0,
		'3bqn','1vln',0,0,0,
		'2bqn','3rdn',0,0,0,
		'1bqn','2rdn',0,0,0,
		'3zgn','1rdn',0,0,0,
		'2zgn','3gcn',0,0,0,
		'1zgn','2gcn',0,0,0,
		'3wsn','bgcn',0,0,0,
		'1wsn',0,0,0,0,
		'3ewn',0,0,0,0,
		'2ewn','4zgn',0,0,0,
		'1ewn',0,0,0,0,
		'nofe',0,0,0,0,
		'lesn'
	};
	static unsigned char topHashArray[] =
	{
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140,  10,
		5,   0,  36, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140,  15,  10, 140,
		140,  55, 140,  31,  20, 140, 140, 140,  20, 140,
		60,  15,   0, 140,  16,   0,  10,   0,   1,  40,
		140, 140,  25, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
		140, 140, 140, 140, 140, 140
	};
	register unsigned int v = topHashArray[(unsigned char)str[3]] + topHashArray[(unsigned char)str[2]] + topHashArray[(unsigned char)str[0]] + 1;
	if ((int&)*str == topHashString[v])
		//v < MAX_HASH_VALUE && 
		return v;
	else
		return 0;
}

char txtOptInit=-1;
int topData[TOP_HASH_MAX][3];
void TxtOptData(char* s,int hp,int min,int max)
{
	int i=TOPHash(s);
	topData[TOPHash(s)][0]=hp;
	topData[TOPHash(s)][1]=min;
	topData[TOPHash(s)][2]=max;
}
void TxtOptInit(char txtDataVer)
{
	if (txtOptInit==-1)
	{
		TxtOptData("efon",300,15,17);
		TxtOptData("hrtt",700,45,55);
		//TxtOptData("hwat",525,18,22);
		TxtOptData("hwt2",675,31,39);
		TxtOptData("hwt3",900,41,49);
		//TxtOptData("ncg2",80,12,14);
		//TxtOptData("ncg3",80,12,14);
		//TxtOptData("ncgb",80,12,14);
		//TxtOptData("ndr1",215,9,10);
		//TxtOptData("ndr2",290,15,17);
		//TxtOptData("ndr3",405,16,19);
		TxtOptData("ngz1",600,19,21);
		TxtOptData("ngz2",900,28,30);
		TxtOptData("ngz3",1200,38,40);
		TxtOptData("ngz4",2250,59,69);
		//TxtOptData("nlv1",425,11,27);
		//TxtOptData("nlv2",550,21,45);
		TxtOptData("nlv3",700,32,56);
		TxtOptData("nqb1",425,13,15);
		TxtOptData("nqb2",515,26,30);
		TxtOptData("nqb3",600,39,45);
		TxtOptData("nqb4",700,52,60);
		TxtOptData("nsel",550,28,37);
		TxtOptData("nsw1",200,11,12);
		TxtOptData("nsw2",300,16,17);
		TxtOptData("nsw3",400,21,22);
		TxtOptData("nwe1",300,0,0);
		//TxtOptData("nwe2",450,21,25);
		//TxtOptData("nwe3",650,48,57);
		TxtOptData("osp1",75,11,13);
		TxtOptData("osp2",135,23,26);
		TxtOptData("osp3",135,41,45);
		TxtOptData("osp4",135,41,45);
		TxtOptData("osw1",200,11,12);
		TxtOptData("osw2",300,16,17);
		//TxtOptData("osw3",500,21,22);
		TxtOptData("ucs1",140,8,9);
		TxtOptData("ucs2",275,15,18);
		TxtOptData("ucs3",410,22,27);
	}
	txtOptInit=txtDataVer;
	if (txtDataVer==0)
	{
		TxtOptData("hwat",450,21,29);		
		TxtOptData("ncg2",125,8,9);
		TxtOptData("ncg3",125,8,9);
		TxtOptData("ncgb",125,8,9);
		TxtOptData("ndr1",200,14,15);
		TxtOptData("ndr2",260,15,17);
		TxtOptData("ndr3",320,16,19);
		TxtOptData("nlv1",425,13,37);
		TxtOptData("nlv2",575,21,45);
		TxtOptData("nwe2",625,21,25);
		TxtOptData("nwe3",875,48,57);
		TxtOptData("osw3",400,21,22);
	}
	else
	{
		TxtOptData("hwat",525,18,22);
		TxtOptData("ncg2",80,12,14);
		TxtOptData("ncg3",80,12,14);
		TxtOptData("ncgb",80,12,14);
		TxtOptData("ndr1",215,9,10);
		TxtOptData("ndr2",290,15,17);
		TxtOptData("ndr3",405,16,19);
		TxtOptData("nlv1",425,11,27);
		TxtOptData("nlv2",550,21,45);
		TxtOptData("nwe2",450,21,25);
		TxtOptData("nwe3",650,48,57);
		TxtOptData("osw3",500,21,22);
	}
}
void TxtOptByBuffer(char* txtPath,char* buffer,int size,char txtDataVer)	//txt优化(input文件)
{
	if (txtOptInit!=txtDataVer)
		TxtOptInit(txtDataVer);
	buffer[size]=NULL;
	char* destStr=(char*)malloc(size);
	char* destStrPos=destStr;
	char* tempFile=buffer;
	char* lineStr;
	char* litStr;
	if (tempFile[0]==-17 && tempFile[1]==-69 && tempFile[2]==-65)	//WTS UTF-8文件头
		tempFile+=3;
	for(;;)
	{
		if (*tempFile==NULL)
		{
			if (destStrPos[-1]=='\n')
				destStrPos--;
			if (destStrPos[-1]==']' && destStrPos[-6]=='[' && destStrPos[-7]=='\n')
				destStrPos-=7;
			break;
		}
		lineStr=tempFile;
		for(;*tempFile!='\r' && *tempFile!='\n' && *tempFile!=NULL;tempFile++)
			;
		if (*tempFile!=NULL)
		{
			*(tempFile++)=NULL;
			for(;*tempFile=='\r' || *tempFile=='\n';tempFile++)
				;
		}

		if (lineStr[0]=='[')
		{
			if (destStrPos[-2]==']' && destStrPos[-7]=='[')
				destStrPos-=7;
			strcat(destStrPos,lineStr);
			chrcat(destStrPos,'\n');
		}
		else if (*lineStr=='/')	//注释
			;
		else if (*lineStr=='E' && lineStr[1]=='d')	//For Editor only
			;
		else
		{
			litStr=lineStr;							//读取行字符串
			for(;*litStr!='=' && *litStr!=NULL;litStr++)	//search for '='
				;
			if (*litStr==NULL)								//if not found,continue loop
				continue;
			litStr++;
			if (*litStr==NULL || (*litStr=='0' && (litStr[1]==NULL || (litStr[1]==',' && litStr[2]=='0' && litStr[3]==NULL) || (litStr[1]=='.' && litStr[2]=='0' && (litStr[3]==NULL || (litStr[3]=='0' && litStr[4]==NULL))))))
				;
			else
			{
				for(;*litStr!=NULL;litStr++)
				{
					if (*litStr=='<' && TOPHash(litStr+1)>0)
					{
						int a=0,b=0;
						if (memcmp(litStr+6,"realHP>",7)==0)
							a=0,b=12;
						else if (memcmp(litStr+6,"mindmg1>",8)==0)
							a=1,b=13;
						else if (memcmp(litStr+6,"maxdmg1>",8)==0)
							a=2,b=13;
						if (b>0)
						{
							*(litStr++)=NULL;
							strcat(destStrPos,lineStr);
							strcat(destStrPos,i2a(topData[TOPHash(litStr)][a]));
							lineStr=litStr+b;
						}
					}
				}

				strcat(destStrPos,lineStr);
				chrcat(destStrPos,'\n');
			}
		}
	}
	int fp=creat(txtPath,S_IWRITE);
	//setmode(fp,O_BINARY);   
	write(fp,destStr,destStrPos-destStr);
	close(fp);
	free(destStr);
}

void TxtOptByFile(char* txtPath,char txtDataVer)	//txt优化(input文件)
{
	int fp=open(txtPath,_O_BINARY|_O_RDWR);
	if (fp<=0)
		return;
	int size=filelength(fp);
	char* buffer=(char*)malloc(size+1);
	read(fp,buffer,size);
	close(fp);
	//buffer[size]=NULL;
	TxtOptByBuffer(txtPath,buffer,size,txtDataVer);
	free(buffer);
}

void SlkOptByBuffer(char* slkPath,char* buffer,int size)	//slk优化(input文件)
{
	buffer[size]='\n';
	char* destStr=(char*)malloc(size);
	char* destStrPos=destStr;
	char* tempStr=buffer;
	int X[128]={};
	int Y[1024]={};
	int x=0,y=0;
	int xCnt=0,yCnt=0;

	InitSlkOptField(slkPath);
	for(;;)
	{
		for(;*tempStr!='\r' && *tempStr!='\n';tempStr++)
			;
		if (tempStr[1]=='\n')
			tempStr+=2;
		else
			tempStr++;

		if (*tempStr=='C')
			{
				for(tempStr++;;)
				{
					tempStr++;
					if (*tempStr=='K')
					{
						tempStr++;
						if (*tempStr=='0')
							tempStr++;
						else if ((*tempStr=='-') && (tempStr[1]=='0'))
							*(++tempStr)='-';
						if (!(*tempStr=='\r' || *tempStr=='\n' || (*tempStr=='"'&&(/*(tempStr[1]=='_'&&tempStr[2]=='"') ||*/ (tempStr[1]=='-'&&tempStr[2]=='"') || (tempStr[1]==' '&&tempStr[2]=='-'&&tempStr[3]==' '&&tempStr[4]=='"')))))
						{
							if (x==1)
								Y[y]=++yCnt;
							if (y==1 && slkOptField[SLKHash(tempStr+1)])
								X[x]=++xCnt;
							if (X[x]!=0 && Y[y]!=0)
							{
								memcat(destStrPos,"C;X",3);
								WriteIntToBuffer(destStrPos,X[x]);
								if (x==1)
								{
									memcat(destStrPos,";Y",2);
									WriteIntToBuffer(destStrPos,Y[y]);
								}
								memcat(destStrPos,";K",2);
								for(; *tempStr!='\n' && *tempStr!='\r'; *(destStrPos++)=*(tempStr++))
									;
								chrcat(destStrPos,'\n');
							}
						}
						break;
					}
					else if (*tempStr=='X')
					{
						tempStr++;
						x=0;
						for(;;)
						{
							if (*tempStr>'9' || *tempStr<'0')
								break;
							else
								x=(*(tempStr++) & 0xf)+x*10;
						}
					}
					else if (*tempStr=='Y')
					{
						tempStr++;
						y=0;
						for(;;)
						{
							if (*tempStr>'9' || *tempStr<'0')
								break;
							else
								y=(*(tempStr++) & 0xf)+y*10;
						}
					}
				}

			}
			else if (*tempStr=='E')
				break;
	}
	{	//Blz bug, Xesn=>XEsn
		char* S;
		for(char* s=slkPath;*s!=NULL;s++)
		{
			if (*s=='\\')
				S=++s;
		}
		if (stricmp(S,"AbilityBuffData.slk")==0)
		{
			*destStrPos=NULL;
			char* s = strstr(destStr,"Xesn");
			if (s!=NULL)
				s[1] = 'E';
		}
	}
	*(destStrPos++)='E';

	int fp=creat(slkPath,S_IWRITE);
	//setmode(fp,O_BINARY);
	write(fp,"ID;PWXL;N;E\nB;X",15);
	WriteInt(fp,xCnt);
	write(fp,";Y",2);
	WriteInt(fp,yCnt);
	write(fp,";D0\n",4);
	write(fp,destStr,destStrPos-destStr);
	close(fp);
	free(destStr);
}

void SlkOptByFile(char* slkPath)	//slk优化(input文件)
{
	int fp=open(slkPath,_O_BINARY|_O_RDWR);
	if (fp<=0)
		return;
	int size=filelength(fp);
	char* buffer=(char*)malloc(size+1);
	read(fp,buffer,size);
	close(fp);
	//buffer[size]=NULL;
	SlkOptByBuffer(slkPath,buffer,size);
	free(buffer);
}

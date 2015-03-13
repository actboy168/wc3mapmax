#include "stdafx.h"

void txtFieldCfgEx()
{
  txtFieldIsValue[TXTHash("Buttonpos")]=1;
  txtFieldIsValue[TXTHash("Casterattachcount")]=1;
  txtFieldIsValue[TXTHash("LoopingSoundFadeIn")]=1;
  txtFieldIsValue[TXTHash("LoopingSoundFadeOut")]=1;
  txtFieldIsValue[TXTHash("Missilearc")]=1;
  txtFieldIsValue[TXTHash("MissileHoming")]=1;
  txtFieldIsValue[TXTHash("Missilespeed")]=1;
  txtFieldIsValue[TXTHash("Requiresamount")]=1;
  txtFieldIsValue[TXTHash("Requirescount")]=1;
  txtFieldIsValue[TXTHash("Researchbuttonpos")]=1;
  txtFieldIsValue[TXTHash("Revive")]=1;
  txtFieldIsValue[TXTHash("Spelldetail")]=1;
  txtFieldIsValue[TXTHash("Targetattachcount")]=1;
  txtFieldIsValue[TXTHash("UnButtonpos")]=1;

  txtFieldNoEmpty[TXTHash("Name")]=1;
  txtFieldNoEmpty[TXTHash("Tip")]=1;
  txtFieldNoEmpty[TXTHash("Ubertip")]=1;
  txtFieldNoEmpty[TXTHash("UnTip")]=1;
  txtFieldNoEmpty[TXTHash("Researchtip")]=1;
  txtFieldNoEmpty[TXTHash("Bufftip")]=1;
  txtFieldNoEmpty[TXTHash("Awakentip")]=1;
  txtFieldNoEmpty[TXTHash("Revivetip")]=1;

  txtFieldIsList[TXTHash("Animnames")]=1;
  txtFieldIsList[TXTHash("animProps")]=1;
  txtFieldIsList[TXTHash("Areaeffectart")]=1;
  txtFieldIsList[TXTHash("Attachmentanimprops")]=1;
  txtFieldIsList[TXTHash("Attachmentlinkprops")]=1;
  txtFieldIsList[TXTHash("Boneprops")]=1;
  txtFieldIsList[TXTHash("Builds")]=1;
  txtFieldIsList[TXTHash("CasterArt")]=1;
  txtFieldIsList[TXTHash("Casterattach")]=1;
  txtFieldIsList[TXTHash("Casterattach1")]=1;
  txtFieldIsList[TXTHash("Casterupgradename")]=1;
  txtFieldIsList[TXTHash("Casterupgradetip")]=1;
  txtFieldIsList[TXTHash("DependencyOr")]=1;
  txtFieldIsList[TXTHash("EffectArt")]=1;
  txtFieldIsList[TXTHash("Effectattach")]=1;
  txtFieldIsList[TXTHash("LightningEffect")]=1;
  txtFieldIsList[TXTHash("Makeitems")]=1;
  txtFieldIsList[TXTHash("Missileart")]=1;
  txtFieldIsList[TXTHash("Propernames")]=1;
  txtFieldIsList[TXTHash("Requires")]=1;
  txtFieldIsList[TXTHash("Requires1")]=1;
  txtFieldIsList[TXTHash("Requires2")]=1;
  txtFieldIsList[TXTHash("Requires3")]=1;
  txtFieldIsList[TXTHash("Requires4")]=1;
  txtFieldIsList[TXTHash("Requires5")]=1;
  txtFieldIsList[TXTHash("Requires6")]=1;
  txtFieldIsList[TXTHash("Requires7")]=1;
  txtFieldIsList[TXTHash("Requires8")]=1;
  txtFieldIsList[TXTHash("Requiresamount")]=1;
  txtFieldIsList[TXTHash("Researches")]=1;
  txtFieldIsList[TXTHash("Reviveat")]=1;
  txtFieldIsList[TXTHash("Sellitems")]=1;
  txtFieldIsList[TXTHash("Sellunits")]=1;
  txtFieldIsList[TXTHash("Specialart")]=1;
  txtFieldIsList[TXTHash("Specialattach")]=1;
  txtFieldIsList[TXTHash("Targetart")]=1;
  txtFieldIsList[TXTHash("Targetattach")]=1;
  txtFieldIsList[TXTHash("Targetattach1")]=1;
  txtFieldIsList[TXTHash("Targetattach2")]=1;
  txtFieldIsList[TXTHash("Targetattach3")]=1;
  txtFieldIsList[TXTHash("Targetattach4")]=1;
  txtFieldIsList[TXTHash("Targetattach5")]=1;
  txtFieldIsList[TXTHash("Trains")]=1;
  txtFieldIsList[TXTHash("Upgrade")]=1;
}

int ReadCFG()	//仅在程序载入时运行
{
  int i, j;

	onlyForCompress           = 0;
	szListFilePath[0]         = '\0';
	keepListFile              = 1;
	fieldNoEmpty              = (bool*)calloc(SLK_HASH_MAX,1);		//alawys used
	txtFieldIsValue           = (char*)calloc(TXT_HASH_MAX,1);	//alawys used
	txtFieldNoEmpty           = (char*)calloc(TXT_HASH_MAX,1);	//alawys used
	txtFieldIsList            = (char*)calloc(TXT_HASH_MAX,1);		//alawys used
	convDooToJScript          = 0;
	digitsNumForDooConv       = 2;
	szMarkStringForDooConv[0] = '\0';
	strcpy(szMarkStringForDooConv,"call Cheat(\"DooConv\")");
	szTxtNullMark[0]          = '\0';
	blpForOpt                 = 1;
  blpMipNum                 = 2;
  jForOpt                   = 1;
	mdxOptLevel               = 1;
	searchObjectInJScript     = 0;
	useRandomCreep            = 0;
	useRandomBuild            = 0;
	useRandomItem             = 0;
	useMarket                 = 0;
	removeWEOnlyFiles         = 0;
	wavAutoBlockSize          = 1;
	memset(objectsForOptimizer,2,7);

	txtFieldCfgEx();
	int mode=0;
	char * fileStr;
	char * tempFile;
	char * lineStr;
	char * litStr;
	int fp=open("Config.ini",_O_TEXT);
	if (fp == EOF)
		return ERR_FILE_NOT_FOUND;
	int tempI=filelength(fp);
	fileStr=(char*)malloc(tempI+3);						//M.函数结束时释放
	tempI=read(fp,fileStr,tempI);
	close(fp);
	tempFile=fileStr;
	tempFile[tempI]='\n';
	tempFile[tempI+1]=NULL;
	tempFile[tempI+2]='\n';
	if (tempFile[0]==-17 && tempFile[1]==-69 && tempFile[2]==-65)	//WTS UTF-8文件头
		tempFile+=3;

	for(;;)
	{
		for(lineStr=tempFile;*tempFile!='\n';tempFile++)
			;
		*(tempFile++)=NULL;
		if (*lineStr=='[')	//读取模式
		{
			if (memcmp(lineStr,"[Config]",8)==0)
				mode=1;
			else if (memcmp(lineStr,"[ObjectsForOptimizer]",21)==0)
				mode=2;
			else if (memcmp(lineStr,"[ObjectAlwaysUsed]",18)==0)
				mode=3;
			else if (memcmp(lineStr,"[FieldNoEmpty]",14)==0)
				mode=4;
			else if (memcmp(lineStr,"[SearchObjectData]",18)==0)
				mode=5;
			else if (memcmp(lineStr,"[Extra]",7)==0)
				mode=6;
			else if (memcmp(lineStr,"[Anei]",6)==0)
				mode=7;
		}
		else if (*lineStr=='/')		//注释
			;
		else if (*lineStr==NULL)	//空行
		{
			if (lineStr[1]==NULL)	//文件结束
				break;
		}
		else						//读取配置
		{
			switch (mode)
			{
			case 1:
				if (memcmp(lineStr,"AutoBlockSize=",14)==0)
					wavAutoBlockSize=a2ui(lineStr+14);
				else if (memcmp(lineStr,"ConvDooToJScript=",17)==0)
					convDooToJScript=a2ui(lineStr+17);
				else if (memcmp(lineStr,"DigitsNumForDooConv=",20)==0)
					digitsNumForDooConv=a2ui(lineStr+20);
				else if (memcmp(lineStr,"MarkStringForDooConv=",21)==0)
          StringCchCopyA(szMarkStringForDooConv, MAX_PATH, lineStr+21);
				else if (memcmp(lineStr,"RemoveWEOnlyFiles=",18)==0)
					removeWEOnlyFiles=a2ui(lineStr+18);
				else if (memcmp(lineStr,"KeepListFile=",13)==0)
					keepListFile=a2ui(lineStr+13);
				else if (memcmp(lineStr,"SearchObjectLevel=",18)==0)
					searchObjectLevel=a2ui(lineStr+18);
        else if (memcmp(lineStr,"UnusedObjectName=",17)==0)
          StringCchCopyA(szUnusedObjectName, MAX_PATH, lineStr+17);
				else if (memcmp(lineStr,"TxtNullMark=",12)==0)
					StringCchCopyA(szTxtNullMark, MAX_PATH, lineStr+12);
				else if (memcmp(lineStr,"BlpForOptimizer=",16)==0)
          blpForOpt=a2ui(lineStr+16);
				else if (memcmp(lineStr,"BlpMipMapNum=",13)==0)
          blpMipNum=a2ui(lineStr+13);
        else if (memcmp(lineStr,"JForOptimizer=",14)==0)
          jForOpt=a2ui(lineStr+14);
				else if (memcmp(lineStr,"MdxOptLevel=",12)==0)
					mdxOptLevel=a2ui(lineStr+12);
				else if (memcmp(lineStr,"SearchObjectInJScript=",22)==0)
					searchObjectInJScript=a2ui(lineStr+22);
				else if (memcmp(lineStr,"UseRandomCreep=",15)==0)
					useRandomCreep=a2ui(lineStr+15);
				else if (memcmp(lineStr,"UseRandomBuild=",15)==0)
					useRandomBuild=a2ui(lineStr+15);
				else if (memcmp(lineStr,"UseRandomItem=",14)==0)
					useRandomItem=a2ui(lineStr+14);
				else if (memcmp(lineStr,"UseMarket=",10)==0)
					useMarket=a2ui(lineStr+10);
				break;
			case 2:
				if (memcmp(lineStr,"Unit=",5)==0)
					OPT_UNIT = a2i(lineStr+5);
				else if (memcmp(lineStr,"Item=",5)==0)
					OPT_ITEM = a2i(lineStr+5);
				else if (memcmp(lineStr,"Destructable=",13)==0)
					OPT_DEST = a2i(lineStr+13);
				else if (memcmp(lineStr,"Doodad=",7)==0)
					OPT_DOOD = a2i(lineStr+7);
				else if (memcmp(lineStr,"Ability=",8)==0)
					OPT_ABIL = a2i(lineStr+8);
				else if (memcmp(lineStr,"AbilityBuff=",12)==0)
					OPT_BUFF = a2i(lineStr+12);
				else if (memcmp(lineStr,"Upgrade=",8)==0)
					OPT_UPGR = a2i(lineStr+8);
				break;
			case 3:
				tempFile[-1]='\n';	//还原被设为Null的换行符
				for(;(*tempFile!='[' && *tempFile!=NULL) || (tempFile[-1]!='\n');tempFile++)
					;
				i=2;
				for(;(tempFile[-i]=='\n');i++)
					;
				tempFile[1-i]=NULL;
				xCopyStr(usedObjectCfgStr,lineStr);
				break;
			case 4:
				for(litStr=lineStr;;lineStr++)
				{
					if (*lineStr==',')
					{
						*lineStr=NULL;
						fieldNoEmpty[SLKHash(litStr)]=true;
						litStr=lineStr+1;
					}
					else if (*lineStr==NULL)
					{
						fieldNoEmpty[SLKHash(litStr)]=true;
						break;
					}
				}
				break;
			case 5:
				i=0;
				if(*lineStr='<')
				{
					if (memcmp(lineStr,"<Unit>",6)==0)
						i=1;
					else if (memcmp(lineStr,"<Item>",6)==0)
						i=2;
					else if (memcmp(lineStr,"<Ability>",9)==0)
						i=3;
					else if (memcmp(lineStr,"<Upgrade>",9)==0)
						i=4;
					else if (memcmp(lineStr,"<AbilityData>",13)==0)
						i=5;
				}
				lineStr=tempFile;
				for(;(*tempFile!='<' && *tempFile!='[' && *tempFile!=NULL) || (tempFile[-1]!='\n');tempFile++)
					;
				j=2;
				for(;(tempFile[-j]=='\n');j++)
					;
				tempFile[1-j]=NULL;
				xCopyStr(objectSearStr[i],lineStr);
				break;
			case 6:
				for(i=0;i<5;i++)
				{
					char* s=extraString[i][0];
					if (lineStr[0]==s[0] && lineStr[1]==s[1] && lineStr[2]==s[2] && lineStr[3]==s[3] && lineStr[4]=='=')
					{
						extraString[i][1] = (char*)malloc(strlen(lineStr+5)+1);	//alawys used
						strcpy(extraString[i][1],lineStr+5);
						break;
					}
				}
				break;
			case 7:
				tempFile[-1]='\n';	//还原被设为Null的换行符
				for(;(*tempFile!='[' && *tempFile!=NULL) || (tempFile[-1]!='\n');tempFile++)
					;
				i=2;
				for(;(tempFile[-i]=='\n');i++)
					;
				tempFile[1-i]=NULL;
				xCopyStr(Anei_Str,lineStr);
				break;
			}
		}
	}
	free(fileStr);
	return 0;
}

int WriteCFG()
{
	char* fileStr;
	char* tempFile;
	char* litStr;
	char* tempStr[5]={0,0,0,0,0};
	int tempStrLen[5];
	int fp=open("Config.ini",_O_TEXT);
	if (fp==EOF)
		goto CFG_L1;
	int tempI=filelength(fp);
	fileStr=(char*)malloc(tempI+2);						//M.函数结束时释放
	tempI=read(fp,fileStr,tempI);
	close(fp);
	fileStr[tempI]='[';
	fileStr[tempI+1]='[';
	tempFile=(char*)memchr(fileStr,'[',60000);

	int i;
	for(;;)
	{
		litStr=tempFile;	//循环读取行字符串
		tempFile=(char*)memchr(tempFile+1,'[',60000);
		if (memcmp(litStr,"[ObjectAlwaysUsed]",18)==0)
			i=0;
		else if (memcmp(litStr,"[FieldNoEmpty]",14)==0)
			i=1;
		else if (memcmp(litStr,"[SearchObjectData]",18)==0)
			i=2;
		else if (memcmp(litStr,"[Extra]",7)==0)
			i=3;
		else if (memcmp(litStr,"[Anei]",6)==0)
			i=4;
		else
		{
			if (litStr[1]=='[')
				break;
			else
				continue;
		}
		tempStr[i]=litStr;
		tempStrLen[i]=tempFile-litStr;

	}
CFG_L1:
	fp=creat("Config.ini",_S_IWRITE);
	write(fp,"\xef\xbb\xbf",3);
	write(fp,"[Config]",8);
	write(fp,"\nAutoBlockSize=",15);
	WriteInt(fp,wavAutoBlockSize);
	write(fp,"\nConvDooToJScript=",18);
	WriteInt(fp,convDooToJScript);
	write(fp,"\nDigitsNumForDooConv=",21);
	WriteInt(fp,digitsNumForDooConv);
	write(fp,"\nMarkStringForDooConv=",22);
	write(fp,szMarkStringForDooConv,strlen(szMarkStringForDooConv));
	write(fp,"\nRemoveWEOnlyFiles=",19);
	WriteInt(fp,removeWEOnlyFiles);	
	write(fp,"\nKeepListFile=",14);
	WriteInt(fp,keepListFile);
	write(fp,"\nSearchObjectLevel=",19);
	WriteInt(fp,searchObjectLevel);
	write(fp,"\nUnusedObjectName=",18);
	write(fp,szUnusedObjectName,strlen(szUnusedObjectName));
	write(fp,"\nTxtNullMark=",13);
	write(fp,szTxtNullMark,strlen(szTxtNullMark));
	write(fp,"\nBlpForOptimizer=",17);
  WriteInt(fp,blpForOpt);
	write(fp,"\nBlpMipMapNum=",14);
  WriteInt(fp,blpMipNum);
  write(fp,"\nJForOptimizer=",15);
  WriteInt(fp,jForOpt);
	write(fp,"\nMdxOptLevel=",13);
	WriteInt(fp,mdxOptLevel);
	write(fp,"\nSearchObjectInJScript=",23);
	WriteInt(fp,searchObjectInJScript);
	write(fp,"\nUseRandomCreep=",16);
	WriteInt(fp,useRandomCreep);
	write(fp,"\nUseRandomBuild=",16);
	WriteInt(fp,useRandomBuild);
	write(fp,"\nUseRandomItem=",15);
	WriteInt(fp,useRandomItem);
	write(fp,"\nUseMarket=",11);
	WriteInt(fp,useMarket);

	write(fp,"\n\n[ObjectsForOptimizer]",23);
	write(fp,"\nUnit=",6);
	WriteInt(fp,OPT_UNIT);
	write(fp,"\nItem=",6);
	WriteInt(fp,OPT_ITEM);
	write(fp,"\nDestructable=",14);
	WriteInt(fp,OPT_DEST);
	write(fp,"\nDoodad=",8);
	WriteInt(fp,OPT_DOOD);
	write(fp,"\nAbility=",9);
	WriteInt(fp,OPT_ABIL);
	write(fp,"\nAbilityBuff=",13);
	WriteInt(fp,OPT_BUFF);
	write(fp,"\nUpgrade=",9);
	WriteInt(fp,OPT_UPGR);
	write(fp,"\n\n",2);

	if (tempStr[0]!=NULL)
		write(fp,tempStr[0],tempStrLen[0]);
	if (tempStr[1]!=NULL)
		write(fp,tempStr[1],tempStrLen[1]);
	if (tempStr[2]!=NULL)
		write(fp,tempStr[2],tempStrLen[2]);
	if (tempStr[3]!=NULL)
		write(fp,tempStr[3],tempStrLen[3]);
	if (tempStr[4]!=NULL)
		write(fp,tempStr[4],tempStrLen[4]);
	free(fileStr);
	close(fp);
	return 0;
}

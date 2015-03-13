#include "StdAfx.h"

MPQHANDLE ydweMpq, patchMpq,localMpq,war3xMpq,war3Mpq;

MPQHANDLE ReadInputFile(char* fileName, MPQHANDLE& hFile)
{
    if ((ydweMpq != (MPQHANDLE)(INVALID_HANDLE_VALUE)) 
        && SFileOpenFileEx(ydweMpq, fileName, NULL, &hFile))
        return hFile;
    else if ((patchMpq != (MPQHANDLE)(INVALID_HANDLE_VALUE)) 
        && SFileOpenFileEx(patchMpq, fileName, NULL, &hFile))
        return hFile;
    else if ((localMpq != (MPQHANDLE)(INVALID_HANDLE_VALUE))
        && SFileOpenFileEx(localMpq, fileName, NULL, &hFile))
        return hFile;
    else if ((war3xMpq != (MPQHANDLE)(INVALID_HANDLE_VALUE)) 
        && SFileOpenFileEx(war3xMpq, fileName, NULL, &hFile))
        return hFile;
    else if ((war3Mpq != (MPQHANDLE)(INVALID_HANDLE_VALUE)) 
        && SFileOpenFileEx(war3Mpq, fileName, NULL, &hFile))
        return hFile;
    else
        return NULL;
}

void GetInputFile(char* fileName)
{
    MPQHANDLE hFile;
    char* buffer = NULL;
    int fileSize=0,nameLength = strlen(fileName);
    if (ReadInputFile(fileName,hFile))
    {
        fileSize = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
        buffer=(char*)malloc(fileSize+1);
        SFileReadFile(hFile,buffer,fileSize,NULL,NULL);
        if (fileName[nameLength-4]=='.' && (fileName[nameLength-3]|32)=='s' && (fileName[nameLength-2]|32)=='l' && (fileName[nameLength-1]|32)=='k')
            SlkOptByBuffer(StrAddTemp("Input\\",fileName),buffer,fileSize);
        else if (fileName[nameLength-4]=='.' && (fileName[nameLength-3]|32)=='t' && (fileName[nameLength-2]|32)=='x' && (fileName[nameLength-1]|32)=='t')
            TxtOptByBuffer(StrAddTemp("Input\\",fileName),buffer,fileSize,1);

        SFileCloseFile(hFile);
    }
    fileName=StrAddTemp("Custom_V1\\",fileName);
    if (ReadInputFile(fileName,hFile))
    {
        int fileSize2 = SFileGetFileInfo(hFile,SFILE_INFO_SIZE);
        if (fileSize2>fileSize)
        {
            if (buffer!=NULL)
                free(buffer);
            buffer=(char*)malloc(fileSize2+1);
        }
        SFileReadFile(hFile,buffer,fileSize2,NULL,NULL);
        nameLength+=10;
        if (fileName[nameLength-4]=='.' && (fileName[nameLength-3]|32)=='s' && (fileName[nameLength-2]|32)=='l' && (fileName[nameLength-1]|32)=='k')
            SlkOptByBuffer(StrAddTemp("Input\\",fileName),buffer,fileSize2);
        else if (fileName[nameLength-4]=='.' && (fileName[nameLength-3]|32)=='t' && (fileName[nameLength-2]|32)=='x' && (fileName[nameLength-1]|32)=='t')
            TxtOptByBuffer(StrAddTemp("Input\\",fileName),buffer,fileSize2,1);

        SFileCloseFile(hFile);
    }
    else if (ReadInputFile(fileName+10,hFile))
    {
        nameLength+=10;
        if ((fileName[nameLength-5]|32)=='s' && fileName[nameLength-4]=='.' && (fileName[nameLength-3]|32)=='t' && (fileName[nameLength-2]|32)=='x' && (fileName[nameLength-1]|32)=='t' && buffer!=NULL)
        {	//xxxStrings.txt
            SFileReadFile(hFile,buffer,fileSize,NULL,NULL);
            TxtOptByBuffer(StrAddTemp("Input\\",fileName),buffer,fileSize,0);
        }

        SFileCloseFile(hFile);
    }
    if (buffer!=NULL)
        free(buffer);
}

void GetInputFileAll()
{
    chdir(szAppDir);
    mkdir("Input\\");
    mkdir("Input\\Custom_V1");
    mkdir("Input\\Doodads");
    mkdir("Input\\Units");
    mkdir("Input\\Custom_V1\\Units");
    GetInputFile("Doodads\\Doodads.slk");
    GetInputFile("Doodads\\DoodadMetaData.slk");
    GetInputFile("Units\\AbilityBuffData.slk");
    GetInputFile("Units\\AbilityBuffMetaData.slk");
    GetInputFile("Units\\AbilityData.slk");
    GetInputFile("Units\\AbilityMetaData.slk");
    GetInputFile("Units\\DestructableData.slk");
    GetInputFile("Units\\DestructableMetaData.slk");
    GetInputFile("Units\\ItemData.slk");
    GetInputFile("Units\\UnitAbilities.slk");
    GetInputFile("Units\\UnitBalance.slk");
    GetInputFile("Units\\UnitData.slk");
    GetInputFile("Units\\UnitUI.slk");
    GetInputFile("Units\\UnitWeapons.slk");
    GetInputFile("Units\\UnitMetaData.slk");
    GetInputFile("Units\\UpgradeData.slk");
    GetInputFile("Units\\UpgradeMetaData.slk");
    GetInputFile("Units\\CampaignAbilityFunc.txt");
    GetInputFile("Units\\CampaignAbilityStrings.txt");
    GetInputFile("Units\\CampaignUnitFunc.txt");
    GetInputFile("Units\\CampaignUnitStrings.txt");
    GetInputFile("Units\\CampaignUpgradeFunc.txt");
    GetInputFile("Units\\CampaignUpgradeStrings.txt");
    GetInputFile("Units\\CommonAbilityFunc.txt");
    GetInputFile("Units\\CommonAbilityStrings.txt");
    GetInputFile("Units\\HumanAbilityFunc.txt");
    GetInputFile("Units\\HumanAbilityStrings.txt");
    GetInputFile("Units\\HumanUnitFunc.txt");
    GetInputFile("Units\\HumanUnitStrings.txt");
    GetInputFile("Units\\HumanUpgradeFunc.txt");
    GetInputFile("Units\\HumanUpgradeStrings.txt");
    GetInputFile("Units\\ItemAbilityFunc.txt");
    GetInputFile("Units\\ItemAbilityStrings.txt");
    GetInputFile("Units\\ItemFunc.txt");
    GetInputFile("Units\\ItemStrings.txt");
    GetInputFile("Units\\NeutralAbilityFunc.txt");
    GetInputFile("Units\\NeutralAbilityStrings.txt");
    GetInputFile("Units\\NeutralUnitFunc.txt");
    GetInputFile("Units\\NeutralUnitStrings.txt");
    GetInputFile("Units\\NeutralUpgradeFunc.txt");
    GetInputFile("Units\\NeutralUpgradeStrings.txt");
    GetInputFile("Units\\NightElfAbilityFunc.txt");
    GetInputFile("Units\\NightElfAbilityStrings.txt");
    GetInputFile("Units\\NightElfUnitFunc.txt");
    GetInputFile("Units\\NightElfUnitStrings.txt");
    GetInputFile("Units\\NightElfUpgradeFunc.txt");
    GetInputFile("Units\\NightElfUpgradeStrings.txt");
    GetInputFile("Units\\OrcAbilityFunc.txt");
    GetInputFile("Units\\OrcAbilityStrings.txt");
    GetInputFile("Units\\OrcUnitFunc.txt");
    GetInputFile("Units\\OrcUnitStrings.txt");
    GetInputFile("Units\\OrcUpgradeFunc.txt");
    GetInputFile("Units\\OrcUpgradeStrings.txt");
    GetInputFile("Units\\UndeadAbilityFunc.txt");
    GetInputFile("Units\\UndeadAbilityStrings.txt");
    GetInputFile("Units\\UndeadUnitFunc.txt");
    GetInputFile("Units\\UndeadUnitStrings.txt");
    GetInputFile("Units\\UndeadUpgradeFunc.txt");
    GetInputFile("Units\\UndeadUpgradeStrings.txt");
}

BOOL ReadMpq(HWND hwnd, LPSTR lpszFileName)
{
    size_t nSize;
    StringCchLengthA(lpszFileName, MAX_PATH, &nSize);

    if (CSTR_EQUAL != CompareStringA(
        LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, 
        lpszFileName+nSize-(_countof("war3patch.mpq")-1), -1,
        "war3patch.mpq", -1))
        return FALSE;

    SFileOpenArchive("ydwe.mpq",0,0,&ydweMpq);
    SFileOpenArchive(lpszFileName, 0, 0, &patchMpq);
    lpszFileName[nSize-(_countof("war3patch.mpq")-1)] = NULL;
    SFileOpenArchive(StrAddTemp(lpszFileName,"war3xlocal.mpq"),0,0,&localMpq);
    SFileOpenArchive(StrAddTemp(lpszFileName,"war3x.mpq"),0,0,&war3xMpq);
    SFileOpenArchive(StrAddTemp(lpszFileName,"war3.mpq"),0,0,&war3Mpq);


    if (ydweMpq == (MPQHANDLE)(INVALID_HANDLE_VALUE))
    {
        MessageBox(hwnd, TEXT("无法找到ydwe.mpq，读取数据可能有错误。"), TEXT("Wc3MapMax++"), 0);
    }
    if (patchMpq == (MPQHANDLE)(INVALID_HANDLE_VALUE))
    {
        MessageBox(hwnd, TEXT("无法找到war3patch.mpq，读取数据可能有错误。"), TEXT("Wc3MapMax++"), 0);
    }
    if (localMpq == (MPQHANDLE)(INVALID_HANDLE_VALUE))
    {
        MessageBox(hwnd, TEXT("无法找到war3xlocal.mpq，读取数据可能有错误。"), TEXT("Wc3MapMax++"), 0);
    }
    if (war3xMpq == (MPQHANDLE)(INVALID_HANDLE_VALUE))
    {
        MessageBox(hwnd, TEXT("无法找到war3x.mpq，读取数据可能有错误。"), TEXT("Wc3MapMax++"), 0);
    }
    if (war3Mpq == (MPQHANDLE)(INVALID_HANDLE_VALUE))
    {
        MessageBox(hwnd, TEXT("无法找到war3.mpq，读取数据可能有错误。"), TEXT("Wc3MapMax++"), 0);
    }

    GetInputFileAll();

    SFileCloseArchive(ydweMpq);
    SFileCloseArchive(patchMpq);
    SFileCloseArchive(localMpq);
    SFileCloseArchive(war3xMpq);
    SFileCloseArchive(war3Mpq);

    return TRUE;
}
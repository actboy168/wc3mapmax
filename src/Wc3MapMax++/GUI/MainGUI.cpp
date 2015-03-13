#include "StdAfx.h"

#include "Include.h"
#include "../Res/resource.h"
#include "../SLKConv.h"

VOID CenterWindow(HWND Window)
{
    INT X, Y;
    RECT TempRect;

    GetWindowRect(Window, &TempRect);

    X = (GetSystemMetrics(SM_CXSCREEN) - (TempRect.right - TempRect.left)) / 2;
    if(X < 0) X = 0;

    Y = (GetSystemMetrics(SM_CYSCREEN) - (TempRect.bottom - TempRect.top)) / 2;
    if(Y < 0) Y = 0;

    SetWindowPos(Window, NULL, X, Y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}

HWND CreateToolTip(HWND hDlg, int toolID, PTSTR pszText)
{
    if (!toolID || !hDlg || !pszText)
    {
        return FALSE;
    }

    HWND hwndTool = GetDlgItem(hDlg, toolID);

    HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
        WS_POPUP |TTS_ALWAYSTIP | TTS_BALLOON,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        hDlg, NULL, 
        NULL, NULL);

    if (!hwndTool || !hwndTip)
    {
        return (HWND)NULL;
    }                              

    TOOLINFO toolInfo = { 0 };
    toolInfo.cbSize = sizeof(toolInfo);
    toolInfo.hwnd = hDlg;
    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId = (UINT_PTR)hwndTool;
    toolInfo.lpszText = pszText;

    SendMessage(hwndTip, TTM_ADDTOOL,        (WPARAM)0, (LPARAM)&toolInfo);
    SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, (WPARAM)0, (LPARAM)256);
    SendMessage(hwndTip, TTM_SETDELAYTIME,   TTDT_INITIAL, MAKELPARAM(200, 0));

    return hwndTip;
}

VOID InitToolTip(HWND hwnd)
{
    CreateToolTip(hwnd, IDC_STATIC_SLK,                     TEXT("使用SLK优化能大量减少地图Loading时间以及一些技能首次使用时的lag现象.\n=>可以双击选择优化."));
    CreateToolTip(hwnd, IDC_COMBO_UNIT,                     TEXT("不优化:      不使用SLK优化;\n优化:        使用SLK优化,保留所有Object;\n优化并简化:  使用SLK优化,删除无用Object."));
    CreateToolTip(hwnd, IDC_COMBO_ITEM,                     TEXT("不优化:      不使用SLK优化;\n优化:        使用SLK优化,保留所有Object;\n优化并简化:  使用SLK优化,删除无用Object."));
    CreateToolTip(hwnd, IDC_COMBO_DEST,                     TEXT("不优化:      不使用SLK优化;\n优化:        使用SLK优化,保留所有Object;\n优化并简化:  使用SLK优化,删除无用Object."));
    CreateToolTip(hwnd, IDC_COMBO_DOOD,                     TEXT("由于war3的一个bug,当你运行一个SLK化的地图后再运行其它SLK化地图，地图上的装饰物会变成紫色方块,所以建议不优化装饰物."));
    CreateToolTip(hwnd, IDC_COMBO_ABIL,                     TEXT("不优化:      不使用SLK优化;\n优化:        使用SLK优化,保留所有Object;\n优化并简化:  使用SLK优化,删除无用Object."));
    CreateToolTip(hwnd, IDC_COMBO_BUFF,                     TEXT("不优化:      不使用SLK优化;\n优化:        使用SLK优化,保留所有Object;\n优化并简化:  使用SLK优化,删除无用Object."));
    CreateToolTip(hwnd, IDC_COMBO_UPGR,                     TEXT("不优化:      不使用SLK优化;\n优化:        使用SLK优化,保留所有Object;\n优化并简化:  使用SLK优化,删除无用Object."));
    CreateToolTip(hwnd, IDC_STATIC_TXT_NULL_MARK,           TEXT("本程序会删除空项以达到最简化效果,但某些String项比如单位说明,即使是空项也要予与保留,否则会显示为tool tip missing!\n该项为空,则软件会保留该类可能出问题的空项.\n若不为空,则优化时会删除这些空项,但以空字符标志输入的项则会作为空项予与保留."));
    CreateToolTip(hwnd, IDC_EDIT_TXT_NULL_MARK,             TEXT("本程序会删除空项以达到最简化效果,但某些String项比如单位说明,即使是空项也要予与保留,否则会显示为tool tip missing!\n该项为空,则软件会保留该类可能出问题的空项.\n若不为空,则优化时会删除这些空项,但以空字符标志输入的项则会作为空项予与保留."));

    CreateToolTip(hwnd, IDC_STATIC_OBJECT,                  TEXT("设置SLK优化的自动搜索选项."));
    CreateToolTip(hwnd, IDC_COMBO_SEARCH_LEVEL,             TEXT("搜索物编相关数据，数值越大表示搜索越多的数据项,0为不搜索,1为推荐值,搜索的数据项可在Config.ini中定义.设置值小于或等于搜索等级的数据项才被列入搜索."));
    CreateToolTip(hwnd, IDC_STATIC_SEARCH_LEVEL,            TEXT("搜索物编相关数据，数值越大表示搜索越多的数据项,0为不搜索,1为推荐值,搜索的数据项可在Config.ini中定义.设置值小于或等于搜索等级的数据项才被列入搜索."));
    CreateToolTip(hwnd, IDC_CHECK_SEARCH_OBJECT_IN_JSCRIPT, TEXT("搜索.j脚本中'xxxx'格式的数据,以及一些会新建Object的特殊函数."));
    CreateToolTip(hwnd, IDC_EDIT_UNUSE_OBJECT_NAME,         TEXT("名字为该项设置值的Object将在优化中被删除(搜索Field: buff为Bufftip,Upgrade为Lv1的Name,其他都为Name)"));
    CreateToolTip(hwnd, IDC_STATIC_UNUSED_OBJECT,           TEXT("名字为该项设置值的Object将在优化中被删除(搜索Field: buff为Bufftip,Upgrade为Lv1的Name,其他都为Name)"));

    CreateToolTip(hwnd, IDC_STATIC_RANDOM,                  TEXT("如果你在地图里使用了相关的随机函数,勾选相关项以保留该类Object.否则请不要勾选,这会使你增加一些无用的Object."));
    CreateToolTip(hwnd, IDC_CHECK_USE_RANDOM_CREEP,         TEXT("触发创建或是初始放置的随机中立野怪"));
    CreateToolTip(hwnd, IDC_CHECK_USE_RANDOM_BUILD,         TEXT("触发创建或是初始放置的随机中立建筑"));
    CreateToolTip(hwnd, IDC_CHECK_USE_RANDOM_ITEM,          TEXT("触发创建或是物品掉落表中设置的随机物品"));
    CreateToolTip(hwnd, IDC_CHECK_USE_MARKET,               TEXT("对战游戏中使用的市场自动更新出售物品"));

    CreateToolTip(hwnd, IDC_STATIC_EXTRA,                   TEXT("其它优化设置."));
    CreateToolTip(hwnd, IDC_CHECK1,                         TEXT("将.doo文件中创建可破坏物的代码转到.j文件中,能减少地图Loading时间和地图体积.\n使用说明:在.j文件中要插入代码的地方添加标志字符,转换后的代码将会替换标志字符,如果添加多个标志字符,则会自动将代码平分到各个位置.\n建议将创建代码放在游戏开始之后."));
    CreateToolTip(hwnd, IDC_STATIC_DOO_NUM,                 TEXT("转换后的代码中所使用实数的最大小数位数.\n例:该值为2时,1.234=>1.23;  0.2=>.2;  3.0=>3"));
    CreateToolTip(hwnd, IDC_EDIT_DOO_CONV_DIGITS_NUM,       TEXT("转换后的代码中所使用实数的最大小数位数.\n例:该值为2时,1.234=>1.23;  0.2=>.2;  3.0=>3."));
    CreateToolTip(hwnd, IDC_STATIC_DOO_MARK,                TEXT("在j脚本中插入标志字符(如果是在触发中可以用CustomScript插入),转换后的代码将在插入点替换该字符串.如果添加多个标志字符,则会自动将代码平分到各个位置.\n建议将创建代码放在游戏开始之后."));
    CreateToolTip(hwnd, IDC_EDIT_DOO_MARK_STRING,           TEXT("在j脚本中插入标志字符(如果是在触发中可以用CustomScript插入),转换后的代码将在插入点替换该字符串.如果添加多个标志字符,则会自动将代码平分到各个位置.\n建议将创建代码放在游戏开始之后."));
    CreateToolTip(hwnd, IDC_CHECK_WTS,                      TEXT("war3地图用到的大部分字符都被保存在war3map.wts文件中,而事实上这是个很不好的设定,将wts文件中记录的字符串还原到各个文件,可以减小地图体积并提升运行速度."));
    CreateToolTip(hwnd, IDC_CHECK12,                        TEXT("使用Wc3MapOptimizer优化J文件."));

    CreateToolTip(hwnd, IDC_CHECK4,                         TEXT("删除在WE中使用,而游戏中不需要的文件,使地图变得更小,但同时你将无法用WE正常编辑该地图."));
    CreateToolTip(hwnd, IDC_CHECK5,                         TEXT("保留listfile文件可以使各种MPQ工具能够读取地图中的所有文件,当然删除该文件也并不会影响到游戏."));
    CreateToolTip(hwnd, IDC_CHECK11,                        TEXT("清除blp头文件中多余的字节,并删除图标和Loading图中多余的mipMap图片.\n不影响图片品质."));
    CreateToolTip(hwnd, IDC_COMBO10,                        TEXT("mip1:优化后体积最小,但在图像设置-纹理质量为低时,会显示不正常.\nmip2:默认选项,保留低纹理质量时所需的mipMap图片.\nmip16:不删除mipMap图片."));
    CreateToolTip(hwnd, IDC_CHECK10,                        TEXT("压缩Mdx模型文件大小,减小其在地图中占用体积,该功能为有损压缩,但不会由于多次压缩导致模型品质降低."));

    CreateToolTip(hwnd, IDC_STATIC_EXLISTFILE,              TEXT("如果你优化的是加密地图,可能需要定义外部listfile文件或是搜索MPQ来获取文件列表(这里并不保证能正常优化加密地图)"));
    CreateToolTip(hwnd, IDC_BUTTON_LIST_FILE,               TEXT("如果你优化的是加密地图,可能需要定义外部listfile文件或是搜索MPQ来获取文件列表(这里并不保证能正常优化加密地图)"));

    CreateToolTip(hwnd, IDC_BUTTON_INPUT_FILE,              TEXT("从魔兽mpq文件中读取本地数据作为input文件."));
    CreateToolTip(hwnd, IDC_CHECK_WAV_AUTO_BLOCKSIZE,       TEXT("地图中有Wav文件存在的时候,使用该选项会比较并选择最佳压缩方式,\n对于没有Wav文件的地图,该选项不起作用."));
    CreateToolTip(hwnd, IDC_CHECK_ONLY_FOR_COMPRESS,        TEXT("仅压缩地图而不进行优化."));
    CreateToolTip(hwnd, IDC_BUTTON_OPEN,                    TEXT("选择需要优化的地图,你也可以直接拖动文件到程序界面."));
    CreateToolTip(hwnd, IDC_BUTTON_OPT,                     TEXT("优化所选地图,优化后的地图会在原地图名前加上new_前缀."));
}

VOID InitComboBox(HWND hwnd)
{
    HWND Contorl;

    Contorl = GetDlgItem(hwnd, IDC_COMBO_UNIT);
    ComboBox_AddString(Contorl, TEXT("不优化"));
    ComboBox_AddString(Contorl, TEXT("优化"));
    ComboBox_AddString(Contorl, TEXT("优化并简化"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_ITEM);
    ComboBox_AddString(Contorl, TEXT("不优化"));
    ComboBox_AddString(Contorl, TEXT("优化"));
    ComboBox_AddString(Contorl, TEXT("优化并简化"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_DEST);
    ComboBox_AddString(Contorl, TEXT("不优化"));
    ComboBox_AddString(Contorl, TEXT("优化"));
    ComboBox_AddString(Contorl, TEXT("优化并简化"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_DOOD);
    ComboBox_AddString(Contorl, TEXT("不优化"));
    ComboBox_AddString(Contorl, TEXT("优化"));
    ComboBox_AddString(Contorl, TEXT("优化并简化"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_ABIL);
    ComboBox_AddString(Contorl, TEXT("不优化"));
    ComboBox_AddString(Contorl, TEXT("优化"));
    ComboBox_AddString(Contorl, TEXT("优化并简化"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_BUFF);
    ComboBox_AddString(Contorl, TEXT("不优化"));
    ComboBox_AddString(Contorl, TEXT("优化"));
    ComboBox_AddString(Contorl, TEXT("优化并简化"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_UPGR);
    ComboBox_AddString(Contorl, TEXT("不优化"));
    ComboBox_AddString(Contorl, TEXT("优化"));
    ComboBox_AddString(Contorl, TEXT("优化并简化"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_SEARCH_LEVEL);
    ComboBox_AddString(Contorl, TEXT("0"));
    ComboBox_AddString(Contorl, TEXT("1"));
    ComboBox_AddString(Contorl, TEXT("2"));
    ComboBox_AddString(Contorl, TEXT("3"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO10);
    ComboBox_AddString(Contorl, TEXT("mip1"));
    ComboBox_AddString(Contorl, TEXT("mip2"));
    ComboBox_AddString(Contorl, TEXT("mip16"));
}

VOID InitControl(HWND hwnd)
{
    ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_UNIT), OPT_UNIT);
    ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_ITEM), OPT_ITEM);
    ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_DEST), OPT_DEST);
    ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_DOOD), OPT_DOOD);
    ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_ABIL), OPT_ABIL);
    if ((OPT_ABIL == 0 && OPT_BUFF > 0) || (OPT_ABIL > 0 && OPT_BUFF == 0))
        ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_BUFF), OPT_ABIL);
    else
        ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_BUFF), OPT_BUFF);
    ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_UPGR), OPT_UPGR);

    WCHAR buffer[0x100];

    MultiByteToWideChar(CP_UTF8, 0, szTxtNullMark, strlen(szTxtNullMark)+1, buffer, 0x100);
    Edit_SetText(GetDlgItem(hwnd, IDC_EDIT_TXT_NULL_MARK), buffer);

    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK1), convDooToJScript);
    Edit_Enable(GetDlgItem(hwnd, IDC_EDIT_DOO_CONV_DIGITS_NUM), convDooToJScript);
    Edit_Enable(GetDlgItem(hwnd, IDC_EDIT_DOO_MARK_STRING), convDooToJScript);

    StringCchPrintf(buffer, 0x100, TEXT("%d"), digitsNumForDooConv);
    Edit_SetText(GetDlgItem(hwnd, IDC_EDIT_DOO_CONV_DIGITS_NUM), buffer);

    MultiByteToWideChar(CP_UTF8, 0, szMarkStringForDooConv, strlen(szMarkStringForDooConv)+1, buffer, 0x100);
    Edit_SetText(GetDlgItem(hwnd, IDC_EDIT_DOO_MARK_STRING), buffer);

    Edit_LimitText(GetDlgItem(hwnd, IDC_EDIT_DOO_CONV_DIGITS_NUM), 1);
    Edit_LimitText(GetDlgItem(hwnd, IDC_EDIT_DOO_MARK_STRING), 255);

    Button_Enable(GetDlgItem(hwnd, IDC_CHECK_WTS), FALSE);

    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK_WTS), 1);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK4), removeWEOnlyFiles);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK5), keepListFile);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK10), mdxOptLevel);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK11), blpForOpt);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK12), jForOpt);

    ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO10), ((blpMipNum>2)? 2: (blpMipNum-1)));

    if ((szListFilePath != NULL) && (*szListFilePath != NULL))
        Button_SetText(GetDlgItem(hwnd, IDC_BUTTON_LIST_FILE), TEXT("使用"));

    ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_SEARCH_LEVEL), searchObjectLevel);

    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK_SEARCH_OBJECT_IN_JSCRIPT), searchObjectInJScript);

    MultiByteToWideChar(CP_UTF8, 0, szUnusedObjectName, strlen(szUnusedObjectName)+1, buffer, 0x100);
    Edit_SetText(GetDlgItem(hwnd, IDC_EDIT_UNUSE_OBJECT_NAME), buffer);

    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK_USE_RANDOM_CREEP), useRandomCreep);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK_USE_RANDOM_BUILD), useRandomBuild);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK_USE_RANDOM_ITEM), useRandomItem);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK_USE_MARKET), useMarket);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK_ONLY_FOR_COMPRESS), onlyForCompress);
    Button_SetCheck(GetDlgItem(hwnd, IDC_CHECK_WAV_AUTO_BLOCKSIZE), wavAutoBlockSize);
}

BOOL ReadInput(HWND hwnd)
{
    BOOL bRet = FALSE;

    if (access("Input",0) == -1)	//不存在Input文件夹
    {
        HKEY hKey;
        if (ERROR_SUCCESS == RegOpenKeyA(HKEY_CURRENT_USER, "Software\\Blizzard Entertainment\\Warcraft III", &hKey))
        {
            CHAR szFilePath[MAX_PATH];
            DWORD dwSize = MAX_PATH;
            if (ERROR_SUCCESS == RegQueryValueExA(hKey, "InstallPath", NULL, NULL, (LPBYTE)szFilePath, &dwSize))
            {
                size_t nSize;
                StringCchLengthA(szFilePath, MAX_PATH, &nSize);
                if (szFilePath[nSize-1] != '\\')
                    szFilePath[nSize] = '\\';

                StringCchCatA(szFilePath, MAX_PATH, "war3patch.mpq");

                bRet = ReadMpq(hwnd, szFilePath);
            }
            RegCloseKey(hKey);
        }
        if (!bRet)
        {
            MessageBox(hwnd, TEXT("War3注册表信息错误,请手动获取数据文件!"), TEXT("Wc3MapMax++"), 0);
        }
        return bRet;
    }
    return TRUE;
}

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
    SETDLGICONS(hwnd, IDR_MAINFRAME);
    CenterWindow(hwnd);

    ReadInput(hwnd);

    ReadCFG();

    InitToolTip(hwnd);
    InitComboBox(hwnd);
    InitControl(hwnd);

    return TRUE;
}

VOID Dlg_OnOnlyForCompress(HWND hwnd)
{
    onlyForCompress = !onlyForCompress;

    ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO_UNIT), !onlyForCompress);
    ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO_ITEM), !onlyForCompress);
    ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO_DEST), !onlyForCompress);
    ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO_DOOD), !onlyForCompress);
    ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO_ABIL), !onlyForCompress);
    ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO_BUFF), !onlyForCompress);
    ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO_UPGR), !onlyForCompress);

    Edit_Enable(GetDlgItem(hwnd, IDC_EDIT_TXT_NULL_MARK), !onlyForCompress);
    Button_Enable(GetDlgItem(hwnd, IDC_CHECK1), !onlyForCompress);
    Edit_Enable(GetDlgItem(hwnd, IDC_EDIT_DOO_CONV_DIGITS_NUM), !onlyForCompress && convDooToJScript);
    Edit_Enable(GetDlgItem(hwnd, IDC_EDIT_DOO_MARK_STRING), !onlyForCompress && convDooToJScript);

    Button_Enable(GetDlgItem(hwnd, IDC_CHECK_WTS), !onlyForCompress);
    Button_Enable(GetDlgItem(hwnd, IDC_CHECK4), !onlyForCompress);
    Button_Enable(GetDlgItem(hwnd, IDC_CHECK12), !onlyForCompress);

    //Button_Enable(GetDlgItem(hwnd, IDC_CHECK10), !onlyForCompress);
    //Button_Enable(GetDlgItem(hwnd, IDC_CHECK11), !onlyForCompress);
    //ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO10), !onlyForCompress);

    ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO_SEARCH_LEVEL), !onlyForCompress);
    Button_Enable(GetDlgItem(hwnd, IDC_CHECK_SEARCH_OBJECT_IN_JSCRIPT), !onlyForCompress);
    Edit_Enable(GetDlgItem(hwnd, IDC_EDIT_UNUSE_OBJECT_NAME), !onlyForCompress);

    Button_Enable(GetDlgItem(hwnd, IDC_CHECK_USE_RANDOM_CREEP), !onlyForCompress);
    Button_Enable(GetDlgItem(hwnd, IDC_CHECK_USE_RANDOM_BUILD), !onlyForCompress);
    Button_Enable(GetDlgItem(hwnd, IDC_CHECK_USE_RANDOM_ITEM), !onlyForCompress);
    Button_Enable(GetDlgItem(hwnd, IDC_CHECK_USE_MARKET), !onlyForCompress);
}

VOID Dlg_OpenMapFile(HWND hwnd)
{
    OPENFILENAME ofn = {0};
    TCHAR wszFile[MAX_PATH];            

    ofn.lStructSize     = sizeof(ofn);  
    ofn.hwndOwner       = hwnd;  
    ofn.lpstrFile       = wszFile;  
    ofn.lpstrFile[0]    = TEXT('\0');  
    ofn.nMaxFile        = sizeof(wszFile);  
    ofn.lpstrFilter     = TEXT("魔兽争霸地图文件\0*.w3x;*.w3m\0");  
    ofn.nFilterIndex    = 1;  
    ofn.lpstrFileTitle  = NULL;  
    ofn.nMaxFileTitle   = 0;  
    ofn.lpstrInitialDir = NULL;  
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  

    if (GetOpenFileName(&ofn))  
    {
        USES_CONVERSION;
        LPSTR lpszFile = T2A(wszFile);

        Edit_SetText(GetDlgItem(hwnd, IDC_EDIT_FILE_NAME), wszFile);
        Edit_SetSel(GetDlgItem(hwnd, IDC_EDIT_FILE_NAME), -1, -1);

        ListMapFile(lpszFile);

        TCHAR buf[MAX_PATH];
        StringCchPrintf(buf, MAX_PATH, TEXT("%d/%d files found."), MapFileNumFound, MapFileNum);
        Edit_SetText(GetDlgItem(hwnd, IDC_STATIC_BOTTON_TEXT), buf);
    }  
}


VOID Dlg_OpenInputFile(HWND hwnd)
{
    OPENFILENAME ofn = {0};
    TCHAR wszFile[MAX_PATH];            

    ofn.lStructSize     = sizeof(ofn);  
    ofn.hwndOwner       = hwnd;  
    ofn.lpstrFile       = wszFile;  
    ofn.lpstrFile[0]    = TEXT('\0');  
    ofn.nMaxFile        = sizeof(wszFile);  
    ofn.lpstrFilter     = TEXT("war3patch.mpq\0war3patch.mpq\0");  
    ofn.nFilterIndex    = 1;  
    ofn.lpstrFileTitle  = NULL;  
    ofn.nMaxFileTitle   = 0;  
    ofn.lpstrInitialDir = NULL;  
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  

    if (GetOpenFileName(&ofn))  
    {
        USES_CONVERSION;
        LPSTR lpszFile = T2A(wszFile);

        if (ReadMpq(hwnd, lpszFile))
            MessageBox(hwnd, TEXT("数据文件读取完成！"), TEXT("Wc3MapMax++"), 0);
    }  
}

VOID Dlg_OpenListFile(HWND hwnd)
{
    OPENFILENAME ofn = {0};
    TCHAR wszFile[MAX_PATH];            

    ofn.lStructSize     = sizeof(ofn);  
    ofn.hwndOwner       = hwnd;  
    ofn.lpstrFile       = wszFile;  
    ofn.lpstrFile[0]    = TEXT('\0');  
    ofn.nMaxFile        = sizeof(wszFile);  
    ofn.lpstrFilter     = TEXT("listfile文件\0*.txt;(listfile)\0");  
    ofn.nFilterIndex    = 1;  
    ofn.lpstrFileTitle  = NULL;  
    ofn.nMaxFileTitle   = 0;  
    ofn.lpstrInitialDir = NULL;  
    ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;  

    if (GetOpenFileName(&ofn))  
    {
        USES_CONVERSION;
        LPSTR lpszFile = T2A(wszFile);
        StringCchCopyA(szListFilePath, MAX_PATH, lpszFile);
    }  
}

void Dlg_UpdataConfig(HWND hwnd)
{
    USES_CONVERSION;
    WCHAR wszFile[MAX_PATH];
    LPSTR  lpszFile;

    ExInit();

    OPT_UNIT = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_UNIT));
    OPT_ITEM = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_ITEM));
    OPT_DEST = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_DEST));
    OPT_DOOD = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_DOOD));
    OPT_ABIL = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_ABIL));
    OPT_BUFF = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_BUFF));
    OPT_UPGR = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_UPGR));

    convDooToJScript = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK1));

    Edit_GetText(GetDlgItem(hwnd, IDC_EDIT_TXT_NULL_MARK), wszFile, MAX_PATH);
    lpszFile = T2A(wszFile);
    StringCchCopyA(szTxtNullMark, MAX_PATH, lpszFile);

    Edit_GetText(GetDlgItem(hwnd, IDC_EDIT_DOO_CONV_DIGITS_NUM), wszFile, MAX_PATH);
    lpszFile = T2A(wszFile);
    digitsNumForDooConv = a2i(lpszFile);

    Edit_GetText(GetDlgItem(hwnd, IDC_EDIT_DOO_MARK_STRING), wszFile, MAX_PATH);
    lpszFile = T2A(wszFile);
    StringCchCopyA(szMarkStringForDooConv, MAX_PATH, lpszFile);

    removeWEOnlyFiles = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK4));
    keepListFile      = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK5));
    mdxOptLevel       = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK10));
    blpForOpt         = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK11));
    jForOpt           = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK12));

    blpMipNum = 1 + ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO10));
    if (blpMipNum > 2) blpMipNum = 16; 

    searchObjectLevel     = ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_SEARCH_LEVEL));
    searchObjectInJScript = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK_SEARCH_OBJECT_IN_JSCRIPT));

    useRandomCreep   = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK_USE_RANDOM_CREEP));
    useRandomBuild   = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK_USE_RANDOM_BUILD));
    useRandomItem    = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK_USE_RANDOM_ITEM));
    useMarket        = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK_USE_MARKET));
    onlyForCompress  = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK_ONLY_FOR_COMPRESS));
    wavAutoBlockSize = Button_GetCheck(GetDlgItem(hwnd, IDC_CHECK_WAV_AUTO_BLOCKSIZE));

    Edit_GetText(GetDlgItem(hwnd, IDC_EDIT_UNUSE_OBJECT_NAME), wszFile, MAX_PATH);
    lpszFile = T2A(wszFile);
    StringCchCopyA(szUnusedObjectName, MAX_PATH, lpszFile);
}

VOID GetDestPath(LPSTR lpszDestPath, size_t cchMax, LPSTR lpszPath)
{
    size_t nSize;

    StringCchLengthA(lpszPath, cchMax, &nSize);
    while (nSize-- > 0)
    {
        if ((lpszPath[nSize] == '\\') || (lpszPath[nSize] == '/'))
        {
            lpszPath[nSize] = '\0';
            StringCchPrintfA(lpszDestPath, cchMax, "%s\\new_%s", lpszPath, &lpszPath[nSize+1]);
            break;
        }
    }

    if (nSize == -1)
        StringCchCopyA(lpszDestPath, cchMax, lpszPath);
}

void Dlg_OnOptButton(HWND hwnd)
{
    USES_CONVERSION;
    WCHAR wszFile[MAX_PATH];
    LPSTR  lpszFile;

    Dlg_UpdataConfig(hwnd);

    WriteCFG();

    Edit_GetText(GetDlgItem(hwnd, IDC_EDIT_FILE_NAME), wszFile, MAX_PATH);
    lpszFile = T2A(wszFile);

    CHAR szMapPath[MAX_PATH];
    CHAR szDestMapPath[MAX_PATH];

    StringCchCopyA(szMapPath, MAX_PATH, lpszFile);
    GetDestPath(szDestMapPath, MAX_PATH, lpszFile);

    int optRes = RunOptimizer(szMapPath, szDestMapPath);

    switch (optRes)
    {
    case OPTIMIZER_FINISH:
        MessageBox(hwnd, TEXT("优化完成!"), TEXT("Wc3MapMax++"), 0);
        DestroyWindow(hwnd);
        break;
    case ERR_FILE_NOT_FOUND:
        MessageBox(hwnd, TEXT("指定文件不存在或无法打开!"), TEXT("Wc3MapMax++"), 0);
        break;
    case ERR_JSCRIPT_LOST:
        MessageBox(hwnd, TEXT("地图脚本文件丢失!"), TEXT("Wc3MapMax++"), 0);
        break;
    default:
        MessageBox(hwnd, TEXT("地图转换失败!"), TEXT("Wc3MapMax++"), 0);
        break;
    }
}

BOOL Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) 
{
    switch (id) 
    {
    case IDC_BUTTON_OPT:
        Dlg_OnOptButton(hwnd);
        break;
    case IDC_CHECK_ONLY_FOR_COMPRESS:
        Dlg_OnOnlyForCompress(hwnd);
        break;
    case IDC_CHECK1:
        convDooToJScript = !convDooToJScript;
        Edit_Enable(GetDlgItem(hwnd, IDC_EDIT_DOO_CONV_DIGITS_NUM), convDooToJScript);
        Edit_Enable(GetDlgItem(hwnd, IDC_EDIT_DOO_MARK_STRING), convDooToJScript);
        break;
    case IDC_CHECK11:
        blpForOpt = !blpForOpt;
        ComboBox_Enable(GetDlgItem(hwnd, IDC_COMBO10), blpForOpt);
        break;
    case IDC_BUTTON_MAP_FILE:
        Dlg_OpenMapFile(hwnd);
        break;
    case IDC_BUTTON_INPUT_FILE:
        Dlg_OpenInputFile(hwnd);
        break;
    case IDC_BUTTON_LIST_FILE:
        if (szListFilePath != NULL && szListFilePath[0] != '\0')
        {
            szListFilePath[0] = '\0';
            Button_SetText(GetDlgItem(hwnd, IDC_BUTTON_LIST_FILE), TEXT("未设置"));
        }
        else
        {
            Dlg_OpenListFile(hwnd);
            Button_SetText(GetDlgItem(hwnd, IDC_BUTTON_LIST_FILE), TEXT("已设置"));
        }
        break;
    case IDC_COMBO_ABIL:
        if (codeNotify == CBN_SELCHANGE)
        {
            if (0 == ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_ABIL)))
            {
                ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_BUFF), 0);
            }
            else if (0 == ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_BUFF)))
            {
                ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_BUFF), ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_ABIL)));
            }
        }
        break;
    case IDC_COMBO_BUFF:
        if (codeNotify == CBN_SELCHANGE)
        {
            if (0 == ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_BUFF)))
            {
                ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_ABIL), 0);
            }
            else if (0 == ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_ABIL)))
            {
                ComboBox_SetCurSel(GetDlgItem(hwnd, IDC_COMBO_ABIL), ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_COMBO_BUFF)));
            }
        }
        break;
    default:
        break;
    }

    return FALSE;
}


BOOL Dlg_OnSysCommand(HWND hwnd, UINT cmd, int x, int y)
{
    if((cmd & 0xFFF0) == SC_CLOSE)
    {
        DestroyWindow(hwnd);
        return TRUE;
    }

    return FALSE;
}

BOOL Dlg_OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
    return TRUE;
}

BOOL Dlg_OnDropFiles(HWND hwnd, HDROP hdrop)
{	
    Dlg_UpdataConfig(hwnd);

    int dropFileNum = DragQueryFileA(hdrop, -1, NULL, 0);
    int size;
    char* fileStr = (char*)malloc(1);
    char slk=0,txt=0,mdx=0,blp=0,js=0;
    char txtDataVer;
    for(int i=0;i<dropFileNum;i++)
    {
        size = DragQueryFileA(hdrop, i, NULL, 0);
        free(fileStr);
        fileStr = (char*)malloc(size+1);
        DragQueryFileA(hdrop, i, fileStr, size+1);
        if (fileStr[size-4]=='.' && (fileStr[size-3] | 32)=='w' && fileStr[size-2]=='3' && ((fileStr[size-1] | 32)=='x' || (fileStr[size-1] | 32)=='m' || (fileStr[size-1] | 32)=='n'))
        {
            USES_CONVERSION;

            Edit_SetText(GetDlgItem(hwnd, IDC_EDIT_FILE_NAME), A2W(fileStr));
            Edit_SetSel(GetDlgItem(hwnd, IDC_EDIT_FILE_NAME), -1, -1);

            ListMapFile(fileStr);

            TCHAR buf[MAX_PATH];
            StringCchPrintf(buf, MAX_PATH, TEXT("%d/%d files found."), MapFileNumFound, MapFileNum);
            Edit_SetText(GetDlgItem(hwnd, IDC_STATIC_BOTTON_TEXT), buf);
            break;
        }
        else if (fileStr[size-4]=='.' && (fileStr[size-3] | 32)=='s' && (fileStr[size-2] | 32)=='l' && (fileStr[size-1] | 32)=='k')
        {
            if (slk==0)
            {
                if (IDYES == MessageBox(hwnd, TEXT("是否简化SLK文件(作为数据文件)?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
                    slk = 1;
                else
                    slk = -1;
            }
            if (slk==1)
                SlkOptByFile(fileStr);
        }
        else if (fileStr[size-4]=='.' && (fileStr[size-3] | 32)=='t' && (fileStr[size-2] | 32)=='x' && (fileStr[size-1] | 32)=='t')
        {
            if (txt==0)
            {
                if (IDYES == MessageBox(hwnd, TEXT("是否简化txt文件(作为数据文件)?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
                {
                    txt=1;
                    if (IDYES == MessageBox(hwnd, TEXT("简化后的txt文件使用Melee数据?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
                        txtDataVer=1;
                    else
                        txtDataVer=0;
                }
                else
                    txt=-1;
            }
            if (txt==1)
            {
                TxtOptByFile(fileStr,txtDataVer);
            }
        }
        else if (fileStr[size-4]=='.' && (fileStr[size-3] | 32)=='b' && (fileStr[size-2] | 32)=='l' && (fileStr[size-1] | 32)=='p')
        {
            if (blp==0)
            {
                if (IDYES == MessageBox(hwnd, TEXT("是否优化blp文件?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
                    blp=1;
                else
                    blp=-1;
            }
            if (blp==1)
            {
                int fp=open(fileStr,_O_BINARY|_O_RDWR);
                if (fp<=0)
                    return TRUE;
                int size=filelength(fp);
                char* buffer=(char*)malloc(size+1);
                read(fp,buffer,size);
                close(fp);
                size = BlpOpt(buffer,size,blpMipNum);
                fp = creat(fileStr,S_IWRITE);
                setmode(fp,_O_BINARY);
                write(fp,buffer,size);
                close(fp);
            }
        }
        else if (fileStr[size-4]=='.' && (fileStr[size-3] | 32)=='m' && (fileStr[size-2] | 32)=='d' && (fileStr[size-1] | 32)=='x')
        {
            if (mdx==0)
            {
                if (IDYES == MessageBox(hwnd, TEXT("是否压缩mdx文件?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
                    mdx=1;
                else
                    mdx=-1;
            }
            if (mdx==1)
            {
                int fp=open(fileStr,_O_BINARY|_O_RDWR);
                if (fp<=0)
                    return TRUE;
                int size=filelength(fp);
                char* buffer=(char*)malloc(size+1);
                read(fp,buffer,size);
                MdxOpt(buffer,size);
                lseek(fp,0,SEEK_SET);
                write(fp,buffer,size);
                close(fp);
            }
        }
        else if (fileStr[size-2]=='.' && (fileStr[size-1] | 32)=='j')
        {
            if (js==0)
            {
                if (IDYES == MessageBox(hwnd, TEXT("是否清理J脚本文件?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
                    js=1;
                else
                    js=-1;
            }
            if (js==1)
            {
                int fp=open(fileStr,_O_BINARY|_O_RDWR);
                if (fp<=0)
                    return TRUE;
                int size=filelength(fp);
                char* buffer=(char*)malloc(size+8);
                read(fp,buffer,size);
                close(fp);
                size=ClearJScript(buffer,size);
                fp = creat(fileStr,S_IWRITE);
                setmode(fp,_O_BINARY);
                write(fp,buffer,size);
                close(fp);
            }
        }
        else if (stricmp(fileStr+size-14,"\\war3patch.mpq")==0)
        {
            if (IDYES == MessageBox(hwnd, TEXT("是否从Mpq中读取数据文件?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
            {
                ReadMpq(hwnd, fileStr);
            }
        }
    }
    free(fileStr);
    txtOptInit=0;
    DragFinish(hdrop);
    return TRUE;
}

INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg) 
    {
        HANDLE_DLGMSG(hwnd, WM_INITDIALOG,    Dlg_OnInitDialog);
        HANDLE_DLGMSG(hwnd, WM_COMMAND,       Dlg_OnCommand);
        HANDLE_DLGMSG(hwnd, WM_DROPFILES,     Dlg_OnDropFiles);
        HANDLE_DLGMSG_SYSCOMMAND(hwnd,        Dlg_OnSysCommand); 
        HANDLE_DLGMSG(hwnd, WM_DESTROY,       Dlg_OnDestroy);
    }

    return FALSE;
}

HWND Dialog_Display(HINSTANCE hInstance)
{
    HWND hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_WC3MAPMAX_DIALOG), NULL, Dlg_Proc);

    ShowWindow(hwnd, SW_SHOW);

    return hwnd;
}

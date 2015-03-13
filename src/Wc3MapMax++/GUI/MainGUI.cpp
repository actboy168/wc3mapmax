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
    CreateToolTip(hwnd, IDC_STATIC_SLK,                     TEXT("ʹ��SLK�Ż��ܴ������ٵ�ͼLoadingʱ���Լ�һЩ�����״�ʹ��ʱ��lag����.\n=>����˫��ѡ���Ż�."));
    CreateToolTip(hwnd, IDC_COMBO_UNIT,                     TEXT("���Ż�:      ��ʹ��SLK�Ż�;\n�Ż�:        ʹ��SLK�Ż�,��������Object;\n�Ż�����:  ʹ��SLK�Ż�,ɾ������Object."));
    CreateToolTip(hwnd, IDC_COMBO_ITEM,                     TEXT("���Ż�:      ��ʹ��SLK�Ż�;\n�Ż�:        ʹ��SLK�Ż�,��������Object;\n�Ż�����:  ʹ��SLK�Ż�,ɾ������Object."));
    CreateToolTip(hwnd, IDC_COMBO_DEST,                     TEXT("���Ż�:      ��ʹ��SLK�Ż�;\n�Ż�:        ʹ��SLK�Ż�,��������Object;\n�Ż�����:  ʹ��SLK�Ż�,ɾ������Object."));
    CreateToolTip(hwnd, IDC_COMBO_DOOD,                     TEXT("����war3��һ��bug,��������һ��SLK���ĵ�ͼ������������SLK����ͼ����ͼ�ϵ�װ���������ɫ����,���Խ��鲻�Ż�װ����."));
    CreateToolTip(hwnd, IDC_COMBO_ABIL,                     TEXT("���Ż�:      ��ʹ��SLK�Ż�;\n�Ż�:        ʹ��SLK�Ż�,��������Object;\n�Ż�����:  ʹ��SLK�Ż�,ɾ������Object."));
    CreateToolTip(hwnd, IDC_COMBO_BUFF,                     TEXT("���Ż�:      ��ʹ��SLK�Ż�;\n�Ż�:        ʹ��SLK�Ż�,��������Object;\n�Ż�����:  ʹ��SLK�Ż�,ɾ������Object."));
    CreateToolTip(hwnd, IDC_COMBO_UPGR,                     TEXT("���Ż�:      ��ʹ��SLK�Ż�;\n�Ż�:        ʹ��SLK�Ż�,��������Object;\n�Ż�����:  ʹ��SLK�Ż�,ɾ������Object."));
    CreateToolTip(hwnd, IDC_STATIC_TXT_NULL_MARK,           TEXT("�������ɾ�������Դﵽ���Ч��,��ĳЩString����絥λ˵��,��ʹ�ǿ���ҲҪ���뱣��,�������ʾΪtool tip missing!\n����Ϊ��,������ᱣ��������ܳ�����Ŀ���.\n����Ϊ��,���Ż�ʱ��ɾ����Щ����,���Կ��ַ���־������������Ϊ�������뱣��."));
    CreateToolTip(hwnd, IDC_EDIT_TXT_NULL_MARK,             TEXT("�������ɾ�������Դﵽ���Ч��,��ĳЩString����絥λ˵��,��ʹ�ǿ���ҲҪ���뱣��,�������ʾΪtool tip missing!\n����Ϊ��,������ᱣ��������ܳ�����Ŀ���.\n����Ϊ��,���Ż�ʱ��ɾ����Щ����,���Կ��ַ���־������������Ϊ�������뱣��."));

    CreateToolTip(hwnd, IDC_STATIC_OBJECT,                  TEXT("����SLK�Ż����Զ�����ѡ��."));
    CreateToolTip(hwnd, IDC_COMBO_SEARCH_LEVEL,             TEXT("�������������ݣ���ֵԽ���ʾ����Խ���������,0Ϊ������,1Ϊ�Ƽ�ֵ,���������������Config.ini�ж���.����ֵС�ڻ���������ȼ���������ű���������."));
    CreateToolTip(hwnd, IDC_STATIC_SEARCH_LEVEL,            TEXT("�������������ݣ���ֵԽ���ʾ����Խ���������,0Ϊ������,1Ϊ�Ƽ�ֵ,���������������Config.ini�ж���.����ֵС�ڻ���������ȼ���������ű���������."));
    CreateToolTip(hwnd, IDC_CHECK_SEARCH_OBJECT_IN_JSCRIPT, TEXT("����.j�ű���'xxxx'��ʽ������,�Լ�һЩ���½�Object�����⺯��."));
    CreateToolTip(hwnd, IDC_EDIT_UNUSE_OBJECT_NAME,         TEXT("����Ϊ��������ֵ��Object�����Ż��б�ɾ��(����Field: buffΪBufftip,UpgradeΪLv1��Name,������ΪName)"));
    CreateToolTip(hwnd, IDC_STATIC_UNUSED_OBJECT,           TEXT("����Ϊ��������ֵ��Object�����Ż��б�ɾ��(����Field: buffΪBufftip,UpgradeΪLv1��Name,������ΪName)"));

    CreateToolTip(hwnd, IDC_STATIC_RANDOM,                  TEXT("������ڵ�ͼ��ʹ������ص��������,��ѡ������Ա�������Object.�����벻Ҫ��ѡ,���ʹ������һЩ���õ�Object."));
    CreateToolTip(hwnd, IDC_CHECK_USE_RANDOM_CREEP,         TEXT("�����������ǳ�ʼ���õ��������Ұ��"));
    CreateToolTip(hwnd, IDC_CHECK_USE_RANDOM_BUILD,         TEXT("�����������ǳ�ʼ���õ������������"));
    CreateToolTip(hwnd, IDC_CHECK_USE_RANDOM_ITEM,          TEXT("��������������Ʒ����������õ������Ʒ"));
    CreateToolTip(hwnd, IDC_CHECK_USE_MARKET,               TEXT("��ս��Ϸ��ʹ�õ��г��Զ����³�����Ʒ"));

    CreateToolTip(hwnd, IDC_STATIC_EXTRA,                   TEXT("�����Ż�����."));
    CreateToolTip(hwnd, IDC_CHECK1,                         TEXT("��.doo�ļ��д������ƻ���Ĵ���ת��.j�ļ���,�ܼ��ٵ�ͼLoadingʱ��͵�ͼ���.\nʹ��˵��:��.j�ļ���Ҫ�������ĵط���ӱ�־�ַ�,ת����Ĵ��뽫���滻��־�ַ�,�����Ӷ����־�ַ�,����Զ�������ƽ�ֵ�����λ��.\n���齫�������������Ϸ��ʼ֮��."));
    CreateToolTip(hwnd, IDC_STATIC_DOO_NUM,                 TEXT("ת����Ĵ�������ʹ��ʵ�������С��λ��.\n��:��ֵΪ2ʱ,1.234=>1.23;  0.2=>.2;  3.0=>3"));
    CreateToolTip(hwnd, IDC_EDIT_DOO_CONV_DIGITS_NUM,       TEXT("ת����Ĵ�������ʹ��ʵ�������С��λ��.\n��:��ֵΪ2ʱ,1.234=>1.23;  0.2=>.2;  3.0=>3."));
    CreateToolTip(hwnd, IDC_STATIC_DOO_MARK,                TEXT("��j�ű��в����־�ַ�(������ڴ����п�����CustomScript����),ת����Ĵ��뽫�ڲ�����滻���ַ���.�����Ӷ����־�ַ�,����Զ�������ƽ�ֵ�����λ��.\n���齫�������������Ϸ��ʼ֮��."));
    CreateToolTip(hwnd, IDC_EDIT_DOO_MARK_STRING,           TEXT("��j�ű��в����־�ַ�(������ڴ����п�����CustomScript����),ת����Ĵ��뽫�ڲ�����滻���ַ���.�����Ӷ����־�ַ�,����Զ�������ƽ�ֵ�����λ��.\n���齫�������������Ϸ��ʼ֮��."));
    CreateToolTip(hwnd, IDC_CHECK_WTS,                      TEXT("war3��ͼ�õ��Ĵ󲿷��ַ�����������war3map.wts�ļ���,����ʵ�����Ǹ��ܲ��õ��趨,��wts�ļ��м�¼���ַ�����ԭ�������ļ�,���Լ�С��ͼ��������������ٶ�."));
    CreateToolTip(hwnd, IDC_CHECK12,                        TEXT("ʹ��Wc3MapOptimizer�Ż�J�ļ�."));

    CreateToolTip(hwnd, IDC_CHECK4,                         TEXT("ɾ����WE��ʹ��,����Ϸ�в���Ҫ���ļ�,ʹ��ͼ��ø�С,��ͬʱ�㽫�޷���WE�����༭�õ�ͼ."));
    CreateToolTip(hwnd, IDC_CHECK5,                         TEXT("����listfile�ļ�����ʹ����MPQ�����ܹ���ȡ��ͼ�е������ļ�,��Ȼɾ�����ļ�Ҳ������Ӱ�쵽��Ϸ."));
    CreateToolTip(hwnd, IDC_CHECK11,                        TEXT("���blpͷ�ļ��ж�����ֽ�,��ɾ��ͼ���Loadingͼ�ж����mipMapͼƬ.\n��Ӱ��ͼƬƷ��."));
    CreateToolTip(hwnd, IDC_COMBO10,                        TEXT("mip1:�Ż��������С,����ͼ������-��������Ϊ��ʱ,����ʾ������.\nmip2:Ĭ��ѡ��,��������������ʱ�����mipMapͼƬ.\nmip16:��ɾ��mipMapͼƬ."));
    CreateToolTip(hwnd, IDC_CHECK10,                        TEXT("ѹ��Mdxģ���ļ���С,��С���ڵ�ͼ��ռ�����,�ù���Ϊ����ѹ��,���������ڶ��ѹ������ģ��Ʒ�ʽ���."));

    CreateToolTip(hwnd, IDC_STATIC_EXLISTFILE,              TEXT("������Ż����Ǽ��ܵ�ͼ,������Ҫ�����ⲿlistfile�ļ���������MPQ����ȡ�ļ��б�(���ﲢ����֤�������Ż����ܵ�ͼ)"));
    CreateToolTip(hwnd, IDC_BUTTON_LIST_FILE,               TEXT("������Ż����Ǽ��ܵ�ͼ,������Ҫ�����ⲿlistfile�ļ���������MPQ����ȡ�ļ��б�(���ﲢ����֤�������Ż����ܵ�ͼ)"));

    CreateToolTip(hwnd, IDC_BUTTON_INPUT_FILE,              TEXT("��ħ��mpq�ļ��ж�ȡ����������Ϊinput�ļ�."));
    CreateToolTip(hwnd, IDC_CHECK_WAV_AUTO_BLOCKSIZE,       TEXT("��ͼ����Wav�ļ����ڵ�ʱ��,ʹ�ø�ѡ���Ƚϲ�ѡ�����ѹ����ʽ,\n����û��Wav�ļ��ĵ�ͼ,��ѡ�������."));
    CreateToolTip(hwnd, IDC_CHECK_ONLY_FOR_COMPRESS,        TEXT("��ѹ����ͼ���������Ż�."));
    CreateToolTip(hwnd, IDC_BUTTON_OPEN,                    TEXT("ѡ����Ҫ�Ż��ĵ�ͼ,��Ҳ����ֱ���϶��ļ����������."));
    CreateToolTip(hwnd, IDC_BUTTON_OPT,                     TEXT("�Ż���ѡ��ͼ,�Ż���ĵ�ͼ����ԭ��ͼ��ǰ����new_ǰ׺."));
}

VOID InitComboBox(HWND hwnd)
{
    HWND Contorl;

    Contorl = GetDlgItem(hwnd, IDC_COMBO_UNIT);
    ComboBox_AddString(Contorl, TEXT("���Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�����"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_ITEM);
    ComboBox_AddString(Contorl, TEXT("���Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�����"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_DEST);
    ComboBox_AddString(Contorl, TEXT("���Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�����"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_DOOD);
    ComboBox_AddString(Contorl, TEXT("���Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�����"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_ABIL);
    ComboBox_AddString(Contorl, TEXT("���Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�����"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_BUFF);
    ComboBox_AddString(Contorl, TEXT("���Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�����"));

    Contorl = GetDlgItem(hwnd, IDC_COMBO_UPGR);
    ComboBox_AddString(Contorl, TEXT("���Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�"));
    ComboBox_AddString(Contorl, TEXT("�Ż�����"));

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
        Button_SetText(GetDlgItem(hwnd, IDC_BUTTON_LIST_FILE), TEXT("ʹ��"));

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

    if (access("Input",0) == -1)	//������Input�ļ���
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
            MessageBox(hwnd, TEXT("War3ע�����Ϣ����,���ֶ���ȡ�����ļ�!"), TEXT("Wc3MapMax++"), 0);
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
    ofn.lpstrFilter     = TEXT("ħ�����Ե�ͼ�ļ�\0*.w3x;*.w3m\0");  
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
            MessageBox(hwnd, TEXT("�����ļ���ȡ��ɣ�"), TEXT("Wc3MapMax++"), 0);
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
    ofn.lpstrFilter     = TEXT("listfile�ļ�\0*.txt;(listfile)\0");  
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
        MessageBox(hwnd, TEXT("�Ż����!"), TEXT("Wc3MapMax++"), 0);
        DestroyWindow(hwnd);
        break;
    case ERR_FILE_NOT_FOUND:
        MessageBox(hwnd, TEXT("ָ���ļ������ڻ��޷���!"), TEXT("Wc3MapMax++"), 0);
        break;
    case ERR_JSCRIPT_LOST:
        MessageBox(hwnd, TEXT("��ͼ�ű��ļ���ʧ!"), TEXT("Wc3MapMax++"), 0);
        break;
    default:
        MessageBox(hwnd, TEXT("��ͼת��ʧ��!"), TEXT("Wc3MapMax++"), 0);
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
            Button_SetText(GetDlgItem(hwnd, IDC_BUTTON_LIST_FILE), TEXT("δ����"));
        }
        else
        {
            Dlg_OpenListFile(hwnd);
            Button_SetText(GetDlgItem(hwnd, IDC_BUTTON_LIST_FILE), TEXT("������"));
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
                if (IDYES == MessageBox(hwnd, TEXT("�Ƿ��SLK�ļ�(��Ϊ�����ļ�)?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
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
                if (IDYES == MessageBox(hwnd, TEXT("�Ƿ��txt�ļ�(��Ϊ�����ļ�)?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
                {
                    txt=1;
                    if (IDYES == MessageBox(hwnd, TEXT("�򻯺��txt�ļ�ʹ��Melee����?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
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
                if (IDYES == MessageBox(hwnd, TEXT("�Ƿ��Ż�blp�ļ�?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
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
                if (IDYES == MessageBox(hwnd, TEXT("�Ƿ�ѹ��mdx�ļ�?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
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
                if (IDYES == MessageBox(hwnd, TEXT("�Ƿ�����J�ű��ļ�?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
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
            if (IDYES == MessageBox(hwnd, TEXT("�Ƿ��Mpq�ж�ȡ�����ļ�?"), TEXT("Wc3MapMax++"), MB_YESNO|MB_ICONQUESTION))
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

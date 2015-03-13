#include "stdafx.h"
#include "Include.h"

#include "MainGUI.h"

BOOL ReadInput(HWND hwnd);
VOID GetDestPath(LPSTR lpszDestPath, size_t cchMax, LPSTR lpszPath);

UINT32 Cmd_OptMap(LPSTR lpszFile)
{
    if (!ReadInput(NULL))
        return 0;

    ReadCFG();
    ExInit();

    CHAR szMapPath[MAX_PATH];
    CHAR szDestMapPath[MAX_PATH];

    StringCchCopyA(szMapPath, MAX_PATH, lpszFile);
    GetDestPath(szDestMapPath, MAX_PATH, lpszFile);

    ListMapFile(&szMapPath[0]);
    return RunOptimizer(szMapPath, szDestMapPath);
}

BOOL Cmd_Main(PSTR szCmdLine, PUINT32 pnRet)
{
    if (szCmdLine != 0 && szCmdLine[0] != 0)
    {
        *pnRet = Cmd_OptMap(szCmdLine);
        return TRUE;
    }

    return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    getcwd(szAppDir, MAX_PATH);
    chdir(szAppDir);

    UINT32 nRet;
    if (Cmd_Main(szCmdLine, &nRet))
        return nRet;

    Dialog_Display(hInstance);

    for (;;)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
        {
            if (msg.message == WM_QUIT) 
            {
                goto _quit;
            }
            else 
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }
_quit:
    return 0;
}

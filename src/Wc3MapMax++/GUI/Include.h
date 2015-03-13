#ifndef INCLUDE_INCLUDE_H
#define INCLUDE_INCLUDE_H

#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h> 

#include <Shlwapi.h>
#include <StrSafe.h>
#include <commctrl.h>

#pragma comment(lib, "Shlwapi.lib")

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define HANDLE_DLGMSG(hWnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hWnd, uMsg, HANDLE_##message((hWnd), (wParam), (lParam), (fn))))

#define HANDLE_DLGMSG_SYSCOMMAND(hWnd, fn)                                 \
   case WM_SYSCOMMAND:                                                     \
    {                                                                      \
      UINT result = (UINT)(fn)((hwnd),(UINT)(wParam),                      \
        (int)(short)LOWORD(lParam),(int)(short)HIWORD(lParam));            \
      SetWindowLongPtr((hWnd), DWLP_MSGRESULT, (LPARAM)(LRESULT)(result)); \
      return result;                                                       \
    }

#define HANDLE_SWM_TRAYMSG(hwnd, wParam, lParam, fn) \
    ((fn)((hwnd), (UINT)(wParam), (UINT)(lParam)))

#define SETDLGICONS(hWnd, idi)                                    \
  SendMessage((hWnd), WM_SETICON, ICON_BIG,   (LPARAM)            \
    LoadIcon((HINSTANCE)GetWindowLongPtr((hWnd), GWLP_HINSTANCE), \
    MAKEINTRESOURCE(idi)));                                       \
  SendMessage((hWnd), WM_SETICON, ICON_SMALL, (LPARAM)            \
    LoadIcon((HINSTANCE)GetWindowLongPtr((hWnd), GWLP_HINSTANCE), \
    MAKEINTRESOURCE(idi)));

#endif
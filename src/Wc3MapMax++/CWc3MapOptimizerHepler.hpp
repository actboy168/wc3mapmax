#pragma once

#include <fstream>
using namespace std;

class CWc3MapOptimizerHepler
{
public:
  CWc3MapOptimizerHepler(HANDLE hFile, char** pszBuf, size_t* pSize, LPTSTR lptzVersion)
  {
    size_t nSize = SFileGetFileInfo(hFile, SFILE_INFO_SIZE);
    char* szBuf = new char[nSize + 8];

    SFileReadFile(hFile, szBuf, nSize, NULL, NULL);

    ofstream outfile(".\\bin\\input.j", ios::binary);
    if (!outfile) return ;
    outfile.write(szBuf, nSize);
    outfile.close();
    delete [] szBuf;

    ::DeleteFile(TEXT(".\\bin\\output.j"));

    RunWc3MapOptimizer(lptzVersion);

    ifstream infile(".\\bin\\output.j", ios::binary);
    if (!infile) return ;
    infile.seekg(0, std::ios::end);
    *pSize = (size_t)infile.tellg();
    infile.seekg(0, std::ios::beg);

    *pszBuf = (char*)malloc(((*pSize)+8)*sizeof(char));
    infile.read(*pszBuf, *pSize);
    infile.close();
  }
private:
  void RunWc3MapOptimizer(LPTSTR lptzVersion)
  {
    static TCHAR s_tzParameters[] = TEXT(".\\bin\\input.j --do .\\bin\\output.j  --checkscriptstuff --tweak .\\bin\\XXX.vxtweak --exit");
    size_t nPos = sizeof(TEXT(".\\bin\\input.j --do .\\bin\\output.j  --checkscriptstuff --tweak .\\bin\\"))/sizeof(TCHAR) - 1;
    memcpy(&s_tzParameters[nPos], lptzVersion, 3*sizeof(TCHAR));

    SHELLEXECUTEINFO ShExecInfo = {0};
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = TEXT(".\\bin\\VXJWTSOPT.exe");
    ShExecInfo.lpParameters = s_tzParameters;
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_HIDE;
    ShExecInfo.hInstApp = NULL;
    ShellExecuteEx(&ShExecInfo);
    ::WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
  }
};

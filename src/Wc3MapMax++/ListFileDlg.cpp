// ListFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Wc3MapMax++.h"
#include "ListFileDlg.h"


// ListFileDlg 对话框

IMPLEMENT_DYNAMIC(ListFileDlg, CDialog)

ListFileDlg::ListFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ListFileDlg::IDD, pParent)
{
}

ListFileDlg::~ListFileDlg()
{
}

void ListFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_LIST, button_searchFile);
}


BEGIN_MESSAGE_MAP(ListFileDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LIST, &ListFileDlg::OnBnClickedButtonList)
	ON_BN_CLICKED(IDC_CHECK_LIST, &ListFileDlg::OnBnClickedCheckList)
	ON_BN_CLICKED(IDCANCEL, &ListFileDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// ListFileDlg 消息处理程序
extern char* listFilePath;
extern char searchForMapFile;

BOOL ListFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	button_searchFile.SetCheck(searchForMapFile);
	if (listFilePath!=NULL && *listFilePath!=NULL)
		SetDlgItemText(IDC_BUTTON_LIST,L"外部listfile: 已设置");
	return true;
}

void ListFileDlg::OnBnClickedButtonList()
{
	if (listFilePath!=NULL && *listFilePath!=NULL)
	{
		free(listFilePath);
		listFilePath="";
		SetDlgItemText(IDC_BUTTON_LIST,L"外部listfile: 未设置");
	}
	else
	{
		CFileDialog dlgFileOpen(true,L"",L"",0,L"文件|*.*|",this);
		DWORD dwVersion = GetVersion();
		if (dwVersion < 0x80000000)
			dlgFileOpen.m_ofn.lStructSize =88;
		else
			dlgFileOpen.m_ofn.lStructSize =76;
		if(dlgFileOpen.DoModal()==IDOK)
		{
			if (listFilePath!=NULL && listFilePath!="")
				free(listFilePath);
            USES_CONVERSION;
			char* tempStr=W2A(dlgFileOpen.m_ofn.lpstrFile);
            listFilePath=(char*)malloc(strlen(tempStr)+1);
            strcpy(listFilePath,tempStr);
			SetDlgItemText(IDC_BUTTON_LIST,L"外部listfile: 已设置");
		}
	}
}

void ListFileDlg::OnBnClickedCheckList()
{
	searchForMapFile = button_searchFile.GetCheck();
}

void ListFileDlg::OnBnClickedCancel()
{
	OnCancel();
}

// ListFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Wc3MapMax++.h"
#include "ListFileDlg.h"


// ListFileDlg �Ի���

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


// ListFileDlg ��Ϣ�������
extern char* listFilePath;
extern char searchForMapFile;

BOOL ListFileDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	button_searchFile.SetCheck(searchForMapFile);
	if (listFilePath!=NULL && *listFilePath!=NULL)
		SetDlgItemText(IDC_BUTTON_LIST,L"�ⲿlistfile: ������");
	return true;
}

void ListFileDlg::OnBnClickedButtonList()
{
	if (listFilePath!=NULL && *listFilePath!=NULL)
	{
		free(listFilePath);
		listFilePath="";
		SetDlgItemText(IDC_BUTTON_LIST,L"�ⲿlistfile: δ����");
	}
	else
	{
		CFileDialog dlgFileOpen(true,L"",L"",0,L"�ļ�|*.*|",this);
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
			SetDlgItemText(IDC_BUTTON_LIST,L"�ⲿlistfile: ������");
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

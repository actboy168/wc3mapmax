#pragma once
#include "afxwin.h"


// ListFileDlg �Ի���

class ListFileDlg : public CDialog
{
	DECLARE_DYNAMIC(ListFileDlg)

public:
	ListFileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ListFileDlg();

// �Ի�������
	enum { IDD = IDD_LIST_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonList();
	CButton button_searchFile;
	afx_msg void OnBnClickedCheckList();
	afx_msg void OnBnClickedCancel();
};

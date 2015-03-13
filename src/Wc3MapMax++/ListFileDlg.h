#pragma once
#include "afxwin.h"


// ListFileDlg 对话框

class ListFileDlg : public CDialog
{
	DECLARE_DYNAMIC(ListFileDlg)

public:
	ListFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ListFileDlg();

// 对话框数据
	enum { IDD = IDD_LIST_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonList();
	CButton button_searchFile;
	afx_msg void OnBnClickedCheckList();
	afx_msg void OnBnClickedCancel();
};

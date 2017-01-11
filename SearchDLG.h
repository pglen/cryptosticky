#pragma once

#include "xbutton.h"
#include "afxwin.h"


// SearchDLG dialog

class SearchDLG : public CDialog
{
	DECLARE_DYNAMIC(SearchDLG)

public:

	SearchDLG(CWnd* pParent = NULL);   // standard constructor
	virtual ~SearchDLG();

	void Iterate(HTREEITEM currentItem, const TCHAR *str);

	int	m_BackColor;
	int stop, in_src, found;

// Dialog Data
	enum { IDD = IDD_DIALOG19 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk4();
	afx_msg void OnLbnSelchangeList2();
	afx_msg void OnBnClickedCheck1();
	
	int Search(void);
	CEdit m_search;
	CListBox m_list;
	CXButton m_ok;
	CXButton m_exit;
	
	virtual BOOL OnInitDialog();
	CButton m_body;
	CXButton m_hide;
	afx_msg void OnBnClickedOk3();
	CEdit m_prog;
};

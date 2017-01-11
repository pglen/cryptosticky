#if !defined(AFX_LOGDLG_H__CDF207E3_7DF9_4957_9BA7_ABA844CF8A8B__INCLUDED_)
#define AFX_LOGDLG_H__CDF207E3_7DF9_4957_9BA7_ABA844CF8A8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogDlg.h : header file
//

#include "Xbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog

class CLogDlg : public CDialog
{
// Construction
public:

	CLogDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLogDlg)
	enum { IDD = IDD_DIALOG31 };
	CXButton	m_ok4;
	CXButton	m_ok;
	//}}AFX_DATA

	int m_BackColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnOk4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGDLG_H__CDF207E3_7DF9_4957_9BA7_ABA844CF8A8B__INCLUDED_)

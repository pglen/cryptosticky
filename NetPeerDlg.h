#if !defined(AFX_NETPEERDLG_H__D1ACE8CD_4707_47D1_831B_9A35F0452FD4__INCLUDED_)
#define AFX_NETPEERDLG_H__D1ACE8CD_4707_47D1_831B_9A35F0452FD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetPeerDlg.h : header file
//

#include "ListVwEx.h"
#include "xButton.h"
#include "SimpleGrid.h"
#include "label.h"

/////////////////////////////////////////////////////////////////////////////
// CNetPeerDlg dialog

class CNetPeerDlg : public CDialog

{

	static CString pro;

// Construction
public:
	void OnOK();
	CNetPeerDlg(CWnd* pParent = NULL);   // standard constructor

	int m_BackColor;
	
	CImageList m_StateImageList;

	void CNetPeerDlg::Fill();

// Dialog Data
	//{{AFX_DATA(CNetPeerDlg)
	enum { IDD = IDD_DIALOG18 };
	CSimpleGrid	m_grid;
	CXButton	m_b4;
	CXButton	m_b3;	
	CXButton	m_b2;
	CXButton	m_b1;
	CXButton	m_ok;
	//CListCtrl	m_list2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetPeerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int oldhelp;

	// Generated message map functions
	//{{AFX_MSG(CNetPeerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CLabel m_msg;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETPEERDLG_H__D1ACE8CD_4707_47D1_831B_9A35F0452FD4__INCLUDED_)

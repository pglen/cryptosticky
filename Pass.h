#if !defined(AFX_PASS_H__EC8A26E4_9E53_4CFA_A884_6426C86934F0__INCLUDED_)
#define AFX_PASS_H__EC8A26E4_9E53_4CFA_A884_6426C86934F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Pass.h : header file
//

#include "xButton.h"

/////////////////////////////////////////////////////////////////////////////
// CPass dialog

class CPass : public CDialog
{
// Construction
public:
	CPass(CWnd* pParent = NULL);   // standard constructor

	CString pass;
	CString title;
	int		m_BackColor, astep;

// Dialog Data
	//{{AFX_DATA(CPass)
	enum { IDD = IDD_DIALOG15 };
	CLabel	m_info2;
	CLabel	m_info;
	CXButton	m_ok;
	CXButton	m_can;
	//}}AFX_DATA

	CBitmap m_Bitmap;
	BITMAP BitMap;
	int	m_nWidth, m_nHeight;

	CPass::~CPass();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	int vpos, hpos, alpha;

	// Generated message map functions
	//{{AFX_MSG(CPass)
	virtual void OnOK();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASS_H__EC8A26E4_9E53_4CFA_A884_6426C86934F0__INCLUDED_)

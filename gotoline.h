#if !defined(AFX_GOTOLINE_H__45A56F45_FD70_11D2_B398_525400D994D7__INCLUDED_)
#define AFX_GOTOLINE_H__45A56F45_FD70_11D2_B398_525400D994D7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// gotoline.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Cgotoline dialog

class Cgotoline : public CDialog
{
// Construction
public:
	CString caption;
	Cgotoline(CWnd* pParent = NULL);   // standard constructor

	CBitmap  m_bitmap;			// Splash Bitmap
	int m_nWidth, m_nHeight;	// Splash Bitmap's width & height

// Dialog Data
	//{{AFX_DATA(Cgotoline)
	enum { IDD = IDD_DIALOG7 };
	CLabel	m_memo;
	CLabel	m_title;
	CString	m_sticky;
	CString	m_str;
	//}}AFX_DATA
	int m_esc;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cgotoline)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Cgotoline)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOTOLINE_H__45A56F45_FD70_11D2_B398_525400D994D7__INCLUDED_)

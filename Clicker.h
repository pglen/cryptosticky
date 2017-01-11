#if !defined(AFX_CLICKER_H__51C64433_3B5A_483D_A184_7367B921AEC8__INCLUDED_)
#define AFX_CLICKER_H__51C64433_3B5A_483D_A184_7367B921AEC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Clicker.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClicker window

class CClicker : public CStatic
{
// Construction
public:
	CClicker();
	int inmouse;
// Attributes
public:

	CWnd *parent;

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClicker)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClicker();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClicker)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLICKER_H__51C64433_3B5A_483D_A184_7367B921AEC8__INCLUDED_)

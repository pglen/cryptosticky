
/* =====[ FakeWin.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the FakeWin.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_FAKEWIN_H__7DD1BCF5_4C38_4060_855C_1596E4362620__INCLUDED_)
#define AFX_FAKEWIN_H__7DD1BCF5_4C38_4060_855C_1596E4362620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FakeWin.h : header file


/////////////////////////////////////////////////////////////////////////////
// FakeWin dialog

class FakeWin : public CDialog
{
// Construction
public:
	void	*cocodlg;

	FakeWin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(FakeWin)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FakeWin)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FakeWin)
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChildActivate();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAKEWIN_H__7DD1BCF5_4C38_4060_855C_1596E4362620__INCLUDED_)

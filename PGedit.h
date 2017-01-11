
/* =====[ PGedit.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the 
					PGedit.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_PGEDIT_H__D65965D6_7BE0_456C_B4E7_9C40E632D0E0__INCLUDED_)
#define AFX_PGEDIT_H__D65965D6_7BE0_456C_B4E7_9C40E632D0E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PGedit.h : header file
//

#define MAX_READ 128

/////////////////////////////////////////////////////////////////////////////
// CPGedit window

class CPGedit : public CEdit

{
public:

	static	int	dic_good;

	static	FILE	*fp1, *fp3;
	static	CDWordArray     widx;
	
	int		cnt, in_spell, oldline, m_Drawn, checkon;
	int		cryptver;

	CString currword;
	CStringArray replace;
	
	void	AddWordToDic();
	void	IgnoreWord();
	void	MoreSuggest();
	void	ReplaceSpellingError(UINT uid);

	TCHAR    buffer[MAX_READ];
    
	CStringList         ignore;
	CStringList         learn;
	
	CString dicmain, dicidx, dicuser;
	void	Suggest(CMenu &menu, CString str);


	UINT	m_uiTimerID;
	DWORD	m_dwLastTick;
	
	void	Init_Spellcheck();
	int		SpellWord(CString &str);

	int		ProcessLine(int line, CString str, CRect &rect, CDC &dc);
	void	ProcessWord(CString &word, int pos, CDC &dc);

	void	AttachEdit(CWnd *pWnd, UINT uiControlID);
	void	DrawSquigly(CDC &dc, int xx, int yy, int iWidth);

// Construction
public:
	CPGedit();
	virtual ~CPGedit();


// Operations

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPGedit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SelAll();
	void Encrypt();
	void Decrypt();

	void Encrypt2();
	void Decrypt2();

	void EmptyClip();

	void CheckLineChange();
	void GetLine(int idx, CString &str);
	
	int		indict(CString &str);
	int		firsttime;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CPGedit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChange();
	afx_msg void OnVscroll();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PGEDIT_H__D65965D6_7BE0_456C_B4E7_9C40E632D0E0__INCLUDED_)

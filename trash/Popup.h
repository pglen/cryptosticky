
/* =====[ Popup.h ]========================================== 
                                                                             
   Description:     The yellowmemo project, implementation of the Popup.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_POPUP_H__6CDEC48B_CA86_4255_9FAF_568533840B37__INCLUDED_)
#define AFX_POPUP_H__6CDEC48B_CA86_4255_9FAF_568533840B37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Popup.h : header file
//
//include "label.h"

/////////////////////////////////////////////////////////////////////////////
// CPopup dialog

class CPopup : public CDialog
{

// Construction
public:
	void SetBackColor(COLORREF rgb);

	void Move(CPoint screen);
	void SetText(const char *str);
	CPopup(CWnd* pParent = NULL);   // standard constructor

	int padding;

protected:

	//RGB(0xff, 0xff, 0x80)) ;

	COLORREF bgc;
	CString str;


	void AutoSize();

	CFont	m_font;
	LOGFONT		m_lf;

// Dialog Data
	//{{AFX_DATA(CPopup)
	enum { IDD = IDD_DIALOG13 };
	//CLabel	m_str;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPopup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_POPUP_H__6CDEC48B_CA86_4255_9FAF_568533840B37__INCLUDED_)

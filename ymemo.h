
/* =====[ ymemo.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the ymemo.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/23/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_STICKY_H__CAC47F92_6792_4DA2_807F_AF5225965BDA__INCLUDED_)
#define AFX_STICKY_H__CAC47F92_6792_4DA2_807F_AF5225965BDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//

#include "popup.h"
#include "transdlg.h"
#include "playsound.h"
#include "picture.h"

#define CYmemo_Magic	0xeeff3344

/////////////////////////////////////////////////////////////////////////////
// CYmemo dialog

class CYmemo : public CTransDlg

{
public:
	
	void	StartFadeCycle();
	void	Refresh();
	void	OnShowMain();
	void	ShowAll();
	void	HideAll();
	void	Edit();
	void	MyKeyDown(UINT wParam, UINT lParam);
	void	MySysKeyDown(UINT wParam, UINT lParam);
	void	Hide();	
	void	Hide2();	
	void	Show();
	void	UpdateScroll();
	void	LoadContent(int refresh = false);
	int		SaveContent();
	void	GetParms();
	
	CPicture backimg;
	int		alpha;

	CPoint	prevmouse;
	DWORD	lasthide; 

	CString	memofile, title, backimage; 

	void	SavePos();

// Construction

	CYmemo(CWnd* pParent = NULL);   // standard constructor
	~CYmemo();

// Vars

	int		GetHeadHeight();
	int		timeout;
	int		fresh, draw, headsize, textsize;
	int		headcol, textcol, headrcol, bodyrcol;
	int		magic, iswave;
	int		penwidth, pencol;
	int		tmptop, alshow;
	int		wantauto;

	//int refresh;

	int		maxcomm, maxcomm2;

	CFont	myhead_font, myhead_fontb, mymemo_font;

	void	AutoSize();
	void	NextMemo();
	
	MemoSupp	memo;

	int		memo_fontcol2, head_fontcol2;

	RECT	rchead;

protected:

	void	LoadDrawing(const TCHAR *drawname);
	void	unrgb(COLORREF col, BYTE *rr, BYTE *gg, BYTE *bb);

	CPlaySound	playsound;

	void	PopMenu();
	void	DelayedInval();

	CDWordArray gesture;

	CString		m_edit;
	CString		wavname, drawname;
	CString		flag, centerflag;
	CString		old_tip;
	
	int		fired, erase;
	int		reenter, modal, idlecount;
	int		sbcap, sizing;	
	int		vscroll, center, xpad, ypad;
	int		scline, linecount, mousein;
	int		moved, capped, errhide;
	int		fadetimer;

	DWORD tipped;
		
	CPoint	mouse, capmouse, sbpoint;
		
	RECT	closerect;
	RECT	copyrect;
	RECT	urlrect;

	RECT	tsrect;
	RECT	bsrect;
	RECT	rxrect;
	RECT	wavrect;
	RECT	sbutt;

	CRect	htxtrect;
	CPopup	tip;
		
// Dialog Data
	//{{AFX_DATA(CYmemo)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYmemo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CYmemo)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnEncrypt();
	afx_msg void OnDecrypt();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void CopyToClip();
	afx_msg void CreateNew();
	afx_msg void OnCloseMain();
	afx_msg void OnHideMain();
	afx_msg void OnConfig();
	afx_msg void OnMove(int x, int y);
	afx_msg void PasteClip();
	afx_msg void Eraser();
	afx_msg void EraseDrawing();
	afx_msg void NewMemo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRclickmemoAppendfromclipboard();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STICKY_H__CAC47F92_6792_4DA2_807F_AF5225965BDA__INCLUDED_)

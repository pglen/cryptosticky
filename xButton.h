
/* =====[ xButton.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the xButton.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_MYTGTBTN_H)
#define AFX_MYTGTBTN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CXButton.h : header file
/////////////////////////////////////////////////////////////////////////////
#define WM_CXTGT_RADIO WM_USER+0x100

class CXButton : public CButton

{
// Construction
public:
	CXButton();
// Attributes
private:
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
// Implementation
public:
	void SetHighColor(COLORREF col);
	void SetBackColor(COLORREF col);
	void EnableTarget(bool enable);
	LOGFONT* GetFont();
	bool SetFont(LOGFONT* pNewStyle);
	bool SetFont(CString sFontName, long lSize=0, long lWeight=400, BYTE bItalic=0, BYTE bUnderline=0);
	void SetButtonStyle( UINT nStyle, BOOL bRedraw = TRUE );
	void SetFlat(bool bFlag);
	void SetTextAlign(UINT nTextAlign);
	void SetIcon(UINT nIcon, UINT nIconAlign=BS_CENTER, UINT nIconDown=0, UINT nIconHighLight=0);
	void SetToolTipText(CString s, CString sDown="");
	COLORREF SetTextColor(COLORREF new_color);
	virtual ~CXButton();
	// Generated message map functions
protected:
	void DrawTarget(CDC* pDC, RECT* r, DWORD color, int border, int width, int height, int offset=0);
	void DrawFocus(CDC *pDC, RECT *r, DWORD color, int border, int width, int height, int offset = 0);
	CFont	m_Font;		//font object
	LOGFONT* m_pLF;		//font structure
	CString m_ToolTipUp,m_ToolTipDw;
	bool	m_Checked;		//radio & check buttons
	bool	m_IsPushLike;	//radio & check buttons
	DWORD	m_Style;
    bool	m_tracking;
    bool	m_button_down;
	void	RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
	CToolTipCtrl m_tooltip;
	short	m_FocusRectMargin;	//dotted margin offset
	COLORREF m_TextColor;		//button text color
	BOOL	m_Border;			//0=flat; 1=3D;
	HICON	m_Icon,m_IconDown,m_IconHighLight;
	CRect	m_IconBox;
	UINT	m_TextAlign;
	UINT	m_IconAlign;
	bool	m_flat;
	bool	m_target;
	COLORREF m_backcolor;
	COLORREF m_highcolor;

//	afx_msg BOOL OnClicked();
	//{{AFX_MSG(CXButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnClicked();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	afx_msg LRESULT OnRadioInfo(WPARAM, LPARAM);
	afx_msg LRESULT OnBMSetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnBMGetCheck(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
	//afx_msg BOOL OnClicked();

};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_MYTGTBTN_H)

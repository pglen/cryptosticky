
/* =====[ TransDlg.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the TransDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_TRANSDLG_H__121743DB_6009_4762_ACB9_6F38424804E5__INCLUDED_)
#define AFX_TRANSDLG_H__121743DB_6009_4762_ACB9_6F38424804E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransDlg.h : header file
//

void	DarkenThread(void *lpParam);

/////////////////////////////////////////////////////////////////////////////
// CTransDlg dialog

class CTransDlg : public CDialog
{ 

	DECLARE_DYNAMIC(CTransDlg)
// Construction
public:
	void Create();
	int shaded;
	void SetTransparency(int tran);
	void OnCancel();
	void OnOK();
	int inited;
	int forcenew;
	int created;
	
	void Hide();
	void Show(CWnd* pParentWnd);
	void Show();

	CTransDlg(int ID2, CWnd* pParent = NULL);   // standard constructor

	BYTE	bAlpha;
	int		autoposition;
	
	CWnd	*Parent;

	//void	*docptr;
	//CxImage* bgImage;
	
	int		drag, drag_x, drag_y;
	int		update_wanted ;
	int		top;
	HICON	m_hIcon;
	
	HBITMAP	hBmp;			// Darkened 
	HBITMAP	hBmp2;			// Original

	HANDLE	hthread;
	int		id2;

	int		reenter;
	int		showstate; 
	int		focus;

	int		old_x, old_y;

	void	GetBG(HBITMAP bmp = 0);
	int		SetWstate(int cmd);
	int		GetWstate();

	void	Darken();
	void	GetNewBG();
	void	put_bg(CDC * pDC);

	lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributesx;

// Dialog Data
	//{{AFX_DATA(CTransDlg)
	//enum { IDD = IDD_CONTRAST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransDlg)
	public:
	virtual BOOL Create(int idd, CWnd *wnd = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransDlg)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSDLG_H__121743DB_6009_4762_ACB9_6F38424804E5__INCLUDED_)

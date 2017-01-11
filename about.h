
/* =====[ about.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the about.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#include "xbutton.h"
#include "GrayDlg.h"
#include "label.h"
#include "afxwin.h"

class CAboutDlg : public CGrayDlg

{

public:
	CAboutDlg();

	int		m_BackColor;

	CBitmap m_Bitmap; BITMAP BitMap;
	int	m_nWidth, m_nHeight;

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CLabel		m_ym2;
	CXButton	m_b3;
	CXButton	m_but2;
	CLabel		m_reg2;
	CLabel		m_reglabel;
	CXButton	m_but1;
	CXButton	m_reg;
	CLabel		m_ym;
	CXButton	m_ok;
	CLabel		m_ym0;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnReg();
	afx_msg void OnButton1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CLabel m_blue;
	CLabel m_copyr;
	afx_msg void OnStnClickedYmem2();
	
};


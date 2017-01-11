
/* =====[ Setup.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Setup.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_SETUP_H__8E631C09_57EA_47FF_8A7D_6B0744505500__INCLUDED_)
#define AFX_SETUP_H__8E631C09_57EA_47FF_8A7D_6B0744505500__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setup.h : header file
//

#include "xbutton.h"
#include "xedit.h"
#include "colrect.h"
#include "alarm.h"
#include "transslider.h"
#include "afxcmn.h"
#include "label.h"
#include "afxwin.h"

#include "TransCheckBox.h"

/////////////////////////////////////////////////////////////////////////////
// CSetup dialog

class CSetup : public CDialog
{
// Construction
public:

	CString tran;

	//CAlarm *al;

	CString snd;
	int modified;

	CSetup(CWnd* pParent = NULL);   // standard constructor

	int	m_BackColor;

	void	OnOK();
	int		want_refresh;

// Dialog Data
	//{{AFX_DATA(CSetup)
	enum { IDD = IDD_DIALOG8 };
	CEdit	m_edit4;
	CSpinButtonCtrl	m_spin3;
	CXButton	m_b37;
	CXButton	m_b36;
	CXButton	m_b33;
	CXButton	m_b32;
	CXButton	m_b31;
	CXButton	m_b30;
	CSpinButtonCtrl	m_spin1;
	CXButton	m_b26;
	CXButton	m_b18;
	CXButton	m_b29;
	CXButton	m_b28;
	CXButton	m_ok2;
	CSliderCtrl		m_slide5;
	CSliderCtrl		m_slide6;   // CTransSlider
	CSliderCtrl		m_slide4;
	CSliderCtrl		m_slide3;
	CSliderCtrl		m_slide2;
	CSliderCtrl		m_slide1;
	CXButton	m_but4;
	CXButton	m_b27;
	CColRect	m_r8;
	CColRect	m_r7;
	CXButton	m_b25;
	CColRect	m_r6;
	CColRect	m_r5;
	CXButton	m_b24;
	CXButton	m_b5;
	CXButton	m_b23;
	CXButton	m_b22;
	CXButton	m_b21;
	CXButton	m_b20;
	CXButton	m_b19;
	CXButton	m_b16;
	CXButton	m_b15;
	CXButton	m_b14;
	CXButton	m_b13;
	CXButton	m_b12;
	CXButton	m_b11;
	CXButton	m_b10;
	CColRect	m_r4;
	CColRect	m_r3;
	CColRect	m_r2;
	CColRect	m_r1;
	CXButton	m_ca;
	CXButton	m_ok;
	CXButton	m_b4;
	CXButton	m_b3;
	CXButton	m_b2;
	CXButton	m_b1;
	
	CxEdit		m_lab1;
	CxEdit		m_lab2;
	CxEdit		m_lab3;
	CxEdit		m_lab4;
	CxEdit		m_lab5;
	CxEdit		m_lab6;
	CxEdit		m_lab7;
	CxEdit		m_lab8;
	CxEdit		m_lab9;
	CxEdit		m_lab10;
	CxEdit		m_lab11;
	CxEdit		m_lab12;
	CxEdit		m_lab13;
	CxEdit		m_lab14;
	CxEdit		m_lab15;
	CxEdit		m_lab16;
	CxEdit		m_lab17;
	
	CString	m_userdir;
	CString	m_wav;
	CString	m_refresh;
	CString	m_notify;
	//}}AFX_DATA

	COLORREF	custom[16];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetup)
	afx_msg void OnButton2();
	afx_msg void OnButton4();
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton15();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButton16();
	afx_msg void OnButton19();
	afx_msg void OnButton20();
	afx_msg void OnButton24();
	afx_msg void OnButton25();
	afx_msg void OnButton21();
	afx_msg void OnButton27();
	afx_msg void OnButton23();
	afx_msg void OnButton22();
	afx_msg void OnButton5();
	afx_msg void OnButton12();
	afx_msg void OnButton18();
	afx_msg void OnOk2();
	afx_msg void OnButton28();
	afx_msg void OnButton29();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButton26();
	afx_msg void OnChangeEdit2();
	afx_msg void OnKillfocusEdit2();
	afx_msg void OnCheck1();
	afx_msg void OnButton30();
	afx_msg void OnChangeEdit1();
	afx_msg void OnCheck4();
	afx_msg void OnCheck10();
	afx_msg void OnButton32();
	afx_msg void OnButton33();
	afx_msg void OnButton31();
	afx_msg void OnCheck11();
	afx_msg void OnButton36();
	afx_msg void OnButton37();
	afx_msg void OnCheck14();
	afx_msg void OnCheck15();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedLab16();

	CLabel m_nul4;
	CLabel m_max1;
	CLabel m_nul2;
	CLabel m_max2;
	CLabel m_nul3;
	CLabel m_max3;
	CLabel m_min1;
	CLabel m_max4;
	CLabel m_min2;
	CLabel m_max5;

	//CTransCheckBox m_check5;
	CButton m_check5;


	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheck16();
	afx_msg void OnBnClickedButton39();
	CXButton m_b39;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUP_H__8E631C09_57EA_47FF_8A7D_6B0744505500__INCLUDED_)

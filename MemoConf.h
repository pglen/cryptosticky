#if !defined(AFX_MEMOCONF_H__ADD54C6F_51DE_498F_943E_E5A89C8FADC2__INCLUDED_)
#define AFX_MEMOCONF_H__ADD54C6F_51DE_498F_943E_E5A89C8FADC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemoConf.h : header file
//

#include "xbutton.h"
#include "colrect.h"
#include "YMemo.h"
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////
// CMemoConf dialog

class CMemoConf : public CDialog
{
// Construction
public:
	CMemoConf(CWnd* pParent = NULL);   // standard constructor

	COLORREF	custom[16];
	int			m_BackColor;

// Dialog Data
	//{{AFX_DATA(CMemoConf)
	enum { IDD = IDD_DIALOG16 };
	CComboBox	m_c2;
	CComboBox	m_c1;
	CXButton	m_b12;
	CXButton	m_b11;
	CXButton	m_b10;
	CXButton	m_ok;
	CXButton	m_b4;
	CXButton	m_b3;
	CXButton	m_b2;
	CXButton	m_b1;
	CColRect	m_mc;
	CColRect	m_hc;
	//}}AFX_DATA

	CYmemo	*buddy;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemoConf)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMemoConf)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CXButton m_img;
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CSliderCtrl m_alpha;
	afx_msg void OnBnClickedButton7();
	CColRect m_txtfont;
	CXButton m_fontcol;
	CColRect m_headfont;
	CXButton m_headfontbutt;
	CXButton m_remimg;
	afx_msg void OnBnClickedButton8();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMOCONF_H__ADD54C6F_51DE_498F_943E_E5A89C8FADC2__INCLUDED_)


/* =====[ Register.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Register.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_REGISTER_H__8A7C7669_6BAD_4C11_BABE_3EFA00184855__INCLUDED_)
#define AFX_REGISTER_H__8A7C7669_6BAD_4C11_BABE_3EFA00184855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Register.h : header file
//

#include "Xbutton.h"
#include "label.h"

/////////////////////////////////////////////////////////////////////////////
// CRegister dialog

class CRegister : public CDialog
{
// Construction
public:
	CRegister(CWnd* pParent = NULL);   // standard constructor

	int m_BackColor;
	
// Dialog Data
	//{{AFX_DATA(CRegister)
	enum { IDD = IDD_REGISTER };
	CEdit	m_keyx;
	CXButton	m_ok;
	CXButton	m_cancel;
	CString	m_cust;
	CString	m_name;
	CString	m_key2;
	//}}AFX_DATA

	int retry;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegister)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegister)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CLabel m_fill;
	CLabel m_explain;
	CLabel m_regname;
	CLabel m_regkey;
	afx_msg void OnStnClickedRegexplain();
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGISTER_H__8A7C7669_6BAD_4C11_BABE_3EFA00184855__INCLUDED_)

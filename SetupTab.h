
/* =====[ SetupTab.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the SetupTab.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_SETUPTAB_H__719ED5F8_F730_4F36_90AC_CA0C53D5C5B9__INCLUDED_)
#define AFX_SETUPTAB_H__719ED5F8_F730_4F36_90AC_CA0C53D5C5B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetupTab.h : header file
//

#include "setup.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupTab dialog

class CSetupTab : public CDialog
{
// Construction
public:
	CSetupTab(CWnd* pParent = NULL);   // standard constructor

	CSetup  setup;

// Dialog Data
	//{{AFX_DATA(CSetupTab)
	enum { IDD = IDD_DIALOG10 };
	CTabCtrl	m_tab;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupTab)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupTab)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPTAB_H__719ED5F8_F730_4F36_90AC_CA0C53D5C5B9__INCLUDED_)

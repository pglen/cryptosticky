
/* =====[ ScribEdit.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the ScribEdit.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_SCRIBEDIT_H__C69FEBBC_4EAA_4CC4_82AE_0D529B63610E__INCLUDED_)
#define AFX_SCRIBEDIT_H__C69FEBBC_4EAA_4CC4_82AE_0D529B63610E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScribEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScribEdit window

class CScribEdit : public CEdit
{
// Construction
public:
	CScribEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScribEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScribEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScribEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIBEDIT_H__C69FEBBC_4EAA_4CC4_82AE_0D529B63610E__INCLUDED_)

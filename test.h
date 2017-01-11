#if !defined(AFX_TEST_H__414898FE_1A28_4C69_99F5_3E9368280D9C__INCLUDED_)
#define AFX_TEST_H__414898FE_1A28_4C69_99F5_3E9368280D9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// test.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Ctest dialog

class Ctest : public CDialog
{
// Construction
public:
	Ctest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Ctest)
	enum { IDD = IDD_DIALOG5 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Ctest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Ctest)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_H__414898FE_1A28_4C69_99F5_3E9368280D9C__INCLUDED_)

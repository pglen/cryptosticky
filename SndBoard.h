#if !defined(AFX_SNDBOARD_H__C05C29A2_318E_46D4_B874_93857941F920__INCLUDED_)
#define AFX_SNDBOARD_H__C05C29A2_318E_46D4_B874_93857941F920__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SndBoard.h : header file
//
#include "waveEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CSndBoard dialog

class CSndBoard : public CDialog
{
// Construction
public:
	CSndBoard(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSndBoard)
	enum { IDD = IDD_DIALOG17 };
	CwaveEdit	m_wav;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSndBoard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSndBoard)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SNDBOARD_H__C05C29A2_318E_46D4_B874_93857941F920__INCLUDED_)

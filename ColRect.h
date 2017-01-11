#if !defined(AFX_COLRECT_H__82124FE2_9B04_4F96_8C70_0667E911C7E3__INCLUDED_)
#define AFX_COLRECT_H__82124FE2_9B04_4F96_8C70_0667E911C7E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColRect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColRect window

class CColRect : public CStatic
{
// Construction
public:
	CColRect();

// Attributes
public:

	COLORREF	bgcolor;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColRect)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor(COLORREF cgb);
	virtual ~CColRect();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColRect)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLRECT_H__82124FE2_9B04_4F96_8C70_0667E911C7E3__INCLUDED_)

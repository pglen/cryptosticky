#pragma once
#include "label.h"
#include "GrayDlg.h"
#include "label.h"


// CBanner dialog

class CBanner : public CGrayDlg
{
	DECLARE_DYNAMIC(CBanner)

public:
	CBanner(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBanner();

	
	CBitmap m_Bitmap; BITMAP BitMap;
	int	m_nWidth, m_nHeight;

	int m_BackColor;
// Dialog Data
	enum { IDD = IDD_DIALOG20 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CLabel m_head;
	CLabel m_head2;
	CLabel m_foot;
	CLabel m_foot2;
	CLabel m_shadow;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

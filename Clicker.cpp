// Clicker.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "Clicker.h"
#include "support.h"

//#include "VideoPhoneDlg.h"
//#include "xpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CClicker

CClicker::CClicker()
{
	inmouse = false;
	//func = NULL;
}

CClicker::~CClicker()
{
}

BEGIN_MESSAGE_MAP(CClicker, CStatic)
	//{{AFX_MSG_MAP(CClicker)
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClicker message handlers

void CClicker::OnLButtonDown(UINT nFlags, CPoint point) 

{
	SetCapture();
	inmouse = true;
	Invalidate();
	CStatic::OnLButtonDown(nFlags, point);
	//P2N(_T("Clicker button\r\n"));
}


void CClicker::OnLButtonUp(UINT nFlags, CPoint point) 

{
	inmouse = false;
	//P2N(_T("Clicker: Left button up\r\n"));		
	ReleaseCapture();
	Invalidate();

	//( (CVideoPhDlg*)parent)->DetachAll();

	CStatic::OnLButtonUp(nFlags, point);
}

BOOL CClicker::OnEraseBkgnd(CDC* pDC) 

{
	CRect rec;
	GetClientRect(rec);
	if(inmouse)
		{
		//P2N(_T("Invalidating with inmouse\r\n"));
		pDC->FillSolidRect(rec, RGB(50,50,50));
		support.YieldToWinEx();
		return true;
		}
	else
		{
		//P2N(_T("Invalidating with no set\r\n"));
		return CStatic::OnEraseBkgnd(pDC);
		}
}

//////////////////////////////////////////////////////////////////////////

void CClicker::PreSubclassWindow() 

{
	ModifyStyle(0, WS_CLIPCHILDREN | SS_NOTIFY);
	
	CStatic::PreSubclassWindow();
}


/* =====[ FakeWin.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the FakeWin.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// FakeWin.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "FakeWin.h"
#include "Yellowdlg.h"

#include "mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// FakeWin dialog


FakeWin::FakeWin(CWnd* pParent /*=NULL*/)
	: CDialog(FakeWin::IDD, pParent)
{
	//{{AFX_DATA_INIT(FakeWin)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void FakeWin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FakeWin)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(FakeWin, CDialog)
	//{{AFX_MSG_MAP(FakeWin)
	ON_WM_ACTIVATE()
	ON_WM_SETFOCUS()
	ON_WM_CHILDACTIVATE()
	ON_WM_ENABLE()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FakeWin message handlers

void FakeWin::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	//P2N(_T("Fakewin Activated\r\n"));	
}

BOOL FakeWin::PreTranslateMessage(MSG* pMsg) 
{
	//P2N(_T("Pre trans %s\r\n"), num2msg(pMsg->message));	
	return CDialog::PreTranslateMessage(pMsg);
}

void FakeWin::OnSetFocus(CWnd* pOldWnd) 
{
	CDialog::OnSetFocus(pOldWnd);
	//P2N(_T("Fakewin setfocus\r\n"));	
}

void FakeWin::OnChildActivate() 
{
	CDialog::OnChildActivate();
	//P2N(_T("Fakewin child activate\r\n"));		
}

void FakeWin::OnEnable(BOOL bEnable) 
{
	CDialog::OnEnable(bEnable);
	
	//P2N(_T("Fakewin enable fired\r\n"));	
}

void FakeWin::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 

{
	CDialog::OnWindowPosChanged(lpwndpos);
	
	//if(((Coco *)cocodlg)->stable)
	//	((Coco *)cocodlg)->ShowMemos();

	//P2N(_T("Fakewin position changed\r\n"));	
}

BOOL FakeWin::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 

{
	//P2N(_T("Fakewin Notify\r\n"));
	return CDialog::OnNotify(wParam, lParam, pResult);
}

BOOL FakeWin::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 

{
	// TODO: Add your specialized code here and/or call the base class
	//P2N(_T("Fakewin child Notify\r\n"));
	return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
}

//////////////////////////////////////////////////////////////////////////

void FakeWin::PreSubclassWindow() 

{
	//P2N(_T("FakeWin::PreSubclassWindow()\r\n"));

	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	
	CDialog::PreSubclassWindow();
}

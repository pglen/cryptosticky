/////////////////////////////////////////////////////////////////////////////
// YellowDlg.cpp : implementation file
//

#include "stdafx.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <mmsystem.h>

#include "CryptoSticky.h"
#include "mxpad.h"
#include "EditDlg.h"
#include "Popup.h"
#include "fakewin.h"
#include "gotoline.h"
#include "io.h"
#include "Yellowdlg.h"
#include "Alarm.h"
#include "ymemo.h"
#include "about.h"
#include "StatList.h"
#include "xArchive.h"
#include "logdlg.h"


void CLeftMouseClickMsgHandler::MouseMsgHandler()

{
	//AfxMessageBox(_T("Hello"));
	//P2N(_T("Tray Icon Left Mouse Click\r\n"));

	// Make sure we are still top of the chain
	((YellowDlg * )AfxGetMainWnd())->ShowAll();
}

//////////////////////////////////////////////////////////////////////////

void CLeftMouseDblClickMsgHandler::MouseMsgHandler()

{

#if 0
	SHORT ks   = GetKeyState(VK_SHIFT);
	SHORT cks  = GetKeyState(VK_LCONTROL);
	P2N(_T("Shift state: %d Control state: %d\r\n"), ks, cks);
#endif

	//P2N(_T("Magnify from tray\r\n"));
	
	HWND win = AfxGetMainWnd()->m_hWnd;

	SetForegroundWindow(win);
	ShowWindow(win, SW_SHOW); ShowWindow(win, SW_RESTORE);
	
	// Old try
	//::SetTimer(win, 5, 300, NULL);
}

void CRightMouseClickMsgHandler::MouseMsgHandler()

{
	CMenu aMenu;
	aMenu.LoadMenu(IDR_TASKBAR_MENU);

	CPoint mouse;
	GetCursorPos(&mouse);
	aMenu.GetSubMenu(0)->TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON, mouse.x, mouse.y, AfxGetMainWnd(), 0);
}

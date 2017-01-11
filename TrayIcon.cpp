
/* =====[ TrayIcon.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the TrayIcon.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
//
//

#include "stdafx.h"
#include "winuser.h"

#include "mxpad.h"
#include "TrayIcon.h"

//////////////////////////////////////////////////////////////////////////

CTrayIcon::CTrayIcon(HWND hWnd, UINT uIconID, HICON hIcon, LPTSTR lpToolTip, MouseMsgHandlerPtr *pMouseMsgHandler, int nHandlers)

{
	//P2N(_T("Construct tray Icon\r\n"));

	SetTrayIcon(hWnd, uIconID, hIcon, lpToolTip);
	SetMouseMsgHandler(pMouseMsgHandler, nHandlers);
}

//////////////////////////////////////////////////////////////////////////

CTrayIcon::~CTrayIcon()

{
	DeleteIcon();
	delete m_pMouseMsgHandler;
}

//////////////////////////////////////////////////////////////////////////

void CTrayIcon::SetTrayIcon(HWND hWnd, UINT uIconID, HICON hIcon, LPTSTR lpToolTip)

{
	m_hWnd = hWnd;
	m_uIconID = uIconID;
	m_hIcon = hIcon;
	m_lpToolTip = lpToolTip;	
}

//////////////////////////////////////////////////////////////////////////

void CTrayIcon::SetMouseMsgHandler(MouseMsgHandlerPtr *pMouseMsgHandler, int nHandlers)

{
	m_pMouseMsgHandler = pMouseMsgHandler;
	m_nHandlers = nHandlers;
}

//////////////////////////////////////////////////////////////////////////

LRESULT CTrayIcon::OnTaskBarCreated(WPARAM wParam, LPARAM lParam)

{
	VERIFY(AddIcon());
    return 0;
}


//////////////////////////////////////////////////////////////////////////
// ONNOTIFY

void CTrayIcon::OnNotifyIcon(WPARAM wParam, LPARAM lParam)

{
	UINT uID; 
    UINT uMouseMsg; 

	//MOUSEMOVEPOINT  mp;
	//		
	//int ret = GetMouseMovePoints(sizeof(MOUSEMOVEPOINT), &mp, NULL, 0, GMMP_USE_DISPLAY_POINTS);
	//P2N(_T("Mouse point ret=%d x=%d y=%d\r\n"), ret, mp.x, mp.y);

	//P2N(_T("Notify: %d %d\r\n"), wParam, lParam);
 
    uID = (UINT) wParam; 
    uMouseMsg = (UINT) lParam; 
 
	for(int i=0; i<m_nHandlers; i++)
		if (uMouseMsg == m_pMouseMsgHandler[i]->GetMouseMsgID() ) 
			if(uID == m_uIconID)
				m_pMouseMsgHandler[i]->MouseMsgHandler();	
}

//////////////////////////////////////////////////////////////////////////
//

BOOL CTrayIcon::AddIcon()

{
	BOOL res; 
    NOTIFYICONDATA tnid; 
 
    tnid.cbSize = sizeof(NOTIFYICONDATA); 
    tnid.hWnd = m_hWnd; 
    tnid.uID = m_uIconID; 
    tnid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP; 
    tnid.uCallbackMessage = WM_TI_NOTIFYICON; 
    tnid.hIcon = m_hIcon; 
    if (m_lpToolTip) 
        lstrcpyn(tnid.szTip, m_lpToolTip, sizeof(tnid.szTip)); 
    else 
        tnid.szTip[0] = '\0'; 
 
    res = Shell_NotifyIcon(NIM_ADD, &tnid); 
 
    if (m_hIcon) 
        DestroyIcon(m_hIcon); 

    return res; 
}

//////////////////////////////////////////////////////////////////////////

BOOL CTrayIcon::SetTooltip(const TCHAR *str)

{
	BOOL res; 
    NOTIFYICONDATA tnid; 
 
    tnid.cbSize = sizeof(NOTIFYICONDATA); 
    tnid.hWnd = m_hWnd; 
	tnid.uID = m_uIconID; 
    tnid.uFlags = NIF_TIP;  
	
	lstrcpyn(tnid.szTip, str, sizeof(tnid.szTip)); 
    
    res = Shell_NotifyIcon(NIM_MODIFY, &tnid); 
 
    return res; 
}

//////////////////////////////////////////////////////////////////////////
//

BOOL CTrayIcon::DeleteIcon()

{
	BOOL res; 
    NOTIFYICONDATA tnid; 
 
    tnid.cbSize = sizeof(NOTIFYICONDATA); 
    tnid.hWnd = m_hWnd; 
    tnid.uID = m_uIconID; 
         
    res = Shell_NotifyIcon(NIM_DELETE, &tnid); 

    return res; 
}


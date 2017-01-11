
/* =====[ TrayIcon.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the TrayIcon.h                
                                                                             
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

#if !defined(TRAYICON_H)
#define TRAYICON_H

const UINT WM_TI_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
const UINT WM_TI_NOTIFYICON = ::RegisterWindowMessage(_T("NotifyIcon"));

class CMouHand;
typedef CMouHand* MouseMsgHandlerPtr;


class CTrayIcon:public CObject
{

public:

	CTrayIcon(HWND hWnd, UINT uIconID, HICON hIcon, LPTSTR lpToolTip, MouseMsgHandlerPtr *pMouseMsgHandler, int nHandlers);
	~CTrayIcon();

	BOOL	SetTooltip(const TCHAR *str);
	void	SetTrayIcon(HWND hWnd, UINT uIconID, HICON hIcon, LPTSTR lpToolTip);
	void	SetMouseMsgHandler(MouseMsgHandlerPtr *pMouseMsgHandler, int nHandlers);
	BOOL	AddIcon();
	BOOL	DeleteIcon();
	LRESULT OnTaskBarCreated(WPARAM wParam, LPARAM lParam);
	void	OnNotifyIcon(WPARAM wParam, LPARAM lParam);	

private:
	
	CTrayIcon();
	HWND m_hWnd;
	UINT m_uIconID;
	HICON m_hIcon;
	LPTSTR m_lpToolTip;
	MouseMsgHandlerPtr *m_pMouseMsgHandler;
	int m_nHandlers;
};

class CMouHand: public CObject

{
public:

	CMouHand()
		{
		m_uMouseMsgID = WM_LBUTTONDBLCLK;
		}

	CMouHand(UINT uMouseMsgID)
		{
		m_uMouseMsgID = uMouseMsgID;
		}

	virtual void MouseMsgHandler(){}

	UINT GetMouseMsgID()
		{
		return m_uMouseMsgID;
		}

	void SetMouseMsgID(UINT uMouseMsgID)
		{
		m_uMouseMsgID = uMouseMsgID;
		}

private:
	
	UINT m_uMouseMsgID;
};

#endif // TRAYICON_H

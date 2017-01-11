
/* =====[ YellowMemo.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the YellowMemo.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// CTrayIconDemo.h : main header file for the CTRAYICONDEMO application
//

#if !defined(AFX_CTRAYICONDEMO_H__9A121454_20B4_4ABC_91E8_EB1DDA97A5BB__INCLUDED_)
#define AFX_CTRAYICONDEMO_H__9A121454_20B4_4ABC_91E8_EB1DDA97A5BB__INCLUDED_

//#pragma warning(disable: 4996)

#include "statlist.h"
#include "PersistInt.h"
#include "PersistStr.h"
#include "resolv.h"
#include "clicker.h"


////////////////////////////////////////////////////////////////
// General defines

#ifndef IDC_HAND
#define IDC_HAND  MAKEINTRESOURCE(32649)
#endif

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef ABS
#define ABS(a) (((a)<0) ? -(a) : (a))
#endif

#ifndef TRUE
#define TRUE (1 == 1)
#endif

#ifndef FALSE
#define FALSE (1 != 1)
#endif

#define		CASTINT(val) ((int)(val))
#define		MATHPI  3.1415926535

#define		DEMO_DAYS	3000
//define		DEMO_DAYS	5

#define		STAMP_MAX	800

// Sizeof variable in TCHAR
#define TSIZEOF(xx)      (sizeof (xx) / sizeof (TCHAR))
// Convert to / from textsize
#define TXT2BYTE(xx)      ((xx) * sizeof (TCHAR))
#define BYTE2TXT(xx)      ((xx) / sizeof (TCHAR))


////////////////////////////////////////////////////////////////

extern	CString		def_sound;
extern	CString		def_notify;

//#define MBOX		AfxMessageBox
//#define MBOXP		AfxPrintf

extern	PersistInt		beepfreq;
extern	PersistInt		beeplen;
extern	PersistInt		notifyflag;
extern	PersistInt		beepflag;
extern	PersistInt		auto_netpop;
extern	PersistInt		voiceflag;
extern  PersistInt		memo_timeout;

extern	int		daysused; 
extern	int		helpcontext;

extern	PersistInt	bAlpha;

extern	PersistStr		alarmsound;
extern	PersistStr		notifysound;

extern	CString  approot;
extern	CString  appdata;

extern int		CheckLic(const TCHAR *str, int *res);
extern int		SumStr(const TCHAR *str);

typedef struct _conf_colors

{
	COLORREF	memocolor;
	COLORREF	memorcolor;

	COLORREF	memohead;
	COLORREF	memorhead;

	COLORREF	alarmcolor;
	COLORREF	alarmhcolor;
	COLORREF	alarmbutton;
	COLORREF	alarmhbutton;

} conf_colors;

extern conf_colors confcol;

////////////////////////////////////////////////////////////////
// Globals

extern	CString userdir;


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

extern	CFont	default_font;
extern	CFont	memo_font;
extern	CFont	head_font;

extern	int		toggle;

extern	PersistInt		idle_trans;
extern	PersistInt		main_trans;
extern	PersistInt		ala_trans;

extern	PersistInt		memo_fontcol;
extern	PersistInt		head_fontcol;
extern	PersistInt		stop_standby;

extern	int		folder_refresh;
extern	int		auto_popup;
extern	int		dlgback;
extern	int		dlgback2;

extern	CResolv		resolver; 

extern	CString		dataroot;

//extern	CString GetFileNameExt(const CString& name);


// These variables should have been defined in some standard header but is not
#ifndef LWA_COLORKEY
#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer
#endif

//  Function pointer for lyering API in User32.dll
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
            (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

extern	lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes ;

#include "resource.h"		// main symbols
#include "TrayIcon.h"
#include "label.h"
#include "MemoSupp.h"

//extern	MemoSupp	memosup;

extern CTrayIcon* m_pTrayIcon;

//extern	CStatList	statlist;


class CLeftMouseClickMsgHandler: public CMouHand

{
public:

	CLeftMouseClickMsgHandler(UINT uMouseMsg) : CMouHand(uMouseMsg){}

	virtual void MouseMsgHandler();
};

class CLeftMouseDblClickMsgHandler: public CMouHand
{
public:

	CLeftMouseDblClickMsgHandler(){}

	virtual void MouseMsgHandler();
};

class CRightMouseClickMsgHandler: public CMouHand
{
public:

	CRightMouseClickMsgHandler(UINT uMouseMsg) : CMouHand(uMouseMsg){}

	virtual void MouseMsgHandler();
};


/////////////////////////////////////////////////////////////////////////////
// CYelloMemoApp:
// See CTrayIconDemo.cpp for the implementation of this class
//

class CYelloMemoApp : public CWinApp
{
	static BOOL CALLBACK _EnumWindowsProc( HWND hwnd,  LPARAM lParam);

public:
		
	int		awaken;
	HWND	piwnd;

	CYelloMemoApp();

	virtual void WinHelp( DWORD dwData, UINT nCmd = HELP_CONTEXT );


	HWND	FindPrevWin();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CYelloMemoApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt);
	//}}AFX_VIRTUAL

	void	CYelloMemoApp::xOnHelp();

// Implementation

	//{{AFX_MSG(CYelloMemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTRAYICONDEMO_H__9A121454_20B4_4ABC_91E8_EB1DDA97A5BB__INCLUDED_)

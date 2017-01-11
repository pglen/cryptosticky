
/* =====[ CryptoSticky.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the CryptoSticky.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/3/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// CTrayIconDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include <direct.h>
#include <io.h>

#include "CryptoSticky.h"
#include "MsgBox.h"
#include "datalog.h"

#include "mxpad.h"
#include "misc.h"
#include "support.h"

#include "EditDlg.h"
#include "gotoline.h"
#include "fakewin.h"
#include "Yellowdlg.h"
#include "LogDlg.h"
#include "xraym.h"
#include "resolv.h"

#include "Clicense.h"
#include "Clicense.cpp"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// Global data

YellowDlg dlg; 	

int		dlgback  = RGB(225, 225, 192);
int		dlgback2 = RGB(255, 255, 192);

CTrayIcon* m_pTrayIcon;

int			daysused = 0; 
int			helpcontext = 0;

CString		approot;
CString		appdata;

CResolv		resolver; 

CString		snd = _T("Sound");
CString		tran(_T("Trans"));

CString		def_sound(_T("notify4.wav"));
CString		def_notify(_T("notify3.wav"));

PersistStr	alarmsound(snd, _T("alarmsound"), _T(""));
PersistStr	notifysound(snd, _T("notifysound"), _T(""));

//////////////////////////////////////////////////////////////////////////
// Ints

PersistInt	notifyflag(snd, _T("notifyflag") ,  true);
PersistInt	beepflag(snd, _T("beepflag"), false);
PersistInt	voiceflag(snd, _T("voiceflag"), false);
PersistInt	auto_netpop(_T(""), _T("auto_netpop"),  0);

PersistInt	beepfreq(snd, _T("beepfreq"), 2000);
PersistInt	beeplen(snd, _T("beeplen"),  100);
	
PersistInt	ala_trans(tran, _T("ala_trans"), 220);
PersistInt	idle_trans(tran, _T("idle_trans"), 100);
PersistInt	main_trans(tran, _T("main_trans"), 240);
PersistInt	bAlpha(tran, _T("bAlpha"), 	240);
PersistInt	memo_timeout(_T(""), _T("memo_timeout"),  5);

PersistInt	memo_fontcol(_T(""), _T("memo_fontco"),  0);
PersistInt	head_fontcol(_T(""), _T("head_fontco"),  0);
PersistInt	stop_standby(_T(""), _T("stop_standby"),  0);

conf_colors confcol;

lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes = NULL;

CString		userdir = _T("");
CString		dataroot = _T("C:\\CryptoSticky\\");

int		folder_refresh = 1;
int		auto_popup = true;

/////////////////////////////////////////////////////////////////////////////
// Fonts

CString strFontName	    = "FontName";
CString strFontSize	    = "FontSize";
CString strFontWeight	= "FontWeight";
CString strFontItalic	= "FontItalic";

CString strFontConf		= "MemoFont";
CString strFontConf2	= "HeadFont";

CFont	default_font, memo_font, head_font;


//PersistInt	testint(_T("", "test"), 2);
//PersistStr	teststr(_T("", "teststr", "hello"));

int			toggle = false;

CString		mainfont(_T("MS Sans Serif"));
CString		docfont(_T("Aria"));

/////////////////////////////////////////////////////////////////////////////
// The one and only CYelloMemoApp object

	CYelloMemoApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CYelloMemoApp

BEGIN_MESSAGE_MAP(CYelloMemoApp, CWinApp)
	//{{AFX_MSG_MAP(CYelloMemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, xOnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYelloMemoApp construction

CYelloMemoApp::CYelloMemoApp()

{
	//P2N(_T("Construct application obj\r\n"));
	//awaken = false;
}

//////////////////////////////////////////////////////////////////////////
//

BOOL CALLBACK CYelloMemoApp::_EnumWindowsProc( HWND hwnd,  LPARAM lParam)

{
	TCHAR str[MAX_PATH+1]; GetWindowText(hwnd, str, sizeof(str));

	if(_tcsstr(str, _T("CryptoSticky Pro")) != NULL)
		{
		//P2N(_T("Found %s\r\n"), str);
		
		*((HWND *)lParam) = hwnd;
		return false;
		}

	//P2N(_T("%d %s\r\n"), hwnd, str);	

	return true;
}

//////////////////////////////////////////////////////////////////////////

HWND	CYelloMemoApp::FindPrevWin()

{
	HWND handle = 0;

	if(!EnumWindows(_EnumWindowsProc, (LPARAM)&handle))
		return handle;

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CYelloMemoApp initialization

BOOL CYelloMemoApp::InitInstance()

{
	AfxEnableControlContainer();

	m_pMainWnd = &dlg;

	C2N;

	CTime ct = CTime::GetCurrentTime();
	P2N(_T("\r\n\r\n"));
	P2N(_T("Started CryptoSticky Application at %s\r\n"), ct.Format(_T("%c")));

	//P2N(_T("appname='%s' cmdline='%s' exename='%s'  charsize=%d  intsize=%d\r\n"), 
	//								m_pszAppName, m_lpCmdLine, m_pszExeName,
	//									sizeof(TCHAR), sizeof(int));

	support.GetSpecialFolder(CSIDL_PERSONAL, userdir);
	//userdir = _T("C:\\Users\\peterglen\\");
	dataroot = userdir + "CryptoSticky\\";

	int mbcs = false; 
#ifdef _MBCS 
		mbcs = true;
#endif

	P2N(_T("DataRoot '%s'\r\n"), dataroot);
	P2N(_T("Sizeof char %d sizeof TCHAR %d MBCS %d\r\n"), sizeof(char), sizeof(TCHAR), mbcs );

	//int gpf = 10;
	//P2N(_T("DataRoot GPF '%s'\r\n"), gpf);

	// Get where the application lives
	TCHAR *buff = approot.GetBuffer(MAX_PATH + 1);
	GetModuleFileName(AfxGetInstanceHandle(), buff, MAX_PATH);

	// Get application root
	TCHAR *ptr = _tcsrchr(buff, '\\');
	if(ptr)
		*(ptr+1) = '\0';

	approot.ReleaseBuffer();
	
	if(_taccess(userdir, 0) < 0)
		{
		_tmkdir(userdir);
		}

	if(_taccess(dataroot, 0) < 0)
		{
		_tmkdir(dataroot);
		}

	// Get where the application data is:
	support.GetSpecialFolder(CSIDL_APPDATA,	appdata);
	appdata += "CryptoSticky";

	if(_taccess(appdata, 0) < 0)
		_tmkdir(appdata);

	appdata += "\\";
	
	P2N(_T("App data '%s'\r\n"), appdata);

	//AfxGetApp()->
	SetRegistryKey(_T("PeterGlen"));

	int usage_count = AfxGetApp()->GetProfileInt(_T(""), _T("Usage_Count"), 0);
	usage_count++;
	AfxGetApp()->WriteProfileInt(_T(""), _T("Usage_Count"), usage_count);

	// Locate previous instance
	//piwnd = FindWindow(NULL, TEXT(_T("CryptoSticky")));

	piwnd = FindPrevWin();
	if(piwnd)
		{
		//oldwin; oldwin.FromHandle(piwnd);
		//YellowDlg *
		
		P2N(_T("CryptoSticky already running ... \r\n"));

		//AfxMessageBox(_T("An instance of CryptoSticky is already running, activating it"));
		
		// Send prepare message, so it CS is ready to encrypt
		PostMessage(piwnd, WM_USER+10, 0, 0);	

		ShowWindow(piwnd, SW_SHOW); ShowWindow(piwnd, SW_RESTORE);
		SetForegroundWindow(piwnd);

		Sleep(300);
		exit(0);
		}
	
	if(!default_font.CreateFont(-13, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, FIXED_PITCH, docfont))
		{
		AfxMessageBox(_T("Cannot set small font, defaulting to system font."));
		}

	datalog.open_datalog();
	datalog.PrintToLog(_T("Started CryptoSticky Application\r\n"));

	// Read in new data root
	dataroot = AfxGetApp()->GetProfileString(_T("Data"), _T("dataroot"), dataroot);

	if(_taccess(dataroot, 0) < 0)
		{
		AfxMessageBox(_T("Cannot access new dataroot, switching back to default"));
		dataroot = userdir + "CryptoSticky\\";
		AfxGetApp()->WriteProfileString(_T("Data"), _T("dataroot"), dataroot);
		}
	P2N(_T("Dataroot %s\r\n"), dataroot);
	// Load registry backed strings
	PersistStr::Load();	PersistInt::Load();

	// Output first usage
	AfxGetApp()->WriteProfileString(_T("Data"), _T("dataroot"), dataroot);

	// Estabilish first time usage
	time_t firsttime    =  GetProfileInt(_T(""),  _T("FirstUse"), 0);
	time_t firsttime2   =  GetProfileInt(_T("License"),  _T("FirstUse"), 0);

	firsttime = min(firsttime, firsttime2);
	
	CString fdate1 = support.GetRegStr(HKEY_CURRENT_USER, _T("Software\\CryptoSticky"), _T("FirstRun1"));
	CString fdate2 = support.GetRegStr(HKEY_CURRENT_USER, _T("Software\\CryptoSticky"), _T("FirstRun2"));
	
	if(fdate1 != "" && fdate2 != "")
		{
		//P2N(_T("fdate1=%s fdate2=%s\r\n"), fdate1, fdate2);

		FILETIME ft;
		ft.dwHighDateTime = _tstoi(fdate1);	ft.dwLowDateTime  = _tstoi(fdate2);
		
		CTime ymt(ft);

		//P2N(_T("Filetime %s\r\n", ymt.Format(L"%c")));

		time_t firsttime3  =  ymt.GetTime();
		firsttime = min(firsttime, firsttime3);
		}

	if(firsttime == 0 || firsttime2 == 0)
		{
		firsttime = time(NULL);;

		//P2N(_T("Wrote first use %x\r\n"), firsttime);
		
		WriteProfileInt(_T(""),  _T("FirstUse"), (int)firsttime);
		WriteProfileInt(_T("License"),  _T("FirstUse"), (int)firsttime);
		}
		
	// Get resources we need:
	CString fontname = 	GetProfileString(strFontConf, strFontName, _T("Aria"));
	int size    =  		GetProfileInt(strFontConf,  strFontSize, -13);
	int weight  =  		GetProfileInt(strFontConf, strFontWeight, 0);
	int italic  =  		GetProfileInt(strFontConf, strFontItalic, 0);

	if(!memo_font.CreateFont(size, 0, 0, 0, weight, italic, 0, 0,
				0, 0, 0, 0, FIXED_PITCH, fontname))
		{
		AfxMessageBox(_T("Cannot set Sticky font, defaulting to system font."));
		}

	CString fontname2 = 	GetProfileString(strFontConf2, strFontName, _T("Aria"));
	int size2    =  		GetProfileInt(strFontConf2,  strFontSize, -13);
	int weight2  =  		GetProfileInt(strFontConf2, strFontWeight, 0);
	int italic2  =  		GetProfileInt(strFontConf2, strFontItalic, 0);

	if(!head_font.CreateFont(size2, 0, 0, 0, weight2, italic2, 0, 0,
				0, 0, 0, 0, FIXED_PITCH, fontname2))
		{
		AfxMessageBox(_T("Cannot set head font, defaulting to system font."));
		}

	//CString snd(_T("Sound"));
	//beepfreq = AfxGetApp()->GetProfileInt(snd, "beepfreq", beepfreq);
	//beeplen  = AfxGetApp()->GetProfileInt(snd, "beeplen", beeplen);

	// Auto assign alarm and notify sounds
	if((CString)alarmsound == "")
		alarmsound = approot + 	def_sound;
;
	if((CString)notifysound == "")
		notifysound = approot + def_notify;

	//alarmsound = AfxGetApp()->GetProfileString(snd, "alarmsound", alarmsound);

	//notifysound = approot + (const char*)notifysound;
	//notifysound = AfxGetApp()->GetProfileString(snd, "notifysound", notifysound);
	
	//notifyflag = AfxGetApp()->GetProfileInt(snd, "notifyflag", notifyflag);
	//beepflag = AfxGetApp()->GetProfileInt(snd, "beepflag", beepflag);

	//main_trans = AfxGetApp()->GetProfileInt(tran, "main_trans", main_trans);
	//idle_trans = AfxGetApp()->GetProfileInt(tran, "idle_trans", idle_trans);
	//bAlpha = AfxGetApp()->GetProfileInt(tran, "bAlpha", bAlpha);

	folder_refresh = AfxGetApp()->GetProfileInt(_T("Data"), _T("folder_refresh"), folder_refresh);
	auto_popup = AfxGetApp()->GetProfileInt(_T("Data"), _T("auto_popup"), auto_popup);

	//P2N(_T("Folder refresh %d\r\n"), folder_refresh);

	time_t ttt = time(NULL);

	CTimeSpan ts(ttt -	firsttime);	

	daysused = (int)ts.GetDays();

	// Testing
	//daysused = ts.GetTotalMinutes();
	
	CString name = AfxGetApp()->GetProfileString(_T("License"), _T("Name"), _T("DEMO"));

	
	//P2N(_T("Licensed name=%s\r\n"), name);

	if(name != "DEMO")
		{
		CString key = AfxGetApp()->GetProfileString(_T("License"), _T("Key"));
		CString all; 

		all += name; all += key;
	
		P2N(_T("License key='%s' name='%s' usage  count=%d\r\n"), 
					key, name, usage_count);

		int check, res = CheckLic(all, &check);
		if(res && ( (check % LIC_MODULUS) == 0) )
			{
			//P2N(_T("Valid License %s\r\n"), name);
			//daysused = 0;
			}
		else
			{
			//P2N(_T("InValid Licensed key=%s name=%s\r\n"), key, name);
			}
		}
	else
		{
		//P2N(_T("Running demo version usage count=%d\r\n"), usage_count);
		}

	//resolver.Announce();

	//P2N(_T("Days (test:sec) in use %d\r\n"), daysused);

	//if(!memo_font.CreateFont(-13, 0, 0, 0, 0, 0, 0, 0,
	//			0, 0, 0, 0, FIXED_PITCH, docfont))
	//	{
	//	AfxMessageBox(_T("Cannot set small font, defaulting to system font."));
	//	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	//AfxEnableMemoryLeakDump(true);
	//AfxEnableMemoryTracking(true);

	HMODULE hUser32 = GetModuleHandle(_T("USER32.DL"));

	
	g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
					GetProcAddress(hUser32, "SetLayeredWindowAttributes");

	P2N(_T("user23 handle %x layered %x\r\n"), hUser32, g_pSetLayeredWindowAttributes);
	
	//SetDialogBkColor(dlgback);
	dlg.m_BackColor = dlgback;

	CXrayM::_dlgbackcol = dlgback;
	CXrayM::_highcol = dlgback2;

	//P2N(_T("About to pop dialog\r\n"));

#ifdef _DEBUG
	CMemoryState oldMemState, newMemState, diffMemState;
	oldMemState.Checkpoint();
#endif

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
		{
		}
	else if (nResponse == IDCANCEL)
		{
		}

#ifdef _DEBUG
	newMemState.Checkpoint();
	diffMemState.Difference(oldMemState, newMemState);
	//diffMemState.DumpAllObjectsSince();
#endif


	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
	
int CYelloMemoApp::ExitInstance() 

{
	P2N(_T("Exit App\r\n"));
	
	//support.HeapDump(_T("Exitinstance"));
	AfxCheckMemory();

	resolver.Denounce();

	m_pTrayIcon->DeleteIcon();
	
	//if(awaken)
	//	{
	//	//PostMessage(wnd, WM_SHOWWINDOW, true, 0);
	//	//ShowWindow(wnd, SW_SHOW);
	//	//SetFocus(wnd);
	//	//YieldToWinEx();
	//	}
	//else 
		{
		// Save font info
		LOGFONT lf; memo_font.GetObject(sizeof(LOGFONT), &lf);

		WriteProfileString(strFontConf,  strFontName, lf.lfFaceName);
		WriteProfileInt(strFontConf,  strFontSize, lf.lfHeight);
		WriteProfileInt(strFontConf,  strFontWeight, lf.lfWeight);
		WriteProfileInt(strFontConf,  strFontItalic, lf.lfItalic);

		// Save font info2
		LOGFONT lf2; head_font.GetObject(sizeof(LOGFONT), &lf2);

		WriteProfileString(strFontConf2,  strFontName, lf2.lfFaceName);
		WriteProfileInt(strFontConf2,  strFontSize, lf2.lfHeight);
		WriteProfileInt(strFontConf2,  strFontWeight, lf2.lfWeight);
		WriteProfileInt(strFontConf2,  strFontItalic, lf2.lfItalic);
		}

	datalog.close_datalog();

	CXrayM::_FreeLast();

	return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////////////////////////////

int CYelloMemoApp::DoMessageBox(LPCTSTR lpszPrompt, UINT nType, UINT nIDPrompt) 

{
	//P2N(_T("Do message box %s\r\n"), lpszPrompt);

	int		ret = CMsgBox::xMsgBox(lpszPrompt, NULL, nType, IDNO);

	return ret;

	//MessageBox(this->m_pMainWnd->m_hWnd, lpszPrompt, "CryptoSticky Application", nType | MB_ICONINFORMATION);
	// return CWinApp::DoMessageBox(lpszPrompt, nType, nIDPrompt);
}

void CYelloMemoApp::WinHelp( DWORD dwData, UINT nCmd )

{
	CString str(approot);
	
#ifdef _DEBUG
	str += "..\\html\\";
#else
	str += "html\\";
#endif
	
	switch(helpcontext)
		{
		case 1:
			str += "memo.htm";
			break;

		case 2:
			str += "setup.htm";
			break;

		case 3:
			str += "network.htm";
			break;

		default:
			str += "index.htm";
			break;
		}

	ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOWMAXIMIZED);		
}

void CYelloMemoApp::xOnHelp()

{
	WinHelp(0,0);
}

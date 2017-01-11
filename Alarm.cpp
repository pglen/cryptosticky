
/* =====[ Alarm.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Alarm.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Alarm.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "Alarm.h"
#include "mmsystem.h"

#include "Yellowdlg.h"
#include "mxpad.h"
#include "logdlg.h"
#include "datalog.h"
#include "misc.h"

// Disable security warnings TODO for later
#pragma warning (disable : 4996)

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef DEBUG_NEW
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#endif

//////////////////////////////////////////////////////////////////////////
// No particular reason ....

void	CAlarm::_ThreadProc(void *ptp )

{
	//P2N(_T("Beep from thread beepfreq  %d, beeplen %d\r\n"), beepfreq & 0xffff, 
	//										beeplen & 0xffff);
	//CAlarm*  me = (CAlarm*)ptp;

	::Beep(beepfreq & 0xffff, beeplen & 0xffff);
}

/////////////////////////////////////////////////////////////////////////////
// CAlarm dialog

CAlarm::CAlarm(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarm)
	//}}AFX_DATA_INIT
	//hIcon = AfxGetApp()->LoadIcon(_T("IDI_ICON2")) ;

	dheadcol = misc.modcolor(confcol.alarmhcolor, 5, 8);

	headcol = confcol.alarmhcolor;		// RGB(0xf0, 0xf0, 0x00);
	dlgcolor = confcol.alarmcolor;		// RGB(0xf0, 0xf0, 0x00);
	hdlgcolor = confcol.alarmhcolor;	// 0x00e0e0;
	butcolor = confcol.alarmbutton;		// RGB(255,255,0);
	hbutcolor = confcol.alarmhbutton;	// RGB(255,255,100);
	
	alarmoffset = snooze = count = bigcount = delay = 0;

	final = false;
	createtime = GetTickCount();
	st = NULL;

	m_BackColor = RGB(192, 192, 192);
}

void CAlarm::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarm)
	DDX_Control(pDX, IDC_BUTTON4, m_5);
	DDX_Control(pDX, IDC_ALAR2, m_alatitle);
	DDX_Control(pDX, IDOK, m_4);
	DDX_Control(pDX, IDC_BUTTON3, m_3);
	DDX_Control(pDX, IDC_BUTTON2, m_2);
	DDX_Control(pDX, IDC_BUTTON1, m_1);
	DDX_Control(pDX, IDC_ALAR, m_alar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAlarm, CGrayDlg)
	//{{AFX_MSG_MAP(CAlarm)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarm message handlers

BOOL CAlarm::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();

	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
	datalog.PrintToLog(_T("Alarm triggered '%s'\r\n"), title);
		
	//m_alar.SetTrans();	m_alar.CenterText();
	//m_alar.SetText(_T("CryptoSticky Alarm"));
	//m_alar.SetFontBold(true); m_alar.SetFontSize(18);

	m_alatitle.SetFontBold(true); m_alatitle.SetFontSize(36);
		
	m_1.SetToolTipText(_T("Snooze for 1 Minute")); 
	m_1.SetBackColor(butcolor); m_1.SetHighColor(hbutcolor);

	m_2.SetToolTipText(_T("Show Sticky"));		
	m_2.SetBackColor(butcolor); m_2.SetHighColor(hbutcolor);
	
	m_3.SetToolTipText(_T("Silence alarm temporarily. Sound will resume in 30 sec."));		
	m_3.SetBackColor(butcolor); m_3.SetHighColor(hbutcolor);
	
	m_4.SetToolTipText(_T("Acknowledge Alarm, Silence Sound and Close Alarm Window")); 
	m_4.SetBackColor(butcolor); m_4.SetHighColor(hbutcolor);

	m_5.SetToolTipText(_T("Snooze for 3 Minutes")); 
	m_5.SetBackColor(butcolor); m_5.SetHighColor(hbutcolor);

#ifdef SPEAK
	sp.PostHere(m_hWnd);
#endif

	// Restore old placement
	WINDOWPLACEMENT wp2;
	GetWindowPlacement(&wp2);

	CString section(_T("Dynamic"));
	CString str = AfxGetApp()->GetProfileString(section, _T("AlarmPos"), _T(""));
	
	if(str != "")
		{
		_stscanf_s(str, _T("%d %d %d %d"),
			&wp2.rcNormalPosition.top, &wp2.rcNormalPosition.left,
				&wp2.rcNormalPosition.bottom,  &wp2.rcNormalPosition.right);	

		//sscanf(str, _T("%d %d %d %d"),
		//	&wp2.rcNormalPosition.top, &wp2.rcNormalPosition.left,
		//		&wp2.rcNormalPosition.bottom,  &wp2.rcNormalPosition.right);	

		// Position window, set to offset
		SetWindowPos(NULL, wp2.rcNormalPosition.left + alarmoffset, 
				wp2.rcNormalPosition.top + alarmoffset, 0, 0, SWP_NOSIZE  | SWP_NOZORDER );
		
		//MoveWindow(&wp2.rcNormalPosition);
		}

	SetWindowText(_T("CryptoSticky Alarm"));

	SetTimer(0, 1000, NULL);		
	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
}

//////////////////////////////////////////////////////////////////////////

void CAlarm::OnTimer(UINT_PTR nIDEvent) 

{
	if(nIDEvent == 0)
		{
		// Initiate a snooze wakeup cycle
		//P2N(_T("CAlarm::OnTimer %d bigcount=%d snooze=%d\r\n"),
		//		nIDEvent, bigcount, snooze);

		if(bigcount == snooze - 1)
			{
			count = 0;
			Show();
			SetTimer(2, 1000, NULL);
			}
		bigcount++;
		}

	// Read in memo
	if(nIDEvent == 1)
		{
		KillTimer(nIDEvent);

		YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
		//ASSERT(cc);

		if(fname != "")
			{
			HTREEITEM item;

			cc->FnameToSel(fname, &item);

			title = cc->GetTreeCtrl()->GetItemText(item);

			//P2N(_T("fname=%s title=%s\r\n"), fname, title);	

			//cc->ShowOne(item, false);
			//cc->ShowOne(item);		
			
			st = cc->Handle2Memo(item);
			//m_title.SetText(st->memo.head);

			CString title2(st->memo.head);	support.ShortenStr(title2, 28);
			m_alatitle.SetText(title2);

			//P2N(_T("CAlarm::OnTimer title2='%s'\r\n"), title2);
	
			SetTimer(2, 1000, NULL);
			}
		}
	
	// Start alarm process
	if(nIDEvent == 2)
		{
		if(delay > 0)
			{
			KillTimer(nIDEvent);
			delay = 0;
		
			if(st && st->memo.flags.readt)
				{
				CString speakstr; speakstr = st->memo.head;  
				speakstr += " .  .  . \r\n"; speakstr += st->memo.body;
#ifdef SPEAK
				sp.Speak(speakstr);				
#endif
				}
			else
				{
				SetTimer(3, 1000, NULL);
				}
			}

		delay++;
		}

	//////////////////////////////////////////////////////////////////////

	if(nIDEvent == 3)
		{
		//P2N(_T("Alarm Dialog Timer\r\n"));

		if(final)
			return;

		// Stop bugging people for too long
		if(bigcount > 300)
			{
			count = 0;
			KillTimer(nIDEvent);
			KillTimer(1);
			return;
			}

		if( (count > 1) && (count < 5))
			{
			if(st && st->memo.flags.beep)			
				Beep(); //beepfreq, beeplen);
			}

		if(count == 6)
			{			
			if(st &&  st->memo.flags.soundon)			
				PlaySound(alarmsound, NULL, SND_FILENAME | SND_ASYNC );
			}

		if(count > 6)
			{
			count = 0;
			KillTimer(nIDEvent);

			// Wait half a minute
			snooze = bigcount + 30; 
			}
		count++;
		}

	CGrayDlg::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CAlarm::OnButton3() 

{
	count = 7;
	PlaySound(NULL, NULL, SND_FILENAME );
#ifdef SPEAK
	sp.Stop();
#endif
}

void CAlarm::OnOK()

{
	// Stop sound
	count = 7;

	PlaySound(NULL, NULL, SND_FILENAME );
#ifdef SPEAK
	sp.Stop();
#endif	
	// Nuke this registry entry
	HKEY key;
	LONG ret = RegOpenKey(HKEY_CURRENT_USER,
					_T("Software\\RobotMonkeySoftware\\CryptoSticky\\Alarms"), &key);
	//RegDeleteValue(key, fname);

	if(ret == ERROR_SUCCESS)
		{
		RegDeleteValue(key, fname);
		RegCloseKey(key);
		}

	datalog.PrintToLog(_T("Acknowledged alarm '%s'\r\n"), title);

	final = true;

	CGrayDlg::OnOK();

	//EndDialog(IDOK);
}

void CAlarm::OnButton1() 

{
	PlaySound(NULL, NULL, SND_FILENAME );
	
	snooze = bigcount + 60; 
	count = 7;

	//count = ((count / snooze) * snooze) +  7;
	Hide();	

	datalog.PrintToLog(_T("Snoozed (1) on alarm: '%s'\r\n"), title);

	// Nuke this registry entry
	HKEY key;
	LONG ret = RegOpenKey(HKEY_CURRENT_USER,
					_T("Software\\RobotMonkeySoftware\\CryptoSticky\\Alarms"), &key);

	//RegDeleteValue(key, fname);

	if(ret == ERROR_SUCCESS)
		{
		RegDeleteValue(key, fname);
		RegCloseKey(key);
		}

}

//////////////////////////////////////////////////////////////////////////

BOOL CAlarm::OnEraseBkgnd(CDC* pDC) 

{

#if 0
	RECT rect; 	GetClientRect(&rect);

	CBrush NewBrush;

	NewBrush.CreateSolidBrush(dlgcolor) ;
	
	pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);
	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
	pDC->SelectObject(pOldBrush);
	
	return(false);

	//return CGrayDlg::OnEraseBkgnd(pDC); 

#endif

	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
	
}

//////////////////////////////////////////////////////////////////////////

void CAlarm::OnDestroy() 

{
	CGrayDlg::OnDestroy();
	
#ifdef SPEAK
	sp.Stop();
#endif
	WINDOWPLACEMENT wp2;
	GetWindowPlacement(&wp2);

	int hh2  = wp2.rcNormalPosition.bottom - wp2.rcNormalPosition.top;	
	int ww2  = wp2.rcNormalPosition.right - wp2.rcNormalPosition.left;

	//P2N(_T("Destroy Alarm win %d %d %d %d\r\n"), 
	//		wp2.rcNormalPosition.top, wp2.rcNormalPosition.left,
	//			wp2.rcNormalPosition.bottom,  wp2.rcNormalPosition.right);	

	CString tmp; tmp.Format(_T("%d %d %d %d"),
		wp2.rcNormalPosition.top, wp2.rcNormalPosition.left,
				wp2.rcNormalPosition.bottom,  wp2.rcNormalPosition.right);	

	CString section(_T("Dynamic"));
	AfxGetApp()->WriteProfileString(section, _T("AlarmPos"), tmp);
}

void CAlarm::OnButton2() 

{
	//AfxMessageBox(fname);	

	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(cc);

	if(fname != "")
		{
		cc->HideMemos();

		HTREEITEM item;

		cc->FnameToSel(fname, &item);
		CString title = cc->GetTreeCtrl()->GetItemText(item);

		//AfxMessageBox(title);	

		cc->ShowOne(item, false);
		cc->ShowOne(item);
		}

	// Nuke this registry entry
	HKEY key;
	LONG ret = RegOpenKey(HKEY_CURRENT_USER,
					_T("Software\\RobotMonkeySoftware\\CryptoSticky\\Alarms"), &key);
	if(ret == ERROR_SUCCESS)
		{
		RegDeleteValue(key, fname);
		RegCloseKey(key);
		}
	//RegDeleteValue(key, fname);

}

void CAlarm::OnPaint() 

{
	//CPaintDC dc(this); // device context for painting

#if 0

	RECT	rc3; GetClientRect(&rc3);

	CRect rect(rc3);

	// Fill in header slice
	rchead.top = 2;	rchead.left = 2;
	rchead.right = rc3.right - 2;
	rchead.bottom = 22;

	dc.FillSolidRect(&rchead, hdlgcolor);

	CPen Pen2;	Pen2.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = (CPen*)dc.SelectObject(&Pen2);

	closerect.top = rc3.top + 8;
	closerect.left = rc3.right - 16;
	closerect.bottom = closerect.top + 8;
	closerect.right = closerect.left + 8;

	dc.MoveTo(closerect.left, closerect.top);
	dc.LineTo(closerect.right,  closerect.bottom);

	dc.MoveTo(closerect.left, closerect.bottom );
	dc.LineTo(closerect.right, closerect.top);

	// Show header line

	dc.SelectObject(pOldPen);
	
#endif

	CGrayDlg::OnPaint();
}

void CAlarm::OnLButtonDown(UINT nFlags, CPoint point) 

{
	CRect cr(closerect);

	cr.InflateRect(5,5);

	// Cloxing on 'x'
	if(cr.PtInRect(point))
		{
		CAlarm::OnOK();
		return;
		}	
	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CAlarm::OnButton4() 

{
	PlaySound(NULL, NULL, SND_FILENAME );
	snooze = bigcount + 180; 
	count = 7; // ((count / snooze) * snooze) +  7;

	Hide();		

	datalog.PrintToLog(_T("Snoozed (3) on alarm: '%s'\r\n"), title);

	// Nuke this registry entry
	HKEY key;
	LONG ret = RegOpenKey(HKEY_CURRENT_USER,
					_T("Software\\RobotMonkeySoftware\\CryptoSticky\\Alarms"), &key);
	if(ret == ERROR_SUCCESS)
		{
		RegDeleteValue(key, fname);
		RegCloseKey(key);
		}
	//RegDeleteValue(key, fname);

}

void CAlarm::Show()
{
	CGrayDlg::Show();

	ShowWindow(SW_SHOWNOACTIVATE   );
}

BOOL CAlarm::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == WM_USER+1)
		{
		int len = LOWORD(pMsg->lParam), start = HIWORD(pMsg->lParam);

		#ifdef SPEAK
		if(pMsg->wParam == (unsigned int)SPRS_DONE)
			{
			//P2N(_T("Done Speaking\r\n"));

			// Start normal sound routine
			SetTimer(3, 1000, NULL);
			}
		#endif

		}
	return CGrayDlg::PreTranslateMessage(pMsg);
}

void CAlarm::Beep()

{
	_beginthread(_ThreadProc, 0, (void*)this);
}

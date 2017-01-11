
/* =====[ EditDlg.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the EditDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "CryptoSticky.h"

#include "io.h"
#include "fakewin.h"

#include "Yellowdlg.h"
#include "EditDlg.h"
#include "mxpad.h"
#include "xArchive.h"
#include "logdlg.h"
#include "MemoConf.h"
#include "SndBoard.h"
#include "datalog.h"

#pragma warning(disable : 4302)

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditDlg dialog

void	CEditDlg::StaticSound(void *org, char *buff, int len) 

{
	CEditDlg *ptr = (CEditDlg*)org;
	ptr->Sound(buff, len);
}

//////////////////////////////////////////////////////////////////////////

CEditDlg::CEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditDlg)
	m_center = FALSE;
	m_hh = _T("");
	m_mm = _T("");
	m_alatime = _T("");
	m_aladate = _T("");
	m_ala = FALSE;
	m_us = _T("");
	m_persist = FALSE;
	m_red = FALSE;
	m_soundon = FALSE;
	m_head = _T("");
	m_folder = FALSE;
	m_beep = FALSE;
	m_ampm = _T("");
	m_text = FALSE;
	//}}AFX_DATA_INIT

	m_sound = "sound";
	section12 = "12-24";
	fresh = false;
	st = NULL;
	ro = false;
	m_BackColor = RGB(192,192,192);
	m_body.cryptver = CURR_MEMO_VER;
}


void CEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditDlg)
	DDX_Control(pDX, IDC_BUTTON19, m_b19);
	DDX_Control(pDX, IDC_BUTTON35, m_b35);
	DDX_Control(pDX, IDC_BUTTON34, m_b34);
	DDX_Control(pDX, IDC_BUTTON17, m_b17);
	DDX_Control(pDX, IDC_BUTTON9, m_b9);
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_BUTTON8, m_b8);
	DDX_Control(pDX, IDC_WAV, m_wav);
	DDX_Control(pDX, IDC_EDIT1, m_body);
	DDX_Control(pDX, IDC_BUTTON7, m_b7);
	DDX_Control(pDX, IDC_BUTTON14, m_b14);
	DDX_Control(pDX, IDC_BUTTON13, m_b13);
	DDX_Control(pDX, IDC_BUTTON12, m_b12);
	DDX_Control(pDX, IDC_BUTTON11, m_b11);
	DDX_Control(pDX, IDC_BUTTON4, m_b4);
	DDX_Control(pDX, IDC_CHECK8, m_folderc);
	DDX_Control(pDX, IDC_EDIT8, m_chead);
	DDX_Control(pDX, IDC_BUTTON3, m_3);
	DDX_Control(pDX, IDC_BUTTON2, m_2);
	DDX_Control(pDX, IDC_BUTTON1, m_1);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_SPIN2, m_spin2);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Check(pDX, IDC_CHECK2, m_center);
	DDX_Text(pDX, IDC_EDIT2, m_hh);
	DDX_Text(pDX, IDC_EDIT4, m_mm);
	DDX_Text(pDX, IDC_EDIT5, m_alatime);
	DDX_Text(pDX, IDC_EDIT6, m_aladate);
	DDX_Check(pDX, IDC_CHECK3, m_ala);
	DDX_Text(pDX, IDC_EDIT7, m_us);
	DDX_Check(pDX, IDC_CHECK6, m_persist);
	DDX_Check(pDX, IDC_CHECK1, m_red);
	DDX_Check(pDX, IDC_CHECK4, m_soundon);
	DDX_Text(pDX, IDC_EDIT8, m_head);
	DDX_Check(pDX, IDC_CHECK8, m_folder);
	DDX_Check(pDX, IDC_CHECK5, m_beep);
	DDX_Text(pDX, IDC_EDIT10, m_ampm);
	DDX_Check(pDX, IDC_CHECK13, m_text);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_TITLETEXT, m_titletext);
	DDX_Control(pDX, IDC_CHECK1, m_redcheck);
	DDX_Control(pDX, IDC_CHECK2, m_centertext);
	DDX_Control(pDX, IDC_CHECK6, m_persisttext);
	DDX_Control(pDX, IDC_CHECK9, m_autotext);
	DDX_Control(pDX, IDC_CALFRAME, m_calframe);
	DDX_Control(pDX, IDC_ALAFRAME, m_alaframe);
	DDX_Control(pDX, IDC_ALAFRAME2, m_alaframe2);
	DDX_Control(pDX, IDC_HOURT, m_hourt);
	DDX_Control(pDX, IDC_MINT, m_mint);
	DDX_Control(pDX, IDC_AMPMT, m_ampmt);
	DDX_Control(pDX, IDC_EDIT6, m_alaht);
	DDX_Control(pDX, IDC_EDIT5, m_alamint);
	DDX_Control(pDX, IDC_EDIT7, m_alarmampmt);
	DDX_Control(pDX, IDC_CHECK3, m_check3t);
	DDX_Control(pDX, IDC_CHECK4, check4t);
	DDX_Control(pDX, IDC_CHECK5, m_check5t);
	DDX_Control(pDX, IDC_CHECK13, m_check13t);
	DDX_Control(pDX, IDC_CHECK17, m_check5t);
	DDX_Control(pDX, IDC_BUTTON38, m_decr);
}

BEGIN_MESSAGE_MAP(CEditDlg, CDialog)
	//{{AFX_MSG_MAP(CEditDlg)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, OnDeltaposSpin2)
	ON_WM_VSCROLL()
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR1, OnSelchangeMonthcalendar1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_NOTIFY(MCN_SELECT, IDC_MONTHCALENDAR1, OnSelectMonthcalendar1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_MONTHCALENDAR1, OnReleasedcaptureMonthcalendar1)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_CHECK8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_CHECK9, OnCheck9)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_BUTTON34, OnButton34)
	ON_BN_CLICKED(IDC_BUTTON35, OnButton35)
	ON_BN_CLICKED(IDC_CHECK13, OnCheck13)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDOK, &CEditDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CEditDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON38, &CEditDlg::OnBnClickedButton38)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CEditDlg message handlers

BOOL CEditDlg::OnInitDialog() 

{
	CDialog::OnInitDialog();

	////ASSERT(st->magic == CYmemo_Magic);

	helpcontext = 1;

	// Color WaveEdit
	m_wav.bgcolor = RGB(64, 64, 32);
	m_wav.selcolor = RGB(128, 128, 64);


	// Init sound player
	playsound.silent = true;
	playsound.WaveOpen();
	m_wav.pplaysnd = &playsound;
	m_wav.PostHere(m_hWnd);
	
	//CenterWindow(GetDesktopWindow());

	CenterWindow(AfxGetApp()->m_pMainWnd);

	//m_body.AttachEdit(this, IDC_EDIT1);
	//m_head = "";

	// Init controls
	m_spin1.SetRange(0, 23); m_spin2.SetRange(0, 59);
	((CButton *)GetDlgItem(IDC_CHECK9))->SetCheck(true);
	
	//P2N(_T("CEditDlg::OnInitDialog  memoroot='%s' parent_root='%s'\r\n"), memoroot, memorootp);

#ifdef SPEAK
	sp.PostHere(m_hWnd);
#endif	
	Decorate();

	// CEdit CWnd
	GetDlgItem(IDC_EDIT1)->ShowWindow(true);

	int the12 = AfxGetApp()->GetProfileInt(section12, _T("Value"), 0);		

	if(!the12)
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	else
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(1);
	
	if(ro)
		((CEdit *)GetDlgItem(IDC_EDIT8))->SetReadOnly();

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////
//

void	CEditDlg::OnOK(void)

{
	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);

	//ASSERT(cc);	////ASSERT(cc->magic);

	UpdateData();

	//P2N(_T("CEditDlg::OnOK(void) caption=%s\r\n"), caption);

	if(m_head == "")
		{
		AfxMessageBox(_T("Memos cannot have an empty header string."));
		GetDlgItem(IDC_EDIT8)->SetFocus();
		return;
		}
	if(m_head.Find(_T("\\")) != -1)
		{
		AfxMessageBox(_T("Memo header cannot contain the Backslash character. ('\\')"));
		GetDlgItem(IDC_EDIT8)->SetFocus();
		return;
		}

	CString newinit, oldname;
	int isnew = false;

	// Create file name
	if(m_folder)
		{
		//P2N(_T("Folder kind\r\n"));
		newinit = memoroot + m_head + "\\";
		}
	else
		{
		newinit = memorootp + m_head + "\\";
		}
		
	if(memo.initial != "")
		{	
		// Remember old name for memo if name changed
		if(memo.initial != newinit)
			{
			oldname = memo.MakeFileName();
			}
		}
	else
		{
		isnew = true;
		}

	//P2N(_T("memo.initial '%s' newinit '%s'\r\n"), memo.initial, newinit);

	// Create new file name
	memo.initial = newinit;
	CString newfile = memo.MakeFileName();

	//P2N(_T("file '%s' oldname '%s' change '%s'\r\n"), file, oldname, newfile);

	// Check for duplicate memo
	if(isnew && _taccess(newfile, 0) != -1)
		{
		AfxMessageBox(_T("Memo with this title already exists.\r\n"));
		return;
		}

	if(oldname != "")
		{
		//P2N(_T("deleting and hiding %s\r\n"), oldname);

		HTREEITEM item, itemp, h1;
		cc->FnameToSel(oldname, &item);		
		
		if(item)
			{					
			cc->CopyDep(oldname, newfile);

			itemp = cc->GetTreeCtrl()->GetNextItem(item, TVGN_PARENT);
			cc->ShowOne(item, false);
			cc->DelMemobyHandle(item);

			h1 = cc->GetTreeCtrl()->InsertItem(m_head, itemp);
			cc->GetTreeCtrl()->Select(h1, TVGN_CARET );

			support.YieldToWinEx();
			cc->GetTreeCtrl()->SetCheck(h1, true);
			cc->WrIterate_Checks(NULL);
			cc->ShowOne(h1, true);
			}
		}

	file = newfile;

	//////////////////////////////////////////////////////////////////////////
	// Write YellowDlg file

	memo.flags.centered = m_center;
	memo.flags.red = m_red;
	memo.flags.enabled = m_persist;
	memo.flags.ala = m_ala;
	memo.flags.persist = m_persist;
	memo.flags.sound = m_sound;
	memo.flags.soundon = m_soundon;
	memo.flags.date = m_aladate;
	memo.flags.time = m_alatime;
	memo.flags.beep  = m_beep;
	memo.flags.readt = m_text;
	memo.flags.ver = CURR_MEMO_VER;

	memo.head		= m_head;
	
	GetDlgItem(IDC_EDIT1)->GetWindowText(memo.body);

	//memo.Dump();
	
	memo.Write(file);
	
	memo.CheckAlarm();

	// See if it is the next alarm
	((YellowDlg *)AfxGetApp()->m_pMainWnd)->ArmNext();
	
	// Save wave if changed
	recsnd.StopRecord();	
	playsound.StopPlay();

	if(m_wav.changed)
		{		
		CString dir = support.GetDirName(file);
		CString fname = support.GetFilename(file);
		CString wavname = dir + "\\" + fname + ".wav";

		//P2N(_T("Saving to wave %s\r\n"), wavname);

		CString tmp; tmp.Format(_T("Writing wave file, please wait .... %d bytes "), 
			m_wav.GetBuffLen());
		SetWindowText(tmp);

		::SetCursor(LoadCursor(NULL,  MAKEINTRESOURCE(IDC_WAIT)));
		
		// Remove file if no wave
		if(m_wav.GetBuffLen() == 0)
			{
			_tunlink(wavname); 
			}
		else
			m_wav.SaveWave(wavname);

		::SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		}

	datalog.PrintToLog(_T("Saved Sticky '%s'\r\n"), memo.initial);

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	CDialog::OnShowWindow(bShow, nStatus);

	((CEdit *)GetDlgItem(IDC_EDIT1))->SetSel(-1, 0);	

	if(caption != "")
		m_chead.SetFocus();
}

void CEditDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	//P2N(_T("Spin the thing %d\r\n"), pNMUpDown->iPos);
	//m_hh.Format(_T("%02d"), pNMUpDown->iPos);
	//UpdateData(false);

	*pResult = 0;
}

void CEditDlg::OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	//m_mm.Format(_T("%02d"), pNMUpDown->iPos);
	//UpdateData(false);

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N(_T("Scroll nPos=%d\r\n"), nPos);

	if(GetDlgItem(IDC_SPIN1) == pScrollBar)
		{
		//if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() )
		//	{
		//	int pos = nPos;
		//	CString ampm(_T("AM")); 
		//	if(nPos > 12) { ampm = "PM"; pos -= 12;}
		//	m_hh.Format(_T("%02d %s"), pos, ampm);
		//	}
		//else
			{
			m_hh.Format(_T("%02d"), nPos);
			}
		}

	if(GetDlgItem(IDC_SPIN2) == pScrollBar)
		{
		m_mm.Format(_T("%02d"), nPos);
		}
	
	if(GetDlgItem(IDC_SPIN3) == pScrollBar)
		{
		if(nSBCode == SB_THUMBPOSITION)
			{	
			if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() )
				{
				if(m_ampm == "PM")
					m_ampm.Format(_T("%s"), _T("AM"));
				else
					m_ampm.Format(_T("%s"), _T("PM"));
				}
			else
				{
				m_ampm.Format(_T("%s"), _T(""));
				}
				
			}
		}

	//m_alatime.Format(_T("%02s:%02s"), m_hh, m_mm);

	//UpdateData(false);
	UpdateAla();

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnSelchangeMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnChangeEdit2() 

{
	UpdateData();
	
	//P2N(_T("CEditDlg::OnChangeEdit2\r\n"));

	//if(((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();

	int the12 = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();

	if(!the12)
		{
		if(_tstoi(m_hh) > 23)
			m_hh = "23";

		if(_tstoi(m_hh) < 0)
			m_hh = "0";
		}
	else
		{
		if(_tstoi(m_hh) > 12)
			m_hh = "12";

		if(_tstoi(m_hh) < 1)
			m_hh = "1";
		}

		//m_alatime.Format(_T("%02s:%02s"), m_hh, m_mm);
	//UpdateData(false);

	UpdateAla();
}

void CEditDlg::OnChangeEdit4() 

{
	UpdateData();

	if(_tstoi(m_mm) > 59)
		m_mm = "59";

	if(_tstoi(m_mm) < 0)
		m_mm = "0";

	//m_alatime.Format(_T("%02s:%02s"), m_hh, m_mm);

	UpdateAla();
	//UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////
//

void CEditDlg::OnSelectMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	//P2N(_T("alarm cal set\r\n"));

	UpdateData(true);

	m_ala = true;

	UpdateAla();
	*pResult = 0;
}

void CEditDlg::OnReleasedcaptureMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	*pResult = 0;
}


void CEditDlg::UpdateAla()

{
	SYSTEMTIME SysTime;

	MonthCal_GetCurSel(GetDlgItem(IDC_MONTHCALENDAR1)->m_hWnd, &SysTime);

	if(!m_ala)	
		{
		m_aladate = "";
		m_alatime = "";
		m_us = "";
		}
	else
		{
		m_spin1.SetPos(_tstoi(m_hh));
		m_spin2.SetPos(_tstoi(m_mm));

		//P2N(_T("sel %02d-%02d-%04d\r\n"), SysTime.wDay, SysTime.wMonth, SysTime.wYear);

		int hhh = _tstoi(m_hh);

		m_aladate.Format(_T("%02d-%02d-%04d"), SysTime.wMonth, SysTime.wDay, SysTime.wYear);

		int the12 = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
		int ish = hhh; 

		if(the12)
			{	
			if(hhh == 0)
				{
				hhh = 12; m_hh = "12";
				}

			//m_us.Format(_T("(%02d:%02s %s)"), hhh, m_mm, m_ampm);

			if(hhh == 12)
				{
				if(m_ampm == "AM")
					hhh = 0;				
				}
			else
				{
				if(m_ampm == "PM")
					hhh += 12;
				}
			}
		else
			{
			if(hhh > 12) 
				ish -= 12;
			if(hhh == 0) 
				ish = 12;			
			}
			
		CString ampm(_T("AM")); if(hhh > 11) ampm = _T(" PM");	
		m_us.Format(_T("(%02d:%02s %s)"), ish, m_mm, ampm);
			
		m_alatime.Format(_T("%02d:%02s"), hhh, m_mm);

		//P2N(_T("ala time %s\r\n"), m_alatime);
		}

	UpdateData(false);
}

void CEditDlg::OnCheck1() 

{
}


void CEditDlg::OnCheck3() 

{
	UpdateData();

	if(!m_ala)
		{
		m_soundon = false;
		m_beep = false;
		m_text = false;
		}
	else
		{
		m_soundon = true;
		m_beep = true;
		m_text = true;		
		}
	UpdateAla();
}


void CEditDlg::OnCheck4() 

{	
	//m_sound = !m_sound;

	UpdateData();

	if(m_sound)
		{
		if(!m_ala)
			{
			m_ala = true;
			UpdateAla();
			}
		}		
}

void CEditDlg::OnButton3() 

{
	UpdateData();
	// Set dialog to NOW
	CTime m_time = CTime::GetCurrentTime();
	CTimeSpan ts(0, 0, 1, 0);
	m_time += ts;

	SetAla(m_time);
}

//////////////////////////////////////////////////////////////////////////
// Copy to clibboard

void CEditDlg::OnButton1() 

{
	UpdateData();

	m_body.SetFocus();
	m_body.Copy();
	m_body.SetSel(-1, -1);

}

//////////////////////////////////////////////////////////////////////////
// Paste into edit box

void CEditDlg::OnButton2() 

{
	//if(!memo.flags.encrypt)
		{	
		m_body.SetFocus();
		m_body.Paste();
		}
}

void CEditDlg::OnCheck8() 
{
	
}

void CEditDlg::OnCheck5() 
{
	UpdateData();	

	//P2N(_T("m_beep=%d\r\n"), m_beep);

	if(m_sound)
		{
		if(!m_ala)
			{
			m_ala = true;
			UpdateAla();
			}
		}	
}

void CEditDlg::SetAla(CTime ct)

{
	SYSTEMTIME timeDest;
	ct.GetAsSystemTime(timeDest );

	MonthCal_SetCurSel(GetDlgItem(IDC_MONTHCALENDAR1)->m_hWnd, &timeDest);

	m_spin1.SetPos(ct.GetHour());
	m_spin2.SetPos(ct.GetMinute());

	m_hh.Format(_T("%02d"), ct.GetHour());		//"%02d"), m_spin1.GetPos()); 
	m_mm.Format(_T("%02d"), ct.GetMinute());

	m_ala = true;
	m_soundon = true;
	m_text = true;
	m_beep = true;

	Prop_ampm();

	UpdateAla();
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnButton4() 

{
	CTime m_time = CTime::GetCurrentTime();
	CTimeSpan ts(0, 1, 0, 0);
	SetAla(m_time + ts);	
}

void CEditDlg::OnButton11() 

{
	CTime m_time = CTime::GetCurrentTime();

	CTimeSpan ts(7,0,0,0);

	SetAla(m_time + ts);		
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnButton12() 

{
	//((CEdit *)GetDlgItem(IDC_EDIT1))->SetSel(0, -1, true);
	//((CEdit *)GetDlgItem(IDC_EDIT1))->RedrawWindow();

	
	m_body.SetFocus();
	
	CString str; m_body.GetWindowText(str) ;

	if(str == "")
		return;

	m_body.SetSel(0, -1, true);
	m_body.RedrawWindow();

	//UpdateData(false);
	//m_ed.EnableWindow(false);
	//m_ed.Invalidate(true);
	//YieldToWinEx();
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnTimer(UINT nIDEvent) 
 
{
 	KillTimer(nIDEvent);

	if(caption != "")
		{
		//P2N(_T("Create new memo\r\n"));

		CTime m_time = CTime::GetCurrentTime();

		SetWindowText(caption);

		m_body.SetWindowText(str);

		// Set dialog to alarm date/time -- default to NOW - 1  
		// (just in case one before that)

		CTimeSpan ts(0, 0, 2, 0);
		m_time -= ts;

		SetAla(m_time);
		}
	else
		{
		//P2N(_T("Load existing memo\r\n"));
		CString old; GetWindowText(old);
	
		::SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_WAIT)));		
	
		SetWindowText(_T("Loading Sticky details ..."));

		LoadMemo();
		
		// Position to the end
		int sum = 0, len = m_body.GetLineCount();		
		for(int loop = 0; loop < len; loop++)
			{
			sum += m_body.LineLength();
 			}
		m_body.SetSel(sum, sum);
		m_body.SetFocus();

		// Load wave as well
		CString dir = support.GetDirName(file);
		CString fname = support.GetFilename(file);

		CString wavname = dir + "\\" + fname + ".wav";

		if(_taccess(wavname, 0) >= 0)
			{
			SetWindowText(_T("Loading Sticky wave file ..."));

			m_wav.OpenWave(wavname);
			m_wav.changed = 0;
			}

		::SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));

		CString tmp(file); support.PathToFname(tmp);
		CString tmp2; tmp2.Format(_T("Sticky %s (%s)"), title, tmp);
		SetWindowText(tmp2);
		}


	CString name = AfxGetApp()->GetProfileString(_T("License"), _T("Name"), _T("DEMO"));
	if(name == "DEMO" && daysused > DEMO_DAYS)
 		{
 		AfxMessageBox(
 			_T(" CryptoSticky Editing is Disabled. However, you are able to set Alarms, Encrypt, \r\n")
 			_T("Decrypt, Copy and Paste your Existing Sticky Contents from the memo's right click menu."), MB_OK	);
 
 		((CWnd *)GetDlgItem(IDC_EDIT1))->EnableWindow(false);
 		return;
 		}
	
 	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnRadio1() 

{
	Prop_ampm();
}

void CEditDlg::OnRadio2() 

{
	Prop_ampm();
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::Prop_ampm()

{
	//CString section12(_T("12-24"));

	int the12 = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();

	AfxGetApp()->WriteProfileInt(section12, _T("Value"), !the12);

	GetDlgItem(IDC_EDIT10)->EnableWindow(the12);

	// Switch between 12 / 24
	if(the12)
		{
		m_spin1.SetRange(1, 12);
		
		if(_tstoi(m_hh) > 12)
			{
			m_hh.Format(_T("%02d"), _tstoi(m_hh) - 12);
			m_ampm = "PM";
			}
		else
			{
			m_ampm = "AM";
			}
		}
	else
		{
		m_spin1.SetRange(0, 23);
		
		if(m_ampm == "PM")
			{
			m_hh.Format(_T("%02d"), _tstoi(m_hh) + 12);
			}
		
		if(m_ampm == "AM" && _tstoi(m_hh) == 12)
			{
			m_hh.Format(_T("%02d"), _tstoi(m_hh) - 12);
			}
		
		if(_tstoi(m_hh) > 23)
			m_hh = "23";
			
		m_ampm = "";	
		}		

	UpdateData(false);
	UpdateAla();
}

void CEditDlg::OnButton13()
 
{
	CTime m_time = CTime::GetCurrentTime();

	CTimeSpan ts(14,0,0,0);

	SetAla(m_time + ts);			
}

void CEditDlg::OnButton14() 
{
	CTime m_time = CTime::GetCurrentTime();

	CTimeSpan ts(21,0,0,0);

	SetAla(m_time + ts);			
}

void CEditDlg::OnButton7() 

{
	//YellowDlg  *pwnd = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	
	if(st)
		{
		CMemoConf mc;
		mc.buddy = st;
		mc.DoModal();
		}
	else
		{
		AfxMessageBox(_T("This is a new Sticky, no window exits yet"));
		}
}

void CEditDlg::OnCheck9() 

{
	m_body.checkon = !m_body.checkon;
	m_body.Invalidate();
	m_body.m_Drawn = false;
}

void CEditDlg::OnCancel()

{
	UpdateData();

	//P2N(_T("m_head = %s\r\n"), m_head);

	//if(m_head == "" )
	//	goto endd;

	int textmod = ((CPGedit*)GetDlgItem(IDC_EDIT1))->GetModify();

	if(m_wav.changed == 0 && textmod == 0)
		goto endd;

	//P2N(_T("CEditDlg::OnCancel() m_wav.changed=%d\r\n"), m_wav.changed);
	
	//if(!((Cm_bodyit*)GetDlgItem(IDC_EDIT8))->GetModify())
	//	goto endd;

	if(AfxMessageBox(_T("Abandon changes made to this Sticky?"), MB_YESNO) != IDYES)
		{
		GetDlgItem(IDC_EDIT1)->SetFocus();
		return;
		}

endd:
	CDialog::OnCancel();
}

void CEditDlg::OnButton5() 

{
	::ShowWindow(AfxGetMainWnd()->m_hWnd, SW_RESTORE);
	::SetWindowPos(AfxGetMainWnd()->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);	
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::LoadMemo() 

{
	// Set dialog to alarm date/time -- default to NOW
	CTime m_time = CTime::GetCurrentTime();
		{	
		CString rstr;

		m_folderc.EnableWindow(false);
	
		if(memo.Read(file))
			{
			CString tmp(memo.initial); int idx = tmp.Find(L'\r');
			if(idx >= 0) tmp = tmp.Left(idx);
			datalog.PrintToLog(_T("Opened Sticky for editing '%s'\r\n"), tmp);

			//memo.Dump();

			m_center = memo.flags.centered ;
			m_red = memo.flags.red ;
			m_persist = memo.flags.persist;
			m_ala = memo.flags.ala;
			m_sound = memo.flags.sound;
			m_soundon = memo.flags.soundon;
			m_aladate = memo.flags.date;
			m_alatime = memo.flags.time;
			m_beep = memo.flags.beep;
			m_text = memo.flags.readt;

			// Get crypt version of the memo
			m_body.cryptver = memo.flags.ver;

			m_head = memo.head;

			//P2N(_T("CEditDlg::LoadMemo memo %s ver=%d\r\n"), memo.head, memo.flags.ver);

			GetDlgItem(IDC_EDIT1)->SetWindowText(memo.body);
			
			//if(memo.flags.encrypt)
			//	GetDlgItem(IDC_EDIT1)->EnableWindow(false);

			UpdateData(false);

			// Remember old stat to check for conflict
			memset(&old_sbuff, 0, sizeof(old_sbuff));

			if(_tstat(file, &old_sbuff) < 0)
				{
				//P2N(_T("Could not get old stat '%s'\r\n"), file);
				}
			}	
		else
			{
			if(title.Mid(0, 9) != "\\Deleted\\")
				{
				TCHAR buff[128]; _tcserror_s(buff, sizeof(buff), errno);
				CString str; str.Format(_T("Cannot open file for '%s' -- %s"), title, buff);
				AfxMessageBox(str);
				}
			EndDialog(IDCANCEL);
			}

		// Sync to date in calendar
		if(m_aladate != "" && m_alatime != "")
			{
			//P2N(_T("aladate='%s' alatime='%s'\r\n"), m_aladate, m_alatime);
			
			int Day = 0, Month = 0, Year = 0, Hour = 0, Minute = 0;

			_stscanf_s(m_aladate, _T("%02d-%02d-%04d"), &Month, &Day, &Year);
			_stscanf_s(m_alatime, _T("%02d:%02d"), &Hour, &Minute);
			
			//P2N(_T("Loaded alarm: %02d-%02d-%04d  %02d:%02d\r\n"), Day, Month, Year, Hour, Minute);
			
			if(BETWEEN_DATES(Year, 0, 2038) && BETWEEN_DATES(Day, 0, 32) && BETWEEN_DATES(Month, 0, 13))
				{
				//P2N(_T("Loaded valid date %02d-%02d-%04d  %02d:%02d\r\n"), Day, Month, Year, Hour, Minute);

				CTime tme(Year, Month, Day, Hour, Minute, 0);
				m_time = tme;
				}
			else
				{
				//P2N(_T("invalid date %02d-%02d-%04d  %02d:%02d\r\n"), Day, Month, Year, Hour, Minute);
				
				time_t tt = time(NULL);
				CTime tme(tt);
				m_time = tme;
				}

			SYSTEMTIME timeDest;
			m_time.GetAsSystemTime(timeDest );

			MonthCal_SetCurSel(GetDlgItem(IDC_MONTHCALENDAR1)->m_hWnd, &timeDest);
			}
		
		m_spin1.SetPos(m_time.GetHour());
		m_spin2.SetPos(m_time.GetMinute());

		m_hh.Format(_T("%02d"), m_time.GetHour());		//"%02d"), m_spin1.GetPos()); 
		m_mm.Format(_T("%02d"), m_time.GetMinute());	//"%02d"),  m_spin2.GetPos()); 

		Prop_ampm();

		UpdateAla();
		}
	}

//////////////////////////////////////////////////////////////////////////

void	CEditDlg::Decorate()

{
	// Decorate Tooltips
	m_3.SetToolTipText(_T("Set Alarm to Now + 1 Minute\r\n")
		_T("Alarm is set to the upcoming Minute boundary\r\n")
		_T("Useful for testing the alarm"));

	m_1.SetToolTipText(_T("Copy text to Clipboard"));
	m_2.SetToolTipText(_T("Paste text from Clipboard"));
	m_b4.SetToolTipText(_T("Set Alarm to Now + 1 Hour"));

	m_b11.SetToolTipText(_T("Set Alarm to Now + 1 Week"));
	m_b12.SetToolTipText(_T("Select the whole contents of the Sticky text window"));
	m_b13.SetToolTipText(_T("Set Alarm to Now + 2 Weeks"));
	m_b14.SetToolTipText(_T("Set Alarm to Now + 3 Weeks"));

	m_b7.SetToolTipText(_T("Configure Sticky colors and font size"));

	m_ok.SetToolTipText(_T("Save Sticky, Evaluate Alarm Queue"));
	m_can.SetToolTipText(_T("Abandon current changes"));
	
	m_b9.SetToolTipText(_T("Start / Stop Recording Voice memo"));
	m_b8.SetToolTipText(_T("Start / Stop Playing Voice memo"));

	m_b34.SetToolTipText(_T("Read text with text to speech"));
	m_b35.SetToolTipText(_T("Stop reading text"));
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnButton8() 

{
	if(recsnd.recording)
		{
		recsnd.StopRecord();
		m_b9.SetWindowText(btext);
		recsnd.WaveInClose();
		}

	//if(playsound.IsPlaying())
	//	{
	//	playsound.StopPlay();
	//	m_b8.SetWindowText(btext2);
	//	return;
	//	}

	if(btext2 == "")
		m_b8.GetWindowText(btext2);

	m_wav.PlayBuff();
	
	m_b8.SetWindowText(_T("Stop"));
	
	m_b8.SetFocus();
	//playsound.Play(m_wav.GetBuff(), m_wav.GetBuffLen());
	//playsound.PlayMonoWave(m_wav.GetBuff(), m_wav.GetBuffLen());
}

//////////////////////////////////////////////////////////////////////////

void CEditDlg::OnButton9() 

{
	
	if(recsnd.WaveInOpen() < 0)
		{
		AfxMessageBox(_T("Cannot open input device"));
		return;
		}

	if(recsnd.recording)
		{
		recsnd.StopRecord();

		m_b9.SetWindowText(btext);
		recsnd.WaveInClose();
		return;
		}

	if(playsound.IsPlaying())
		{
		//m_b8.SetWindowText(btext2);
		playsound.StopPlay();
		}

	m_wav.changed++;
	recsnd.orgclass = this;
	recsnd.callthis2 = StaticSound;

	recsnd.StartRecord();

	m_b9.GetWindowText(btext);
	m_b9.SetWindowText(_T("Stop"));
	m_b9.SetFocus();
}

//////////////////////////////////////////////////////////////////////////

int	CEditDlg::Sound(char *buff, int len) 

{
	//P2N(_T("CEditDlg::Sound %p %d\r\n"), buff, len);
	m_wav.AppendBuff(buff, len);
	free(buff);

	return 0;
}


void CEditDlg::OnDestroy() 

{
	CDialog::OnDestroy();
	
	recsnd.StopRecord();	
	playsound.StopPlay();

	helpcontext = 0;
}

void CEditDlg::OnButton17() 

{
	CSndBoard sb;
	
	sb.DoModal();
}

void CEditDlg::OnButton34() 

{
	//P2N(_T("CEditDlg::Read text\r\n"));

#ifdef SPEAK
	if(sp.reenter)
		{
		//P2N(_T("Stopping speak\r\n"));
		sp.Stop(); return;
		}
#endif

	CString str; m_body.GetWindowText(str) ;

	if(str == "")
		return;

	if(oldbutt == "")
		m_b34.GetWindowText(oldbutt);

	m_b34.SetWindowText(_T("Stop Speak"));
#ifdef SPEAK
	sp.Speak(str);
#endif

}

void CEditDlg::OnButton35() 

{
#ifdef SPEAK
	sp.Stop();	
#endif
}

//////////////////////////////////////////////////////////////////////////

BOOL CEditDlg::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == WM_KEYDOWN)
		{
		//P2N(_T("CEditDlg::PreTranslateMessage keydown %c\r\n"), pMsg->wParam);

		if(pMsg->wParam == VK_F1)
			{			
			return true;
			}
		}

	if(pMsg->message == WM_USER+1)
		{
		int len = LOWORD(pMsg->lParam), start = HIWORD(pMsg->lParam);

		m_body.SetFocus();

		#ifdef SPEAK

		// CEdit 
		if(pMsg->wParam == SPRS_IS_SPEAKING)
			{
			//P2N(_T("Set sel %d %d\r\n"), start, start + len + 1);
			m_body.SetSel(start, start + len);
			}
		else
			{
			//P2N(_T("Clear Sel\r\n"));
			m_body.SetSel(-1, 0);
			}
		#endif
		m_body.Invalidate();
		}

	// Speak Messages
	if(pMsg->message == WM_USER + 1)
		{
		//P2N(_T("Text 2 Speech message W=%d LpLO=%d LpHI=%d\r\n"), 
		//		pMsg->wParam, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));

#ifdef SPEAK
		if(pMsg->wParam == SPRS_DONE)
			{
			m_b34.SetWindowText(oldbutt);
			}
#endif
		}

	// Sound editor Messages
	if(pMsg->message == WM_USER + 4)
		{
		//P2N(_T("Sound editor / player message %d %d\r\n"),
		//	pMsg->wParam, pMsg->lParam);

		if(pMsg->wParam == 0)
			{
			if(btext2 != "")
				m_b8.SetWindowText(btext2);
			}
		}

	return CDialog::PreTranslateMessage(pMsg);
}

void CEditDlg::OnCheck13() 

{
	UpdateData();

	//P2N(_T("m_text=%d\r\n"), m_text);

	if(m_text)
		{
		
		}
}

//////////////////////////////////////////////////////////////////////////
//

void CEditDlg::OnButton19() 

{
	//YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;

	//P2N(_T("CEditDlg::Encrypt\r\n"));
	
	m_body.Encrypt2();
	m_body.SetFocus();
}

HBRUSH CEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if(nCtlColor == CTLCOLOR_STATIC)
		{
		hbr = CreateSolidBrush(dlgback);
		}
	//else if(nCtlColor == CTLCOLOR_EDIT)
	//	{
	//	hbr = CreateSolidBrush(dlgback);
	//	}
	else if(nCtlColor == CTLCOLOR_LISTBOX)
		{
		hbr = CreateSolidBrush(dlgback);
		}
	else if(nCtlColor == CTLCOLOR_DLG)
		{
		hbr = CreateSolidBrush(dlgback);
		}
	else
		{		
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}

	return hbr;

}

BOOL CEditDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CEditDlg::OnBnClickedOk()
{
	OnOK();
}

void CEditDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CEditDlg::OnBnClickedButton38()
{
	//YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	//P2N(_T("CEditDlg Decrypt\r\n"));
	
	m_body.Decrypt2();
	m_body.SetFocus();
}

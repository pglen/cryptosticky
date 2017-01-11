
/* =====[ Setup.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Setup.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
// Setup.cpp : implementation file
//

#include "stdafx.h"
#include "io.h"

#include "mxpad.h"
#include "mmsystem.h"

#include "CryptoSticky.h"
#include "xbutton.h"
#include "Yellowdlg.h"
#include "Setup.h"
#include "FileDialogST.h"
#include "DlgGetPath.h"
#include "NetPeerDlg.h"
#include "logdlg.h"

// Disable security warnings TODO for later
#pragma warning (disable : 4996)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetup dialog


CSetup::CSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetup)
	m_userdir = _T("");
	m_wav = _T("");
	m_refresh = _T("");
	m_notify = _T("");
	//}}AFX_DATA_INIT

	want_refresh = 0;
	
	modified = 0;
	tran = "Trans";
	m_BackColor = RGB(192, 192, 192);

}

void CSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetup)
	DDX_Control(pDX, IDC_SLIDER6, m_slide6);
	DDX_Control(pDX, IDC_EDIT4, m_edit4);
	DDX_Control(pDX, IDC_SPIN3, m_spin3);
	DDX_Control(pDX, IDC_BUTTON37, m_b37);
	DDX_Control(pDX, IDC_BUTTON36, m_b36);
	DDX_Control(pDX, IDC_BUTTON33, m_b33);
	DDX_Control(pDX, IDC_BUTTON32, m_b32);
	DDX_Control(pDX, IDC_BUTTON31, m_b31);
	DDX_Control(pDX, IDC_BUTTON30, m_b30);
	DDX_Control(pDX, IDC_SPIN1, m_spin1);
	DDX_Control(pDX, IDC_BUTTON26, m_b26);
	DDX_Control(pDX, IDC_BUTTON18, m_b18);
	DDX_Control(pDX, IDC_BUTTON29, m_b29);
	DDX_Control(pDX, IDC_BUTTON28, m_b28);
	DDX_Control(pDX, IDOK2, m_ok2);
	DDX_Control(pDX, IDC_SLIDER5, m_slide5);
	DDX_Control(pDX, IDC_BUTTON4, m_but4);
	DDX_Control(pDX, IDC_BUTTON27, m_b27);
	DDX_Control(pDX, IDC_SLIDER4, m_slide4);
	DDX_Control(pDX, IDC_SLIDER3, m_slide3);
	DDX_Control(pDX, IDC_R8, m_r8);
	DDX_Control(pDX, IDC_R7, m_r7);
	DDX_Control(pDX, IDC_BUTTON25, m_b25);
	DDX_Control(pDX, IDC_R6, m_r6);
	DDX_Control(pDX, IDC_R5, m_r5);
	DDX_Control(pDX, IDC_BUTTON24, m_b24);
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_BUTTON23, m_b23);
	DDX_Control(pDX, IDC_BUTTON22, m_b22);
	DDX_Control(pDX, IDC_BUTTON21, m_b21);
	DDX_Control(pDX, IDC_BUTTON20, m_b20);
	DDX_Control(pDX, IDC_BUTTON19, m_b19);
	DDX_Control(pDX, IDC_BUTTON16, m_b16);
	DDX_Control(pDX, IDC_BUTTON15, m_b15);
	DDX_Control(pDX, IDC_SLIDER2, m_slide2);
	DDX_Control(pDX, IDC_SLIDER1, m_slide1);
	DDX_Control(pDX, IDC_BUTTON14, m_b14);
	DDX_Control(pDX, IDC_BUTTON13, m_b13);
	DDX_Control(pDX, IDC_BUTTON12, m_b12);
	DDX_Control(pDX, IDC_BUTTON11, m_b11);
	DDX_Control(pDX, IDC_BUTTON10, m_b10);
	DDX_Control(pDX, IDC_R4, m_r4);
	DDX_Control(pDX, IDC_R3, m_r3);
	DDX_Control(pDX, IDC_R2, m_r2);
	DDX_Control(pDX, IDC_R1, m_r1);
	DDX_Control(pDX, IDOK, m_ca);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);

	DDX_Control(pDX, IDC_LAB1, m_lab1);
	DDX_Control(pDX, IDC_LAB2, m_lab2);
	DDX_Control(pDX, IDC_LAB3, m_lab3);
	DDX_Control(pDX, IDC_LAB4, m_lab4);
	DDX_Control(pDX, IDC_LAB5, m_lab5);
	DDX_Control(pDX, IDC_LAB6, m_lab6);
	DDX_Control(pDX, IDC_LAB7, m_lab7);
	DDX_Control(pDX, IDC_LAB8, m_lab8);
	DDX_Control(pDX, IDC_LAB9, m_lab9);
	DDX_Control(pDX, IDC_LAB10, m_lab10);
	DDX_Control(pDX, IDC_LAB11, m_lab11);
	DDX_Control(pDX, IDC_LAB12, m_lab12);
	DDX_Control(pDX, IDC_LAB13, m_lab13);
	DDX_Control(pDX, IDC_LAB14, m_lab14);
	DDX_Control(pDX, IDC_LAB15, m_lab15);
	DDX_Control(pDX, IDC_LAB16, m_lab16);
	DDX_Control(pDX, IDC_LAB17, m_lab17);


	DDX_Text(pDX, IDC_EDIT1, m_userdir);
	DDX_Text(pDX, IDC_EDIT3, m_wav);
	DDX_Text(pDX, IDC_EDIT2, m_refresh);
	DDX_Text(pDX, IDC_EDIT11, m_notify);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_NUL4, m_nul4);
	DDX_Control(pDX, IDC_MAX1, m_max1);
	DDX_Control(pDX, IDC_NUL2, m_nul2);
	DDX_Control(pDX, IDC_MAX2, m_max2);
	DDX_Control(pDX, IDC_NUL3, m_nul3);
	DDX_Control(pDX, IDC_MAX3, m_max3);
	DDX_Control(pDX, IDC_MIN1, m_min1);
	DDX_Control(pDX, IDC_MAX4, m_max4);
	DDX_Control(pDX, IDC_MIN2, m_min2);
	DDX_Control(pDX, IDC_MAX5, m_max5);
	DDX_Control(pDX, IDC_CHECK15, m_check5);
	DDX_Control(pDX, IDC_BUTTON39, m_b39);
}

BEGIN_MESSAGE_MAP(CSetup, CDialog)
	//{{AFX_MSG_MAP(CSetup)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON13, OnButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnButton14)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton15)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON16, OnButton16)
	ON_BN_CLICKED(IDC_BUTTON19, OnButton19)
	ON_BN_CLICKED(IDC_BUTTON20, OnButton20)
	ON_BN_CLICKED(IDC_BUTTON24, OnButton24)
	ON_BN_CLICKED(IDC_BUTTON25, OnButton25)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_BUTTON27, OnButton27)
	ON_BN_CLICKED(IDC_BUTTON23, OnButton23)
	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_BN_CLICKED(IDC_BUTTON18, OnButton18)
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_BN_CLICKED(IDC_BUTTON28, OnButton28)
	ON_BN_CLICKED(IDC_BUTTON29, OnButton29)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON26, OnButton26)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT2, OnKillfocusEdit2)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_BUTTON30, OnButton30)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_BUTTON32, OnButton32)
	ON_BN_CLICKED(IDC_BUTTON33, OnButton33)
	ON_BN_CLICKED(IDC_BUTTON31, OnButton31)
	ON_BN_CLICKED(IDC_CHECK11, OnCheck11)
	ON_BN_CLICKED(IDC_BUTTON36, OnButton36)
	ON_BN_CLICKED(IDC_BUTTON37, OnButton37)
	ON_BN_CLICKED(IDC_CHECK14, OnCheck14)
	ON_BN_CLICKED(IDC_CHECK15, OnCheck15)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_LAB16, &CSetup::OnBnClickedLab16)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK16, &CSetup::OnBnClickedCheck16)
	ON_BN_CLICKED(IDC_BUTTON39, &CSetup::OnBnClickedButton39)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CSetup::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetup message handlers

void CSetup::OnButton2() 

{
	CString str;
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = confcol.memohead;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		confcol.memohead = cd.m_cc.rgbResult;
		m_r2.SetColor(confcol.memohead);

		// Send refresh message to all stickers
		YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
		//ASSERT(cc);
	
		cc->HideMemos(); 
		cc->ShowMemos();
		}
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnButton4() 

{
	PlaySound(alarmsound, NULL, SND_FILENAME | SND_ASYNC );
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnButton1() 

{
	CString str;
	
	CColorDialog cd;

	//memset(&cd.m_cc, 0, sizeof(CHOOSECOLOR));
	//cd.m_cc.lStructSize = sizeof(CHOOSECOLOR);
	
	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = confcol.memocolor;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		confcol.memocolor = cd.m_cc.rgbResult;
		m_r1.SetColor(confcol.memocolor);

		// Send refresh message to all stickers
		YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
		//ASSERT(cc);
	
		cc->HideMemos(); 
		cc->ShowMemos();
		}	
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnButton3() 

{
	CString str;
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = confcol.memorcolor;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		confcol.memorcolor = cd.m_cc.rgbResult;
		m_r3.SetColor(confcol.memorcolor);

		// Send refresh message to all stickers
		YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
		//ASSERT(cc);
	
		cc->HideMemos(); 
		cc->ShowMemos();
		}	
}

//////////////////////////////////////////////////////////////////////////

BOOL CSetup::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	CenterWindow(GetDesktopWindow());

	helpcontext = 2;

	// Init custom color array
	//memset(custom, 0, sizeof(custom));

	m_wav = (const TCHAR *)alarmsound;
	m_notify = (const TCHAR *)notifysound;

	//CAlarm *al = new CAlarm(this);
	////ASSERT(al);

	//al->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);	

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), true);
	
	//al->title = "Test Alarm Window";
	
	m_refresh.Format(_T("%d"), folder_refresh);
	m_spin1.SetRange(0, 300);
	m_spin1.SetPos(folder_refresh);

	if(folder_refresh > 0)
		{
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(true);
		}
	else
		{
		((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(false);
		}

	((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(auto_popup);

	int flag = AfxGetApp()->GetProfileInt(_T("Setup"), _T("flag12"), 0);
	((CButton *)GetDlgItem(IDC_CHECK4))->SetCheck(flag);

	((CButton *)GetDlgItem(IDC_CHECK10))->SetCheck(notifyflag);
	((CButton *)GetDlgItem(IDC_CHECK11))->SetCheck(beepflag);
	((CButton *)GetDlgItem(IDC_CHECK16))->SetCheck(auto_netpop);
	((CButton *)GetDlgItem(IDC_CHECK14))->SetCheck(voiceflag);
	((CButton *)GetDlgItem(IDC_CHECK15))->SetCheck(stop_standby);

	// Load old custom array:
	CString section(_T("Colors"));
	CString colentry;

	for(int loop = 0; loop < 16; loop++)
		{
		colentry.Format(_T("custom%02d"), loop);
		custom[loop] = AfxGetApp()->GetProfileInt(section, colentry, 0);
		}

	// Overwrite 
	custom[0] = RGB(0xff, 0xff, 0x80);
	custom[1] = RGB(0xff, 0xa0, 0x80);

	custom[2] = RGB(0xf0, 0xf0, 0x00);
	custom[3] = RGB(0xf0, 0xb0, 0x00); 

	custom[4] = RGB(0xe0, 0xe0, 0x0);
	custom[5] = RGB(0xff, 0xff, 0x64);

	m_r1.SetColor(confcol.memocolor);
	m_r2.SetColor(confcol.memohead);
	m_r3.SetColor(confcol.memorcolor);
	m_r4.SetColor(confcol.memorhead);

	m_r5.SetColor(confcol.alarmcolor);
	m_r6.SetColor(confcol.alarmhcolor);
	m_r7.SetColor(confcol.alarmbutton);
	m_r8.SetColor(confcol.alarmhbutton);
	
	m_userdir = dataroot;

	m_slide1.SetRangeMin(20);
	m_slide1.SetRangeMax(255);

	m_slide2.SetRangeMin(00);
	m_slide2.SetRangeMax(255);

	m_slide3.SetRangeMin(300);
	m_slide3.SetRangeMax(3000);
	m_slide3.SetPos(beepfreq);

	m_slide4.SetRangeMin(20);
	m_slide4.SetRangeMax(200);
	m_slide4.SetPos(beeplen);

	m_slide5.SetRangeMin(50);
	m_slide5.SetRangeMax(255);
	m_slide5.SetPos(bAlpha);

	m_slide6.SetRangeMin(50);
	m_slide6.SetRangeMax(255);
	m_slide6.SetPos(ala_trans);

	m_slide1.SetPos(main_trans);
	m_slide2.SetPos(idle_trans);	

	//int mm, xx;	m_spin3.GetRange(mm, xx);
	//int rev = mm - (int)memo_timeout;

	CString tmp; tmp.Format(_T("%d"), (int)memo_timeout);

	m_spin3.SetPos((int)memo_timeout);
	m_edit4.SetWindowText(tmp);

	UpdateData(false);

	m_lab1.m_BackColor = m_BackColor;
	m_lab2.m_BackColor = m_BackColor;
	m_lab3.m_BackColor = m_BackColor;
	m_lab4.m_BackColor = m_BackColor;
	m_lab5.m_BackColor = m_BackColor;
	m_lab15.m_BackColor = m_BackColor;
	m_lab16.m_BackColor = m_BackColor;
	m_lab17.m_BackColor = m_BackColor;
	
	//m_slide1.m_BackColor = m_BackColor;
	//m_slide2.m_BackColor = m_BackColor;
	//m_slide3.m_BackColor = m_BackColor;
	//m_slide4.m_BackColor = m_BackColor;
	//m_slide5.m_BackColor = m_BackColor;
	//m_slide6.m_BackColor = m_BackColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnDestroy() 

{
	CDialog::OnDestroy();
	
	helpcontext = 0;

	// Save color array
	CString section(_T("Colors"));
	CString colentry;

	for(int loop = 0; loop < 16; loop++)
		{
		colentry.Format(_T("custom%02d"), loop);
		AfxGetApp()->WriteProfileInt(section, colentry, custom[loop]);
		}
}

void CSetup::OnButton10() 

{
	CString str;
	CColorDialog cd;

	//cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR;
	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = confcol.memorhead;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		confcol.memorhead = cd.m_cc.rgbResult;
		m_r4.SetColor(confcol.memorhead);

		// Send refresh message to all stickers
		YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
		//ASSERT(cc);
	
		cc->HideMemos(); 
		cc->ShowMemos();
		}	
}

void CSetup::OnButton11() 

{
	confcol.memocolor = RGB(0xff, 0xff, 0x80);
	confcol.memorcolor = RGB(0xff, 0xa0, 0x80);
	confcol.memohead = RGB(0xf0, 0xf0, 0x00);
	confcol.memorhead= RGB(0xf0, 0xb0, 0x00); 

	m_r1.SetColor(confcol.memocolor);
	m_r2.SetColor(confcol.memohead);
	m_r3.SetColor(confcol.memorcolor);
	m_r4.SetColor(confcol.memorhead);

	// Send refresh message to all stickers
	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(cc);

	cc->HideMemos(); 
	cc->ShowMemos();

	Invalidate(); SetFocus();
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnButton13() 

{
    LOGFONT lf;

	memo_font.GetObject(sizeof(LOGFONT), &lf);
    CFontDialog dlg(&lf,  CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS);

	dlg.m_cf.rgbColors = memo_fontcol;

    if (dlg.DoModal() == IDOK)
		{
        // Switch to new font.
        memo_font.DeleteObject();
        if (!memo_font.CreateFontIndirect(&lf))
			{
            MessageBox(_T("Cannot set CryptoSticky font"));
			}
		else
			{
			// Send refresh message to all stickers
			YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
			//ASSERT(cc);
	
			memo_fontcol = (int)dlg.m_cf.rgbColors;
			
			cc->HideMemos(); 
			cc->ShowMemos();
			}
		}
}

void CSetup::OnButton14() 

{
	LOGFONT lf;

	head_font.GetObject(sizeof(LOGFONT), &lf);

    CFontDialog dlg(&lf,  CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS);

	dlg.m_cf.rgbColors = head_fontcol;

    if (dlg.DoModal() == IDOK)
		{
        // Switch to new font.
        head_font.DeleteObject();
        if (!head_font.CreateFontIndirect(&lf))
			{
            MessageBox(_T("Cannot set CryptoSticky header font"));
			}
		else
			{
			// Send refresh message to all stickers
			YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
			//ASSERT(cc);
		
			head_fontcol = (int)dlg.m_cf.rgbColors;

			cc->HideMemos(); 
			cc->ShowMemos();
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnButton15() 

{
	memo_font.DeleteObject();
	if(!memo_font.CreateFont(-13, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, FIXED_PITCH, _T("Aria")))
		{
		AfxMessageBox(_T("Cannot set Sticky font, defaulting to system font."));
		}

	head_font.DeleteObject();
	if(!head_font.CreateFont(-13, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, FIXED_PITCH, _T("Aria")))
		{
		AfxMessageBox(_T("Cannot set Sticky font, defaulting to system font."));
		}

	// Send refresh message to all stickers
	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(cc);

	cc->HideMemos(); 
	cc->ShowMemos();

	Invalidate(); SetFocus();
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnOK()

{
	if(want_refresh)
		{
		// Send refresh message to all stickers
		YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
		//ASSERT(cc);

		cc->HideMemos(); 
		cc->ShowMemos();	
		}

	if(modified)
		{
		OnButton30() ;
		}

	// Checbox overrides
	if(((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck() == 0)
		m_refresh = "";

	folder_refresh = _tstoi(m_refresh);
	//P2N(_T("Edit changed %s, \r\n"), m_refresh, folder_refresh);
	AfxGetApp()->WriteProfileInt(_T("Data"), _T("folder_refresh"), folder_refresh);

	auto_popup = ((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck();
	AfxGetApp()->WriteProfileInt(_T("Data"), _T("auto_popup"), auto_popup);

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	//P2N(_T("HScroll bar=%d -- pos=%d pos2=%d\r\n"), nSBCode, m_slide1.GetPos(), m_slide2.GetPos());

	UpdateData();


	// Live influence
	if(nSBCode == 8)
		{
		if(GetDlgItem(IDC_SLIDER3) == pScrollBar)
			{
			P2N(_T("freq %d\r\n"), m_slide3.GetPos());
			
			beepfreq = m_slide3.GetPos();
			Beep(beepfreq, beeplen);
	
			//AfxGetApp()->WriteProfileInt(snd, "beepfreq", beepfreq);
			}
		else if(GetDlgItem(IDC_SLIDER4) == pScrollBar)
			{
			P2N(_T("freq %d\r\n"), m_slide3.GetPos());
			
			beeplen = m_slide4.GetPos();
			Beep(beepfreq, beeplen);

			//AfxGetApp()->WriteProfileInt(snd, "beeplen", beeplen);
			}
		else if(GetDlgItem(IDC_SLIDER5) == pScrollBar)
			{
			P2N(_T("freq %d\r\n"), m_slide5.GetPos());
			
			bAlpha = m_slide5.GetPos();

			// Send refresh message to all stickers
			YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
			//ASSERT(cc);

			if(g_pSetLayeredWindowAttributes)
				g_pSetLayeredWindowAttributes(cc->m_hWnd, 0, bAlpha, LWA_ALPHA);

			cc->Invalidate();
			AfxGetApp()->WriteProfileInt(tran, _T("bAlpha"), bAlpha);
			}
		else if(GetDlgItem(IDC_SLIDER1) == pScrollBar)
			{
			main_trans = m_slide1.GetPos();
			idle_trans = m_slide2.GetPos();

			// Send refresh message to all stickers
			YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
			//ASSERT(cc);

			cc->HideMemos(); 
			cc->ShowMemos();
			
			AfxGetApp()->WriteProfileInt(tran, _T("main_trans"), main_trans);
			}
		else if(GetDlgItem(IDC_SLIDER2) == pScrollBar)
			{
			main_trans = m_slide1.GetPos();
			idle_trans = m_slide2.GetPos();

			// Send refresh message to all stickers
			//YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
			////ASSERT(cc);
			//cc->HideMemos(); 
			//cc->ShowMemos();

			AfxGetApp()->WriteProfileInt(tran, _T("idle_trans"), idle_trans);
			}
		else if(GetDlgItem(IDC_SLIDER6) == pScrollBar)
			{
			//P2N(_T("alarmalpha %d\r\n"), m_slide6.GetPos());
			
			ala_trans = m_slide6.GetPos();
			}
		}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnButton16() 

{
	main_trans = 240;
	idle_trans = 100;

	m_slide1.SetPos(main_trans);
	m_slide2.SetPos(idle_trans);

	// Send refresh message to all stickers
	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(cc);

	cc->HideMemos(); 
	cc->ShowMemos();	

	AfxGetApp()->WriteProfileInt(tran, _T("main_trans"), main_trans);
	AfxGetApp()->WriteProfileInt(tran, _T("idle_trans"), idle_trans);

	SetFocus();
}


void CSetup::OnButton19() 
{
	CString str;
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = confcol.alarmcolor;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		confcol.alarmcolor = cd.m_cc.rgbResult;
		m_r5.SetColor(confcol.alarmcolor);
		}		
}

void CSetup::OnButton20() 

{
	CString str;
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = confcol.alarmhcolor;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		confcol.alarmhcolor = cd.m_cc.rgbResult;
		m_r6.SetColor(confcol.alarmhcolor);

		}		
}

void CSetup::OnButton24() 

{
	CString str;
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = confcol.alarmbutton;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		confcol.alarmbutton = cd.m_cc.rgbResult;
		m_r7.SetColor(confcol.alarmbutton);
		}			
}

void CSetup::OnButton25() 

{
	CString str;
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = confcol.alarmhbutton;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		confcol.alarmhbutton = cd.m_cc.rgbResult;
		m_r8.SetColor(confcol.alarmhbutton);
		}			
}

void CSetup::OnButton21() 

{
	confcol.alarmcolor =	RGB(0xf0, 0xf0, 0xc0);
	confcol.alarmhcolor =	RGB(0xc0, 0xc0, 0xa0);
	confcol.alarmbutton =	RGB(255,255,0);
	confcol.alarmhbutton =  RGB(255,255,100);

	m_r5.SetColor(confcol.alarmcolor);
	m_r6.SetColor(confcol.alarmhcolor);
	m_r7.SetColor(confcol.alarmbutton);
	m_r8.SetColor(confcol.alarmhbutton);

	// Send refresh message to all stickers
	//YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	////ASSERT(cc);

	//cc->HideMemos(); 
	//cc->ShowMemos();
	//Invalidate(); SetFocus();

	//al->Create(al->IDD);
	//al->Show();
}

void CSetup::OnButton27() 
{
	beepfreq = 2000;
	beeplen  = 100;

	m_slide3.SetPos(beepfreq);
	m_slide4.SetPos(beeplen);

	Beep(beepfreq, beeplen);

	//AfxGetApp()->WriteProfileInt(snd, "beepfreq", beepfreq);
	//AfxGetApp()->WriteProfileInt(snd, "beeplen", beeplen);

}

void CSetup::OnButton23() 

{
	CFileDialogST dlgFile(true);
	CString  fileName;

	CString title = "Configure alarm sound"; 

	dlgFile.m_ofn.Flags |= OFN_EXPLORER ; //OFN_ALLOWMULTISELECT | ;

	dlgFile.m_ofn.nFilterIndex = 0;

	CString strDefExt = "wav";
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter;
	
	strFilter += "Wave files (*.wav)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.wav";
	strFilter += (TCHAR)NULL;
	
	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrInitialDir = approot;
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(1000 * _MAX_PATH);

	dlgFile.m_ofn.lpstrInitialDir = approot;

	int bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;

	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	//P2N(_T("Got file name %s\r\n"), fileName);

	alarmsound = m_wav = fileName;
	UpdateData(false);

	//AfxGetApp()->WriteProfileString(snd, "alarmsound", alarmsound);
}


void CSetup::OnButton22() 

{
	alarmsound = m_wav = approot + def_sound;
	UpdateData(false);

	//AfxGetApp()->WriteProfileString(snd, "alarmsound", alarmsound);
}


void CSetup::OnButton5() 

{
	CDlgGetPath gp; gp.m_BackColor = dlgback;

	gp.SetPath(m_userdir);

	if(gp.DoModal() == IDCANCEL)
		return;

	P2N(_T("Got dir: '%s'\r\n"), gp.GetPath());

	m_userdir = gp.GetPath();
	UpdateData(false);
	modified = true;

	OnButton30();
}


void CSetup::OnButton12() 

{
	//P2N(_T("Restore default Path\r\n"));

	m_userdir = userdir + "CryptoSticky\\";
	UpdateData(false);
	
	//if(modified)
		{
		OnButton30();
		}
}

void CSetup::OnButton18() 

{
	bAlpha = 240;

	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(cc);

	if(g_pSetLayeredWindowAttributes)
		g_pSetLayeredWindowAttributes(cc->m_hWnd, 0, bAlpha, LWA_ALPHA);

	m_slide5.SetPos(bAlpha);

	cc->Invalidate();

	AfxGetApp()->WriteProfileInt(_T(""), _T("bAlpha"), bAlpha);

}

void CSetup::OnOk2() 

{
	EndDialog(IDOK);
}

void CSetup::OnButton28() 

{
	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(cc);

	cc->ShowMissed();
}

void CSetup::OnButton29() 

{	
	YellowDlg *ccc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(ccc);

	ccc->HideMemos();

		// Remove all that is in the past
	time_t tt =	time(NULL);

	HKEY	key;
	LONG	ret = RegOpenKey(HKEY_CURRENT_USER,
		_T("Software\\RobotMonkeySoftware\\CryptoSticky\\Alarms"), &key);

	// Remove all entries
	TCHAR	kname[_MAX_PATH]; 
	BYTE	kval[_MAX_PATH];
	int idx = 0;
	while(true)
		{
		DWORD ksize = _MAX_PATH, kvsize = _MAX_PATH, ktype;

		kname[0] = '\0'; kval[0]  = '\0';

		LONG ret = RegEnumValue(key, idx, kname, &ksize, 0, &ktype, kval, &kvsize);

		//if(ret != ERROR_SUCCESS && ret != ERROR_NO_MORE_ITEMS)
		//	break;

		if(ret != ERROR_SUCCESS)
			break;

		//time_t att =  (*(time_t*)kvname);

		//if(ktype == REG_DWORD)
		if(ktype == REG_SZ)
			{
			//P2N(_T("Reg enumeration %s, %s\r\n"), kname, support.time_t2str(att));
			//P2N(_T("\r\nReg del enumeration '%s' '%s'\r\n"), kname, kval);

			struct tm stm; memset(&stm, 0, sizeof(stm));
			
			//int Month, Day, Year, Hour, Minute;
			//Month = Day = Year = Hour = Minute = 0;

			// Decompile date from reg
			sscanf((const char*)kval, "%02d/%02d/%04d %02d:%02d", 
				&stm.tm_mon, &stm.tm_mday, &stm.tm_year, &stm.tm_hour,
						&stm.tm_min);

			//P2N(_T("Decomp date %02d-%02d-%04d  %02d:%02d\r\n"), 
			//	stm.tm_mday, stm.tm_mon, stm.tm_year, stm.tm_hour, stm.tm_min);

			// Fix tm structure
			stm.tm_year -= 1900; stm.tm_mon -= 1; stm.tm_hour -= 1;			
			time_t att =  mktime(&stm);

			 //P2N(_T("Unconvert '%s'\r\n"), support.time_t2str(att));

				// Alarm in the past?
				if(att < tt)
					{
					//P2N(_T("Reg delete %s - %s\r\n"), kname, support.time_t2str(att));			
					LONG ret = RegDeleteValue(key, kname);
					}
			}
		idx++;
		if(ret == ERROR_NO_MORE_ITEMS)
			{
			//P2N(_T("No more Items\r\n"));
			break;
			}
		}

	// Fill it back up
	ccc->ArmNext();
}

void CSetup::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{	
	if(GetDlgItem(IDC_SPIN3) == pScrollBar)
		{
		//int mm, xx;	m_spin3.GetRange(mm, xx);
		//int rev = mm - nPos;

		CString tmp; tmp.Format(_T("%d"), nPos);
		//P2N(_T("Spin %d\r\n"), nPos);
		m_edit4.SetWindowText(tmp);
		memo_timeout = (int)nPos;
		want_refresh = true;
		}

	if(GetDlgItem(IDC_SPIN1) == pScrollBar)
		{
		m_refresh.Format(_T("%d"), nPos);
		UpdateData(false);
		if(nPos > 0)
			{
			((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(true);
			}
		else
			{
			((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(false);
			}
		}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSetup::OnButton26() 

{
	//m_refresh.Format(_T("%d"), 0);
	//((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(false);
	//UpdateData(false);

	// CFileDialog
	CFileDialogST dlgFile(true);
	CString  fileName;

	CString title = "Import Stickies"; 

	dlgFile.m_ofn.Flags |= OFN_EXPLORER  | OFN_ALLOWMULTISELECT;

	dlgFile.m_ofn.nFilterIndex = 0;

	CString strDefExt = "txt";
	dlgFile.m_ofn.lpstrDefExt = strDefExt;

	dlgFile.m_ofn.lpstrInitialDir = userdir;

	CString strFilter;
	
	strFilter += "Sticky files (*.txt)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.txt";
	strFilter += (TCHAR)NULL;
	
	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	
	dlgFile.m_ofn.nMaxFile = 1000 * _MAX_PATH;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(1000 * _MAX_PATH);
	
	//dlgFile.m_ofn.lpstrInitialDir = appdata;

	int bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;

	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);	//ASSERT(cc);

	POSITION pos = dlgFile.GetStartPosition( );
	while(true)
		{
		if(!pos) 
			break;
		CString str = dlgFile.GetNextPathName(pos);
		cc->ImportMemo(str);		
		}
	cc->ReadAllMemos(true, true, false);
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnChangeEdit2() 

{
	UpdateData();

	//P2N(_T("Edit changed %s\r\n"), m_refresh);
	m_spin1.SetPos(_tstoi(m_refresh));
}

void CSetup::OnKillfocusEdit2() 
{
	
}

void CSetup::OnCheck1() 

{
	if(((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck() == 0)
		{
		m_refresh = _T("0");
		UpdateData(false);
		m_spin1.SetPos(_tstoi(m_refresh));
		}	
	else
		{
		m_refresh = _T("1");
		UpdateData(false);
		m_spin1.SetPos(_tstoi(m_refresh));
		}
}

void CSetup::OnButton30() 

{
	UpdateData();
	
	// Send close message to all stickers
	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(cc);

	cc->HideMemos(); 
	dataroot = m_userdir;

	// Send refresh to main window, complete refresh
	cc->ReadAllMemos(true, false, false);
	cc->Iterate_Checks(NULL );	

	if(cc->m_auto)
		cc->ShowMemos();

	AfxGetApp()->WriteProfileString(_T("Data"), _T("dataroot"), dataroot);	

	modified = false;
}

void CSetup::OnChangeEdit1() 

{
	modified = true;	
}

void CSetup::OnCheck4() 

{
	int flag = ((CButton *)GetDlgItem(IDC_CHECK4))->GetCheck();

	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(cc);

	cc->flag12 = flag;
	AfxGetApp()->WriteProfileInt(_T("Setup"), _T("flag12"), flag);
}

void CSetup::OnCheck10() 

{
	int flag = ((CButton *)GetDlgItem(IDC_CHECK10))->GetCheck();

	notifyflag = flag;
	//AfxGetApp()->WriteProfileInt(_T("Sound", "notifyflag"), flag);
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnButton32() 

{
	CFileDialogST dlgFile(true);
	CString  fileName;

	CString title = "Configure Notify sound"; 

	dlgFile.m_ofn.Flags |= OFN_EXPLORER ; //| OFN_ALLOWMULTISELECT ;

	dlgFile.m_ofn.nFilterIndex = 0;

	CString strDefExt = "wav";
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter;
	
	strFilter += "Wave files (*.wav)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.wav";
	strFilter += (TCHAR)NULL;
	
	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	dlgFile.m_ofn.lpstrInitialDir = approot;
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(1000 * _MAX_PATH);

	//dlgFile.m_ofn.lpstrInitialDir = appdata;

	int bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;

	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	//P2N(_T("Got file name %s\r\n"), fileName);

	notifysound = m_notify = fileName;
	UpdateData(false);

	//AfxGetApp()->WriteProfileString(snd, "notifysound", notifysound);	
}

void CSetup::OnButton33() 

{
	PlaySound(notifysound, NULL, SND_FILENAME | SND_ASYNC );	
}

void CSetup::OnButton31() 

{
	notifysound = m_notify = approot + def_notify;
	UpdateData(false);
	//AfxGetApp()->WriteProfileString(snd, "notifysound", notifysound);	
}

void CSetup::OnCheck11() 

{
	int flag = ((CButton *)GetDlgItem(IDC_CHECK11))->GetCheck();
	beepflag = flag;
	//AfxGetApp()->WriteProfileInt(_T("Sound", "beepflag"), flag);	
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnButton36() 

{
	CDlgGetPath gp;	gp.m_BackColor = dlgback;
	gp.SetPath(m_userdir);

	gp.header = "Select directory to export to";
	gp.SetPath(userdir);

	if(gp.DoModal() == IDCANCEL)
		return;

	P2N(_T("Export into dir '%s'\r\n"), gp.GetPath());	

	YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	//ASSERT(cc);

	if(cc->ExportMemos(gp.GetPath()) < 0)
		{
		AfxMessageBox(_T("One or more Sticky could not be exported\r\n"));
		}
	else
		{
		CString tmp;
		tmp.Format(_T("Exported all Sticky to: '%s'"), gp.GetPath());
		
		AfxMessageBox(tmp);
		}
}


//////////////////////////////////////////////////////////////////////////

void CSetup::OnButton37() 

{	
	CNetPeerDlg	netp;
	netp.m_BackColor = dlgback;
	netp.DoModal();
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnCheck14() 

{
	int flag = ((CButton *)GetDlgItem(IDC_CHECK14))->GetCheck();
	voiceflag = flag;	
}

//////////////////////////////////////////////////////////////////////////

void CSetup::OnCheck15() 

{
	int flag = ((CButton *)GetDlgItem(IDC_CHECK15))->GetCheck();
	stop_standby = flag;
}

BOOL CSetup::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

void CSetup::OnBnClickedLab16()
{
	// TODO: Add your control notification handler code here
}

HBRUSH CSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
#if 1
	HBRUSH hbr2;

	//P2N(_T("CSetup::OnCtlColor nCtlColor=%x dlgback=%x\r\n"), nCtlColor, dlgback);

	if(nCtlColor == CTLCOLOR_STATIC)
		{
		hbr2 = CreateSolidBrush(dlgback);
		pDC->SetBkColor(dlgback);
		}
	else if(nCtlColor == CTLCOLOR_EDIT)
		{
		//hbr2 = CreateSolidBrush(dlgback);
		hbr2 = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}
	else if(nCtlColor == CTLCOLOR_LISTBOX)
		{
		hbr2 = CreateSolidBrush(dlgback);
		pDC->SetBkColor(dlgback);

		}
	else if(nCtlColor == CTLCOLOR_DLG)
		{
		hbr2 = CreateSolidBrush(dlgback);
		pDC->SetBkColor(dlgback);
		}
	else if(nCtlColor == CTLCOLOR_BTN)
		{
		hbr2 = CreateSolidBrush(dlgback);
		pDC->SetBkColor(dlgback);
		}

	else
		{		
		hbr2 = CreateSolidBrush(dlgback);
		}
	// TODO:  Change any attributes of the DC here
	
	return hbr2;
	
#else
	// TODO:  Return a different brush if the default is not desired
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	return hbr;
#endif

	
}

void CSetup::OnBnClickedCheck16()
{
	int flag = ((CButton *)GetDlgItem(IDC_CHECK16))->GetCheck();
	auto_netpop = flag;	
}

void CSetup::OnBnClickedButton39()
{
	CLogDlg		logdlg;
	P2N(_T("Log dialog called\r\n"));
	logdlg.m_BackColor = dlgback;
	logdlg.DoModal();
}

void CSetup::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

// MemoConf.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "yellowdlg.h"
#include "MemoConf.h"
#include "FileDialogST.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemoConf dialog


CMemoConf::CMemoConf(CWnd* pParent /*=NULL*/)
	: CDialog(CMemoConf::IDD, pParent)
{
	m_BackColor = RGB(192,192,192);

	//{{AFX_DATA_INIT(CMemoConf)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMemoConf::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemoConf)
	DDX_Control(pDX, IDC_COMBO2, m_c2);
	DDX_Control(pDX, IDC_COMBO1, m_c1);
	DDX_Control(pDX, IDC_BUTTON12, m_b12);
	DDX_Control(pDX, IDC_BUTTON11, m_b11);
	DDX_Control(pDX, IDC_BUTTON10, m_b10);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_BUTTON4, m_b4);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_HCOL2, m_mc);
	DDX_Control(pDX, IDC_HCOL, m_hc);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON13, m_img);
	DDX_Control(pDX, IDC_SLIDER1, m_alpha);
	DDX_Control(pDX, IDC_HCOL3, m_txtfont);
	DDX_Control(pDX, IDC_BUTTON7, m_fontcol);
	DDX_Control(pDX, IDC_HCOL4, m_headfont);
	DDX_Control(pDX, IDC_BUTTON8, m_headfontbutt);
	DDX_Control(pDX, IDC_BUTTON14, m_remimg);
}


BEGIN_MESSAGE_MAP(CMemoConf, CDialog)
	//{{AFX_MSG_MAP(CMemoConf)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnButton12)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeCombo2)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//ON_BN_CLICKED(IDC_BUTTON13, &CMemoConf::OnBnClickedButton13)
	//ON_BN_CLICKED(IDC_BUTTON14, &CMemoConf::OnBnClickedButton14)
	//ON_BN_CLICKED(IDCANCEL, &CMemoConf::OnBnClickedCancel)
	ON_WM_HSCROLL()
	//ON_BN_CLICKED(IDC_BUTTON7, &CMemoConf::OnBnClickedButton7)
	//ON_BN_CLICKED(IDC_BUTTON8, &CMemoConf::OnBnClickedButton8)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoConf message handlers

void CMemoConf::OnButton1() 

{
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = buddy->headcol;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		buddy->headcol = cd.m_cc.rgbResult;
		m_hc.SetColor(buddy->headcol);
		
		// Send refresh message		

		buddy->Refresh();
		}		
}

//////////////////////////////////////////////////////////////////////////

BOOL CMemoConf::OnInitDialog() 

{
	CDialog::OnInitDialog();
	
	//ASSERT(buddy->magic == CYmemo_Magic);

	buddy->StartFadeCycle();

	// Get initial settings from original
	m_hc.SetColor(buddy->headcol);
	m_mc.SetColor(buddy->textcol);
	
	m_txtfont.SetColor(buddy->memo_fontcol2);
	m_headfont.SetColor(buddy->head_fontcol2);

	GetDlgItem(IDC_EDIT1)->SetWindowText(buddy->backimage);

	LOGFONT lf;
	buddy->myhead_font.GetObject(sizeof(LOGFONT), &lf);
	CString str; str.Format(_T("%d"), abs(lf.lfHeight));
	m_c1.SelectString(0, str);

	LOGFONT lf2;
	buddy->mymemo_font.GetObject(sizeof(LOGFONT), &lf2);
	CString str2; str2.Format(_T("%d"), abs(lf2.lfHeight));
	m_c2.SelectString(0, str2);

	m_alpha.SetRange(0, 255);

	m_alpha.SetPos(buddy->alpha);

	// Fill in dropdown sizes
	for(int loop = 10; loop <= 96; loop++)
		{
		CString str; str.Format(_T("%d"), loop);

		m_c1.AddString(str);
		m_c2.AddString(str);

		if(loop >= 18)
			loop++;
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMemoConf::OnButton2() 

{
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = buddy->headcol;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		buddy->textcol = cd.m_cc.rgbResult;
		m_mc.SetColor(buddy->textcol);
		
		// Send refresh message				
		buddy->Refresh();
		}			
}

//////////////////////////////////////////////////////////////////////////

void CMemoConf::OnButton3() 

{
	LOGFONT lf;

	buddy->myhead_font.GetObject(sizeof(LOGFONT), &lf);
	
    CFontDialog dlg(&lf,  CF_NOFACESEL | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS);

 	dlg.m_cf.rgbColors = buddy->head_fontcol2;

    if (dlg.DoModal() == IDOK)
		{
        // Switch to new font.
        buddy->myhead_font.DeleteObject();
		buddy->myhead_fontb.DeleteObject();
		
        buddy->myhead_font.CreateFontIndirect(&lf);
		lf.lfWeight = FW_BOLD;
		buddy->myhead_fontb.CreateFontIndirect(&lf);
			
		buddy->head_fontcol2 = dlg.m_cf.rgbColors;

		// Send refresh message		
		buddy->headsize = lf.lfHeight;
		buddy->Refresh();

		}
}

//////////////////////////////////////////////////////////////////////////

void CMemoConf::OnButton4() 

{
	LOGFONT lf;

	buddy->mymemo_font.GetObject(sizeof(LOGFONT), &lf);

    CFontDialog dlg(&lf,  CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS);

	dlg.m_cf.rgbColors = buddy->memo_fontcol2;

    if (dlg.DoModal() == IDOK)
		{
        // Switch to new font.
        buddy->mymemo_font.DeleteObject();
		
        if (!buddy->mymemo_font.CreateFontIndirect(&lf))
			{
            MessageBox(_T("Cannot set CryptoSticky font"));
			}
		else
			{
			buddy->memo_fontcol2 = dlg.m_cf.rgbColors;
				
			// Send refresh message		
			buddy->textsize = lf.lfHeight;
			buddy->Refresh();
			}
		}		
}

//////////////////////////////////////////////////////////////////////////

void CMemoConf::OnButton10() 

{
	//buddy->headcol = confcol.memohead;
	//buddy->head_fontcol2 = head_fontcol;
	
	m_hc.SetColor(buddy->headcol);
	m_mc.SetColor(buddy->textcol);
	m_headfont.SetColor(buddy->head_fontcol2);
		
	LOGFONT lf;

	head_font.GetObject(sizeof(LOGFONT), &lf);
	buddy->myhead_font.DeleteObject();
	buddy->myhead_font.CreateFontIndirect(&lf);

	buddy->myhead_fontb.DeleteObject();
	lf.lfWeight = FW_BOLD;
	buddy->myhead_fontb.CreateFontIndirect(&lf);
	
	CString str; str.Format(_T("%d"), abs(lf.lfHeight));
	m_c1.SelectString(0, str);

	buddy->headsize = lf.lfHeight;			
	buddy->Refresh();
}

//////////////////////////////////////////////////////////////////////////

void CMemoConf::OnButton11() 

{
	//buddy->textcol = confcol.memocolor;
	//buddy->memo_fontcol2 = memo_fontcol;

	m_txtfont.SetColor(buddy->memo_fontcol2);	
	m_hc.SetColor(buddy->headcol);
	m_mc.SetColor(buddy->textcol);

	LOGFONT lf;

	memo_font.GetObject(sizeof(LOGFONT), &lf);
	buddy->mymemo_font.DeleteObject();
	buddy->mymemo_font.CreateFontIndirect(&lf);	

	CString str; str.Format(_T("%d"), abs(lf.lfHeight));
	m_c2.SelectString(0, str);

	buddy->textsize = lf.lfHeight;			
	buddy->Refresh();
}

void CMemoConf::OnButton12() 

{
	buddy->AutoSize();
	buddy->Refresh();
}

void CMemoConf::OnSelchangeCombo1() 

{
	LOGFONT lf;

	CString str;

	buddy->myhead_font.GetObject(sizeof(LOGFONT), &lf);

	int curr = m_c1.GetCurSel();
	
	if(curr >= 0)
		{
		m_c1.GetLBText(curr, str);
		int val = _tstoi(str);

		lf.lfHeight = -val;
		
		//P2N(_T("New Height %d\r\n"), lf.lfHeight);
		
		buddy->myhead_font.DeleteObject();
		buddy->myhead_font.CreateFontIndirect(&lf);	

		buddy->myhead_fontb.DeleteObject();
		lf.lfWeight = FW_BOLD;
		buddy->myhead_fontb.CreateFontIndirect(&lf);
	
		buddy->headsize = val;
		buddy->Refresh();
		}
}

void CMemoConf::OnSelchangeCombo2() 

{
	LOGFONT lf;

	CString str;

	buddy->mymemo_font.GetObject(sizeof(LOGFONT), &lf);

	int curr = m_c2.GetCurSel();
	
	if(curr >= 0)
		{
		m_c2.GetLBText(curr, str);
		int val = _tstoi(str);

		lf.lfHeight = -val;
		
		//P2N(_T("New Height %d\r\n"), lf.lfHeight);
		
		buddy->mymemo_font.DeleteObject();
		buddy->mymemo_font.CreateFontIndirect(&lf);	

		buddy->textsize = val;
		buddy->Refresh();
		}
}

BOOL CMemoConf::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
	//return CGrayDlg::OnEraseBkgnd(pDC);
}

HBRUSH CMemoConf::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
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
}

void CMemoConf::OnBnClickedButton13()

{
	//P2N(_T("CMemoConf::OnBnClickedButton13\r\n"));

	CFileDialogST dlgFile(true);
	CString  fileName;

	CString title = "Set Sticky Image"; 

	dlgFile.m_ofn.Flags |= OFN_EXPLORER ; //| OFN_ALLOWMULTISELECT ;

	dlgFile.m_ofn.nFilterIndex = 0;

	CString strDefExt = "jpg";
	dlgFile.m_ofn.lpstrDefExt = strDefExt;
		
	CString strFilter;
	
	strFilter += "JPG files (*.jpg)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.jpg";
	strFilter += (TCHAR)NULL;
	
	strFilter += "GIF files (*.gif)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.gif";
	strFilter += (TCHAR)NULL;
	
	strFilter += "PNG files (*.png)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.png";
	strFilter += (TCHAR)NULL;
	
	strFilter += "BMP files (*.bmp)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.bmp";
	strFilter += (TCHAR)NULL;
	
	strFilter += "All Files (*.*)";
	strFilter += (TCHAR)NULL;
	strFilter += "*.*";
	strFilter += (TCHAR)NULL;
	strFilter += (TCHAR)NULL;

	CString homedir; support.GetHomeDir(homedir);
	dlgFile.m_ofn.lpstrInitialDir = homedir;
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(10 * _MAX_PATH);

	//dlgFile.m_ofn.lpstrInitialDir = appdata;

	int bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;

	fileName.ReleaseBuffer();

	if(!bRet)
		return;

	//P2N(_T("Got file name '%s'\r\n"), fileName);

	GetDlgItem(IDC_EDIT1)->SetWindowText(fileName);


	//notifysound = m_notify = fileName;
	UpdateData(false);

	buddy->backimage = fileName;

	CString section(_T("BackImage"));
	CString fname = buddy->memo.MakeFileName();
	P2N(_T("Writing registry: %s %s\r\n"), fname, fileName);
	AfxGetApp()->WriteProfileString(section, fname, fileName);
	
	buddy->Refresh();

	//AfxGetApp()->WriteProfileString(snd, "notifysound", notifysound);	
}

void CMemoConf::OnBnClickedButton14()
{
	CString section(_T("BackImage"));
	buddy->backimage = "none";

	GetDlgItem(IDC_EDIT1)->SetWindowText(_T(""));

	CString fname = buddy->memo.MakeFileName();

	//AfxGetApp()->WriteProfileString(section, fname, "none");

	HKEY key;
	LONG ret = RegOpenKey(HKEY_CURRENT_USER,
		_T("Software\\RobotMonkeySoftware\\CryptoSticky\\BackImage"), &key);

	if(ret == ERROR_SUCCESS)
		{
		RegDeleteValue(key, fname);
		RegCloseKey(key);
		}

	//AfxGetApp()->DelRegTree(
	
	buddy->Refresh();
}

void CMemoConf::OnBnClickedCancel()
{
	OnCancel();
}

	void CMemoConf::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData();

	// Live influence
	//CSlider *psl = (CSlider *)GetDlgItem(IDC_SLIDER1);
	if(nSBCode == 8)
		{
		if(GetDlgItem(IDC_SLIDER1) == pScrollBar)
			{
			//P2N(_T("Alpha %d\r\n"), m_alpha.GetPos());
			buddy->alpha = m_alpha.GetPos();
			buddy->StartFadeCycle();
			buddy->Invalidate();
			}
		}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMemoConf::OnBnClickedButton7()
{
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = buddy->memo_fontcol2;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		buddy->memo_fontcol2 = cd.m_cc.rgbResult;
		m_txtfont.SetColor(buddy->memo_fontcol2);
		
		// Send refresh message				
		buddy->Refresh();
		}			
}

void CMemoConf::OnBnClickedButton8()
{
	CColorDialog cd;

	cd.m_cc.Flags = CC_RGBINIT | CC_FULLOPEN | CC_ANYCOLOR | CC_ENABLEHOOK;
	cd.m_cc.rgbResult = buddy->head_fontcol2;
	cd.m_cc.lpCustColors = custom;

	if(cd.DoModal() == IDOK)
		{
		buddy->head_fontcol2 = cd.m_cc.rgbResult;
		m_headfont.SetColor(buddy->head_fontcol2);
		
		// Send refresh message				
		buddy->Refresh();
		}	
}

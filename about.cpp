
/* =====[ about.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the about.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  9/20/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"

#include <direct.h>

#include "CryptoSticky.h"
#include "about.h"
#include "mxpad.h"
#include "label.h"
#include "register.h"
#include "logdlg.h"
#include "XntMask.h"

CAboutDlg::CAboutDlg() : CGrayDlg(CAboutDlg::IDD, NULL)

{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT

	//m_Bitmap.LoadBitmap(IDB_BITMAP15);
	m_Bitmap.LoadBitmap(IDB_BITMAP8);
	
	if(m_Bitmap.m_hObject != NULL)
		{
		m_Bitmap.GetBitmap(&BitMap);
		m_nWidth  = BitMap.bmWidth;
		m_nHeight = BitMap.bmHeight;
		}

	noheader = true;
	m_BackColor = RGB(192, 192, 192);
	//bAlpha = 255;
	noremember = true;

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_YMEM2, m_ym2);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON2, m_but2);
	DDX_Control(pDX, IDC_REG2, m_reg2);
	DDX_Control(pDX, IDC_REG, m_reglabel);
	DDX_Control(pDX, IDC_BUTTON1, m_but1);
	DDX_Control(pDX, IDREG, m_reg);
	DDX_Control(pDX, IDC_YMEM, m_ym);
	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BLUEPOINT, m_blue);
	DDX_Control(pDX, IDC_COPYRIGHT, m_copyr);
	DDX_Control(pDX, IDC_YMEM3, m_ym0);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CGrayDlg)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDREG, OnReg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


//#include "winver.h"

BOOL CAboutDlg::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();

	//VS_VERSIONINFO  vi;
	//int ret = GetFileVersionInfo(AfxGetApp()->m_pszExeName, 0, sizeof(vi), &vi);
	////ASSERT(ret);

	CString name = AfxGetApp()->GetProfileString(_T("License"), _T("Name"), _T("DEMO"));

	CString tmp; tmp.Format(_T("%s"), name);
	
	m_reg2.SetTrans();
	m_reg2.CenterText();
	m_reg2.SetFontSize(24);
	m_reg2.SetText(tmp);

	//SetAlpha(main_trans);

	m_ym0.CenterText();
	m_ym0.SetTrans();
	m_ym0.SetFontSize(36);
	m_ym0.SetFontBold(true);
	
	m_ym.CenterText();
	m_ym.SetTrans();
	m_ym.SetFontSize(24);
	m_ym.SetFontBold(true);

	m_ym2.SetTrans();
	m_ym2.CenterText();
	
	//CString str; str.LoadString(IDS_ABOUT); 
	//m_ym2.SetText(str); 
	
	m_reglabel.SetTrans();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnReg() 

{
	EndDialog(IDCANCEL);
}

void CAboutDlg::OnButton1() 

{
	CRegister rg; rg.m_BackColor = dlgback;
	rg.DoModal();
	
	CString name = AfxGetApp()->GetProfileString(_T("License"), _T("Name"), _T("DEMO") );

	CString tmp; tmp.Format(_T("%s"), name);
	m_reg2.SetText(tmp);
}

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{

	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));		

	CGrayDlg::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnButton2() 

{
	CString str(approot);

#ifdef _DEBUG
	str += "..\\html\\index.htm";
#else
	str += "html\\index.htm";
#endif
	
	ShellExecute(NULL, _T("open"), str, NULL, NULL, SW_SHOWMAXIMIZED);		
}

//////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnButton3() 

{
	extern CString buyURLstr;
	ShellExecute(NULL, _T("open"), buyURLstr, NULL, NULL, SW_SHOWMAXIMIZED);	
}

//////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnPaint() 

{
	
#if 1
	//CPaintDC dc(this); // device context for painting
	CClientDC dc(this); // device context for painting

	CRect rect; GetClientRect(&rect); 
	
	XntMask xmask; 

	CDC  dcMem; dcMem.CreateCompatibleDC(&dc);
	int  save = dcMem.SaveDC();

	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_Bitmap);

	HBITMAP hBmpMask = xmask.MakeMaskBmpDC(dcMem, m_nWidth, m_nHeight, RGB(255, 255, 255));

	int  xxx = rect.Width() / 2 - m_nWidth / 2;
	//int  yyy = rect.Height() / 2 - m_nHeight / 2;

	// Put it out
	xmask.MaskBlt(dc.GetSafeHdc(), xxx, 130,	
				m_nWidth, m_nHeight,
					dcMem, 0, 0, hBmpMask, 0, 0,
						MAKEROP4(SRCCOPY, DSTCOPY), 255);

	dcMem.SelectObject(pOldBitmap);
	dcMem.RestoreDC(save);
	
#endif

	CGrayDlg::OnPaint();

	// Do not call CGrayDlg::OnPaint() for painting messages
}

BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
	//return CGrayDlg::OnEraseBkgnd(pDC);
}

// Banner.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "Banner.h"
#include "XntMask.h"

// CBanner dialog

IMPLEMENT_DYNAMIC(CBanner, CDialog)

CBanner::CBanner(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CBanner::IDD, pParent)
{
	noheader = true;
	m_BackColor = RGB(192, 192, 192);
	//bAlpha = 200;
	noremember = true;
}

CBanner::~CBanner()
{
}

void CBanner::DoDataExchange(CDataExchange* pDX)
{
	CGrayDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEAD, m_head);
	DDX_Control(pDX, IDC_HEAD2, m_head2);
	DDX_Control(pDX, IDC_HEAD3, m_foot);
	DDX_Control(pDX, IDC_HEAD4, m_foot2);
	DDX_Control(pDX, IDC_HEAD5, m_shadow);
}


BEGIN_MESSAGE_MAP(CBanner, CDialog)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CBanner message handlers

BOOL CBanner::OnInitDialog()
{
	CGrayDlg::OnInitDialog();
	
	m_Bitmap.LoadBitmap(IDB_BITMAP13);
	
	if(m_Bitmap.m_hObject != NULL)
		{
		m_Bitmap.GetBitmap(&BitMap);
		m_nWidth  = BitMap.bmWidth;
		m_nHeight = BitMap.bmHeight;
		}
	
	m_head.CenterText();
	m_head.SetTrans();
	m_head.SetFontSize(48);
	m_head.SetFontBold(true);
	m_head.SetTextColor(RGB(200,200,200));

	m_shadow.CenterText();
	m_shadow.SetTrans();
	m_shadow.SetFontSize(48);
	m_shadow.SetFontBold(true);
	
	m_head2.CenterText();
	m_head2.SetTrans();
	m_head2.SetFontSize(24);
	m_head2.SetFontBold(true);

	m_foot.CenterText();
	m_foot.SetTrans();
	//m_foot.SetFontSize(24);
	//m_foot.SetFontBold(true);

	m_foot2.CenterText();
	m_foot2.SetTrans();
	m_foot2.SetFontSize(14);
	m_foot2.SetFontBold(true);

	SetTimer(1, 3000, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBanner::OnTimer(UINT_PTR nIDEvent)
{

	//P2N(_T("CBanner::OnTimer\r\n"));

	KillTimer(nIDEvent);
	OnOK();
	CGrayDlg::OnTimer(nIDEvent);
}

BOOL CBanner::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;

	//return CDialog::OnEraseBkgnd(pDC);
}

void CBanner::OnPaint()
{
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
	
	CGrayDlg::OnPaint();
}

void CBanner::OnLButtonDown(UINT nFlags, CPoint point)
{
	//P2N(_T("CBanner::OnLButtonDown \r\n"));
	OnOK();
	
	CGrayDlg::OnLButtonDown(nFlags, point);
}

void CBanner::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//P2N(_T("CBanner::OnKeyDown \r\n"));
	OnOK();
	CGrayDlg::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBanner::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//P2N(_T("CBanner::OnSysKeyDown \r\n"));
	OnOK();
	
	CGrayDlg::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


void CBanner::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CGrayDlg::OnActivate(nState, pWndOther, bMinimized);
	SetFocus();
	// TODO: Add your message handler code here
}

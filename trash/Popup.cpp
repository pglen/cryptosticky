
/* =====[ Popup.cpp ]========================================== 
                                                                             
   Description:     The yellowmemo project, implementation of the Popup.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// Popup.cpp : implementation file
//

#include "stdafx.h"
#include "yellowmemo.h"

//include "shades.h"

#include "Popup.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopup dialog


CPopup::CPopup(CWnd* pParent /*=NULL*/)
	: CDialog(CPopup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopup)
	//}}AFX_DATA_INIT

	bgc = RGB(0xff, 0xff, 0xee);
	padding = 12;
}

void CPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopup)

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopup, CDialog)
	//{{AFX_MSG_MAP(CPopup)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopup message handlers

BOOL CPopup::OnEraseBkgnd(CDC* pDC) 
{
	RECT rect; // ,m_rDataBox;
	GetClientRect(&rect);

	CBrush NewBrush;

	NewBrush.CreateSolidBrush(bgc) ;

	pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);
	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
	pDC->SelectObject(pOldBrush);
	
	//return CTransDlg::OnEraseBkgnd(pDC);
	return(false);
}


BOOL CPopup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BYTE	bAlpha = 230;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	//m_lf.lfHeight = -26;

	m_font.CreateFontIndirect(&m_lf);

	//m_str.SetTrans();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPopup::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	//P2N("CPopup::OnShowWindow tooltip\r\n");

	//SetTimer(1, 10000, NULL);
	CDialog::OnShowWindow(bShow, nStatus);
}

void CPopup::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("Key down on tooltip\r\n");
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CPopup::PreTranslateMessage(MSG* pMsg) 

{
	//P2N("Pretranslate message on tooltip:  %s\r\n", num2msg(pMsg->message));

	if(pMsg->message == WM_KEYDOWN)
		{
		ShowWindow(false);	
		return 0;
		}
	return CDialog::PreTranslateMessage(pMsg);
}


void CPopup::AutoSize()
	
{
	PAINTSTRUCT Paint;
	
	CDC* dc = BeginPaint( &Paint );

	CFont* oldFont = dc->SelectObject(&m_font);
	//CSize textsize = dc->GetTextExtent(str);
	CRect rc; dc->DrawText(str, &rc, DT_CALCRECT);
	dc->SelectObject(oldFont);
	
	EndPaint(&Paint);

	SetWindowPos(NULL, 0, 0, rc.Width() + padding, rc.Height() + padding, SWP_NOMOVE | SWP_NOZORDER);

	WINDOWPLACEMENT wp;	GetWindowPlacement(&wp); CRect rc2; rc2 = wp.rcNormalPosition;

	HMONITOR mon = MonitorFromRect(rc2, MONITOR_DEFAULTTONEAREST);
	MONITORINFO mi;	mi.cbSize = sizeof(mi);	GetMonitorInfo(mon, &mi);
 
	//P2N("mi.rcMonitor t=%d l=%d b=%d r=%d\r\n", 
	//	mi.rcMonitor.top, mi.rcMonitor.left, mi.rcMonitor.bottom, mi.rcMonitor.right);

	int  xsize = mi.rcMonitor.right, ysize = mi.rcMonitor.bottom;

	// Needs a turn around to show to the left
	if(rc2.right > xsize)
		{
		//P2N("Needs a turn around to show to the left\r\n");
		SetWindowPos( NULL, xsize - rc2.Width(), rc2.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE );  	
		}

	// Refresh window placement info
	GetWindowPlacement(&wp); rc2 = wp.rcNormalPosition;

	// Needs a turn around to show to the upper  side
	if(rc2.bottom > ysize)
		{
		//P2N("Needs a turn around to show to the upper side\r\n");
		SetWindowPos( NULL, rc2.left, ysize - rc2.Height(), 0, 0, SWP_NOZORDER | SWP_NOSIZE );  	
		}
}
			
void CPopup::SetText(const char *xstr)

{
	//P2N("Setting tooltip text: '%s'\r\n", xstr);
	str = xstr;
	AutoSize();
}

void CPopup::Move(CPoint screen)

{
	//SetWindowPos(NULL, screen.x, screen.y, 0, 0, SWP_NOSIZE  | SWP_NOZORDER);
}

void CPopup::OnPaint() 

{
	CPaintDC dc(this); // device context for painting

	CFont* oldFont = dc.SelectObject(&m_font);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,0));	

	CRect rc; GetClientRect(&rc);
	rc.left += padding/2; rc.top += padding/2;
	dc.DrawText(str, &rc, DT_NOPREFIX | DT_LEFT );
	
	dc.SelectObject(oldFont);

	// Do not call CDialog::OnPaint() for painting messages
}

void CPopup::OnTimer(UINT nIDEvent) 

{
	EndDialog(IDOK);
	CDialog::OnTimer(nIDEvent);
}

void CPopup::SetBackColor(COLORREF rgb)
{
	bgc = rgb;
	Invalidate();
}

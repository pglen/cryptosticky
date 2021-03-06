
/* =====[ MsgBox.cpp ]========================================== 
                                                                             
   Description:     The xraynotes project, implementation of the MsgBox.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/24/2008  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// MsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"

#include "MsgBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgBox dialog

CMsgBox::CMsgBox(CWnd* pParent /*=NULL*/)
	: CGrayDlg(CMsgBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgBox)
	//}}AFX_DATA_INIT

	drag		= 0;
	yesno		= MBTYPE_YESOK;
	idret		= 0;
	tocount		= 0;
	timeout		= AUTOCLOSE;
	//bAlpha		= 240;
	bAlpha		= 255;

	m_centerdlg = true; 
	noremember = true;
	windowname = "CMsgBox";
	bgcolor = RGB(196, 196, 196);
}

void CMsgBox::DoDataExchange(CDataExchange* pDX)

{
	CGrayDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgBox)
	DDX_Control(pDX, ID_MBOX_STOP, m_stop);
	DDX_Control(pDX, ID_MBOX_YES, m_yes);
	DDX_Control(pDX, ID_MBOX_NO, m_no);
	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsgBox, CGrayDlg)
	//{{AFX_MSG_MAP(CMsgBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_MBOX_YES, OnOk2)
	ON_BN_CLICKED(ID_MBOX_NO, OnOk3)
	//ON_WM_TIMER()
	ON_BN_CLICKED(ID_MBOX_STOP, OnOk4)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgBox message handlers

BOOL CMsgBox::OnInitDialog() 

{
	CGrayDlg::OnInitDialog();
	
	//m_title		= "XrayNotes MessageBox";

	//P2N(_T("Messagebox showing with: '%s'\r\n"), m_text);

	//VERIFY(m_bitmap.LoadBitmap(IDB_BITMAP34));
	//m_bitmap.LoadBitmap(IDB_BITMAP9);

	BITMAP BitMap;
	
	if(m_bitmap.m_hObject != NULL)
		{
		m_bitmap.GetBitmap(&BitMap);
		m_nWidth  = BitMap.bmWidth;
		m_nHeight = BitMap.bmHeight;
		}

	LOGFONT		m_lf;

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(m_lf), &m_lf);
	m_lf.lfHeight = -14;
	m_font.CreateFontIndirect(&m_lf);

	m_lf.lfWeight  = FW_BOLD;
	m_lf.lfHeight = -16;
	m_fontb.CreateFontIndirect(&m_lf);

	RECT	rc; GetClientRect(&rc); 
	
	int adjust = false;

	// Collect text sizes	
	titlesss =  GetTextSize(m_header, &m_fontb);
	textsss	 =  GetTextSize(m_text, &m_font);

	// Horizontal
	if(titlesss.cx > rc.right - rc.left)
		{
		adjust = true;
		rc.right = rc.left + titlesss.cx;
		}
	if(textsss.cx > rc.right - rc.left)
		{
		adjust = true;
		rc.right = rc.left + textsss.cx;
		}
	
	// Vertical
	if(titlesss.cy + textsss.cy > (rc.bottom - rc.top) - 100)
		{
		adjust = true;
		rc.bottom = rc.top + titlesss.cy + textsss.cy + 100;
		}
	
	// Execute it
	if(adjust)
		SetWindowPos(NULL, 0, 0, rc.right  + 50, rc.bottom - rc.top, SWP_NOMOVE | SWP_NOOWNERZORDER);

	// Center and show buttons
	RECT	rc3; GetClientRect(&rc3);
	int		www = rc3.right - rc3.left;

	if(yesno == MBTYPE_YESNO)
		{
		WINDOWPLACEMENT wp;
		m_no.GetWindowPlacement(&wp);

		int ww = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		int hh = wp.rcNormalPosition.bottom  - wp.rcNormalPosition.top;

		m_no.SetWindowPos(NULL, (rc3.right - rc3.left)/2 + 4 , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		m_yes.GetWindowPlacement(&wp);

		ww = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		hh = wp.rcNormalPosition.bottom  - wp.rcNormalPosition.top;

		m_yes.SetWindowPos(NULL, (rc3.right - rc3.left)/2 - (ww + 4) , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);
	
		m_ok.ShowWindow(false);
		m_stop.ShowWindow(false);
		}
	else if(yesno == MBTYPE_YESNOSTOP)
		{
		WINDOWPLACEMENT wp;
		m_no.GetWindowPlacement(&wp);

		// No in Middle
		int ww = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		int hh = wp.rcNormalPosition.bottom  - wp.rcNormalPosition.top;

		m_no.SetWindowPos(NULL, www/2 - ww/2 , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		// Yes on left
		WINDOWPLACEMENT wp2;
		m_yes.GetWindowPlacement(&wp2);

		int ww2 = wp2.rcNormalPosition.right - wp2.rcNormalPosition.left;
		int hh2 = wp2.rcNormalPosition.bottom  - wp2.rcNormalPosition.top;

		m_yes.SetWindowPos(NULL, www/2 - (ww2 + 4 +  ww/2) , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		// Stop on right
		m_stop.GetWindowPlacement(&wp2);

		ww2 = wp2.rcNormalPosition.right - wp2.rcNormalPosition.left;
		hh2 = wp2.rcNormalPosition.bottom  - wp2.rcNormalPosition.top;

		m_stop.SetWindowPos(NULL, www/2 + (ww/2 + 4) , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		m_ok.ShowWindow(false);
		}
	else
		{
		WINDOWPLACEMENT wp;
		m_ok.GetWindowPlacement(&wp);

		int ww = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		int hh = wp.rcNormalPosition.bottom  - wp.rcNormalPosition.top;

		//GetClientRect(&rc);

		m_ok.SetWindowPos(NULL, (rc3.right - rc3.left)/2 - ww/2 , 		
				rc3.bottom - (hh + 10), 0, 0, SWP_NOSIZE | SWP_NOOWNERZORDER);

		m_stop.ShowWindow(false);
		m_yes.ShowWindow(false);
		m_no.ShowWindow(false);
		}

	SetTimer(1, 1000, NULL);

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////
// Override to provide enddialog

void CMsgBox::OnLButtonDown(UINT nFlags, CPoint point) 

{
	if(closerect.PtInRect(point))
		{		
		P2N(_T("CMsgBox::OnLButtonDown - Click in Close Box\r\n"));
		//EndDialog(IDOK);
		
		//if(yesno == YESNO)
		//		ret = false;
		//	if(yesno == YESNOSTOP)
		//		ret = -1;
		//	else
		//		ret = false;
		//	

		EndDialog(idret);
		return;
		}

	// Reset timer
	tocount = AUTOWARN;

	CGrayDlg::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CMsgBox::OnLButtonUp(UINT nFlags, CPoint point) 

{

	CGrayDlg::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CMsgBox::OnMouseMove(UINT nFlags, CPoint point) 

{
	CGrayDlg::OnMouseMove(nFlags, point);
}

BOOL CMsgBox::OnEraseBkgnd(CDC* pDC) 

{
	RECT rect,m_rDataBox;
	
	GetClientRect(&rect); CopyRect(&m_rDataBox,&rect);

	CBrush NewBrush(bgcolor); 
	
	pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);   
	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top ,PATCOPY);
	pDC->SelectObject(pOldBrush);

	if(m_bitmap.m_hObject)
		{
		CDC  dcMem; dcMem.CreateCompatibleDC(pDC);
		int  save = dcMem.SaveDC();
		
		CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);
		CClientDC dc(this);

		int xx = 0, yy = 0, ww = rect.right-rect.left, hh = rect.bottom-rect.top;
		int loopx = 0, loopy = 0;
		for (loopy = 0; loopy < hh; loopy += m_nHeight)
			{
			for (loopx = 0; loopx < ww; loopx += m_nWidth)
				{
				pDC->BitBlt(loopx, loopy, m_nWidth, m_nHeight, &dcMem, 0,0, SRCCOPY ); 		
				}
			}
		dcMem.SelectObject(pOldBitmap);
		dcMem.RestoreDC(save);
		}
	return true;

	//return CGrayDlg::OnEraseBkgnd(pDC); 
}

//////////////////////////////////////////////////////////////////////////

void CMsgBox::OnPaint() 

{
	CGrayDlg::OnPaint();

	//CPaintDC dc(this); // device context for painting
	CClientDC dc(this);
	
	dc.SetBkMode(TRANSPARENT);

	RECT	rc;
	GetClientRect(&rc);

	// Vert
	RECT	rc3;
	GetClientRect(&rc3);

	//dc.SetTextColor(m_crText);
	dc.SetBkMode(TRANSPARENT);

	// Header
	RECT rc4 = rc; 
	rc4.top = 24;
	CFont* oldFont = dc.SelectObject(&m_fontb);
	dc.DrawText(m_header, m_header.GetLength(), &rc4, DT_CENTER);
	dc.SelectObject(oldFont);

	// Text
	RECT rc2 = rc;  rc2.top = 52;
	oldFont = dc.SelectObject(&m_font);
	dc.DrawText(m_text, m_text.GetLength(), &rc2, DT_CENTER);
	
	// Autoclose
	RECT rc5 = rc; rc5.top = rc5.bottom - 56;
	dc.DrawText(m_timer, m_timer.GetLength(), &rc5, DT_CENTER);

	//////////////////////////////////////////////////////////////////////
	dc.SelectObject(oldFont);
}

//////////////////////////////////////////////////////////////////////////

CSize CMsgBox::GetTextSize(const TCHAR *str, CFont *font)

{
	CPaintDC dc(this); // device context for painting
	
	//P2N(_T("GetTextSize(): Sizing: '%s'\r\n"), str);

	CFont *oldFont = dc.SelectObject(font);

	CSize sst(0,0), sst2(0, 0), sss(0, 0);
	CString tmp, text2 = str;

	int flen = text2.GetLength();
	int pos = 0, start = 0, count = 0, text_lines = 0, oneheight = 0;	
	int	ww = 0, hh = 0;
	
	// Draw box to this size ....
	while(true)
		{
		pos = text2.Find(_T("\r"), pos);
		if(pos < 0)
			{
			tmp = text2.Mid(start, flen - start);
			sst2 = dc.GetTextExtent(tmp);
			ww =  MAX(sst2.cx, ww);
			oneheight = MAX(oneheight, sst2.cy);

			if(!sst2.cy)
				sst2.cy = oneheight;

			hh += sst2.cy;

			//P2N(_T("GetTextSize() last line: ww=%d hh=%d - text2='%s'\r\n"), sst2.cx, sst2.cy, tmp);

			break;
			}
		text_lines++;

		// Exclude new line
		tmp = text2.Mid(start, (pos - start));

		pos++; start = pos;
		sst = dc.GetTextExtent(tmp);

		oneheight = MAX(oneheight, sst.cy);

		ww =  MAX(sst.cx, ww);

		if(!sst.cy)
			sst.cy = oneheight;

		hh += sst.cy;

		//P2N(_T("GetTextSize() line: ww=%d hh=%d - text1='%s'\r\n"), sst.cx, sst.cy, tmp);

		//if(count ++ > 50)
		//	break;
		}

	dc.SelectObject(oldFont);

	//P2N(_T("GetTextSize(): ww=%d hh=%d\r\n"), ww, hh);
		
	sss.cx = ww + 100; sss.cy = hh;

	return(sss);
}

void CMsgBox::OnCancel()

{
	AfxMessageBox(_T("Cance"));
}

void CMsgBox::OnOK()

{
	//AfxMessageBox(_T("OK"));
	// Sensitive to current button

	//P2N(_T("YES  button on exit %d\r\n"), m_yes.GetState());
	//P2N(_T("NO   button on exit %d\r\n"), m_no.GetState());
	//P2N(_T("OK   button on exit %d\r\n"), m_ok.GetState());
	//P2N(_T("STOP button on exit %d\r\n"), m_stop.GetState());
	
	if(m_yes.GetState() & 0x8)
		idret = MBOXYES;

	if(m_no.GetState()  & 0x8)
		idret = MBOXNO;

	if(m_stop.GetState()  & 0x8)
		idret = MBOXSTOP;
	 
	EndDialog(idret);
}

void CMsgBox::OnOk2() 
{
	//AfxMessageBox(_T("OK2"));
	idret = MBOXYES;
	OnOK();
}

void CMsgBox::OnOk3() 
{
	//AfxMessageBox(_T("OK3"));
	idret = MBOXNO;
	OnOK();
}

void CMsgBox::OnOk4() 

{
	//AfxMessageBox(_T("OK4"));
	idret = MBOXSTOP;
	OnOK();
}

//////////////////////////////////////////////////////////////////////////

void CMsgBox::MyKeyDown(UINT wParam, UINT lParam)

{
	// Restart count when key pressed
	tocount = AUTOWARN;

	if(wParam == VK_ESCAPE)
		{
		//P2N(_T("CMsgBox::PreTranslateMessage Esc Pressed\r\n"));
		EndDialog(false);
		}

	if(wParam == VK_SPACE || wParam == VK_RETURN)
		{
		CWnd	*curr = GetFocus();

		if(curr == GetDlgItem(ID_MBOX_YES))
			EndDialog(MBOXYES);

		if(curr == GetDlgItem(ID_MBOX_NO))
			EndDialog(MBOXNO);

		if(curr == GetDlgItem(IDOK))
			EndDialog(MBOXYES);

		if(curr == GetDlgItem(ID_MBOX_STOP))
			EndDialog(-1);
		}

	if(tolower(wParam)  == 'n')
		{
		EndDialog(false);
		}

	if(tolower(wParam) == 'y')
		{
		EndDialog(true);
		}
}

//////////////////////////////////////////////////////////////////////////

BOOL CMsgBox::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == WM_KEYDOWN)
		{
		//P2N(_T("mag pretrans keydown %c (%d) %x\r\n"), pMsg->wParam, pMsg->wParam, pMsg->lParam);

		if(pMsg->wParam == VK_TAB || pMsg->wParam == VK_LEFT || 
					pMsg->wParam == VK_RIGHT)

			return CGrayDlg::PreTranslateMessage(pMsg);

		MyKeyDown(pMsg->wParam, pMsg->lParam);

		return true;
		}

	return CGrayDlg::PreTranslateMessage(pMsg);
}

INT_PTR CMsgBox::DoModal() 

{
	INT_PTR ret = CGrayDlg::DoModal();
	return ret;
}

void CMsgBox::OnTimer(UINT nIDEvent) 

{	
	if(tocount++ > timeout)
		{
		//EndDialog(IDOK);
		OnOK();
		}

	if(tocount > AUTOWARN)
		{
		m_timer.Format(_T("AutoClosing Dialog in %d sec"), (timeout + 2) - tocount);

		RECT rc5; GetClientRect(&rc5);

		// Calculate Invalidate 
		rc5.top = rc5.bottom - 56; rc5.bottom = rc5.top + 20;

		InvalidateRect(&rc5);
		}
	else
		{
		}

	CGrayDlg::OnTimer(nIDEvent);
}

//include "winuser.h"
//loadlibrary

//int  WINAPI GetMouseMovePoints(
//  UINT cbSize,                // size of the MOUSEMOVEPOINT struct
//  LPMOUSEMOVEPOINT lppt,     // pointer to current mouse move point
//  LPMOUSEMOVEPOINT lpptBuf,  // buffer to store the points
//  int nBufPoints,            // how many points the buffer can store
//  DWORD resolution           // resolution of the points
//);
 

void CMsgBox::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	// Set def button on CDialog

	// No choice default on OK
	if (yesno == MBTYPE_YESOK)
		SetDefID(IDOK);
	else
		switch(idret)
			{
			case DEFYES:
				//P2N(_T("Setting default to YES\r\n"));

				SetDefID(ID_MBOX_YES);
				GetDlgItem(ID_MBOX_YES)->SetFocus();
				break;

			case DEFNO:
				//P2N(_T("Setting default to NO\r\n"));

				SetDefID(ID_MBOX_NO);
				GetDlgItem(ID_MBOX_NO)->SetFocus();
				break;
			
			case DEFSTOP:
				//P2N(_T("Setting default to STOP\r\n"));

				SetDefID(ID_MBOX_STOP);
				GetDlgItem(ID_MBOX_STOP)->SetFocus();
				break;

			default:
				//P2N(_T("Setting default to OK\r\n"));

				SetDefID(IDOK);
				GetDlgItem(IDOK)->SetFocus();
				break;
			}

	//CShadesDlg *dlg = ((CShadesDlg* )AfxGetApp()->m_pMainWnd);
	////ASSERT(dlg->magic == SHADE_MAGIC);

	// !!! THIS DOES NOT WORK IN MSVC 6.0 !!!
	//HINSTANCE  hi = LoadLibrary(_T("user32.dl"));
	//FARPROC ptr = GetProcAddress(hi, "GetMouseMovePointsA");
	//P2N(_T("loaded %d %p\r\n"), hi, ptr);
	//UINT cbSize                // size of the MOUSEMOVEPOINT struct
	//LPMOUSEMOVEPOINT lppt,     // pointer to current mouse move point
	//LPMOUSEMOVEPOINT lpptBuf,  // buffer to store the points
	//int nBufPoints,            // how many points the buffer can store
	//DWORD resolution           // resolution of the points
	//MOUSEMOVEPOINT mmp, mmp2;
	//int ret = GetMouseMovePoints(sizeof(mmp), &mmp, &mmp2, 1, GMMP_USE_DISPLAY_POINTS);
	//P2N(_T("mmp ret=%d x=%d y=%d\r\n"), ret, mmp.x, mmp.y);
	// !!! THIS DOES NOT WORK IN MSVC 6.0 !!!
	
#if 0
	// Position in the center of the current dialog 
	if(appwnd)
		if(::IsWindow(appwnd))
			{
			// Catch rogue window 
			CWnd *wnd = FromHandlePermanent(appwnd);
			if(wnd)
				CenterWindow(wnd);
			else
				CenterWindow(GetDesktopWindow());
			}
#endif
	CGrayDlg::OnShowWindow(bShow, nStatus);
}

//////////////////////////////////////////////////////////////////////////
// Pop up a message box pertaining into the last current window
// for centering

int		CMsgBox::xMsgBox(const TCHAR *str, const TCHAR *title, int yesno, int defa) 

{
	CWnd *dlg = AfxGetApp()->m_pMainWnd;

	CWnd *oldw = dlg->FromHandle(GetForegroundWindow()->m_hWnd);
	
	//P2N(_T("Forground window %p %p\r\n"), GetForegroundWindow(), oldw);
	
	CMsgBox mb(oldw);

	mb.bgcolor  = dlgback;

	if(title == NULL || *title == '\0')
		mb.m_header = "CryptoSticky MessageBox";
	else
		mb.m_header = title;

	mb.m_text = str;
	
	if(yesno == MB_YESNO)
		mb.yesno = MBTYPE_YESNO;
	else
		mb.yesno = MBTYPE_YESOK;
		
	mb.idret = DEFNO;

	INT_PTR ret = mb.DoModal();

	if(ret == MBOXYES)
		ret = IDYES;
	else if(ret == MBOXNO)
		ret = IDNO;

	return ret;
}

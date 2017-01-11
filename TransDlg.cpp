
/* =====[ TransDlg.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the TransDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
// TransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "mxpad.h"
#include "TransDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#endif

IMPLEMENT_DYNAMIC(CTransDlg, CDialog)

/////////////////////////////////////////////////////////////////////////////

static COLORREF fade(COLORREF cr, int ulim, int dlim)

{
	unsigned int color = cr;

#if 0
	static int cnt = 0;
	
    int color_r = ((color >> 16) & 0xff);
    int color_g = ((color >> 8 ) & 0xff);
    int color_b = ((color >> 0 ) & 0xff);

	//if(cnt++ < 10)
	//	PrintToNotepad(_T("Color: r:%d g:%d b:%d\r\n"), 
	//					color_r,color_g, color_b);

	// Limit color to a certain brightness
	color_r = MIN(color_r, ulim);
	color_g = MIN(color_g, ulim);
	color_b = MIN(color_b, ulim);

	// Limit color to a certain brightness
	color_r = MAX(color_r, dlim);
	color_g = MAX(color_g, dlim);
	color_b = MAX(color_b, dlim);

    // Scale back
	color_r -=  (color_r >> 2); //- (color_r >> 3);
    color_g -=  (color_g >> 2); //- (color_g >> 3);
    color_b -=  (color_b >> 2); //- (color_b >> 3);

	//if(cnt++ < 10)
	//	PrintToNotepad(_T("Color: r:%d g:%d b:%d\r\n"), 
	//					color_r,color_g, color_b);

    int rgb = (color_r << 16) + (color_g << 8) + color_b ;


    return (COLORREF)rgb;
	#endif

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTransDlg dialog

CTransDlg::CTransDlg(int ID2, CWnd* pParent /*=NULL*/)
	: CDialog(ID2, pParent)
{
	//{{AFX_DATA_INIT(CTransDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//P2N(_T("Construct TransDlg id=%d wnd%p\r\n"), ID2, pParent);

	id2			= ID2;
	drag		= 0;
	showstate	= 0;
	hthread		= 0;
	created		= 0;
	reenter		= 0;
	autoposition = 1;
	top			= false;
	focus		= false;	
	Parent		= pParent;
	shaded		= false;
	m_hIcon		= NULL;
	bAlpha		= 240;

	//CDialog::CDialog(ID2, Parent);
	//update_wanted = 0;
}

void CTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTransDlg, CDialog)
	//{{AFX_MSG_MAP(CTransDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransDlg message handlers

void	CTransDlg::Darken()

{
	RECT rect;  CDialog::GetClientRect(&rect);
	RECT wrect; CDialog::GetWindowRect(&wrect);

	//PrintToNotepad(_T("transdlg Local: %d %d  fglobal: %d %d\r\n"),
	//		rect.top, rect.left, wrect.top, wrect.left);

	PAINTSTRUCT ps;
	CDC *cdc = BeginPaint(&ps);

	CDC hdcmem;
	hdcmem.CreateCompatibleDC(cdc);
	
	
	//::GetBitmapBits(hBmp, DWORD dwCount, LPVOID lpBits )
		
	BITMAPINFO  info;
		
	GetDIBits(cdc->GetSafeHdc(), hBmp, 0, 0, 
					NULL, &info, DIB_RGB_COLORS);
	
	//PrintToNotepad(_T("transdlg Bitmap1: ww=%d hh=%d\r\n"),
	//				info.bmiHeader.biWidth, 
	//					info.bmiHeader.biHeight);

	//PrintToNotepad(_T("Error %d %s\r\n"), errno, strerror(errno));

	hdcmem.SelectObject(hBmp);
	//PrintToNotepad(_T("Error %d %s\r\n"), errno, strerror(errno));
	
	CDC hdcmem2;

	hdcmem2.CreateCompatibleDC(cdc);
	hdcmem2.SelectObject(hBmp2);
	
	for (int loopx = 0; loopx < rect.right; loopx++)
		{
		COLORREF cr;
		for (int loopy = 0; loopy < rect.bottom; loopy++)
			{
			cr =  hdcmem2.GetPixel(loopx, loopy);
			cr = fade(cr, 0xff, 0x99);
			hdcmem.SetPixel(loopx, loopy, cr); 
			}
		}

	//PrintToNotepad(_T("Error %d %s\r\n"), errno, strerror(errno));
	
	EndPaint( &ps );
}

//////////////////////////////////////////////////////

void	CTransDlg::GetBG(HBITMAP bmp)

{	
	RECT rect;  GetClientRect(&rect);
	RECT wrect; GetWindowRect(&wrect);

    //return;

	HBITMAP lbmp;

	HDC		hdc = ::GetDC(HWND_DESKTOP);

	int		xsize =  GetDeviceCaps(hdc, HORZRES);
	int		ysize =  GetDeviceCaps(hdc, VERTRES);
	//PrintToNotepad(_T("transdlg Getting desktop: %d %d\r\n"), xsize, ysize);

	if(!bmp) 
		lbmp = hBmp2;
	else
		lbmp = bmp;
	
	PAINTSTRUCT ps;
	CDC *cdc = BeginPaint(&ps);

	CDC hdcmem;
	hdcmem.CreateCompatibleDC(cdc);
	hdcmem.SelectObject(lbmp);

	::BitBlt(hdcmem.GetSafeHdc(), rect.left, rect.top,
				rect.right - rect.left, 
					rect.bottom - rect.top, hdc, 
						wrect.left, wrect.top,
							SRCCOPY);
	EndPaint(&ps);

	::ReleaseDC(HWND_DESKTOP, hdc);
}

////////////////////////////////////////////////////////////////

void	CTransDlg::GetNewBG()

{

#if 0
	//if(!showstate)
	//	return;

	if(drag)
		return;

	//if(hthread)
	//{
	//	update_wanted = TRUE;
	//	return;
	//}

	int old = SetWstate(SW_HIDE);
	YieldToWinEx();
	GetBG();
	SetWstate(old);
	Darken();
	//hthread = (HANDLE) _beginthread(DarkenThread, 0, this);
#endif

}

////////////////////////////////////////////////////////////////
// cmd = SW_SHOW or SW_HIDE;

int		CTransDlg::SetWstate(int cmd)

{
	if(!created)
		{
		created = true;
		Create(id2, NULL);
		}
	int	old;
	WINDOWPLACEMENT wp;

	CDialog::GetWindowPlacement(&wp);
	old = wp.showCmd;
	wp.showCmd = cmd; 
	CDialog::SetWindowPlacement(&wp);
	return old;
}


////////////////////////////////////////////////////////////////
// ret = SW_SHOW or SW_HIDE;

int		CTransDlg::GetWstate()

{
	if(!created)
		{
		created = true;
		Create(id2, NULL);
		}

	WINDOWPLACEMENT wp;
	CDialog::GetWindowPlacement(&wp);
	return wp.showCmd;;
}

/////////////////////////////////////////////////////////////

void CTransDlg::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	//P2N(_T("TransDlg OnShowWindow: %d\r\n"), bShow);

	CDialog::OnShowWindow(bShow, nStatus);

	showstate = bShow;

	if(inited)
		return;

	inited = true;
}
	
////////////////////////////////////////////////////////////////

BOOL CTransDlg::OnEraseBkgnd(CDC* pDC) 

{ 
	//PrintToNotepad(_T("CTransDlg erase bg\r\n"));

	CDialog::OnEraseBkgnd(pDC);
	return true;
}

void	CTransDlg::put_bg(CDC *pDC)

{

	RECT rect;  GetClientRect(&rect);
	
	CDC hdcmem;
	hdcmem.CreateCompatibleDC(pDC);
	hdcmem.SelectObject(hBmp);

	pDC->BitBlt(rect.left, rect.top, 
			rect.right - rect.left, 
				rect.bottom - rect.top, &hdcmem, 0, 0, SRCCOPY);
}

//////////////////////////////////////////////////////////////////////////

BOOL CTransDlg::OnInitDialog() 

{
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	g_pSetLayeredWindowAttributesx = (lpfnSetLayeredWindowAttributes)
                      GetProcAddress(hUser32, "SetLayeredWindowAttributes");

	CDialog::OnInitDialog();
		
	if (bAlpha < 255)
		{
		//P2N("CTransDlg::OnInitDialog g_pSetLayeredWindowAttributesx %p alpha %d\r\n",
		//	g_pSetLayeredWindowAttributesx, bAlpha);

        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        
        //  Call it with 255 as alpha - opacity
		//if(g_pSetLayeredWindowAttributesx)
		//	g_pSetLayeredWindowAttributesx(m_hWnd, 0, bAlpha, LWA_ALPHA);

		SetLayeredWindowAttributes(RGB(100,100,100), bAlpha, LWA_ALPHA);
		}

	WINDOWPLACEMENT wp;	GetWindowPlacement(&wp);
	
	int ww  = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
	int hh  = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;	
	
	//PrintToNotepad(_T("OnInit TransDlg id=%d ww=%d hh=%d\r\n"), 
	//				id2, ww, hh);

	PAINTSTRUCT ps;
	CDC *cdc = BeginPaint(&ps);
	hBmp  = ::CreateCompatibleBitmap(cdc->m_hDC, ww, hh);
	hBmp2 = ::CreateCompatibleBitmap(cdc->m_hDC, ww, hh);
	EndPaint(&ps);

	////ASSERT(hBmp); 
	////ASSERT(hBmp2);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTransDlg::OnMouseMove(UINT nFlags, CPoint point) 

{
	//PrintToNotepad(_T("OnMouseMove TransDlg\r\n"));
	RECT rc;
	
	if(!focus)
		{
		goto endd;
		}

	if(drag)
		{
		GetWindowRect(&rc);
				SetWindowPos( NULL, (point.x +  rc.left) - drag_x , 
						(point.y + rc.top) - drag_y, 0, 0, 
								SWP_NOSIZE | SWP_NOZORDER ) ;  
		goto endd;
		}

	// Moved?
	if(drag_x != point.x || drag_y != point.y)
		{
		// Is drag treshold?

		if((abs(drag_x - point.x)  > 3 ) || (abs(drag_y - point.y) > 3) )
			{
			//P2N(_T("Moved\r\n"));

			drag_x = point.x;
			drag_y = point.y;

			// Button?
			if(nFlags & MK_LBUTTON)
				{
				SetCapture();
				drag = 1;
				}
			}
		goto endd;
		}

endd:
	CDialog::OnMouseMove(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////

void CTransDlg::OnLButtonDown(UINT nFlags, CPoint point) 

{
	drag_x = point.x; drag_y = point.y;

	//PrintToNotepad(_T("Mouse down: Drag_x=%d drag_y=%d\r\n"), drag_x, drag_y);
	focus = true;

	CDialog::OnLButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CTransDlg::OnLButtonUp(UINT nFlags, CPoint point) 

{
	drag_x = point.x; drag_y = point.y;

	focus = false;
	if(drag)
		{
		drag = 0; 
		ReleaseCapture();
		InvalidateRect(NULL, TRUE);
		}

	//PrintToNotepad(_T("Mouse up\r\n"), drag_x, drag_y);

	CDialog::OnLButtonUp(nFlags, point);	
}


// Wrapper for darkening thread

static void	DarkenThread(void *lpParam)

{
	CTransDlg *dlg;

	dlg = (CTransDlg *)lpParam;
	//ASSERT(dlg);

	//PrintToNotepad(_T("transdlg Darken Thread\r\n"));

	dlg->Darken();
	
	PAINTSTRUCT ps;
	CDC *cdc = dlg->BeginPaint(&ps);

	dlg->put_bg(cdc);
	dlg->EndPaint (&ps);
	//dlg->InvalidateRect(NULL, TRUE);
	dlg->hthread = 0;

	//PrintToNotepad(_T("Darken Thread done\r\n"));
}

void CTransDlg::OnOK() 

{
 	//PrintToNotepad(_T("ON OK\r\n"));
 	//CDialog::OnOK();
	Hide();
}

void CTransDlg::Show(CWnd* pParentWnd)

{
	Create(id2, Parent);
	forcenew = TRUE;
	//ShowWindow(true);
	showstate = true;
}

void CTransDlg::Show()

{
	Create(id2,  Parent);
	forcenew = TRUE;
	//ShowWindow(true);
	showstate = true;
}

void CTransDlg::OnSetFocus(CWnd* pOldWnd) 

{
	focus = true;
	CDialog::OnSetFocus(pOldWnd);
	//P2N(_T("Focus: set\r\n"));
}

void CTransDlg::Hide()

{
	//P2N(_T("CTransDlg::Hide\r\n"));

	Create(id2, Parent);
	ShowWindow(false);
	showstate = false;
}

void CTransDlg::OnCancel()

{
	Hide();
}

void CTransDlg::OnKillFocus(CWnd* pNewWnd) 

{ 
	focus = false;	

	CDialog::OnKillFocus(pNewWnd);	
	//P2N(_T("Focus: loose\r\n"));
}


BOOL CTransDlg::Create(int idd, CWnd *wnd )

{
	//P2N(_T("Create CTRANS %p idd=%p\r\n"), this, idd);

	int ret = 0;

	if(!created)
		ret = CDialog::Create(idd, wnd); 

	created = true;
	if(m_hIcon)
		{
		//P2N(_T("Setting ICON %x\r\n"), m_hIcon);
		CDialog::SetIcon(m_hIcon, true);
		CDialog::SetIcon(m_hIcon, false);
		}
	return ret;
}

//////////////////////////////////////////////////////////////////////////

void CTransDlg::SetTransparency(int tran)

{
	//tran = 200;
	//P2N(_T("CTransDlg::SetTransparency %p %d\r\n"), this, tran);

	bAlpha = tran;

	if(IsWindow(m_hWnd))
		SetLayeredWindowAttributes(RGB(100,100,100), bAlpha, LWA_ALPHA);
		
		//if(g_pSetLayeredWindowAttributes)
		//	g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
}

void CTransDlg::Create()

{
	Create(id2, Parent);
}

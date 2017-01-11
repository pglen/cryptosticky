
/* =====[ ymemo.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the ymemo.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  10/23/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "CryptoSticky.h"

#include "Popup.h"
#include "xraym.h"
#include "bluepoint.h"
#include "MemoConf.h"

#include "Pass.h"
#include "fakewin.h"
#include "Yellowdlg.h"
#include "xArchive.h"

#include "TransDlg.h"
#include "mxpad.h"
#include "editdlg.h"
#include "ymemo.h"
#include "ColRect.h"
#include "support.h"
#include "xRect.h"
#include "misc.h"

//#pragma warning(disable: 4244)
#pragma warning(disable : 4302)
		
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
//static TCHAR THIS_FILE[] = _T(__FILE__);
static const char THIS_FILE[] = __FILE__;
#endif

void	CYmemo::unrgb(COLORREF col, BYTE *rr, BYTE *gg, BYTE *bb)

{ 
	*rr = BYTE(col & 0xff);
	*gg = BYTE((col >> 8 ) & 0xff);
	*bb = BYTE((col >> 16 ) & 0xff);
}

/////////////////////////////////////////////////////////////////////////////
// CYmemo dialog

CYmemo::CYmemo(CWnd* pParent /*=NULL*/)
	: CTransDlg(CYmemo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYmemo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	draw = fresh = sbcap = 0, reenter =  scline = 0;
	fired = sizing = mousein = tmptop = 0;	
	linecount = 0, moved = 0, idlecount = 0, capped = 0;
	maxcomm = maxcomm2 = 0;
	alpha = 255;
	xpad = 8; ypad = 8;
	erase = tipped = 0;

	center = true; 
	vscroll = false, alshow = false;
	errhide = false, modal = false;
	iswave = false;
		
	fadetimer = timeout = (int)memo_timeout;
	headsize = textsize = 0;

	penwidth = 0;
	pencol = RGB(0, 0, 0);

	lasthide = GetTickCount();

	head_fontcol2 = (int)head_fontcol;
	memo_fontcol2 = (int)memo_fontcol;
	
	magic = CYmemo_Magic;
}

CYmemo::~CYmemo()

{
	//P2N(_T("CYmemo::~CYmemo() %p \r\n"), this);
}

void CYmemo::DoDataExchange(CDataExchange* pDX)

{
	CTransDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYmemo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

//#define ON_WM_TIMER2() \
//	{ WM_TIMER, 0, 0, 0, AfxSig_vw, \
//		(AFX_PMSG)(AFX_PMSGW) \
//		(static_cast< void (AFX_MSG_CALL CWnd::*)(UINT_PTR) > ( &ThisClass :: OnTimer)) },
       
BEGIN_MESSAGE_MAP(CYmemo, CTransDlg)
	//{{AFX_MSG_MAP(CYmemo)
	ON_WM_ERASEBKGND()                                                         
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_RCLICKMEMO_ENCRYPTDECRYPTMEMO, OnEncrypt)
	ON_COMMAND(ID_RCLICKMEMO_DECRYPTMEMO, OnDecrypt)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_RCLICKMEMO_COPYTOCLIPBOARD, CopyToClip)
	ON_COMMAND(ID_RCLICKMEMO_CREATENEWMEMO, CreateNew)
	ON_COMMAND(ID_RCLICKMEMO_CLOSEMAINPANEL, OnCloseMain)
	ON_COMMAND(ID_RCLICKMEMO_HIDEMAINPANEL, OnHideMain)
	ON_COMMAND(ID_RCLICKMEMO_CONFIGTHISMEMO, OnConfig)
	ON_WM_MOVE()
	ON_COMMAND(ID_RCLICKMEMO_PASTEFROMCLIPBOARD, PasteClip)
	ON_COMMAND(ID_RCLICKMEMO_ERASER, Eraser)
	ON_COMMAND(ID_RCLICKMEMO_ERASEDRAWING, EraseDrawing)
	ON_COMMAND(ID_RCLICKMEMO_NEWMEMO, NewMemo)
	ON_COMMAND(ID_RCLICKMEMO_SHOWMAINPANEL, OnShowMain)
	ON_COMMAND(ID_RCLICKMEMO_HIDEMEMO, Hide)
	ON_COMMAND(ID_RCLICKMEMO_HIDEMEMO2, Hide2)	
	ON_COMMAND(ID_RCLICKMEMO_EDITMEMO, Edit )
	ON_COMMAND(ID_RCLICKMEMO_SHOWALLSELECTED, ShowAll )
	ON_COMMAND(ID_RCLICKMEMO_HIDEALLMEMOS, HideAll )
	ON_COMMAND(ID_RCLICKMEMO_AUTOSIZE, AutoSize)
	ON_COMMAND(ID_RCLICKMEMO_GOTONEXTSTICKY, NextMemo)

	
	//}}AFX_MSG_MAP

	ON_COMMAND(ID_RCLICKMEMO_APPENDFROMCLIPBOARD, &CYmemo::OnRclickmemoAppendfromclipboard)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYmemo message handlers

void	CYmemo::EraseDrawing()

{
	gesture.RemoveAll();
	SaveContent();
	Invalidate();

}

void	CYmemo::Eraser()

{
	//P2N(_T("Eraser\r\n"));
	erase = !erase;
}

//////////////////////////////////////////////////////////////////////////

void	CYmemo::PasteClip()

{
	fadetimer = timeout;

	CString text;

	OpenClipboard();

	HANDLE htext = GetClipboardData(CF_UNICODETEXT);

	if (htext)
		{
		LPVOID ptr = GlobalLock(htext);
		//ASSERT(ptr);
		text = (const TCHAR *)ptr;
		GlobalUnlock(htext);
		}

	CloseClipboard();

	if(text == _T(""))
		{
		AfxMessageBox(_T("No text on clipboard.\r\n"));
		return;
		}

	memo.body = text;
	memo.Write(memofile); 		
	
	GetParms();
	//AutoSize();
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void	CYmemo::CopyToClip()

{
	//P2N_T("CYmemo::CopyToClip '%s'\r\n", memo.body);

	if(!OpenClipboard())
		{
		AfxMessageBox(_T("Cannot open clipboard"));
		return;
		}

	// Alloc it, Assemble it
	int biglen = memo.body.GetLength() * sizeof(TCHAR) + sizeof(TCHAR);
	HANDLE bighandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, biglen);
	if(!bighandle)
		{
		AfxMessageBox(_T("Cannot allocate memory for memo contents"));
		return;
		}

	TCHAR *bigptr = (TCHAR *)GlobalLock(bighandle);
	if(!bigptr)
		{
		AfxMessageBox(_T("Cannot lock memory for memo contents"));
		return;
		}

	memcpy(bigptr, memo.body, biglen);

	::GlobalUnlock(bighandle);	
	EmptyClipboard();

	//::SetClipboardData(CF_TEXT, bighandle);
	::SetClipboardData(CF_UNICODETEXT, bighandle);

	CloseClipboard();
}

//////////////////////////////////////////////////////////////////////////

void	CYmemo::NewMemo()

{
	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	cc->CreateBranch(TRUE);
}


void	CYmemo::HideAll()

{
	//P2N(_T("CYmemo::HideAll\r\n"));

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	cc->HideAll();
}


void	CYmemo::OnConfig()

{
	CMemoConf mc; mc.m_BackColor = dlgback;
	mc.buddy = this;
	mc.DoModal();
}

void	CYmemo::ShowAll()

{
	//P2N(_T("CYmemo::ShowAll\r\n"));

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	cc->ShowAll();
}

//////////////////////////////////////////////////////////////////////////

void	CYmemo::Hide2()

{
	//P2N(_T("CYmemo::Hide2\r\n"));

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;

	// Update count only if it was shown
	if(showstate)
		lasthide = GetTickCount();

	// Find this memo on main panel
	
	HTREEITEM hh = cc->Memo2Handle(memo.MakeFileName());
	if(hh)
		{
		//P2N(_T("CYmemo::Hide2 %x\r\n"), hh);
		cc->GetTreeCtrl()->SetCheck(hh, false);
		cc->WrIterate_Checks(hh);
		}
	CTransDlg::Hide();
}

//////////////////////////////////////////////////////////////////////////

void	CYmemo::OnDecrypt()

{	
	lasthide = GetTickCount();

	P2N(_T("CYmemo::OnDecrypt\r\n"));

	// Alloc it, Assemble it
	int biglen = memo.body.GetLength() * sizeof(TCHAR) + sizeof(TCHAR);
	HANDLE bighandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, biglen);
	if(!bighandle)
		{
		AfxMessageBox(_T("Cannot allocate memory for clipboard contents"));
		return;
		}

	TCHAR *bigptr = (TCHAR *)GlobalLock(bighandle);
	if(!bigptr)
		{
		AfxMessageBox(_T("Cannot lock memory for clipboard contents"));
		return;
		}

	memcpy(bigptr, memo.body, biglen);

	OpenClipboard(); EmptyClipboard();

	::GlobalUnlock(bighandle);	
	::SetClipboardData(CF_UNICODETEXT, bighandle);

	CloseClipboard();

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;

	cc->Decrypt_Clipboard();

	OpenClipboard();

	HANDLE htext = GetClipboardData(CF_UNICODETEXT);
	
	if (htext)
		{
		LPVOID ptr = GlobalLock(htext);	//ASSERT(ptr);
		memo.body = (const TCHAR *)ptr;
		GlobalUnlock(htext);
		}

	EmptyClipboard();
	CloseClipboard();

	if(memo.body  == _T(""))
		{
		AfxMessageBox(_T("No text on clipboard.\r\n"));
		return;
		}

	memo.Write(memofile); 

	//AutoSize();

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void	CYmemo::OnEncrypt()

{
	lasthide = GetTickCount();

	//P2N(_T("CYmemo::OnEncrypt\r\n"));

	//P2N(_T("memo body '%s'\r\n"), str);


	// Alloc it, Assemble it
	int biglen = memo.body.GetLength() * sizeof(TCHAR) + sizeof(TCHAR);
	HANDLE bighandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, biglen);
	if(!bighandle)
		{
		AfxMessageBox(_T("Cannot allocate memory for clipboard contents"));
		return;
		}

	TCHAR *bigptr = (TCHAR *)GlobalLock(bighandle);
	if(!bigptr)
		{
		AfxMessageBox(_T("Cannot lock memory for clipboard contents"));
		return;
		}

	memcpy(bigptr, memo.body, biglen);

	OpenClipboard(); EmptyClipboard();

	::GlobalUnlock(bighandle);	
	::SetClipboardData(CF_UNICODETEXT, bighandle);

	CloseClipboard();

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	
	cc->Encrypt_Clipboard();

	// Get it back from clipboard

	OpenClipboard();

	HANDLE htext = GetClipboardData(CF_UNICODETEXT);
	
	if (htext)
		{
		LPVOID ptr = GlobalLock(htext);	//ASSERT(ptr);
		memo.body = (const TCHAR *)ptr;
		GlobalUnlock(htext);
		}

	EmptyClipboard();
	CloseClipboard();

	if(memo.body  == _T(""))
		{
		AfxMessageBox(_T("No text on clipboard.\r\n"));
		return;
		}

	memo.Write(memofile); 
	Invalidate();
}

void	CYmemo::CreateNew()

{
	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	cc->CreateBranch(TRUE);	
}


//////////////////////////////////////////////////////////////////////////

void	CYmemo::OnHideMain()

{
	//P2N(_T("CYmemo::OnCloseMain\r\n"));

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	cc->HideMe();
}

void	CYmemo::OnCloseMain()

{
	//P2N(_T("CYmemo::OnCloseMain\r\n"));

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	cc->Close();	
}

//////////////////////////////////////////////////////////////////////////

void	CYmemo::OnShowMain()

{
	//P2N(_T("CYmemo::OnShowMain\r\n"));

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	cc->OnShowMain();	
}

//////////////////////////////////////////////////////////////////////////

BOOL CYmemo::OnInitDialog() 

{
	CTransDlg::OnInitDialog();

	//P2N(_T("CYmemo::OnInitDialog %p\r\n", this));

	//ModifyStyleEx(WS_EX_APPWINDOW, 0);

	SetTimer(1, 1000, NULL);

	EnableToolTips();

	headcol = confcol.memohead;
	textcol = confcol.memocolor;
	headrcol = confcol.memorhead;
	bodyrcol = confcol.memorcolor;

	LOGFONT lf;
	head_font.GetObject(sizeof(LOGFONT), &lf);
	headsize = lf.lfHeight;
	myhead_font.CreateFontIndirect(&lf);

	lf.lfWeight = FW_BOLD;
	myhead_fontb.CreateFontIndirect(&lf);
	
	LOGFONT lf2;
	memo_font.GetObject(sizeof(LOGFONT), &lf2);
	textsize = lf2.lfHeight;
	mymemo_font.CreateFontIndirect(&lf2);
	
	BYTE bAlpha = main_trans;

	SetTransparency(bAlpha);

	// Testing 
	//backimage =_T("c:\\images\\cassy.jpg");
	//int ret = backimg.Load(backimage);
	//P2N(_T("Loaded image %s ret=%d\r\n"), fname, ret);

	head_fontcol2 = (int)head_fontcol;
	memo_fontcol2 = (int)memo_fontcol;
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CYmemo::OnEraseBkgnd(CDC* pDC) 

{
	RECT rect; GetClientRect(&rect);

	CBrush NewBrush;

	if(!memo.flags.red)
		{
		NewBrush.CreateSolidBrush(textcol) ;
		}
	else
		{
		NewBrush.CreateSolidBrush(bodyrcol) ;
		}

	pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);
	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
	pDC->SelectObject(pOldBrush);
	
	if(backimg.IsValid())
		{
		//backimg.Draw(pDC, 0, 0, rect.right, rect.bottom);
		backimg.Draw(pDC, alpha, 0, 0, rect.right, rect.bottom);
		}

	//return CTransDlg::OnEraseBkgnd(pDC);
	return(false);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::OnPaint() 

{
	CPaintDC dc(this); // device context for painting

	//P2N(_T("CYmemo::OnPaint %p %s\r\n"), this, memo.head);

	CFont *oldfont;
	
	if(GetFocus() == this)
		{
		oldfont = dc.SelectObject( &myhead_fontb );
		}
	else
		{
		oldfont = dc.SelectObject( &myhead_font );
		}
	
	dc.SetTextColor(head_fontcol2);
	// See if text is bigger than header
	dc.DrawText(memo.head, htxtrect, DT_EXPANDTABS | DT_CALCRECT );

	int	hheight = htxtrect.bottom - htxtrect.top;
	int	hwidth = htxtrect.right - htxtrect.left;
	
	//P2N(_T("Header text hh=%d ww=%d \r\n"), hheight, hwidth);
	
	// Vert
	RECT	rc3; GetClientRect(&rc3); CRect rect(rc3);

	//P2N(_T("Window hh=%d ww=%d \r\n"), rc3.bottom, rc3.right);

	if(hwidth > rc3.right)
		{
		//P2N(_T("Partial display of header\r\n"));
		//ShortenStr(memo.head, ((rc.right - rc.left) - 32)/10);	
		}

	// Fill in header slice
	rchead.top = 2;	rchead.left = 2;
	rchead.right = rc3.right - 2;
	//rchead.bottom = 22;
	
	rchead.bottom = hheight +  4;

	//if(alshow)
	//	{
	///	dc.FillSolidRect(&rchead, RGB(255, 255, 255));
	//	}
	//else 

	if(!backimg.IsValid())
		{
		if(!memo.flags.red)
			{
			dc.FillSolidRect(&rchead, headcol);
			}
		else
			{
			dc.FillSolidRect(&rchead, headrcol);
			}
		}

	// Draw alarm status 
	if(alshow)
		{
		CPen Pen4; Pen4.CreatePen(PS_SOLID, 1, RGB(0xff, 0xff, 0xf0));
		CPen* pOldPen = (CPen*)dc.SelectObject(&Pen4);

		for(int loop = 2; loop <= hheight + 3; loop += 3)
			{
			dc.MoveTo(rc3.left + 2, rc3.top + loop);
			dc.LineTo(rc3.right - 2, rc3.top + loop);
			}

		dc.SelectObject(pOldPen);
		}

	// Calc rect bounds
	closerect.top = rc3.top + 8;
	closerect.left = rc3.right - 16;
	closerect.bottom = closerect.top + 8;
	closerect.right = closerect.left + 8;

	copyrect.top = rc3.top + 8;
	copyrect.left = rc3.right - 32;
	copyrect.bottom = copyrect.top + 8;
	copyrect.right = copyrect.left + 8;

	urlrect.top = rc3.top + 8;
	urlrect.left = rc3.right - 48;
	urlrect.bottom = urlrect.top + 8;
	urlrect.right = urlrect.left + 8;

	CRect hrect(rchead); 
	
	if(iswave)
		{
		wavrect.top = rc3.top + 8;
		wavrect.left = rc3.right - 64;
		wavrect.bottom = wavrect.top + 8;
		wavrect.right = wavrect.left + 8;

		hrect.right -= 72;
		}
	else
		{
		hrect.right -= 56;
		}

	// Draw header text
	dc.SetBkMode(TRANSPARENT);	
	//dc.SetTextAlign(TA_CENTER);
	//dc.ExtTextOut((rc3.right - rc3.left)/2, 3, 0, NULL, title, NULL);

	dc.DrawText(memo.head, hrect, DT_CENTER | DT_EXPANDTABS);

	// Draw window operating icons
	CPen Pen2;	Pen2.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen* pOldPen = (CPen*)dc.SelectObject(&Pen2);

	
	// Draw close rectangle
	dc.MoveTo(closerect.left, closerect.top);
	dc.LineTo(closerect.right,  closerect.bottom);
	dc.MoveTo(closerect.left, closerect.bottom );
	dc.LineTo(closerect.right, closerect.top);

	// Draw clipboad rectangle
	dc.MoveTo(copyrect.left, copyrect.bottom - 2);
	dc.LineTo(copyrect.right,  copyrect.bottom - 2);

	dc.MoveTo(copyrect.left, copyrect.top + 2);
	dc.LineTo(copyrect.right, copyrect.top + 2);

	dc.MoveTo(copyrect.left, copyrect.top + 1);
	dc.LineTo(copyrect.left, copyrect.bottom - 2);

	// Draw URL rectangle	
	dc.MoveTo(urlrect.left + 2,  urlrect.top);
	dc.LineTo(urlrect.left + 2,  urlrect.bottom -1);
	dc.LineTo(urlrect.right - 1, urlrect.bottom -1);
	dc.LineTo(urlrect.right - 1, urlrect.top );

	// Draw Wave handles
	if(iswave)
		{		
		dc.MoveTo(wavrect.left + 0,  wavrect.top );
		dc.LineTo(wavrect.left + 4,  wavrect.bottom - 1);
		dc.LineTo(wavrect.left + 8 , wavrect.top);
		}

	// Draw scroll bars
	tsrect.top = rc3.top + 24;
	tsrect.left = rc3.right - 16;
	tsrect.bottom = tsrect.top + 10;
	tsrect.right = tsrect.left + 10;

	bsrect.top = rc3.bottom - 16;
	bsrect.left = rc3.right - 16;
	bsrect.bottom = bsrect.top + 10;
	bsrect.right = bsrect.left + 10;

	CPen Pen3;	

	if(vscroll)
		{
		rxrect.left = tsrect.left, rxrect.top = tsrect.bottom, 
		rxrect.right = tsrect.right + 1, rxrect.bottom = bsrect.top;

		BYTE rr,gg,bb;
		BYTE rrr,ggg,bbb;
			
		unrgb(textcol, &rr, &gg, &bb);
		unrgb(textcol, &rrr, &ggg, &bbb);

		//if(rr > 128) rr -= 20; else rr += 20;
		//if(gg > 128) gg -= 20; else gg += 20;
		//if(bb > 128) bb -= 20; else bb += 20;

		//if(rrr > 128) rrr -= 40; else rrr+= 40;
		//if(ggg > 128) ggg -= 40; else ggg+= 40;
		//if(bbb > 128) bbb -= 40; else bbb+= 40;

		rr -= 20; 
		gg -= 20; 
		bb -= 20; 

		rrr -= 40;
		ggg -= 40; 
		bbb -= 40; 

#if 0
		if(!flags.red)
			{
			//Pen3.CreatePen(PS_SOLID, 2, RGB(0xcf, 0xcf, 0x50));
			Pen3.CreatePen(PS_SOLID, 2, RGB(rrr, ggg, bbb));
			dc.SelectObject(&Pen3);
			//dc.FillSolidRect(&rxrect, RGB(0xef, 0xef, 0x70) );
			dc.FillSolidRect(&rxrect, RGB(rr, gg, bb ));
			}
		else
			{
			//Pen3.CreatePen(PS_SOLID, 2, RGB(0xcf, 0x70, 0x50));
			Pen3.CreatePen(PS_SOLID, 2, RGB(rrr, ggg, bbb));
			dc.SelectObject(&Pen3);
			//dc.FillSolidRect(&rxrect, RGB(0xef, 0x90, 0x70) );
			dc.FillSolidRect(&rxrect, RGB(rr, gg, bb ));
			}
#endif

		dc.FillSolidRect(&rxrect, RGB(rr, gg, bb ));
		Pen3.CreatePen(PS_SOLID, 2, RGB(rr, gg, bb));
		dc.SelectObject(&Pen3);

		dc.MoveTo(tsrect.left, tsrect.bottom);
		dc.LineTo(tsrect.left + 5, tsrect.bottom - 5);
		dc.LineTo(tsrect.right, tsrect.bottom);

		dc.MoveTo(bsrect.left, bsrect.top);
		dc.LineTo(bsrect.left + 5, bsrect.top + 5);
		dc.LineTo(bsrect.right, bsrect.top);

		// Draw scroll button
		int sbh = 30;
		if( (rxrect.bottom - rxrect.top) < 30)
			sbh = rxrect.bottom - rxrect.top;

		sbutt.left = rxrect.left;
		sbutt.right = rxrect.right;
		sbutt.top = (long) (( (float)scline / linecount) * ((rxrect.bottom - sbh) - rxrect.top)) + rxrect.top; 
		sbutt.bottom = sbutt.top + sbh;
	
		if(!memo.flags.red)
			{
			//dc.FillSolidRect(&sbutt, RGB(0xdf, 0xdf, 0x60)) ;

			dc.FillSolidRect(&sbutt, RGB(rrr, ggg, bbb)) ;
			}
		else
			{
			//dc.FillSolidRect(&sbutt, RGB(0xdf, 0x80, 0x60)) ;
			dc.FillSolidRect(&sbutt, RGB(rrr, ggg, bbb)) ;
			}
		}
	
	// Draw LR corner
	CPen Pen4; Pen4.CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	dc.SelectObject(&Pen4);
			
	dc.MoveTo(rc3.right - 4, rc3.bottom - 2);
	dc.LineTo(rc3.right - 2, rc3.bottom - 4);
		
	dc.MoveTo(rc3.right - 7, rc3.bottom - 2);
	dc.LineTo(rc3.right - 2, rc3.bottom - 7);
	
	// Free resource(s)
	dc.SelectObject(pOldPen);
	
	
	rect.DeflateRect(xpad + 1, ypad + 1);
	rect.top += hheight + 4;

	dc.SelectObject(&mymemo_font);
	dc.SetTextColor(memo_fontcol2);

	//dc.SetTextColor(RGB(255,255,0));

	dc.SetTextAlign(TA_LEFT);

	int old = 0, cnt = scline;
	while(true)
		{
		int pos = memo.body.Find(_T("\r\n"), old);
		
		if(pos < 0)
			 break;

		if(cnt-- == 0)
			break;

		old = pos + 2;
		}

	CString str = memo.body.Right(memo.body.GetLength() - old);

	if(memo.flags.centered)
		dc.DrawText(str, rect, DT_CENTER | DT_WORDBREAK | DT_EXPANDTABS );
	else
		dc.DrawText(str, rect, DT_LEFT | DT_WORDBREAK | DT_EXPANDTABS );

	dc.SelectObject( oldfont );

	CPen Penx;
	Penx.CreatePen(PS_SOLID, penwidth, pencol);
	pOldPen = (CPen*)dc.SelectObject(&Penx);
		
	int alen = gesture.GetSize();
	//P2N("alen=%d\r\n", alen);
	for(int loop = 0; loop < alen; loop++)
		{
		DWORD val = gesture.GetAt(loop);		
		int xx = val & 0xffff, yy =  val >> 16;

		//P2N(_T("lineto %d %d\r\n"), xx, yy);

		// Skip all zero stuff
		if(val == 0)
			{	
			DWORD val2 = 0;
			while(true)
				{
				loop++;
				if(loop >= alen)
					break;				
				val2 = gesture.GetAt(loop);
				if(val2)
					break;							
				}
			if(val2)
				{
				int xx2 = val2 & 0xffff, yy2 =  val2 >> 16;
				dc.MoveTo(xx2, yy2);
				}
			}
		else
			{
			dc.LineTo(xx, yy);
			}
 		}

	// Free resource(s)
	dc.SelectObject(pOldPen);
	
	// Do not call CTransDlg::OnPaint() for painting messages
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::OnLButtonDown(UINT nFlags, CPoint point) 

{
	//if(GetFocus() != this)
	//	SetFocus();

	RECT rect; GetClientRect(&rect);

	if( abs(point.x - rect.right) < 10 && abs(point.y - rect.bottom) < 10)
		{
		sizing = true;
		SetCapture();
		CTransDlg::OnLButtonDown(nFlags, point);
		return;	
		}

	// Clicking on 'x'
	CRect cr(closerect); cr.InflateRect(5,5);
	if(cr.PtInRect(point))
		{
		//HideAll();
		CTransDlg::OnLButtonDown(nFlags, point);
		//EndDialog(IDOK);
		Hide();
		return;
		}	

	// Clicking on '='
	CRect cr2(copyrect);	cr2.InflateRect(5,5);
	if(cr2.PtInRect(point))
		{
		P2N(_T("Click on copy rectangle\r\n"));

		//UpdateData();
		//SetTransparency(idle_trans);
		//Invalidate();

		StartFadeCycle();
		//SetTransparency(main_trans);

		CopyToClip();

		CTransDlg::OnLButtonDown(nFlags, point);
		return;
		}	

		// Clicking on 'u'
	{ CRect cr5(urlrect);	cr5.InflateRect(5,5);
	if(cr5.PtInRect(point))
		{
		//P2N(_T("Clicked on URL rect\r\n"));

		int idx, idx2, idx3, idxx;

		idx = memo.body.Find(_T("http://"));
		if(idx < 0)
			{
			AfxMessageBox(_T("No URL in this Sticky"));
			}
		else
			{
			idx2 = memo.body.Find(_T("\r"), idx);
			idx3 = memo.body.Find(_T(" "),  idx);

			if(idx3 >= 0)
				{
				idxx = idx3;
				}
			else if (idx2 >= 0)
				{
				idxx = idx2;
				}
			else
				{
				idxx = memo.body.GetLength();
				}

			CString str; str = memo.body.Mid(idx, idxx - idx);

			//P2N(_T("Parsed URL: '%s' idx=%d idx2=%d idx3=%d\r\n"), str, idx, idx2, idx3);
			
			// Show URL
			ShellExecute(NULL,_T("open"), str, NULL, NULL, SW_SHOWMAXIMIZED);
			}
		}
	}

	// Clicking on 'play'
	CRect cr5(wavrect); cr5.InflateRect(5,5);
	if(cr5.PtInRect(point))
		{
		P2N(_T("Play Wave %s\r\n"), wavname);

		playsound.WaveOpen();

		if(playsound.IsPlaying())
			{
			playsound.StopPlay();
			}
		else
			{
			if(_taccess(wavname, 04) >= 0)
				{
				char *buf; int len;
				int ret = playsound.LoadWave(wavname, &buf, &len);
				if(ret)
					playsound.PlayWave(buf, len, true);
				}
			}
		return;
		}	

	// Clicking on scroll button
	{
	CRect cr3(sbutt); cr3.InflateRect(5,5);
	if(cr3.PtInRect(point))
		{
		//P2N(_T("Click on scroll button\r\n"));

		sbcap = true;
		sbpoint = point;
		SetCapture();
		return;
		}	
	}

	//////////////////////////////////////////////////////////////////////////
	// Clicking on 'scroll bar'

	{
	CRect cr3(rxrect); 	cr3.InflateRect(5,5);
	if(cr3.PtInRect(point))
		{
		// Half Way?
		if(point.y > sbutt.top + 15)
			{
			//P2N(_T("click on scroll bar down\r\n"));

			scline += 5;
			}
		else
			{
			//P2N(_T("click on scroll bar up\r\n"));

			scline -= 5;
			}

		UpdateScroll();

		// Set scroll parameters
		if(scline < 0) scline = 0;
		if(scline > linecount) scline = linecount;

		int sc = scline; int old = 0;

		while(1==1)
			{
			if(!sc)
				break;
			int pos = m_edit.Find(_T("\r\n"), old);
			if(pos < 0)
				 break;
			sc--;
			old = pos + 2;
			}
		//m_str = m_edit.Right(m_edit.GetLength() - (old ));

		Invalidate();

		goto endd;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Clicking on header

	{ CRect cr6(rchead);	cr6.InflateRect(5,5);
	if(cr6.PtInRect(point))
		{
		StartFadeCycle();
		//SetTransparency(main_trans); timer = timeout;
		goto endd;
		}
	}

	// Nothing left but the body
	draw = true;
	gesture.Add(0);	

	//if(memo.flags.persist)
	//	{
	//	SetTransparency(main_trans); timer = timeout;
	//	}
		
endd:

	CTransDlg::OnLButtonDown(nFlags, point);
	return;	
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::OnMouseMove(UINT nFlags, CPoint point) 

{
	mouse.x = point.x;	mouse.y = point.y;
	
	//P2N(_T("CYmemo::OnMouseMove(UINT nFlags, CPoint point) \r\n"));

	if(tipped + 200 < GetTickCount())
		{
		tip.Hide();
		tipped = false;
		}

	mousein = true;

	if(sizing)
		{
		//CRect rc; GetClientRect(&rc);
		//P2N(_T("CYmemo::OnMouseMove - sizing top=%d left=%d %d %d\r\n"),
		//									rc.top, rc.left, point.x, point.y);

		fadetimer = timeout;

		//int	hheight = htxtrect.bottom - htxtrect.top;
		int	hheight = rchead.bottom;
		
		// Fake minmax info here
		//if(point.x > 100 && point.y > hheight + 2)
		point.x  = max(point.x,  100);
		point.y  = max(point.y, hheight + 2);

		SetWindowPos(NULL, 0, 0, point.x, point.y,
						SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
					
		Invalidate();

		moved = true;

		// Would move it at this point, do not call
		//CTransDlg::OnMouseMove(nFlags, point);
		return;
		}

	if(sbcap)
		{
		int	hheight = htxtrect.bottom - htxtrect.top;
		RECT rc; GetClientRect(&rc);
	
		//P2N(_T("Scroll bar x=%d y=%d %d %d\r\n"), point.x, point.y, 
		//							sbpoint.x, sbpoint.y);

		int hh = (rc.bottom - rc.top) - 24;

		int scline2 = (point.y - 24) * linecount / hh;

		if(scline2 != scline)
			{
			scline = scline2;
			UpdateScroll();
			}
		}

	TRACKMOUSEEVENT te;

	if(!capped)
		{
		capped = true;
		
		capmouse.x = point.x;
		capmouse.y = point.y;
		
		te.cbSize = sizeof(TRACKMOUSEEVENT);
		te.dwFlags = TME_LEAVE;
		te.hwndTrack = this->m_hWnd;
		te.dwHoverTime = 1500;
	
		TrackMouseEvent(&te);

		te.cbSize = sizeof(TRACKMOUSEEVENT);
		te.dwFlags = TME_HOVER;
		te.hwndTrack = this->m_hWnd;
		te.dwHoverTime = 1500;
	
		TrackMouseEvent(&te);
		}
	else
		{
		// Moved more than 3 pixels?
		if	(
			ABS(capmouse.x - point.x) > 3 ||
			ABS(capmouse.y - point.y) > 3
			)

		//tip.ShowWindow(false);
		//tip.Hide();
		
		//P2N(_T("Move mouse x=%d y=%d diffx=%d old_y=%d)\r\n"), point.x, point.y, 
		//	ABS(capmouse.x - point.x), ABS(capmouse.y - point.y) );

		// Re initialize tracking
		te.cbSize = sizeof(TRACKMOUSEEVENT);
		te.dwFlags = TME_HOVER;
		te.hwndTrack = this->m_hWnd;
		te.dwHoverTime = 1500;
	
		TrackMouseEvent(&te);
		}

	if(draw)
		{
		//P2N(_T("CYmemo::OnMouseMove draw\r\n"));

		DWORD  val =  ((int)point.y << 16) + point.x;

		if(erase || GetAsyncKeyState (VK_SHIFT))
			{
			int alen = gesture.GetSize();
			for(int loop = 0; loop < alen; loop++)
				{
				int val = gesture.GetAt(loop);
				int xx = val & 0xffff, yy =  val >> 16;
	
				if(xx && yy)
					{
					CRect rc(xx-8, yy-8, xx+8, yy+8);
					if(rc.PtInRect(point))
						{
						//P2N(_T("Erase %d %d\r\n"), xx, yy);
						gesture.SetAt(loop, 0);
						Invalidate();
						}
					}
 				}
			}
		else
			{
			gesture.Add(val);
			}

		//Invalidate();
		//DelayedInval();

		// Smart invalidate
		CRect irc;
		irc.top		= min(point.y, prevmouse.y);
		irc.left	= min(point.x, prevmouse.x);
		irc.bottom	= max(point.y, prevmouse.y);
		irc.right	= max(point.x, prevmouse.x);

		irc.InflateRect(4, 4);
		InvalidateRect(irc);

		// Test inval rect
		//CClientDC pdc(this);
		//pdc.Draw3dRect(irc, RGB(100,100,100), RGB(200,200,200));

		prevmouse = point;
		return;
		}

	//if(timer == 0)
	//	SetTransparency(main_trans);
	//
	
	fadetimer = timeout;

	CTransDlg::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

//void CYmemo::OnTimer(INT_PTR nIDEvent) 
void CYmemo::OnTimer(UINT nIDEvent) 

{
	if(nIDEvent == 1)
		{
		//P2N(_T("timer=%d timeout=%d\r\n"), timer, timeout);

		if(fadetimer)
			fadetimer--;

		if(fadetimer == 1)
			{
			// First time ontop, disable
			if(tmptop && !memo.flags.persist)
				{
				//P2N(_T("First idle tmptop message\r\n"));

				SetWindowPos(&wndNoTopMost, 0, 0, 0, 0,
							SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE   );
				tmptop = false;
				}

			// Auto Hide if it is a error window
			if(errhide)
				{
				errhide = false;
				Hide();
				}

			if(mousein)
				{
				//P2N(_T("Re-triggered timeout\r\n"));
				fadetimer = timeout;
				goto endd;
				}

			// Set to non obtrusive
			if(timeout != 0)  // and if there is a timeout
				SetTransparency(idle_trans);
			}
		}

	if(nIDEvent == 2)
		{
		KillTimer(nIDEvent);

		//P2N(_T("Delayed Inval\r\n"));

		if(IsWindow(m_hWnd))
			Invalidate();

		fired =  GetTickCount();
		}

	if(nIDEvent == 3)
		{		
		KillTimer(nIDEvent);

		//P2N(_T("Delayed Write\r\n"));

		if(!draw)
			SaveContent();
		}

	if(nIDEvent == 4)
		{
		KillTimer(nIDEvent);
	
		//P2N(_T("Delayed Load\r\n"));

		LoadContent(true);
		Invalidate();
		}

endd:
	CTransDlg::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::OnLButtonDblClk(UINT nFlags, CPoint point) 

{
	//P2N(_T("CYmemo::OnLButtonDblClk(UINT nFlags, CPoint point) \r\n"));

	// Clicking on 'scroll bar'
	CRect cr3(rxrect);	cr3.InflateRect(5,5);

	if(cr3.PtInRect(point))
		return;
	
	SHORT ks   = GetKeyState(VK_SHIFT);

	if(ks & 0x8000)
		{
		::ShowWindow(AfxGetMainWnd()->m_hWnd, SW_RESTORE);
		::SetWindowPos(AfxGetMainWnd()->m_hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
		}
	else
		{
		Edit();
		}

	CTransDlg::OnLButtonDblClk(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::Edit()

{
	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	//ASSERT(cc);

	if(modal)
		{
		AfxMessageBox(_T("An instance of the editing window is already open."));
		if(cc->ped)
			{
			//cc->ped-> sSetActiveWindow();
			cc->ped->EndModalLoop(0);
			}
		return;
		}

	modal = true;

	// Select this entry in YellowDlg, re-read
	HTREEITEM h1;
	cc->FnameToSel(memofile, &h1);	cc->GetTreeCtrl()->SelectItem(h1);
	cc->EditCurrent(true);
	modal = false;
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::OnSetFocus(CWnd* pOldWnd) 

{
	//P2N(_T("CYmemo setting focus %p %s\r\n"), this, title);
	StartFadeCycle();
	Invalidate();
	CTransDlg::OnSetFocus(pOldWnd);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::MySysKeyDown(UINT wParam, UINT lParam)

{
	fadetimer = timeout;

	//P2N(_T("CYmemo::MySysKeyDown(UINT wParam=%d, UINT lParam=%d)\r\n"), wParam, lParam);

	// Alt-F4
	if(wParam == VK_F4)
		{
		//EndDialog(IDOK);
		Hide();
		}

	// Alt-X
	if(wParam == L'X')
		{
		int ret = AfxMessageBox(_T("Close the CryptoSticky Program?"),  MB_YESNO);
		if(ret == IDYES)
			OnCloseMain();	
		}

	// Alt-A
	if(wParam == L'A')
		{
		AutoSize();
		}
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::MyKeyDown(UINT wParam, UINT lParam)

{
	fadetimer = timeout;

	//P2N(_T("CYmemo::MyKeyDown(UINT wParam=%d, UINT lParam=%d)\r\n"), wParam, lParam);
	//P2N(_T("CYmemo::MyKeyDown ctrl=%d, shift=%d)\r\n"), misc.IsCtrlDown(), misc.IsShiftDown());

	switch(wParam)
		{
		case '\t':
			{
				//P2N(_T("CYmemo::MyKeyDown tab\r\n"));
				NextMemo();
				break;
			}
		
		case 'C':
			if(misc.IsCtrlDown())
				//P2N(_T("CYmemo::MyKeyDown Ctrl-C\r\n"));
				CopyToClip();
			break;

		case 'V':
			if(misc.IsCtrlDown())
				//P2N(_T("CYmemo::MyKeyDown Ctrl-V\r\n"));
				PasteClip();
			break;

		case VK_PRIOR:
			scline -= 5;
			UpdateScroll();
			break;

		case VK_NEXT:
			scline += 5;
			UpdateScroll();
			break;
	
		case VK_UP:
			scline--;
			UpdateScroll();
			break;

		case VK_DOWN:
			scline++;
			UpdateScroll();			
			break;

		case VK_HOME:
			scline = 0;
 			UpdateScroll();
			break;

 		case VK_END:
	 		scline = linecount - 1;
 			UpdateScroll();
			break;
		}
}

//////////////////////////////////////////////////////////////////////////

BOOL CYmemo::PreTranslateMessage(MSG* pMsg) 

{
	//P2N(_T("Pretranslate:  %s\r\n"), mxpad.num2msg(pMsg->message));

	if(pMsg->message == WM_KEYDOWN)
		{
		//if(pMsg->wParam == VK_TAB || pMsg->wParam == VK_ESCAPE || 
		//	pMsg->wParam == VK_RETURN)
		//	return CTransDlg::PreTranslateMessage(pMsg);

		//P2N(_T("CYmemo::PreTranslateMessage key=%d\r\n"), pMsg->wParam);

		MyKeyDown(pMsg->wParam, pMsg->lParam);

		return true;
		}

	if(pMsg->message == WM_SYSKEYDOWN)
		{
		if(pMsg->wParam == VK_F10)
			{
			//P2N(_T("CYmemo::PreTranslateMessage F10 on Ymemo\r\n"));
			PopMenu();
			}
		else
			{
			//P2N(_T("CYmemo::PreTranslateMessage sys key=%d\r\n"),
			//					pMsg->wParam);
			MySysKeyDown(pMsg->wParam, pMsg->lParam);
			}
		return true;
		}
	
	if(pMsg->message == WM_MOUSELEAVE)
		{
		//P2N(_T("Mouse leave message\r\n"));
		
		mousein = false;
		if(tipped + 200 < GetTickCount())
			{
			tip.Hide();	tipped = 0;
			}

		capped = false;
		return 0;
		}

	if(pMsg->message == WM_MOUSEHOVER )
		{
		//P2N(_T("CYmemo::PreTranslateMessage Mouse hover message\r\n"));
		
		if(!tipped && GetFocus() == this)
			{
			//CPoint screen = mouse; 	ClientToScreen(&screen);

			CPoint mouse2; GetCursorPos(&mouse2);
			mouse2.x += 20;	mouse2.y += 20;

			//tip.SetWindowPos(NULL, screen.x + 15, screen.y + 15, 0, 0, SWP_NOSIZE  | SWP_NOZORDER | SWP_NOACTIVATE);
			tip.Move(mouse2);

			CRect cr(closerect);	cr.InflateRect(5,5);
			CRect cr2(copyrect);	cr2.InflateRect(5,5);
			CRect cr3(rchead);		cr3.InflateRect(5,5);
			CRect cr4(rxrect);		cr4.InflateRect(5,5);
			CRect cr5(urlrect);		cr5.InflateRect(5,5);
			CRect cr6(wavrect);		cr6.InflateRect(5,5);
			
			if(cr.PtInRect(mouse))
				{
				tip.SetText(_T("Click to Close Memo Window.\r\n")
						_T(" To permanently close this Sticky, \r\n")
						_T(" uncheck its entry from CryptoSticky main contro"));
				}
			else if(cr2.PtInRect(mouse))
				{
				tip.SetText(_T(" Copy Sticky to clipboard. "));
				}
			else if(cr5.PtInRect(mouse))
				{
				tip.SetText (_T(" Jump to Sticky URL.\r\n")
							_T("The memo URL is parsed from the first line of the Sticky."));
				}
			else if(cr6.PtInRect(mouse))
				{
				tip.SetText(_T(" Click to play Sticky sound, Click again to stop play. "));
				}
			else if(cr3.PtInRect(mouse))
				{
				tip.SetText(_T(" Click, hold and drag Mouse to move Sticky Window. "));
				}
			else if(cr4.PtInRect(mouse))
				{
				tip.SetText(_T(" Click on scroll bar to scroll text "));
				}
			else
				{
				tip.SetText(_T(" Double click to Edit Sticky. \r\n")
						_T(" Shift Double Click to open CryptoSticky Main Window.\r\n ")
						_T(" Right Click (or Press F10) to show Memo Menu."));
				}

			tipped = GetTickCount();
			
			if(old_tip != tip.GetText())
				tip.Show();	
			
			old_tip = tip.GetText();
			
			capped = false;
			}
		return 0;
		}
	
	return CTransDlg::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::UpdateScroll()

{
	// Set scroll parameters
	if(scline < 0) scline = 0;
	if(scline > linecount) scline = linecount;

	//int sc = scline; int old = 0;
	//while(1==1)
	//	{
	//	if(!sc)
	//		break;
	//	int pos = m_edit.Find(_T("\r\n"), old);
	//	if(pos < 0)
	//		 break;
	//	sc--;
	//	old = pos + 2;
	//	}
	//m_str = m_edit.Right(m_edit.GetLength() - (old ));

	Invalidate();
}

BOOL CYmemo::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 

{
	// TODO: Add your message handler code here and/or call default

	if(zDelta > 0)
		scline -= 5;
	else
		scline += 5;

	UpdateScroll();
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::OnMove(int x, int y) 
 
{
	moved = true; 	
	CTransDlg::OnMove(x, y);
 	
 	//P2N(_T("CYmemo::OnMove %d %d\r\n"), x, y);	
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::OnLButtonUp(UINT nFlags, CPoint point) 

{
	if(sizing)
		{
		sizing = false;
		ReleaseCapture();
		}

	if(sbcap)
		{
		sbcap = false;
		ReleaseCapture();
		}

	if(draw)
		{
		draw = false;
		//SaveContent();
		SetTimer(3, 1000, NULL);				
		}

	SavePos();

	CTransDlg::OnLButtonUp(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::OnRButtonDblClk(UINT nFlags, CPoint point) 

{
	::ShowWindow(AfxGetMainWnd()->m_hWnd, SW_RESTORE);
	::SetWindowPos(AfxGetMainWnd()->m_hWnd, HWND_TOP, 0, 0, 0, 0, 
			SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);

	CTransDlg::OnRButtonDblClk(nFlags, point);
}

void CYmemo::Show()

{
	CTransDlg::Show();

	WINDOWPLACEMENT wp2; GetWindowPlacement(&wp2);

	//P2N(_T("CYmemo::Show '%s' T=%d L=%d B=%d R=%d\r\n"), title, 		
	//		wp2.rcNormalPosition.top, wp2.rcNormalPosition.left,
	//			wp2.rcNormalPosition.bottom, wp2.rcNormalPosition.right);	
	
	//AssertValid();	//ASSERT(IsWindow(m_hWnd));

	// Set to persist if needed
	const CWnd *wndref = &wndNoTopMost;

	//if(memo.flags.persist || tmptop)
	if(memo.flags.persist)
		wndref = &wndTopMost;

	SetWindowPos(wndref, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_SHOWWINDOW   );
	
	fadetimer = timeout = (int)memo_timeout;

	SetTimer(4, 20, NULL);	 // To load content
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::LoadContent(int refresh)

{
	//P2N(_T("CYmemo::LoadContent Loading Memo file '%s'\r\n"), support.GetFileNameExt(memofile));

	if(tmptop)
		{
		refresh = true;
		}	

	if(!refresh)
		{
		//P2N(_T("No refresh requested\r\n"));
		return;
		}
	
	m_edit = _T(""); linecount = 0; scline = 0;

	// Read in file
	int ret = memo.Read(memofile);

	//P2N(_T("Reading memo head %p '%s'\r\n"), this, memo.head);
	//P2N(_T("Reading memo body %p '%s'\r\n"), this, memo.body);

	if(!ret)
		{
		if(title.Mid(0, 9) !=_T("\\Deleted\\"))
			{
			memo.head =_T("Deleted Item");
			memo.body.Format( 
			_T("This is a deleted folder Item, \r\n")
			_T("The file is associated with it does not exist.\r\n")
			_T("%s\r\n"),  memofile);
			}
		else
			{
			memo.body =_T("ERROR: Missing File for this Sticky. (Possibly ... network memo deleted by other user)");
			}
		errhide = true;
		}
	else
		{
		refresh = false;
		}
	GetParms();

	// Re-assemble to new file name
	CString dir = support.GetDirName(memofile);
	CString fname = support.GetFilename(memofile);

	drawname = dir +_T("\\") + fname +_T(".ymd");
	
	//P2N(_T("Loading entries from %s\r\n"), drawname);	
	LoadDrawing(drawname);
	
	wavname = dir +_T("\\") + fname +_T(".wav");
	//P2N(_T("Loading entries from %s\r\n"), wavname);
	
	if(_taccess(wavname, 0) >= 0)
		{
		iswave = true;
		}
	else
		{
		iswave = false;
		}

	// See if it has an image
	CString section(_T("BackImage"));
	CString fname2 = memo.MakeFileName();
	backimage = AfxGetApp()->GetProfileString(section, fname2);
	//P2N(_T("Reading registry: '%s' '%s'\r\n"), fname2, backimage);
	
	if(backimage != _T(""))
		backimg.Load(backimage);	

	if(wantauto)
		{	
		wantauto = false;
		AutoSize();
		}

	
}

//////////////////////////////////////////////////////////////////////////

int CYmemo::SaveContent()

{
	int ret = memo.Write(memofile);

	// Save scribble as well	
	int alen = gesture.GetSize();
	
	// Re-assemble to new file name
	CString dir = support.GetDirName(memofile);
	CString fname = support.GetFilename(memofile);

	CString newname = dir +_T("\\") + fname +_T(".ymd");
	
	//P2N(_T("Saving %d entries to %s\r\n"), alen, newname);

	if(alen == 0)
		{
		_tunlink(newname);
		return 0;		
		}

	CFile cf;  
    if(cf.Open(newname, CFile::modeWrite | CFile::modeCreate ) == 0)
		{
		P2N(_T("CYmemo::SaveContent cannot write %s\r\n"), newname);
		return -1;
		}
	
	for(int loop = 0; loop < alen; loop++)
		{
		int val = gesture.GetAt(loop);
		//P2N(_T("%d "), val);

		CString tmp; tmp.Format(_T("%d "), val);
		cf.Write(tmp, tmp.GetLength());

		if((loop != 0) && (loop % 10) == 0)
			{
			cf.Write(_T("\r\n"), 2);
			}
 		}
	//P2N(_T("\r\n"));

	cf.Close();

	return ret;
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::Hide()

{
	//P2N(_T("CYmemo::Hide\r\n"));

	// Update count only if it was shown
	if(showstate)
		lasthide = GetTickCount();

	CTransDlg::Hide();
}

void CYmemo::OnRButtonDown(UINT nFlags, CPoint point) 

{
	//P2N(_T("CYmemo::OnRButtonDown\r\n"));

	//CMenu   mm = IDR_MENU4, *pp; pp = mm.GetSubMenu( 0 );
	//pp->TrackPopupMenu( TPM_LEFTALIGN, mouse.x, mouse.y, this);

	PopMenu();
	
	CTransDlg::OnRButtonDown(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::PopMenu() 

{
	//CXrayM   *mm = new CXrayM(IDD_MENUBASE, this);
	CXrayM   *mm = new CXrayM(this);
	
	lasthide = GetTickCount();

	if(tmptop)
		mm->ontop = true;
	
	mm->sendto = this;
	mm->LoadMenu(IDR_MENU4, 1);

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	cc->MenuAddNetList(mm);

	int idx = mm->FindMenuItem(ID_RCLICKMEMO_ERASER);
	mm->SetCheck(idx, erase);

	// Add old drawings list to menu
	CXrayM *rr = mm->FindSubmenu(_T("Drawings")); 

	// If this is the right menu to go for ...
	if(rr)
		{
		int cnt = 1;
		rr->Clear();

		rr->AppendMenuItem(_T("Show current drawing"), ID_RCLICKMEMO_OLDERDRAWINGS_NONE);

		// Fill in history		
		CString name = support.GetFilename(memofile);

		long hFile; int ret = 0;
		struct _tfinddata_t c_file;

		CString fname; fname.Format(_T("%sbackup\\%s*.ymd"), dataroot, name);
		hFile = _tfindfirst(fname, &c_file);
		while(TRUE)
			{
			if(hFile == -1 || ret)
				break;

			CString tmp; tmp.Format(_T("Older Drawing %s"), c_file.name);
			rr->AppendMenuItem(tmp, ID_RCLICKMEMO_OLDERDRAWINGS_NONE + cnt);
			cnt++;
			
			ret = _tfindnext( hFile, &c_file );
			}

		maxcomm = cnt;
		}

	// Add old wave list to menu
	CXrayM *rr2 = mm->FindSubmenu(_T("Recordings")); 

	// If this is the right menu to go for ...
	if(rr2)
		{
		int cnt = 1;
		rr2->Clear();

		rr2->AppendMenuItem(_T("Play current recording"), ID_RCLICKMEMO_OLDERRECORDINGS_NONE);

		// Fill in history		
		CString name = support.GetFilename(memofile);

		long hFile; int ret = 0;
		struct _tfinddata_t c_file;

		CString fname; fname.Format(_T("%sbackup\\%s*.wav"), dataroot, name);
		hFile = _tfindfirst(fname, &c_file);
		while(TRUE)
			{
			if(hFile == -1 || ret)
				break;

			CString tmp; tmp.Format(_T("Play Recording %s"), c_file.name);

			rr2->AppendMenuItem(tmp, ID_RCLICKMEMO_OLDERRECORDINGS_NONE + cnt);
			cnt++;
			
			ret = _tfindnext( hFile, &c_file );
			}

		maxcomm2 = cnt;
		}

	CPoint mouse; GetCursorPos(&mouse);	mm->Show(mouse.x, mouse.y);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::OnKillFocus(CWnd* pNewWnd) 

{
	CTransDlg::OnKillFocus(pNewWnd);
	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::NextMemo()

{
	//P2N(_T("NextMemo\r\n"));

	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;

	// Build temporery list of shown windows
	CPtrList tmplist;
	POSITION pos = cc->ylist.GetHeadPosition();
	while(true)
		{
		if(!pos)
			break;

		CYmemo *st = (CYmemo *) cc->ylist.GetNext(pos);

		//ASSERT(st->magic == CYmemo_Magic);

		if(st->showstate)
			{
			//P2N(_T("list '%s'\r\n"), st->memo.head);
			tmplist.AddTail(st);
			}
		}

	// Find the one with the same file name:
	POSITION pos2 = tmplist.GetHeadPosition();
	while(true)
		{
		if(!pos2)
			break;

		CYmemo *st = (CYmemo *) tmplist.GetNext(pos2);

		//P2N(_T("scan => '%s'\r\n"), st->memo.head);

		if(st->memofile == memofile)
			{	
			CYmemo *st2;

			// Last one? Switch to first
			if(!pos2)
				{
				pos2 = tmplist.GetHeadPosition();
				st2 = (CYmemo *) cc->ylist.GetNext(pos2);
				}
			else
				{
				st2 = (CYmemo *) tmplist.GetNext(pos2);
				}

			//P2N(_T("hit => '%s'\r\n"), st2->memo.head);

			st2->SetActiveWindow();			
			//st2->SetTransparency(main_trans);	
			st2->StartFadeCycle();

			break;
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::AutoSize()

{
	CDC* pDC = GetDC( ); //	//ASSERT(pDC);
	if(!pDC) return;

	// No autosize on image
	if(backimg.IsValid())
		return;

	CxRect monrc; misc.WinMonitorRect(this, &monrc);

	//P2N(_T("CYmemo::AutoSize %s\r\n"), memo.head);

	CxRect rc, rc4, rc5, rc6; GetClientRect(&rc);
	rc5.right = rc.right;

	CFont *oldfont = pDC->SelectObject( &mymemo_font );
	
	pDC->SetTextAlign(TA_LEFT);
	pDC->DrawText(_T("A"), &rc4, DT_CALCRECT | DT_EXPANDTABS );
	pDC->DrawText(memo.body, &rc5, DT_CALCRECT | DT_EXPANDTABS | DT_WORDBREAK );

	pDC->SelectObject( &myhead_fontb );
	pDC->DrawText(memo.head, &rc6, DT_CALCRECT | DT_EXPANDTABS);

	rc5.bottom += GetHeadHeight();

	if(iswave)
		{
		rc6.right += 72;
		}
	else
		{
		rc6.right += 56;
		}

	rc5.right = max(rc5.right, rc6.right);

	pDC->SelectObject(oldfont);	ReleaseDC(pDC);

	//P2N(_T("CYmemo::AutoSize body left=%d right=%d top=%d bottom=%d\r\n"), rc5.left, rc5.right, rc5.top, rc5.bottom);
	//P2N(_T("CYmemo::AutoSize head left=%d right=%d top=%d bottom=%d\r\n"), rc6.left, rc6.right, rc6.top, rc6.bottom);

	// Adjust for padding
	rc5.right += xpad + 2;	rc5.bottom += ypad + 2;

	// Contain min/max size of memo
	if(rc5.right < 100)
		rc5.right = 100;

	if(rc5.right > 600)
		rc5.right = 600;

	if(rc5.bottom > 3 * monrc.Height() / 4)
		{
		//vscroll = true;
		rc5.bottom = 3 * monrc.Height() / 4;
		}
	else
		{
		vscroll = false;
		}

	int alen = gesture.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		DWORD val = gesture.GetAt(loop);
		int xx = val & 0xffff, yy =  val >> 16;
		
		if(xx > rc5.right)
			rc5.right = xx;

		if(yy > rc5.bottom)
			rc5.bottom = yy;
		}

	const CWnd *wndref = &wndNoTopMost;
	
	// Set to persist
	if(memo.flags.persist || tmptop)
		{
		wndref = &wndTopMost;
		}
	
	SetWindowPos(wndref, 0, 0, rc5.right + rc4.Width(), 
								rc5.bottom + rc4.Height(), SWP_NOMOVE);
	moved = true;
	SavePos();

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

BOOL CYmemo::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 

{
	RECT rect; GetClientRect(&rect);
	POINT point; GetCursorPos(&point); ScreenToClient(&point);

	if( abs(point.x - rect.right) < 10 && abs(point.y - rect.bottom) < 10)
		{
		//P2N(_T("CYmemo::OnSetCursor Close to corner\r\n"));
		::SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE))
		);
		return true;
		}

	// Clicking on close
	CRect cr1(closerect); cr1.InflateRect(5,5);
	if(cr1.PtInRect(point))
		{
		::SetCursor(AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_CLOSE)));
		return true;
		}

	// Clicking on copy
	CRect cr2(copyrect); cr2.InflateRect(5,5);
	if(cr2.PtInRect(point))
		{
		::SetCursor(AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_COPY)));
		return true;
		}

	// Clicking on 'u'
	CRect cr5(urlrect);	cr5.InflateRect(5,5);
	if(cr5.PtInRect(point))
		{
		::SetCursor(AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_URL)));
		return true;
		}

	// Clicking on 'v'
	if(iswave)
		{		
		CRect cr7(wavrect);	cr7.InflateRect(5,5);
		if(cr7.PtInRect(point))
			{
			::SetCursor(AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_PLAY)));
			return true;
			}
		}
	
	CRect cr6(rchead);	cr6.InflateRect(5,5);
	if(cr6.PtInRect(point))
		{
		::SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
		return true;
		}

	if(draw)
		{
		if(erase)
			::SetCursor(AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_CURSOR3)));
		else
			::SetCursor(AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_CURSOR2)));
		return true;
		}
	
	return CTransDlg::OnSetCursor(pWnd, nHitTest, message);
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::Refresh()

{
	memo.flags.headsize = headsize;
	memo.flags.textsize = textsize;

	memo.flags.textcol = textcol;
	memo.flags.headcol = headcol;

	memo.flags.headfontcol = head_fontcol2;	
	memo.flags.textfontcol = memo_fontcol2;

	memo.Write(memofile);
	
	//SetTransparency(main_trans); fadetimer = timeout;

	if(backimage != _T(""))
		if(backimage != backimg.current)
			backimg.Load(backimage);

	StartFadeCycle();

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::SavePos()

{
	if(moved)
		{
		//P2N(_T("CYmemo::SavePos()\r\n"));
		moved = false;

		WINDOWPLACEMENT wp2; GetWindowPlacement(&wp2);

		int hh2  = wp2.rcNormalPosition.bottom - wp2.rcNormalPosition.top;	
		int ww2  = wp2.rcNormalPosition.right - wp2.rcNormalPosition.left;

		CString tmp; tmp.Format(_T("%d %d %d %d"),
			wp2.rcNormalPosition.top, wp2.rcNormalPosition.left,
					wp2.rcNormalPosition.bottom,  wp2.rcNormalPosition.right);	

		CString section(_T("Positions"));	
		AfxGetApp()->WriteProfileString(section, memofile, tmp);		
		}
}

//////////////////////////////////////////////////////////////////////////

void CYmemo::GetParms()

{
	int old = 0;

	while(true)
		{
		int pos = memo.body.Find(_T("\r\n"), old);
		
		if(pos < 0)
			 break;

		old = pos + 2;
		linecount++;	
		}

	if(memo.flags.headcol)
		headcol = memo.flags.headcol;

	if(memo.flags.textcol)
		textcol = memo.flags.textcol;

	if(memo.flags.headsize)
	    headsize = memo.flags.headsize;

	if(memo.flags.textsize)
		textsize = memo.flags.textsize;

	if(memo.flags.headfontcol)
		head_fontcol2 = memo.flags.headfontcol;

	if(memo.flags.textfontcol)
		memo_fontcol2 = memo.flags.textfontcol;

	LOGFONT lf;
	myhead_font.GetObject(sizeof(LOGFONT), &lf);
	lf.lfHeight = headsize;
	myhead_font.DeleteObject();
	myhead_font.CreateFontIndirect(&lf);
	
	lf.lfWeight = FW_BOLD;
	myhead_fontb.DeleteObject();
	myhead_fontb.CreateFontIndirect(&lf);
	
	LOGFONT lf2;
	mymemo_font.GetObject(sizeof(LOGFONT), &lf2);
	lf2.lfHeight = textsize;
	mymemo_font.DeleteObject();
	mymemo_font.CreateFontIndirect(&lf2);	
}

//////////////////////////////////////////////////////////////////////////
//

void CYmemo::DelayedInval()

{
	//P2N(_T("CYmemo::DelayedInval\r\n"));

	DWORD  tick = GetTickCount();

	// Timer has been retriggered for too long, force it
	if(tick - fired > 100)
		{
		//P2N(_T("CYmemo::DelayedInval force\r\n"));

		if(IsWindow(m_hWnd))
			Invalidate();

		fired = tick;
		}
	else
		{
		//P2N(_T("CYmemo::DelayedInval timer\r\n"));
		SetTimer(2, 100, NULL);				
		}
}

//////////////////////////////////////////////////////////////////////////
// Get header height on the fly

int		CYmemo::GetHeadHeight()

{	
	CPaintDC dc(this); // device context for painting

	//P2N(_T("CYmemo::OnPaint %p %s\r\n"), this, memo.head);

	CFont *oldfont;
	oldfont = dc.SelectObject( &myhead_fontb );

	if(memo.head == _T(""))
		dc.DrawText(_T("A"), htxtrect, DT_EXPANDTABS | DT_CALCRECT );
	else
		dc.DrawText(memo.head, htxtrect, DT_EXPANDTABS | DT_CALCRECT );

	//P2N(_T("GetHeadHeight %s = %d\r\n"), memo.head, htxtrect.Height());

	return(htxtrect.Height());
}

void	CYmemo::StartFadeCycle()

{
	SetTransparency(main_trans); fadetimer = timeout;
}

//////////////////////////////////////////////////////////////////////////

BOOL CYmemo::OnCommand(WPARAM wParam, LPARAM lParam) 

{
	DWORD num_coms = 0;
	CXrayM *mm2 = (CXrayM*)FromHandlePermanent((HWND)lParam);

	//P2N(_T("CYmemo::OnCommand l=%d h=%d \r\n"), LOWORD(wParam), HIWORD(wParam));

	// Is menu command?
	if(HIWORD(wParam) == 0 && mm2->magic == MENU_MAGIC)
		{
		num_coms = mm2->arr.GetSize();
		//P2N(_T("Command from menu %d %d\r\n"), num_coms, maxcomm);
		}

	if(wParam >= ID_RIGHT_SENDNETWORKSTICKYTO_FIRST && 
					wParam < ID_RIGHT_SENDNETWORKSTICKYTO_FIRST + num_coms)
		{
		// Forward this to the main dialog
		YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
		cc->OnCommand(wParam, lParam);
		}
	
	if(wParam >= ID_RCLICKMEMO_OLDERDRAWINGS_NONE &&
				wParam < ID_RCLICKMEMO_OLDERDRAWINGS_NONE + num_coms)
		{
			// Figure out the machine to send to
		CXrayM *mm = (CXrayM*)FromHandlePermanent((HWND)lParam);
		//ASSERT(mm); //ASSERT(mm->magic == MENU_MAGIC);

		if(ID_RCLICKMEMO_OLDERDRAWINGS_NONE == wParam)
			{
			//P2N(_T("Restoring original\r\n"));
			LoadContent(true);
			Invalidate();
			}
		else
			{
			CString str;
			mm->GetMenuItemText(wParam - ID_RCLICKMEMO_OLDERDRAWINGS_NONE, str);

			int idx = str.ReverseFind(' ');
			if(idx >= 0)
					str = str.Mid(idx + 1);

			//P2N(_T("Command from older drawing '%s'\r\n"), str);
			
			CString backup = support.GetDirName(memofile);
			backup +=_T("\\backup\\");	backup += str;

			LoadDrawing(backup);		
			Invalidate();
			}
		}

	if(wParam >= ID_RCLICKMEMO_OLDERRECORDINGS_NONE &&
				wParam < ID_RCLICKMEMO_OLDERRECORDINGS_NONE + num_coms)
		{
		CString wname;

		CXrayM *mm = (CXrayM*)FromHandlePermanent((HWND)lParam);
		//ASSERT(mm); //ASSERT(mm->magic == MENU_MAGIC);

		if(ID_RCLICKMEMO_OLDERRECORDINGS_NONE == wParam)
			{			
			CString dir = support.GetDirName(memofile);
			CString fname = support.GetFilename(memofile);
			wname = dir +_T("\\") + fname +_T(".wav");		
			//P2N(_T("Play original wav '%s'\r\n"), wname);
			}
		else
			{
			CString str;
			mm->GetMenuItemText(wParam - ID_RCLICKMEMO_OLDERRECORDINGS_NONE, str);

			int idx = str.ReverseFind(' ');
			if(idx >= 0)
				str = str.Mid(idx + 1);
						
			// Re-assemble to new file name			
			CString dir = support.GetDirName(memofile);
			wname = dir +_T("\\backup\\") + str;

			P2N(_T("Play older wav '%s'\r\n"), wname);
			}

		if(_taccess(wname, 0) >= 0)
			{
			P2N(_T("Playing file '%s'\r\n"), wname);
			
			playsound.WaveOpen();

			char *buf; int len;
			int ret = playsound.LoadWave(wname, &buf, &len);
			if(ret)
				playsound.PlayWave(buf, len, true);
			}
		}

	return CTransDlg::OnCommand(wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
//

void	CYmemo::LoadDrawing(const TCHAR *drawname)

{
	gesture.RemoveAll();

	CFile cf;  
    if(cf.Open(drawname, CFile::modeRead) !=0 )
		{	
		CString entry;	TCHAR num = 0, eof = 0;

		int len = (int)cf.GetLength();
		char *mem = (char*)malloc(len);

		if(!mem)
			{
			AfxMessageBox(_T("Cannot allocate memory for drawing"));
			return;
			}

		int rlen = cf.Read(mem, len);

		int prog = 0;
		while(true)
			{
			entry = _T("");		
			while(true)
				{
				num = mem[prog++];

				if(!isdigit(num))
					break;				

				entry += num;

				if(prog >= len)
					break;
 				}

			if(entry != _T(""))
				gesture.Add(_tstoi(entry));

			if(prog >= len)
				break;
			}
		cf.Close();
		free(mem);
		}
	else
		{
		//P2N(_T("CYmemo::LoadContent cannot open %s\r\n"), drawname);
		}
}
//////////////////////////////////////////////////////////////////////////

void CYmemo::OnRclickmemoAppendfromclipboard()

{
	//P2N(_T("OnRclickmemoAppendfromclipboard\r\n"));

	fadetimer = timeout;

	CString text;

	if(!OpenClipboard())
		{
		AfxMessageBox(_T("Cannot open clipboard"));
		return;
		}

	HANDLE htext = GetClipboardData(CF_UNICODETEXT);

	if (htext)
		{
		LPVOID ptr = GlobalLock(htext);
		//ASSERT(ptr);
		text = (const TCHAR *)ptr;
		GlobalUnlock(htext);
		}

	CloseClipboard();

	if(text == _T(""))
		{
		AfxMessageBox(_T("No text on clipboard.\r\n"));
		return;
		}

	memo.body = memo.body + text;
	memo.Write(memofile); 		
	
	GetParms();
	//AutoSize();
	Invalidate();
}


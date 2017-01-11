// gotoline.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "gotoline.h"

//include "srcsel.h"					  
//include "strlist.h"
//include "Weddoc.h"
//include "WedView.h"

#include "mxpad.h"
//include "editor.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// Cgotoline dialog


Cgotoline::Cgotoline(CWnd* pParent /*=NULL*/)
	: CDialog(Cgotoline::IDD, pParent)
{
	//{{AFX_DATA_INIT(Cgotoline)
	m_sticky = _T("");
	m_str = _T("");
	//}}AFX_DATA_INIT
	m_esc = FALSE;

}


void Cgotoline::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Cgotoline)
	DDX_Control(pDX, IDC_MEMO, m_memo);
	DDX_Control(pDX, IDC_TITLE, m_title);
	DDX_Text(pDX, IDC_EDIT3, m_sticky);
	DDX_Text(pDX, IDC_EDIT1, m_str);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Cgotoline, CDialog)
	//{{AFX_MSG_MAP(Cgotoline)
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cgotoline message handlers

BOOL Cgotoline::OnInitDialog() 
 
{ 
	CDialog::OnInitDialog();
	SetWindowText(caption);   

	if(!m_bitmap.LoadBitmap(IDB_BITMAP5))    
		{
		//P2N(_T("Cannot load bitmap\r\n"));   
		}
 
	return TRUE;
}

void Cgotoline::OnCancel() 

{
	// TODO: Add extra cleanup here
	m_esc = TRUE;	
	CDialog::OnCancel();
}

BOOL Cgotoline::OnEraseBkgnd(CDC* pDC) 

{
#if 0
	CDC  dcMem;
	dcMem.CreateCompatibleDC(pDC);

	RECT rect;
	GetClientRect(&rect);

	//P2N(_T("Called on erase background %d\r\n"), m_bf.SourceConstantAlpha);

	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_bitmap);

	//CClientDC dc(this);
	//AlphaBlend(dc, 0,0, m_nWidth, m_nHeight, dcMem, 0,0, m_nWidth, m_nHeight, m_bf); 
	
	pDC->BitBlt(0, 0, rect.right - rect.left, rect.bottom - rect.top, &dcMem, 0,0, SRCCOPY ); 		
	
	dcMem.SelectObject(pOldBitmap);

	//image.Draw(pDC->GetSafeHdc());

	// We handled it
	//return(true);
	
#endif
	return CDialog::OnEraseBkgnd(pDC);
}

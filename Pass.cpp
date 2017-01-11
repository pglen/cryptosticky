
/* =====[ Pass.cpp ]========================================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Pass.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/16/2010  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */


#include "stdafx.h"
#include "CryptoSticky.h"
#include "support.h"
#include "Pass.h"
#include "XntMask.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CPass dialog


CPass::CPass(CWnd* pParent /*=NULL*/)
	: CDialog(CPass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPass)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	vpos = hpos = 0; alpha = 80; astep = 10;
	m_BackColor = RGB(192,192,192);

	m_Bitmap.LoadBitmap(IDB_BITMAP12);
	
	if(m_Bitmap.m_hObject != NULL)
		{
		m_Bitmap.GetBitmap(&BitMap);
		m_nWidth  = BitMap.bmWidth;
		m_nHeight = BitMap.bmHeight;
		}
}

//////////////////////////////////////////////////////////////////////////

CPass::~CPass()

{
	// Destroy pass
	//CString str; int lenp = pass.GetLength(); 
	//support.RandStr(lenp, lenp, str); pass = str;

	// Destroy pass
	support.ObfuscateStr(pass);
}

//////////////////////////////////////////////////////////////////////////

void CPass::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPass)
	DDX_Control(pDX, IDC_INFO2, m_info2);
	DDX_Control(pDX, IDC_INFO, m_info);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_can);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPass, CDialog)
	//{{AFX_MSG_MAP(CPass)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPass message handlers

void CPass::OnOK() 

{
	CString str1, str2;
	
	((CEdit*)GetDlgItem(IDC_EDIT1))->GetWindowText(str1);
	((CEdit*)GetDlgItem(IDC_EDIT2))->GetWindowText(str2);

	if(str1 != str2)
		{
		MessageBox(_T("Passwords/Keys do not match."));

		((CEdit*)GetDlgItem(IDC_EDIT1))->SetSel(0, -1);
		((CEdit*)GetDlgItem(IDC_EDIT1))->SetFocus();

		return;
		}

	pass = str1;

	// OverWrite with random
	CString strx, stry;
	int len1 = str1.GetLength(); support.RandStr(len1, len1, strx);
	int len2 = str2.GetLength(); support.RandStr(len2, len2, stry);

	// Because they are the same length, will not realloc
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetWindowText(strx);
	((CEdit*)GetDlgItem(IDC_EDIT2))->SetWindowText(stry);

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////
//

BOOL CPass::OnEraseBkgnd(CDC* pDC) 

{
	// No bitmap, just paint
	if(!m_Bitmap.m_hObject)  
		{
		return CDialog::OnEraseBkgnd(pDC);
		}

	CDialog::OnEraseBkgnd(pDC);

	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	//CClientDC dc(this);	

	return true;	
	//return CDialog::OnEraseBkgnd(pDC);
}

BOOL CPass::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(title != "")
		{
		SetWindowText(title);
		}

	SetTimer(1, 300, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CPass::OnTimer(UINT nIDEvent) 

{
	vpos++;	hpos++;	
	
	if(alpha > 150)  
		{
		astep = -10;
		}
	if(alpha < 50)  
		{
		astep = 10;
		}

	alpha += astep;

	RECT rc; rc.top = 20; rc.left = 5;
	rc.bottom = m_nHeight;
	rc.right = m_nWidth;

	//CPaintDC dc(this);
	//dc.FillSolidRect(&rc, RGB(100,100,100));

	InvalidateRect(&rc);
	//Invalidate();

	CDialog::OnTimer(nIDEvent);
}

void CPass::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect; GetClientRect(&rect); 
	
	XntMask xmask; 

	CDC  dcMem; dcMem.CreateCompatibleDC(&dc);
	int  save = dcMem.SaveDC();

	CBitmap *pOldBitmap  =  dcMem.SelectObject(&m_Bitmap);

	//CBrush brush(RGB(111,222, 33));
	//CPen pen(PS_SOLID, 1, RGB(222, 111, 33));
	//dcMem.SelectObject(&brush);	dcMem.SelectObject(&pen);
	
	//CRect rc; 
	//rc.top = 0; rc.left = 0; rc.right = m_nWidth; rc.bottom = m_nHeight;

	//dcMem.Ellipse(&rc); 

	HBITMAP hBmpMask = xmask.MakeMaskBmpDC(dcMem, m_nWidth, m_nHeight, RGB(255, 255, 255));

	//pDC->BitBlt(0, 0, m_nWidth, m_nHeight, &dcMem, 0,0, SRCCOPY); 		
	//pDC->BitBlt(0, 0, m_nWidth, m_nHeight, &dcMem2, 0,0, SRCCOPY); 		
	 
	int loopx = 0, loopy = 0;  
	//for (loopy = 0; loopy < rect.Height(); loopy += m_nHeight)
		{
		for (loopx = 0; loopx < rect.Width(); loopx += m_nWidth)
			{
			//pDC->BitBlt(loopx, loopy, m_nWidth, m_nHeight, &dcMem, 0,0, SRCCOPY ); 		

				// Put it out
				xmask.MaskBlt(dc.GetSafeHdc(), loopx, loopy,	
							m_nWidth, m_nHeight,
								dcMem, 0, 0, hBmpMask, 0, 0,
									MAKEROP4(SRCCOPY, DSTCOPY), alpha);
				}

		}

	dcMem.SelectObject(pOldBitmap);
	dcMem.RestoreDC(save);
	
}

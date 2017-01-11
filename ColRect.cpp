
/* =====[ ColRect.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the ColRect.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// ColRect.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "ColRect.h"

#include "mxpad.h"


#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#undef DEBUG_NEW
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CColRect

CColRect::CColRect()
{
	bgcolor = 0;
}

CColRect::~CColRect()
{
}


BEGIN_MESSAGE_MAP(CColRect, CStatic)
	//{{AFX_MSG_MAP(CColRect)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColRect message handlers

BOOL CColRect::OnEraseBkgnd(CDC* pDC) 

{
	//P2N(_T("CColRect: Erase background\r\n"));

	RECT rect; // ,m_rDataBox;
	GetClientRect(&rect);

	CBrush NewBrush;
	NewBrush.CreateSolidBrush(bgcolor) ;
	
	pDC->SetBrushOrg(0,0) ;
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&NewBrush);
	pDC->PatBlt(rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, PATCOPY);
	pDC->SelectObject(pOldBrush);
	
	//return CStatic::OnEraseBkgnd(pDC);

	return(true);	
}

//////////////////////////////////////////////////////////////////////////

void CColRect::SetColor(COLORREF cgb)

{
	//P2N(_T("Setting Color to: %x\r\n"), cgb);
	bgcolor = cgb;

	Invalidate();
	this->GetParent()->Invalidate();
}


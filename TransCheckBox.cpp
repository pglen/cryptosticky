// TransCheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "TransCheckBox.h"
#include "mxpad.h"


// CTransCheckBox

IMPLEMENT_DYNAMIC(CTransCheckBox, CButton)

CTransCheckBox::CTransCheckBox()
{
	P2N(_T("CTransCheckBox::CTransCheckBox\r\n"));

	m_BackColor = RGB(12, 192, 192);
}

CTransCheckBox::~CTransCheckBox()
{
}


BEGIN_MESSAGE_MAP(CTransCheckBox, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTransCheckBox message handlers



int CTransCheckBox::Hello(void)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL CTransCheckBox::OnEraseBkgnd(CDC* pDC)

{
	P2N(_T("CTransCheckBox::OnEraseBkgnd\r\n"));

	//CRect rec; GetClientRect(rec);
	//pDC->FillSolidRect(rec, m_BackColor);

	//return true;
	return CButton::OnEraseBkgnd(pDC);
}

void CTransCheckBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	P2N(_T("CTransCheckBox::OnPaint\r\n"));

	//CRect rec; GetClientRect(rec);
	//dc.FillSolidRect(rec, m_BackColor);

	// TODO: Add your message handler code here
	// Do not call CButton::OnPaint() for painting messages

	//CButton::OnPaint();
}

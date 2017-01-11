
/* =====[ xEdit.cpp ]========================================================== 

   Description:     project,
// xEdit.cpp : implementation file
//
*/

#include "stdafx.h"
#include "CryptoSticky.h"
#include "xEdit.h"
#include "mxpad.h"


IMPLEMENT_DYNAMIC(CxEdit, CEdit)

CxEdit::CxEdit()
{
	m_TextColor  = RGB(0, 0, 0);
    m_BackColor = TRANS_BACK;
}

CxEdit::~CxEdit()
{
}


BEGIN_MESSAGE_MAP(CxEdit, CEdit)
ON_WM_CTLCOLOR_REFLECT()
ON_CONTROL_REFLECT(EN_UPDATE, &CxEdit::OnEnUpdate)
END_MESSAGE_MAP()


// CxEdit message handlers

HBRUSH CxEdit::CtlColor(CDC* pDC, UINT nCtlColor)

{
	//P2N(_T("CxEdit::CtlColor\r\n"));

	m_Brush.DeleteObject();

    if (m_BackColor == TRANS_BACK) {
        m_Brush.CreateStockObject(HOLLOW_BRUSH);
        pDC->SetBkMode(TRANSPARENT);
    }
    else {
        m_Brush.CreateSolidBrush(m_BackColor);
        pDC->SetBkColor(m_BackColor);
    }

	 //pDC->SetTextColor(m_TextColor);

	return m_Brush;
}

void CxEdit::OnEnUpdate()
{
	//P2N(_T("CxEdit::OnEnUpdate\r\n"));

	CWnd* pParent = GetParent();
    CRect   rect;

    GetWindowRect(rect);
    pParent->ScreenToClient(rect);
    rect.DeflateRect(2, 2);

    pParent->InvalidateRect(rect, FALSE); 
}

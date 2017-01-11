// GetDirName.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "GetDirName.h"
#include "support.h"


#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CGetDirName dialog

CGetDirName::CGetDirName(CWnd* pParent /*=NULL*/)
	: CDialog(CGetDirName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetDirName)
	m_dir = _T("");
	m_BackColor = RGB(192, 192, 192);
	//}}AFX_DATA_INIT
}

void CGetDirName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetDirName)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_can);
	DDX_Control(pDX, IDC_PTEXT, m_ptext);
	DDX_Text(pDX, IDC_EDIT1, m_dir);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetDirName, CDialog)
	//{{AFX_MSG_MAP(CGetDirName)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//ON_BN_CLICKED(IDOK, &CGetDirName::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetDirName message handlers

BOOL CGetDirName::OnInitDialog() 
{
	CDialog::OnInitDialog();

	support.ShortenStr(txt, 64);
	m_ptext.SetText(txt);

	//m_ptext.SetTrans();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGetDirName::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HBRUSH hbr2;

	//P2N(_T("CSetup::OnCtlColor nCtlColor=%x dlgback=%x\r\n"), nCtlColor, dlgback);

	if(nCtlColor == CTLCOLOR_STATIC)
		{
		hbr2 = CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);
		}
	else if(nCtlColor == CTLCOLOR_EDIT)
		{
		//hbr2 = CreateSolidBrush(dlgback);
		hbr2 = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}
	else if(nCtlColor == CTLCOLOR_LISTBOX)
		{
		hbr2 = CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);

		}
	else if(nCtlColor == CTLCOLOR_DLG)
		{
		hbr2 = CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);
		}
	else if(nCtlColor == CTLCOLOR_BTN)
		{
		hbr2 = CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);
		}
	else
		{		
		hbr2 = CreateSolidBrush(m_BackColor);
		}
	// TODO:  Change any attributes of the DC here
	
	return hbr2;
	
}

BOOL CGetDirName::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
}

void CGetDirName::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

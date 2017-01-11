// LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "serials.h"
#include "LogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog


CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialog)
	//{{AFX_MSG_MAP(CLogDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDlg message handlers

BOOL CLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT1);
	ASSERT(ed);

	ed->SetWindowText(text);	

	CString tmp; tmp.Format(L"Number of licencss: %d", cnt);
	SetWindowText(tmp);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// NetPeerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "NetPeerDlg.h"
#include "support.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif


CString CNetPeerDlg::pro(_T("Access"));


/////////////////////////////////////////////////////////////////////////////
// CNetPeerDlg dialog

CNetPeerDlg::CNetPeerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNetPeerDlg::IDD, pParent)
{
	m_BackColor = RGB(192, 192, 192);
	
	//{{AFX_DATA_INIT(CNetPeerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CNetPeerDlg::DoDataExchange(CDataExchange* pDX)

{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetPeerDlg)
	DDX_Control(pDX, IDC_GRID, m_grid);
	//DDX_Control(pDX, IDC_BUTTON4, m_b4);
	//DDX_Control(pDX, IDC_BUTTON3, m_b3);
	//DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDOK, m_ok);
	//DDX_Control(pDX, IDC_LIST2, m_list2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MESSAGE, m_msg);
}

BEGIN_MESSAGE_MAP(CNetPeerDlg, CDialog)
	//{{AFX_MSG_MAP(CNetPeerDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetPeerDlg message handlers

BOOL CNetPeerDlg::OnInitDialog() 

{
	CDialog::OnInitDialog();

	oldhelp = helpcontext;
	helpcontext = 3;

	Fill();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CNetPeerDlg::OnButton1() 

{
	Fill() ;
}

//////////////////////////////////////////////////////////////////////////

void CNetPeerDlg::OnButton2() 

{
	for(int loop = 0; loop < 100; loop++)
		{
		CString str;

		for(int loop2 = 0; loop2 < (rand() % 20) + 4; loop2++)
			{
			str += (char) ((rand() % ('z' - 'a') + 'a'));
			}
		m_grid.AddStr(rand() % 20,  rand() % 20, str, false);		
		}

	m_grid.AutoSize();
	m_grid.Invalidate();

}

//////////////////////////////////////////////////////////////////////////

void CNetPeerDlg::Fill() 

{
	m_grid.Clear();
	m_grid.allowpopup = true;

	// Header
	m_grid.first_bold = true;
	m_grid.AddStr(0,  0, _T("Send"));
	m_grid.AddStr(0,  1, _T("Recv"));
	//m_grid.AddStr(0,  2, _T("Show"));
	m_grid.AddStr(0,  2, _T("Machine"));
	m_grid.AddStr(0,  3, _T("User Name"));
	m_grid.AddStr(0,  4, _T("IP Address"));
	m_grid.AddStr(0,  5, _T("Last Resolved"));
	
	//CString pro(_T("Access"));

	// CResolv	
	int alen = resolver.namearr.GetSize();
	
	for(int loop = 0; loop < alen; loop++)
		{
		CNameEntry *val = (CNameEntry*)resolver.namearr.GetAt(loop);

		CTime tme((time_t)val->regtime);
		CString ttt = tme.Format(_T("%d %b %Y %H:%M:%S"));

		// Grab entries from registry
		CString tmp; 
		tmp.Format(_T("%s::%s::%s"), *val->machine, *val->username, _T("send"));
		int val1 = AfxGetApp()->GetProfileInt(pro, tmp, 1);

		tmp.Format(_T("%s::%s::%s"), *val->machine, *val->username, _T("recv"));
		int val2 = AfxGetApp()->GetProfileInt(pro, tmp, 1);

		//tmp.Format(_T("%s::%s::%s", *val->machine, *val->username, "show"));
		//int val3 = AfxGetApp()->GetProfileInt(pro, tmp, 1);

		m_grid.SetCheck(loop + 1,	0, val1);
		m_grid.SetCheck(loop + 1,	1, val2);
		//m_grid.SetCheck(loop + 1,	2, val3);
		
		m_grid.AddStr(loop + 1,		2, *val->machine);
		m_grid.AddStr(loop + 1,		3, *val->username);
		m_grid.AddStr(loop + 1,		4, *val->ip);
		m_grid.AddStr(loop + 1,		5, ttt);
		}

	m_grid.AutoSize();
	m_grid.Invalidate();
}

void CNetPeerDlg::OnOK()

{
	//P2N(_T("CNetPeerDlg::OnOK()\r\n"));

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////

BOOL CNetPeerDlg::PreTranslateMessage(MSG* pMsg) 

{
	if(pMsg->message == WM_USER)	
		{
		int col = pMsg->wParam & 0xffff;
		int row = pMsg->wParam >> 16;

		CString str, str2, str3, tmp;

		m_grid.GetStr(row, 2, str);
		m_grid.GetStr(row, 3, str2);

		switch(col)
			{
			case 0: str3 = "send"; break;
			case 1: str3 = "recv"; break;
			case 2: str3 = "show"; break;
			}
		
		//P2N(_T("msg from %d %d %d %s::%s\r\n"), row, col, pMsg->lParam, str, str2);
		
		tmp.Format(_T("%s::%s::%s"), str, str2, str3);

		//P2N(_T("reg='%s' = %d\r\n"), tmp, pMsg->lParam);
		AfxGetApp()->WriteProfileInt(pro, tmp, pMsg->lParam);

		return true;
		}

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////

void CNetPeerDlg::OnDestroy() 

{
	CDialog::OnDestroy();
	
	helpcontext = oldhelp;	
}

BOOL CNetPeerDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
	//return CGrayDlg::OnEraseBkgnd(pDC);	
}

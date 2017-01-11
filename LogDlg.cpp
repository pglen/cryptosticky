
/* =====[ LogDlg.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the LogDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "LogDlg.h"
#include "mxpad.h"
#include "datalog.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif


/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog


CLogDlg::CLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLogDlg::IDD, pParent)
{
	m_BackColor = RGB(192, 192, 192);

	//{{AFX_DATA_INIT(CLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogDlg)
	DDX_Control(pDX, IDOK4, m_ok4);
	DDX_Control(pDX, IDOK, m_ok);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialog)
	//{{AFX_MSG_MAP(CLogDlg)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK4, OnOk4)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDlg message handlers

BOOL CLogDlg::OnInitDialog() 

{
	CDialog::OnInitDialog();

	SetTimer(1, 100, NULL);

	return TRUE;  
}

//////////////////////////////////////////////////////////////////////////

void CLogDlg::OnTimer(UINT nIDEvent) 

{
	KillTimer(nIDEvent);

	//P2N(_T("CLogDlg::OnTimer\r\n"));
	//Sleep(1000);

	TCHAR *buff;	
	CFile dlog;	CString str(appdata);

	if(!dlog.Open(str + "activity.txt", CFile::modeRead | CFile::shareDenyNone))
		{
		//P2N(_T("Cannot open 'datalog' log file.\r\n"));
		AfxMessageBox(_T("Cannot open 'datalog' log file.\r\n"));
		return;
		}

	// We do not expect large files .. cast OK
	int  len = (int)dlog.GetLength();
	int  len2 = min( 3 * SHRT_MAX, len);		// Show last 100k
	
	//P2N(_T("CLogDlg::OnTimer dlog len %I64u\r\n"), len);

	// Plus terminating null
	buff = (TCHAR *)malloc(len2 + sizeof(TCHAR));
	if(!buff)
		{
		AfxMessageBox(_T("Not enough memory to show log file or log file is too big."));
		dlog.Close();
		return;
		}

	// Make sure it is terminated
	buff[0] = 0;

	// Get the end portion
	if(len != len2)
		{
		// Make sure seel is on even boundary
			int len3 = len - len2; if(len3 % 2) { len3++;};

		P2N(_T("Would seek: len=%d len2=%d len3=%d\r\n"), len, len2, len3);

		dlog.Seek(len3, SEEK_SET);

		// Seek to next newline
		TCHAR chh;
		while(true)
			{
			int rrr = dlog.Read(&chh, sizeof(TCHAR));
			if(rrr == 0)
				break;

			if(chh == '\n')
				break;
			len2--;
			}
		}
	
	int len4 = dlog.Read(buff, len2);

	//P2N(_T("Read %d bytes\r\n"), len3);

	// Terminate buffer
	if(len4 > 0)
		buff[BYTE2TXT(len4)] = 0;

	dlog.Close();
	
	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT1);	//ASSERT(ed);
	ed->SetWindowText(buff);
	free(buff);	
	ed->SetSel(ed->GetWindowTextLength(), ed->GetWindowTextLength());

	CDialog::OnTimer(nIDEvent);
}

void CLogDlg::OnOk4() 

{
	int ret = AfxMessageBox(_T("Are you sure you want to delete log file?"), MB_YESNO);

	if(ret != IDYES)
		return;

	CString str(appdata);
	
	//P2N(_T("CLogDlg Deleting log file '%s'\r\n", str + "activity.txt"));

	datalog.delete_datalog();
	datalog.PrintToLog(_T("Restarted log after logfile delete\r\n"));

	SetTimer(1, 100, NULL);
}

BOOL CLogDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

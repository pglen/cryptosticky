// crstartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "crstart.h"
#include "crstartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include "../../common/work/mxpad.h"
#include "../../common/work/support.h"

// CcrstartDlg dialog


CcrstartDlg::CcrstartDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CcrstartDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcrstartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CcrstartDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CcrstartDlg message handlers

BOOL CcrstartDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	P2N(L"CcrstartDlg::OnInitDialog\r\n");

	SetTimer(1, 100, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CcrstartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CcrstartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//////////////////////////////////////////////////////////////////////////

void CcrstartDlg::OnTimer(UINT_PTR nIDEvent)

{
	P2N(L"CcrstartDlg::OnTimer %d\r\n", nIDEvent);

	KillTimer(1);
	  
	
	//support.GetEnvStr(L"ProgramFiles", progdir);
	//support.GetEnvStr(L"TEMP", progdir);
	
	CString progdir;

	TCHAR *env; size_t esize = _MAX_PATH;
	_tdupenv_s(&env, &esize, L"ProgramFiles");

	if(env)
		{
		progdir = env; free(env);
		}

	P2N(L"CcrstartDlg::OnTimer exe progdir '%s'\r\n", env);
	
	//support.GetSpecialFolder(CSIDL_COMMON_PROGRAMS, progdir);
	
	CString cryptoexe(progdir);
	cryptoexe  += L"\\CryptoSticky\\CryptoSticky.exe";
	
	P2N(L"CcrstartDlg::OnTimer exe '%s'\r\n", cryptoexe);
	
	//Sleep(1000);

	HINSTANCE  ret = ShellExecute(NULL, L"open", cryptoexe, 
							NULL, NULL, 1);

	//P2N(L"CcrstartDlg::OnTimer %d\r\n", ret);

	CDialog::OnTimer(nIDEvent);

	//Sleep(1000);

	PostMessage(WM_QUIT);
}

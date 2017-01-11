
/* =====[ serialsDlg.cpp ]========================================================== 
                                                                             
   Description:     The serials project, implementation of the serialsDlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/31/2011  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"

#include <io.h>
#include <direct.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "serials.h"
#include "serialsDlg.h"
#include "logdlg.h"
#include "../../../common/work/bluepoint.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLogDlg		logdlg;


//#define MBOX AfxMessageBox

#define NUM_DIGITS	5
#define NUM_GROUPS	4

#define COUNTSIZE 24

// Fetch routines from main program

#include "..\CLicense.h"
#include "..\CLicense.cpp"

CString fname("license.txt");


void	CSerialsDlg::Str2Clip(const TCHAR *str)

{
	CString all = str;

	// Determine if we have an object sitting on the clipboard
	if (!OpenClipboard())
		{
		AfxMessageBox(L"Cannot open Clipboard.");
		return;
		}

	int len = all.GetLength() + 1;

    // Build a copy of the text itself
    HANDLE  txthandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, len * sizeof(TCHAR));
	TCHAR *txtptr = (TCHAR *)GlobalLock(txthandle);
    ASSERT(txtptr);
    _tcscpy(txtptr, all);
    ::GlobalUnlock(txthandle);

	EmptyClipboard();
	//SetClipboardData(CF_TEXT, txthandle);
	SetClipboardData(CF_UNICODETEXT, txthandle);
	CloseClipboard();	
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialsDlg dialog

CSerialsDlg::CSerialsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialsDlg)
	m_name = _T("");
	m_cust = _T("");
	m_serial = _T("");
	saved = true;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialsDlg)
	DDX_Text(pDX, IDC_EDIT1, m_name);
	DDX_Text(pDX, IDC_EDIT2, m_cust);
	DDX_Text(pDX, IDC_EDIT3, m_serial);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT4, m_checktext);
}

BEGIN_MESSAGE_MAP(CSerialsDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON9, &CSerialsDlg::OnBnClickedButton9)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialsDlg message handlers

BOOL CSerialsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	int		result;
	struct _stat buf;

#if 0
	struct _finddata_t c_file;
    long hFile;

    /* Find first .c file in current directory */
    if( (hFile = _findfirst( "license.txt", &c_file )) == -1L )
		MBOX( "No lic. file in current directory!\n" );

	//MBOX((char*)&c_file);
#endif

	result = _tstat(fname, &buf );
	if( result < 0 )
		{
		//MBOX( "Problem getting file information" );
		//return(0);
		}

	//m_stat.Format("inode = %d", buf.st_ino);
	UpdateData(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CSerialsDlg::OnDestroy()
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialsDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSerialsDlg::OnOK() 
{
	if(!saved)
		{
		if(AfxMessageBox(L"Unsaved Key. Arey you sure you want to exit?", MB_YESNO) != IDYES)
			return;
		}
	//AfxMessageBox("hello");
	CDialog::OnOK();
}


void CSerialsDlg::OnButton1() 

{
	int sum = 0, sum2;
	int loop, loop2, loop3;

	UpdateData();

	m_checktext.SetWindowText(L"");

	if(m_name == L"")
		{
		AfxMessageBox(L"Please enter a Customer Name");
		return;
		}

	sum += SumStr(m_name); 
	//sum += SumStr(m_cust);
	srand(clock());

	// Loop until found
	for(loop2 = 0; loop2 < 100000; loop2++)
		{
		m_serial = "";

		// Produce base
	
		for(loop3 = 0; loop3 < NUM_GROUPS; loop3++)
			{
			for(loop = 0; loop < NUM_DIGITS; loop++)
				m_serial += (char)(rand() % ('Z'-'A') + 'A');

			if(loop3 < NUM_GROUPS - 1)
				m_serial += "-";
			}

		sum2 = sum + SumStr(m_serial);

		// Terminal condition
		if(!(sum2 % LIC_MODULUS))
			{
			//CString str; str.Format("loop iterations %d sum=%d", loop2, sum2);
			//MBOX(str);
			break;
			}
		}

	//AfxMessageBox(m_serial);
	saved = false;

	UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////


void CSerialsDlg::OnButton2() 

{
	UpdateData();

	if(m_serial == L"")
		{
		AfxMessageBox(L"Please generate a key first");
		return;
		}

	int sum = 0;
	sum += SumStr(m_name); //sum += SumStr(m_cust);
	int sum2 = sum + SumStr(m_serial);

	// Terminal condition
	if(!(sum2 % LIC_MODULUS))
	{
		//AfxMessageBox(L"Key Is Valid");
		m_checktext.SetWindowTextW( L"Key Is Valid");
	}
	else
		{
		//AfxMessageBox(L"Key is Invalid");
		m_checktext.SetWindowTextW(L"Key Is Invalid");
		}			
	UpdateData(false);
}

//////////////////////////////////////////////////////////////////////////

void CSerialsDlg::OnCancel() 

{
	//CDialog::OnCancel();
}

void CSerialsDlg::OnOk2() 

{

}

void CSerialsDlg::OnButton3() 

{
	UpdateData();

	if(m_name == "")
		{
		//AfxMessageBox("Please enter a Customer Name");
		//return;
		}

	if(m_serial == L"")
		{
		AfxMessageBox(L"Please generate a key first");
		return;
		}

	if(saved)
		{
		AfxMessageBox(L"This license was saved already");
		return;
		}

	CString all;

	all+= m_name; 
	//all+= m_cust; 
	all += m_serial;

	if(!CheckLic(all, NULL))
		{
		AfxMessageBox(L"Not a valid license, cannot save");
		return;
		}

	time_t osBinaryTime;            // C run-time time (defined in <time.h>)
    time( &osBinaryTime ) ;         // Get the current time from the OS
    CTime time3(osBinaryTime );
	CString dstr = time3.Format( "%a %c");

	all = L"";

	all += L"\r\n------- Begin License ------------\r\n\r\n";

	all += L"Issued: ....... ";	all += dstr;		all += "\r\n";
	all += L"Name: ......... ";	all += m_name;		all += "\r\n";
	all += L"Company: ...... ";	all += m_cust;		all += "\r\n";
	all += L"License: ...... ";	all += m_serial;	all += "\r\n";

	all += L"\r\n------- End License ------------\r\n\r\n";

	Str2Clip(all);

	//CFileException fileException;
	CFile fp;

	if (!fp.Open(fname, CFile::modeReadWrite | CFile::modeNoTruncate  |
						CFile::modeCreate, NULL))
		{
		AfxMessageBox(L"Cannot open/create the license file.");
		return;
		}

	fp.SeekToEnd();

	fp.Write(all, all.GetLength() * sizeof(TCHAR));
	fp.Close();

	CFile fp2;

	if (!fp2.Open(L"counter.txt", 	CFile::modeReadWrite | CFile::modeNoTruncate  |
						CFile::modeCreate, NULL))
		{
		AfxMessageBox(L"Cannot open/create the license counter file.");
		return;
		}

	char	buff[COUNTSIZE + 1];

	fp2.Read(buff, COUNTSIZE);
	int	cnt = atoi(buff); cnt++;
	fp2.SeekToBegin();
	CString buf2; buf2.Format(L"%d", cnt);
	fp2.Write(buf2, buf2.GetLength());
	fp2.Close();

	// -------------------------------------------------------------------

	CFile fp3;

	if (!fp3.Open(L"license.csv", CFile::modeReadWrite | CFile::modeNoTruncate  |
						CFile::modeCreate, NULL))
		{
			AfxMessageBox(L"Cannot open/create the license CSV file.");
		return;
		}
	

#if 0
    dstr.Format("%s %02d-%02d-%02d %02d:%02d:%02d",
            (int)time3.GetMonth(),
                    (int)time3.GetDay(),
                        (int)time3.GetYear() - 2000,	
							(int)time3.GetHour(), 
								(int)time3.GetMinute(), 
									(int)time3.GetSecond());
#endif

	fp3.SeekToEnd();
	
	fp3.Write(dstr, dstr.GetLength() * sizeof(TCHAR));
	fp3.Write(L",", sizeof(TCHAR));

	fp3.Write(m_name, m_name.GetLength() * sizeof(TCHAR));
	fp3.Write(L",", sizeof(TCHAR));

	fp3.Write(m_cust, m_cust.GetLength() * sizeof(TCHAR));
	fp3.Write(L",", sizeof(TCHAR));

	fp3.Write(m_serial, m_serial.GetLength() * sizeof(TCHAR));
	fp3.Write(L"\r\n", 2 * sizeof(TCHAR));

	fp3.Close();

	// Also save an Encrypted file

	CFile fp4;

	if (fp4.Open(L"license.dat", CFile::modeReadWrite | CFile::modeNoTruncate  |
						CFile::modeCreate, NULL))
		{
		CString all2(all);

		TCHAR *buff = all2.GetBuffer(all2.GetLength());
		CString pass = L"hello";
		bluepoint.encrypt(buff, all2.GetLength(), pass, pass.GetLength());
		
		all2.ReleaseBuffer();

		P2N(L"data len %d\r\n", all2.GetLength() * sizeof(TCHAR));

		CString tmp; tmp.Format(L"BUFFER %4d ", all2.GetLength() * sizeof(TCHAR));
		all2 = tmp + all2 + L"ENDBUFF";
		D2N(all2, all2.GetLength() * sizeof(TCHAR));

		fp4.SeekToEnd();
		fp4.Write(all2, all2.GetLength() * sizeof(TCHAR));
		fp4.Close();
		}
	else
		{

		P2N(L"Cannot open/create the license file.");
		return;
		}

	saved = true;
}


void CSerialsDlg::OnButton4() 

{
	UpdateData();
	Str2Clip(m_name);	
}

//////////////////////////////////////////////////////////////////////////


void CSerialsDlg::OnButton5() 
{
	UpdateData();
	Str2Clip(m_cust);	
}


void CSerialsDlg::OnButton6() 
{
	UpdateData();
	Str2Clip(m_serial);	
}

void CSerialsDlg::OnButton7() 
{
	//	
	UpdateData();

	CString all = "";

	all +="\r\n------- Begin License ------------\r\n\r\n";

	all += "Name: ......... ";	all += m_name;		all += "\r\n";
	all += "Company: ...... ";	all += m_cust;		all += "\r\n";
	all += "License: ...... ";	all += m_serial;	all += "\r\n";

	all +="\r\n------- End License ------------\r\n\r\n";

	Str2Clip(all);
}

//////////////////////////////////////////////////////////////////////////

void CSerialsDlg::OnButton8() 

{
	TCHAR *buff;
	CFile dlog;

	CString str; //(appdata);

	if(!dlog.Open(str + L"license.txt", CFile::modeRead | CFile::shareDenyNone))
		{
		//P2N("Cannot open 'datalog' log file.\r\n");
		AfxMessageBox(L"Cannot open 'datalog' log file.\r\n");
		return;
		}

	int flen = (int) dlog.GetLength();

	buff = (TCHAR *) malloc(flen + sizeof(TCHAR));
	if(!buff)
		{
		AfxMessageBox(L"Not enough memory to show log file or log file is too big.");
		dlog.Close();
		return;
		}

	int len = (int)dlog.Read(buff, flen);
	buff[len / sizeof(TCHAR)] = 0;
	dlog.Close();
	
	//P2N(L"Log buffer %s\r\n", buff);

	CFile fp2;
	if (fp2.Open(L"counter.txt", 	CFile::modeRead, NULL))
		{
		char buff[COUNTSIZE + 1];

		fp2.Read(buff, COUNTSIZE);
		logdlg.cnt = atoi(buff);
		}

	logdlg.text = buff;
	logdlg.DoModal();

	free(buff);
	
}

void CSerialsDlg::OnBnClickedButton9()
{
	P2N(L"Show secret log\r\n");

	CFile fp4;

	if (fp4.Open(L"license.dat", CFile::modeRead, NULL))
		{
		while(true)
			{
			int headlen = 12;
			CString head; TCHAR *buff = head.GetBuffer(headlen);
			int readin = (int)fp4.Read(buff, headlen * sizeof(TCHAR));
			head.ReleaseBuffer(headlen);

			if(readin == 0)
				break;
		
			P2N(L"got head '%s'\r\n", head);
		
			if(_tcsnccmp(buff, L"BUFFER ", 7) == 0)
				{
				int cnt = _tstoi(buff + 7);
				P2N(L"Got BUFF str cnt=%d\r\n", cnt);				

				CString body; 
				TCHAR *buffb = body.GetBuffer(cnt / sizeof(TCHAR));
		
				/// Eat it
				int readin = (int)fp4.Read(buffb, cnt);

				CString pass = L"hello";
				bluepoint.decrypt(buffb, cnt / sizeof(TCHAR) , pass, pass.GetLength());
		
				body.ReleaseBuffer(cnt / sizeof(TCHAR));

				if(readin == 0)
					break;
			
				P2N(L"'%s'\r\n", body);

				CString foot; TCHAR *bufff = foot.GetBuffer(7);
				int readinf = (int)fp4.Read(bufff, 7 * sizeof(TCHAR));
				foot.ReleaseBuffer(7);

				if(readin == 0)
					break;
			
				P2N(L"got foot '%s'\r\n", foot);
				if(foot != "ENDBUFF")
					{
						P2N(L"Corrupt licence file");
					}
				}
			}

		//TCHAR *buff = all2.GetBuffer(all2.GetLength());
		//bluepoint.encrypt(buff, all2.GetLength(), pass, pass.GetLength());
		
	
		//CString tmp; tmp.Format(L"BUFFER %4d ", all2.GetLength() * sizeof(TCHAR));
		//all2 = tmp + all2 + L"ENDBUFF";
		//D2N(all2, all2.GetLength() * sizeof(TCHAR));

		//fp4.SeekToEnd();
		//fp4.Write(all2, all2.GetLength() * sizeof(TCHAR));
		fp4.Close();
		}
	else
		{
		P2N(L"Cannot open the license file.");
		return;
		}

	saved = true;

}

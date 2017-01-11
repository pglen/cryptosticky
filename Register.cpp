
/* =====[ Register.cpp ]========================================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Register.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  1/31/2011  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "CryptoSticky.h"
#include "Register.h"
#include "support.h"

#include "CLicense.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegister dialog

CRegister::CRegister(CWnd* pParent /*=NULL*/)
	: CDialog(CRegister::IDD, pParent)
{
	m_BackColor = RGB(192, 192, 192);
	
	//{{AFX_DATA_INIT(CRegister)
	m_cust = _T("");
	m_name = _T("");
	m_key2 = _T("");
	retry = 0;
	m_key2 = _T("");
	//}}AFX_DATA_INIT
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegister)
	DDX_Control(pDX, ID_KEY, m_keyx);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_cancel);
	DDX_Text(pDX, ID_NAME, m_name);
	DDX_Text(pDX, ID_KEY, m_key2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_FILL, m_fill);
	DDX_Control(pDX, IDC_REGEXPLAIN, m_explain);
	DDX_Control(pDX, IDC_REGNAME, m_regname);
	DDX_Control(pDX, IDC_REGKEY, m_regkey);
}

BEGIN_MESSAGE_MAP(CRegister, CDialog)
	//{{AFX_MSG_MAP(CRegister)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	//ON_STN_CLICKED(IDC_REGEXPLAIN, &CRegister::OnStnClickedRegexplain)
	//ON_BN_CLICKED(IDOK, &CRegister::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegister message handlers

BOOL CRegister::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//BYTE	bAlpha = 235;
	BYTE	bAlpha = 255;

	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		if(g_pSetLayeredWindowAttributes)
			g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

	m_name = AfxGetApp()->GetProfileString(_T("License"), _T("Name"), _T("DEMO"));
	m_cust = AfxGetApp()->GetProfileString(_T("License"), _T("Company"));

	// CEdit
	CString tmp = AfxGetApp()->GetProfileString(_T("License"), _T("Key"));

	CString all; int check;
	all += m_name;	
	//all += m_cust;	// no name dependency
	all += m_key2;
	int	res = CheckLic(all, &check);

	//P2N(_T("len %d \r\n"), tmp.GetLength());
	
#if 0
	// Obfuscate old Licensed
	if(tmp.GetLength() >= 23)
		{
		tmp.SetAt(6,'*');
		tmp.SetAt(7,'*');
		tmp.SetAt(8,'*');
		tmp.SetAt(9,'*');
		tmp.SetAt(10,'*');
		
		tmp.SetAt(12,'*');
		tmp.SetAt(13,'*');
		tmp.SetAt(14,'*');
		tmp.SetAt(15,'*');
		tmp.SetAt(16,'*');		
		}
#endif

	m_key2 = tmp;

	//if(res)
	//	m_keyx.SetPasswordChar('*');

	UpdateData(false);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////

void CRegister::OnOK() 

{
	// Limit the tries 
	retry++;
	
	int check;

	UpdateData();

	// Registered already

	CString key2 = AfxGetApp()->GetProfileString(_T("License"), _T("Key"));
	CString name2 = AfxGetApp()->GetProfileString(_T("License"), _T("Name"));
	CString allc = name2 + key2;

	int	res2 = CheckLic(allc, &check);
	if(res2)
		{
		int ret = AfxMessageBox(
			_T("This copy of CryptoSticky is registered already. Register new?"), MB_YESNO );

		if(ret !=IDYES)
			{
			CDialog::OnOK();
			return;
			}
		}
	
	if(retry > 5)
		{
		AfxMessageBox(_T("Too many tries."));
		CDialog::OnOK();
		return;
		}
	
	if(m_name == "")
		{
		AfxMessageBox(_T("Please enter a name."));
		GetDlgItem(ID_NAME)->SetFocus();
		return;
		}

	if(m_name == "DEMO")
		{
		AfxMessageBox(_T("'DEMO' cannot be used as a registered name. Please enter name."));
		GetDlgItem(ID_NAME)->SetFocus();
		return;
		}
	
	if(m_key2 == "")
		{
		AfxMessageBox(_T("Please enter a license key."));
		return;
		}

	CString all; all += m_name;	
	//all += m_cust;	
	all += m_key2;
	int	res = CheckLic(all, &check);
	if(!res || check % LIC_MODULUS != 0)
		{
		AfxMessageBox(_T("Invalid Registration Key entered. Please try again."));
		GetDlgItem(ID_KEY)->SetFocus();		
		AfxGetApp()->WriteProfileString(_T("License"), _T("Name"), _T("DEMO"));
		
		return;
		}

	//daysused = 0;		
	CString tmp; 
	tmp.Format(_T("CryptoSticky Pro - Registered to %s"), m_name);
	
	AfxGetApp()->m_pMainWnd->SetWindowText(tmp);
	AfxMessageBox(_T("License Key Accepted."));
	
	// Save to registry
	CString  str3 = "License";	
	AfxGetApp()->WriteProfileString(_T("License"), _T("Name"), m_name);
	AfxGetApp()->WriteProfileString(_T("License"), _T("Key"), m_key2);		

	P2N(_T("Licensed user '%s' key '%s' \r\n"), m_name, m_key2);
	CDialog::OnOK();
}


BOOL CRegister::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
}

void CRegister::OnStnClickedRegexplain()
{
	// TODO: Add your control notification handler code here
}

void CRegister::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

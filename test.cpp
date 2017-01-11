// test.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#endif

#include "mxpad.h"
/////////////////////////////////////////////////////////////////////////////
// Ctest dialog


Ctest::Ctest(CWnd* pParent /*=NULL*/)
	: CDialog(Ctest::IDD, pParent)
{
	//{{AFX_DATA_INIT(Ctest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

//	P2N(_T("Construct test\r\n"));
}

void Ctest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Ctest)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Ctest, CDialog)
	//{{AFX_MSG_MAP(Ctest)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Ctest message handlers

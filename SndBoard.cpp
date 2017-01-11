// SndBoard.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "SndBoard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#endif

/////////////////////////////////////////////////////////////////////////////
// CSndBoard dialog


CSndBoard::CSndBoard(CWnd* pParent /*=NULL*/)
	: CDialog(CSndBoard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSndBoard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSndBoard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSndBoard)
	DDX_Control(pDX, IDC_WAV, m_wav);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSndBoard, CDialog)
	//{{AFX_MSG_MAP(CSndBoard)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSndBoard message handlers

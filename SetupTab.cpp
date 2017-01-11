
/* =====[ SetupTab.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the SetupTab.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// SetupTab.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "SetupTab.h"
#include "mxpad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupTab dialog

CSetupTab::CSetupTab(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupTab::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupTab)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSetupTab::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupTab)
	DDX_Control(pDX, IDC_TAB1, m_tab);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetupTab, CDialog)
	//{{AFX_MSG_MAP(CSetupTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupTab message handlers

BOOL CSetupTab::OnInitDialog() 

{
	CDialog::OnInitDialog();

	RECT  rc;
	GetClientRect(&rc);

	POINT	point;

	point.x = 0; point.y = 40;
	this->ClientToScreen(&point);
	
	setup.Create(IDD_DIALOG8, this);
	setup.SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	setup.ShowWindow(SW_SHOW);

	//CTabCtrl
	TCITEM TabCtrlItem;
	
	TabCtrlItem.pszText = _T("Looks & Colors");
	TabCtrlItem.lParam = (long) &setup;
	TabCtrlItem.mask = TCIF_PARAM | TCIF_TEXT ;

	m_tab.InsertItem(0, &TabCtrlItem );

	TabCtrlItem.pszText = _T("Alarms");
	//TabCtrlItem.lParam = (long)&m_edit1;
	TabCtrlItem.mask = TCIF_PARAM | TCIF_TEXT ;
	m_tab.InsertItem(1, &TabCtrlItem );

	m_tab.SetCurSel(0);
	
	//setup.Create(_T("IDD_DIALOG8"));
	//setup.ShowWindow(SW_HIDE);

	TabCtrlItem.pszText = _T("Misc");
	//TabCtrlItem.lParam = (long)&m_edit3;
	TabCtrlItem.mask = TCIF_PARAM | TCIF_TEXT ;
	m_tab.InsertItem(2, &TabCtrlItem );

	m_tab.SetCurSel(0);


	//m_edit3.ShowWindow(SW_HIDE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetupTab::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int sel = m_tab.GetCurSel();

	TC_ITEM tc;

	tc.mask = TCIF_PARAM;
	m_tab.GetItem(sel, &tc);
			
	//P2N(_T("Change tab %x\r\n"), tc.lParam);

	CEdit *m_edit = (CEdit	*) tc.lParam;

	//m_edit->ShowWindow(SW_SHOW);
	
	*pResult = 0;
}

// SearchDLG.cpp : implementation file
//

#include <afxmt.h>

#include "stdafx.h"
#include "CryptoSticky.h"

#include "Yellowdlg.h"
#include "SearchDLG.h"

// SearchDLG dialog

IMPLEMENT_DYNAMIC(SearchDLG, CDialog)

SearchDLG::SearchDLG(CWnd* pParent /*=NULL*/)
	: CDialog(SearchDLG::IDD, pParent)
{
	m_BackColor = RGB(192,192,192);
	stop = 0; in_src = 0;

}

SearchDLG::~SearchDLG()
{
}

void SearchDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDC_EDIT1, m_search);
	DDX_Control(pDX, IDC_LIST2, m_list);
	DDX_Control(pDX, IDOK4, m_exit);
	DDX_Control(pDX, IDC_CHECK1, m_body);
	DDX_Control(pDX, IDOK3, m_hide);
	DDX_Control(pDX, IDC_EDIT2, m_prog);
}


BEGIN_MESSAGE_MAP(SearchDLG, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &SearchDLG::OnBnClickedOk)
	ON_BN_CLICKED(IDOK4, &SearchDLG::OnBnClickedOk4)
	ON_LBN_SELCHANGE(IDC_LIST2, &SearchDLG::OnLbnSelchangeList2)
	ON_BN_CLICKED(IDC_CHECK1, &SearchDLG::OnBnClickedCheck1)
	ON_BN_CLICKED(IDOK3, &SearchDLG::OnBnClickedOk3)
END_MESSAGE_MAP()


// SearchDLG message handlers

int SearchDLG::Search(void)
{
	return 0;
}

BOOL SearchDLG::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH SearchDLG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
HBRUSH hbr2;

	//P2N(_T("CSetup::OnCtlColor nCtlColor=%x dlgback=%x\r\n"), nCtlColor, dlgback);

	if(nCtlColor == CTLCOLOR_STATIC)
		{
		hbr2 = CreateSolidBrush(dlgback);
		pDC->SetBkColor(dlgback);
		}
	else if(nCtlColor == CTLCOLOR_EDIT)
		{
		//hbr2 = CreateSolidBrush(dlgback);
		hbr2 = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}
	else if(nCtlColor == CTLCOLOR_LISTBOX)
		{
		hbr2 = CreateSolidBrush(dlgback);
		pDC->SetBkColor(dlgback);

		}
	else if(nCtlColor == CTLCOLOR_DLG)
		{
		hbr2 = CreateSolidBrush(dlgback);
		pDC->SetBkColor(dlgback);
		}
	else if(nCtlColor == CTLCOLOR_BTN)
		{
		hbr2 = CreateSolidBrush(dlgback);
		pDC->SetBkColor(dlgback);
		}
	else
		{		
		hbr2 = CreateSolidBrush(dlgback);
		}
	// TODO:  Change any attributes of the DC here
	
	return hbr2;
	//return hbr;
}

//////////////////////////////////////////////////////////////////////////

void SearchDLG::Iterate(HTREEITEM currentItem, const TCHAR *strx)

{	
	YellowDlg *cc = (YellowDlg *) AfxGetApp()->m_pMainWnd;
	
	CTreeCtrl *pm_tree = cc->GetTreeCtrl();
	if(currentItem == 0)
		currentItem = TVI_ROOT;

	HTREEITEM last = 0;
	HTREEITEM hChildItem = pm_tree->GetChildItem(currentItem);

	while(hChildItem)
		{
		int addflag = 0;

		//stop = funct(this, hChildItem, parm);
		//P2N(_T("iterating %x\r\n"), hChildItem);

		if(stop)
			break;

		// Reflect checked state
		CString str2;   cc->BuildTree(str2, hChildItem);
		CString fname;  cc->GetNodePath(fname, hChildItem);
		CString title = cc->GetTreeCtrl()->GetItemText(hChildItem);
		CString title2(title); title2.MakeUpper();
		//CString strall; strall.Format(_T("%s %s"), str2, title);
		CString strall; strall.Format(_T("%s"), str2);
	
		if(title2.Find(strx) >= 0)
			{
			// Read in body, if needed
			addflag = true;	
			}

		if(m_body.GetCheck())
			{		
			//P2N(_T("Checked\r\n"));
			MemoSupp	memo;

			// Read YellowDlg file
			int ret = memo.Read(fname);
			if(!ret)
				{
				CString tmp; 
				tmp.Format(_T("Cannot read Sticky '%s'"), fname);

				AfxMessageBox(tmp);
				stop = true;
				return;
				}
			else
				{
				memo.body.MakeUpper();

				//P2N(_T("Body %s '%s'\r\n"), title, memo.body.Left(32));

				if(memo.body.Find(strx) >= 0)
					{
					addflag = true;
					}
				}
			}

		// Got one
		if(addflag)
			{
			int idx = m_list.AddString(strall);
			if(idx >= 0)
				m_list.SetItemData(idx, (int)hChildItem);
			found++;
			}
		
		Iterate(hChildItem, strx);

		hChildItem = pm_tree->GetNextItem(hChildItem, TVGN_NEXT);
		}
}


//////////////////////////////////////////////////////////////////////////

void SearchDLG::OnBnClickedOk()

{
	CString str; m_search.GetWindowText(str);
	str.MakeUpper();

	if(str == "")
		{
		AfxMessageBox(_T("Please enter a string to search for"));
		return;
		}

	P2N(_T("Searching for %s\r\n"), str);

	if(in_src)
		{
		stop = true;
		return;
		}

	stop = false;
	in_src = true;

	m_list.ResetContent(); found = 0;

	//m_list.AddString(_T("Searching ..."));
	Iterate(NULL, str);
	in_src = false;
	if (!found)
	{
		m_list.AddString(_T("No match found ..."));
	}
}

//////////////////////////////////////////////////////////////////////////

void SearchDLG::OnBnClickedOk4()
{
	OnOK();
}

void SearchDLG::OnLbnSelchangeList2()
{
	int sel = m_list.GetCurSel();
	if(sel < 0) return;

	CString str; m_list.GetText(sel, str);
	CString str2; str2.Format(_T("Showing '%s'"), str);
	m_prog.SetWindowText(str2);

	HTREEITEM hChildItem = (HTREEITEM)m_list.GetItemData(sel);
			
	//P2N(_T("Clicked on to %s %d\r\n"), str, hChildItem);

	YellowDlg *cc = (YellowDlg *) AfxGetApp()->m_pMainWnd;
	
	if(hChildItem)
		{
		cc->ShowOne(hChildItem);

		// Grab focus back
		SetFocus(); m_list.SetFocus();
		}
}

void SearchDLG::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}

BOOL SearchDLG::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_search.SetFocus();
	// TODO:  Add extra initialization here

	//return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SearchDLG::OnBnClickedOk3()
{

	YellowDlg *cc = (YellowDlg *) AfxGetApp()->m_pMainWnd;
	cc->HideAll();
	m_prog.SetWindowText(_T(""));
}

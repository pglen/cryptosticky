// Mytree.cpp : implementation file
//

#include "stdafx.h"
#include "CryptoSticky.h"
#include "yellowdlg.h"
#include "Mytree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMytree

CMytree::CMytree()
{
}

CMytree::~CMytree()
{
}

BEGIN_MESSAGE_MAP(CMytree, CTreeCtrl)
	//{{AFX_MSG_MAP(CMytree)
	ON_WM_KEYDOWN()
	ON_WM_SYSKEYDOWN()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMytree message handlers

void CMytree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N("CMytree::OnKeyDown(UINT nChar=%d UINT nRepCnt, UINT nFlags)\r\n", nChar); 
	
	//if(nChar == 46)
	//	{
	//	YellowDlg *dlg = (YellowDlg*)AfxGetApp()->m_pMainWnd;
	//	
	//	dlg->DelMemo();
	//	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

//////////////////////////////////////////////////////////////////////////

void CMytree::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//if(nChar == VK_F10)
	//	{
	//	P2N("F10 on CMytree\r\n");
	//	}

	CTreeCtrl::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CMytree::OnCommand(WPARAM wParam, LPARAM lParam) 

{
	//P2N("CMytree::OnCommand wParam=%d  lParam=%d\r\n", wParam, lParam);

	return CTreeCtrl::OnCommand(wParam, lParam);
}

BOOL CMytree::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{	
	//P2N("CMytree::OnCmdMsg nID=%d  nCode=%d\r\n", nID, nCode);

	return CTreeCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMytree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 

{
	//P2N("CMytree::OnNotify wParam=%d  lParam=%d\r\n", wParam, lParam);
	return CTreeCtrl::OnNotify(wParam, lParam, pResult);
}

//////////////////////////////////////////////////////////////////////////

void CMytree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	P2N("CMytree::OnSelchanged old=%d new=%d action=%d\r\n", 
						pNMTreeView->itemOld, pNMTreeView->itemNew,
							pNMTreeView->action);

	*pResult = 0;

	//CTreeCtrl::OnSelchanged(pNMHDR, pResult);
}


BOOL CMytree::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 

{
	//P2N("CMytree::OnChildNotify message=%s wParam=%d\r\n", mxpad.num2msg(message), wParam);
	return CTreeCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

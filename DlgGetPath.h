#if !defined(AFX_DLGGETPATH_H__0D8256E4_05E4_11D3_9FA0_00C0F019557F__INCLUDED_)
#define AFX_DLGGETPATH_H__0D8256E4_05E4_11D3_9FA0_00C0F019557F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgGetPath.h : header file
//

#include "Xbutton.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgGetPath dialog
class CDlgGetPath : public CDialog
{
// Construction
public:
	CDlgGetPath(CWnd* pParent = NULL);   // standard constructor

	CString header;

// Dialog Data
	//{{AFX_DATA(CDlgGetPath)
	enum { IDD = IDD_GET_PATH };
	CXButton	m_ok;
	CXButton	m_bc;
	CXButton	m_b1;
	//CMytree		m_tree;
	CTreeCtrl		m_tree;
	//}}AFX_DATA

	//Quick and Nasty one liners.
	CString GetPath() const					{ return m_sPath; }
	void	SetPath( CString sPath )		{ m_sPath = sPath; }

	int		m_BackColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGetPath)
	public:
	//virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	void	SelectPath();
	TCHAR*	MakeObjectDynamic( LPTSTR szData );
	CString WindowName( CString sName );
	bool	ParsePath( CString sPath, int nLevel, CString *const psDirName );
	void	ShortenPath(CString &str, int len);

	// Generated message map functions
	//{{AFX_MSG(CDlgGetPath)
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton1();
	afx_msg void OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightmkdirMakedirectory();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnRdblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

//Operators
private:
	CString GetItemPath( HTREEITEM hItem );
	bool	PopulateTree( CString sPath, HTREEITEM hParent );
	HTREEITEM InsertItem( HTREEITEM hParent, NETRESOURCE *const pNetResource, CString sText, int iImage, int iImageSelected = -1 );
	bool	EnumNetwork( HTREEITEM hParent );

	void MkDir();

//Attributes
private:
	CImageList	m_ImageListTree;					//Item in the Tree image list				
	HTREEITEM	m_hNetworkRoot;						//Network neighbourhood root
	CString		m_sPath;							//Highlighted path
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGGETPATH_H__0D8256E4_05E4_11D3_9FA0_00C0F019557F__INCLUDED_)

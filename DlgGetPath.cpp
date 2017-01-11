
/* =====[ DlgGetPath.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the DlgGetPath.cpp                
					Display a dialog to allow the user to select a file path.
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  9/21/2007  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>

#include "Resource.h"								//Definitions for resource ID's
#include "DlgGetPath.h"								//Definition to get path
#include "mxpad.h"							
#include "support.h"							

#include "GetDirName.h"

// Disable security warnings TODO for later
#pragma warning (disable : 4996)

#ifdef _DEBUG
#undef THIS_FILE
//static TCHAR THIS_FILE[] = _T(__FILE__);
static const char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define NETWORK_NEIGHBOUR   _T("Network Neighborhood")


/////////////////////////////////////////////////////////////////////////////
// CDlgGetPath dialog
CDlgGetPath::CDlgGetPath(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGetPath::IDD, pParent)
{
	//P2N( _T("CDlgGetPath::CDlgGetPath(%p)\r\n"), pParent );
	//{{AFX_DATA_INIT(CDlgGetPath)
	//}}AFX_DATA_INIT
	m_hNetworkRoot = NULL;

	// Assume current dir
	CString dir = support.GetCurrentDir();
	//P2N(_T("Setting current dir %s\r\n"), dir);
	SetPath(dir);

	m_BackColor = RGB(192, 192, 192);
}

void CDlgGetPath::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGetPath)
	DDX_Control(pDX, IDOK, m_ok);
	DDX_Control(pDX, IDCANCEL, m_bc);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_TREE, m_tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgGetPath, CDialog)
	//{{AFX_MSG_MAP(CDlgGetPath)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE, OnItemexpandingTree)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRclickTree)
	ON_COMMAND(ID_RIGHTMKDIR_MAKEDIRECTORY, OnRightmkdirMakedirectory)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RDBLCLK, IDC_TREE, OnRdblclkTree)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE, OnSelchangingTree)
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
//
//	sPath		Full path to parse.
//	nLevel		Level to return 0 = the root or drive name.
//	psDirName	returned directory name
//
//	return	true	if a directory or drive name was returned.
//			false	if no more directorys

bool CDlgGetPath::ParsePath( CString sPath, int nLevel, CString *const psDirName )
{
	//ASSERT( psDirName );

	int nStart = 0;	

	while( nLevel )
		{
		nStart = sPath.Find( _T('\\'), nStart );
		
		if( nStart < 0 )
			return false;

		nLevel--;
		nStart++;
		}

	// We now have the start point to find the end.
	int nEnd = sPath.Find( _T('\\'), nStart );
	
	if( nEnd < 0 )
		*psDirName = sPath.Mid( nStart );
	else
		*psDirName = sPath.Mid( nStart, nEnd-nStart);

	return true;
}

/////////////////////////////////////////////////////////////////////////////
//FUNCTION
//	Setup inital Dialog Data.
//CREATED
//	8-5-1999  19:57:09
//RETURNS
//	See help

BOOL CDlgGetPath::OnInitDialog() 
{
	//P2N( _T("CDlgGetPath::OnInitDialog()\r\n") );
	CDialog::OnInitDialog();

	if(header != "")
		SetWindowText(header);

	//////////////////////////////////////////////////////////////////////////
	// Setup the Image lists

	if( !m_ImageListTree.Create( IDB_FILE_TYPES, 17, 1, RGB( 255, 255, 255 ) ) )
		{
		//P2N( _T("Image list creation fault") );
		}
	
	m_tree.SetImageList( &m_ImageListTree, TVSIL_NORMAL );

	//////////////////////////////////////////////////////////////////////////
	// List the local drives

	for( TCHAR cLetter = _T('A'); cLetter <= _T('Z'); cLetter++ )
		{
		CString sDrive = cLetter;
		sDrive += _T(":");						
		UINT nType = GetDriveType( sDrive + _T("\\") );

		if( DRIVE_REMOVABLE <= nType && nType <= DRIVE_RAMDISK )
			InsertItem( TVI_ROOT, NULL, sDrive, nType );
		}

	//////////////////////////////////////////////////////////////////////////
	//Network neighbour hood

	m_hNetworkRoot = InsertItem( TVI_ROOT, NULL, NETWORK_NEIGHBOUR, DRIVE_RAMDISK+1 );

	SelectPath();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

///////////////////////////////////////////////////////////////////////////////
//DESCRIPTION:
//	Insert an Item in to the List
//CREATED:
//	10-5-1999  13:49:20
//PARAMS:
//	hParent		Parent to Attach to.
//	sText		Text to Add.
//	iImage		Image.
//RETURN:
//	Handle of Item created

HTREEITEM CDlgGetPath::InsertItem( HTREEITEM hParent, NETRESOURCE *const pNetResource, CString sText, int iImage, int iImageSelected /* = -1 */ )

{
	//P2N( _T("CDlgGetPath::InsertItem(%p,%p,%s +++ )\r\n"), hParent, pNetResource, sText );

	sText = WindowName( sText );
	TVINSERTSTRUCT InsertStruct;
	InsertStruct.hParent		= hParent;	
	InsertStruct.hInsertAfter	= TVI_LAST;
	InsertStruct.itemex.mask	= TVIF_IMAGE | TVIF_TEXT | TVIF_CHILDREN | 
								  TVIF_SELECTEDIMAGE | TVIF_PARAM;
	InsertStruct.itemex.pszText	= sText.GetBuffer( sText.GetLength() );  sText.ReleaseBuffer();
	InsertStruct.itemex.iImage	= iImage;
	InsertStruct.itemex.cChildren= 1;
	InsertStruct.itemex.lParam	= (LPARAM)pNetResource;
	if( iImageSelected == -1 )
		InsertStruct.itemex.iSelectedImage = iImage;
	else
		InsertStruct.itemex.iSelectedImage = iImageSelected;

	return m_tree.InsertItem( &InsertStruct );
}

/////////////////////////////////////////////////////////////////////////////
//	Called in request to expand an item in the tree.
//	Return true if some children were added

bool CDlgGetPath::PopulateTree( CString sPath, HTREEITEM hParent ) 

{
	//P2N( _T("CDlgGetPath::PopulateTree( %s )\r\n"), sPath );

	bool bGotChildren = false;						//True if a child is added.	

	//Populate Network neighbourhood tree (Entire network and Local-Computers)
	//Network item(Search deep into the network)
	
	if( hParent == m_hNetworkRoot )
		{
		bGotChildren = EnumNetwork( hParent );
		}
	else if( m_tree.GetItemData( hParent ) )
		{
		bGotChildren = EnumNetwork( hParent );
		}
	else
		{
		//Search for files and populate the CTreeCtrl
	
		CFileFind finder;
		BOOL bWorking = finder.FindFile( sPath+_T("*.*") );
		while (bWorking)   
			{
			bWorking = finder.FindNextFile();
			if( finder.IsDots() )
				continue;
			if( finder.IsDirectory() )
				{
				InsertItem( hParent, NULL, finder.GetFileName(), DRIVE_NO_ROOT_DIR, DRIVE_UNKNOWN );
				bGotChildren = true;
				}
			}
		}

	//Remove the [+] if no children
	if( !bGotChildren )
		{
		// Bad idea, when creating new directories

		//TVITEM item		= { 0 };
		//item.mask		= TVIF_HANDLE | TVIF_CHILDREN;
		//item.hItem		= hParent;
		//item.cChildren	= 0;
		//m_tree.SetItem( &item );
		}

	return bGotChildren;
}

/////////////////////////////////////////////////////////////////////////////
//FUNCTION
//	Get the Part from a given selection point.
//CREATED
//	8-5-1999  22:29:00
//PARAMS
//	hItem	Item to Get Path for.
//RETURNS
//	Full path as a string.

CString CDlgGetPath::GetItemPath( HTREEITEM hItem) 

{
	//P2N( _T("CDlgGetPath::GetItemPath(%p)\r\n"), hItem );

	CString sRet;
	do
		{
		// End with a share name.
		NETRESOURCE *const pNetResource = (NETRESOURCE *)(m_tree.GetItemData( hItem ) );
		if( pNetResource )
			{
			sRet = CString(pNetResource->lpRemoteName) + _T('\\')+ sRet; 		
			break;
			}

		//Add the directory name to the path.
		sRet = m_tree.GetItemText( hItem ) + _T('\\')+ sRet; 
		hItem = m_tree.GetParentItem( hItem );

		} while( hItem );

	//P2N( _T("CDlgGetPath::GetItemPath str '%s'\r\n"), sRet );

	return sRet;
}

/////////////////////////////////////////////////////////////////////////////
//FUNCTION
//	Node on the tree is about to expand
//	Called when some user tries to expand the tree.
//CREATED
//	8-5-1999  19:56:55
//PARAMS
//	See help.

void CDlgGetPath::OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult) 

{
	//P2N( _T("CDlgGetPath::OnItemexpandingTree(%p)\r\n"), pNMHDR );

	CWaitCursor CursorWaiting;						//Show the wait cursor while expanding
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	//ASSERT( pNMTreeView );
	//ASSERT( pResult );

	//Only action 2 notifications
	if( pNMTreeView->action == 2 )
	{
		//Update location display
		CString sPath = GetItemPath( pNMTreeView->itemNew.hItem );
		//Refresh children
		if( !m_tree.GetChildItem( pNMTreeView->itemNew.hItem ) )
		{
			PopulateTree( sPath, pNMTreeView->itemNew.hItem );
			if( m_tree.GetSelectedItem( ) != pNMTreeView->itemNew.hItem )
				m_tree.SelectItem( pNMTreeView->itemNew.hItem );
		}
	}


	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
//FUNCTION
//	Update the Edit dox with the new selection path
//CREATED
//	9-5-1999  12:54:48
//PARAMS
//	Seel help	

/////////////////////////////////////////////////////////////////////////////
//  void	ShortenPath(CString &str, int len)
// Shorten path to len

void	CDlgGetPath::ShortenPath(CString &str, int len)

{
	int slen = str.GetLength();

	// Nothing to do
	if(slen <= len)
		return;

	CString lstr = str.Left ((len-5)/2);
	CString rstr = str.Right((len-5)/2);
	str = lstr + " ... " + rstr;
}

void CDlgGetPath::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 

{
	//P2N( _T("CDlgGetPath::OnSelchangedTree(%p)\r\n"), pNMHDR );

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	//ASSERT( pNMTreeView );
	//ASSERT( pResult );
	CString sPath;									//Currently selected path or empty if not valid

	// Disable search on Workstation roots
	if( m_tree.GetItemData( pNMTreeView->itemNew.hItem ) || 
			pNMTreeView->itemNew.hItem == m_hNetworkRoot )
		{
		GetDlgItem( IDOK )->EnableWindow( false );
		}
	else
		{
		GetDlgItem( IDOK )->EnableWindow( true );
		sPath = GetItemPath( pNMTreeView->itemNew.hItem );
		}
	*pResult = 0;
	
	//m_sPath = sPath;  // bad idea

	CString tmp(sPath);	ShortenPath(tmp, 70);

	//ASSERT( GetDlgItem( IDC_STATIC_NOTE ) );
	SetDlgItemText( IDC_STATIC_NOTE, tmp );
}

					
/////////////////////////////////////////////////////////////////////////////
//FUNCTION
//	Created a duplicate of the given string. (There must be a function for this)
//CREATED
//	23-5-1999  15:55:53
//PARAMS
//	String to be duplicated
//RETURNS
//	Pointer to the duplicated string

TCHAR* CDlgGetPath::MakeObjectDynamic( LPTSTR szData )
{
	//P2N( _T("MakeObjectDynamic( %s )\r\n"), szData );

	//Assume a NULL = empty string
	TCHAR * szRet = NULL;
	int nLength	  = 0;
	if( szData )
		//nLength = strlen( szData )+ 1;
		nLength = _tcslen( szData )+ sizeof(TCHAR);

	if( nLength > 0 )
	{
		szRet = new TCHAR[nLength];
		//ASSERT( szRet );
		_tcscpy_s( szRet, nLength, szData );
		//strcpy( szRet, szData );
	}
	return szRet;
}


/////////////////////////////////////////////////////////////////////////////
//FUNCTION
//	To display the same was window explorer
//	..names all uppercase letters are converted
//	..to inital caps.
//CREATED
//	23-5-1999  16:10:34
//PARAMS
//	sName		sName to convert.
//RETURNS
//	Converted name

CString CDlgGetPath::WindowName( CString sName )
{
	CString sRet;
	//Scan all charactors to determine if their are any lower case items
	for( int n = 0; n < sName.GetLength(); n++ )
	{
		TCHAR ch = sName[n];
		if ((ch >= 'a') && (ch <= 'z') )
			return sName;
	}
	sName.MakeLower();
	if( sName.GetLength() > 0 )
	{
		CString sFirstChar = sName[0];
		sFirstChar.MakeUpper();
		sName = sFirstChar + sName.Mid( 1 );
	}
	return sName;
}

///////////////////////////////////////////////////////////////////////////////
//DESCRIPTION:
//	Enumerate the given network resource. This is where the magic happens.
//CREATED:
//	10-5-1999  15:19:02
//PARAMS:
//	hParent	Item to create the child items from
//			..(item.lParam) == NULL if finding the NETWORK root.
//RETURN:
//	ture if one or more items were found

bool CDlgGetPath::EnumNetwork( HTREEITEM hParent )
{
	//P2N( _T("CDlgGetPath::EnumNetwork( %p )\r\n"), hParent );

	bool bGotChildren = false;						//True if a child is added.	

	//Check if the item already has a network resource and use it.
	NETRESOURCE *const pNetResource = (NETRESOURCE *)(m_tree.GetItemData( hParent ) );
	//
	//Setup
	//
	DWORD dwResult;      
	HANDLE hEnum;      
	DWORD cbBuffer = 16384;
	DWORD cEntries = 0xFFFFFFFF;      
	LPNETRESOURCE lpnrDrv;      
	DWORD i;
	dwResult = WNetOpenEnum(	pNetResource ?  RESOURCE_GLOBALNET  : RESOURCE_CONTEXT,
								RESOURCETYPE_ANY,//RESOURCETYPE_DISK,
								0,                               
								pNetResource ? 	pNetResource		: NULL,
								&hEnum );      
	//Was the read sucessfull
	if (dwResult != NO_ERROR)      
	{
		//P2N( _T("*** ERROR %d - Cannot enumerate network drives.\r\n"), dwResult );
		return false;
	} 
	
	//
	//Get items until no more remain.
	//
	do      
	{
		lpnrDrv = (LPNETRESOURCE) GlobalAlloc( GPTR, cbBuffer );
		dwResult = WNetEnumResource( hEnum, &cEntries, lpnrDrv, &cbBuffer   );
		if (dwResult == NO_ERROR)         
		{
			//Scann through the results
			for( i = 0; i < cEntries; i++ )            
			{
				CString sNameRemote = lpnrDrv[i].lpRemoteName;
				int nType = 9;
				if( sNameRemote.IsEmpty() )
				{
					sNameRemote = lpnrDrv[i].lpComment;
					nType = 8;
				}
				//
				//Remove leading back slashes 
				//
				if( sNameRemote.GetLength() > 0 && sNameRemote[0] == _T('\\') )
					sNameRemote = sNameRemote.Mid( 1 );
				if( sNameRemote.GetLength() > 0 && sNameRemote[0] == _T('\\') )
					sNameRemote = sNameRemote.Mid( 1 );

				//
				//Display a share or the appropiate icon
				//
				if( lpnrDrv[i].dwDisplayType == RESOURCEDISPLAYTYPE_SHARE )
				{
					//Display only the share name
					int nPos = sNameRemote.Find( _T('\\') );
					if( nPos >= 0 )
						sNameRemote = sNameRemote.Mid( nPos+1 );
					InsertItem( hParent, NULL, sNameRemote, DRIVE_NO_ROOT_DIR, DRIVE_UNKNOWN );
				}
				else
				{
					NETRESOURCE* pResource = new NETRESOURCE;
					//ASSERT( pResource );
					*pResource = lpnrDrv[i];
					pResource->lpLocalName	= MakeObjectDynamic( pResource->lpLocalName );
					pResource->lpRemoteName = MakeObjectDynamic( pResource->lpRemoteName );
					pResource->lpComment	= MakeObjectDynamic( pResource->lpComment );
					pResource->lpProvider	= MakeObjectDynamic( pResource->lpProvider );
					InsertItem( hParent, pResource, sNameRemote, pResource->dwDisplayType+7 );
				}
				bGotChildren = true;
			}
		}         
		GlobalFree( (HGLOBAL) lpnrDrv );      
		if( dwResult != ERROR_NO_MORE_ITEMS )
		{            
			//P2N( _T("*** ERROR %d - Cannot complete network drive enumeration\r\n"), dwResult );
			break;         
		}
	}
	while( dwResult != ERROR_NO_MORE_ITEMS );      

	//
	//Let go and go home
	//
	WNetCloseEnum(hEnum);   
	return bGotChildren;
}

//////////////////////////////////////////////////////////////////////////

void CDlgGetPath::OnButton1() 

{
	MkDir();
}

//////////////////////////////////////////////////////////////////////////
// Expand the Tree to the Inital Path.

void CDlgGetPath::SelectPath()

{
	int nLevel = 0;	CString sDirName;
	HTREEITEM hCurrent = TVI_ROOT;					//Current item in the list being expanded.

	while( ParsePath( m_sPath, nLevel, &sDirName ) )
		{
		// Network or Drive expand
		if( !nLevel && sDirName.IsEmpty() )
				sDirName = NETWORK_NEIGHBOUR;
		
		//P2N(_T("CDlgGetPath::SelectPath %s\r\n"), sDirName); 

		// Search for the matching tree item and expand
		HTREEITEM hItem = m_tree.GetChildItem( hCurrent );	
		while( hItem )
			{
			if( sDirName.CompareNoCase( m_tree.GetItemText( hItem ) ) == 0 )
				{
				hCurrent = hItem;
				m_tree.Expand( hCurrent, TVE_EXPAND );
				
				break;
				}
			hItem = m_tree.GetNextSiblingItem( hItem );
			}
		nLevel++;
	}
}

void CDlgGetPath::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 

{
	//P2N(_T("CDlgGetPath::OnRclickTree\r\n"));

    CMenu   mm;

	CPoint mouse, hittest; GetCursorPos(&mouse);
	int menu = IDR_MENU3;

	mm.LoadMenu(menu);
	CMenu *pp = mm.GetSubMenu( 0 );
	pp->TrackPopupMenu( TPM_LEFTALIGN, mouse.x, mouse.y, this);
    	
	*pResult = 0;
}

void CDlgGetPath::OnRightmkdirMakedirectory() 

{
	MkDir();
}

void CDlgGetPath::OnOK() 

{
	HTREEITEM h1 = m_tree.GetSelectedItem();
	if(h1)
		m_sPath = GetItemPath(h1);
	
	CDialog::OnOK();
}

void CDlgGetPath::MkDir()

{
	CGetDirName gd; gd.m_BackColor = m_BackColor;

	SelectPath(); gd.txt = GetPath();

	if(gd.DoModal() == IDCANCEL)
		return;

	if(gd.m_dir == "")
		return;

	CString newdir;
	newdir = GetPath(); 

	newdir += "\\";
	newdir += gd.m_dir;

	if(_taccess(newdir, 0) >= 0)
		{
		CString tmp; tmp.Format(_T("This directory already exists ('%s')"), newdir);
		AfxMessageBox(tmp);
		return;
		}
	else
		{
		int ret = _tmkdir(newdir);
		if(ret < 0)
			{
			TCHAR *buff = _tcserror(errno);
			CString tmp; tmp.Format(
				_T("Error on creating new dir: '%s' Error description: %s"),
						newdir, buff);
			AfxMessageBox(tmp);
			return;
			}
		else
			{
			HTREEITEM hParent;

			hParent = m_tree.GetSelectedItem();

			//P2N(_T("Seting new path %s\r\r\n"), newdir);

			if(hParent)
				{
				//SetPath(newdir); SelectPath();
				PopulateTree(newdir,  hParent);
				}
			}
		}
}

void CDlgGetPath::OnDestroy() 

{
	//P2N( _T("CDlgGetPath::OnDestroy()\r\n") );

	//Get the current selection before heading home
	m_sPath = GetItemPath( m_tree.GetSelectedItem( ) );

	//Start at the root deleting.
	HTREEITEM hItemCur = m_tree.GetRootItem( );		//Current item under inspection

	// While their is still data in the tree

	while( hItemCur )
	{
		HTREEITEM hItem = m_tree.GetChildItem( hItemCur );
		// Has children then make the child current
		if( hItem )
			{
			hItemCur = hItem;
			continue;
			}

		// Item has no children so we shall destroy it.
		// but first we must decide who is to take its place.

		HTREEITEM hNextSibItem = m_tree.GetNextSiblingItem( hItemCur );
		HTREEITEM hPrevSibItem = m_tree.GetPrevSiblingItem( hItemCur );
		HTREEITEM hParentItem  = m_tree.GetParentItem( hItemCur );
		
		// Get item data to check if lparam is to be destroyed
		NETRESOURCE *const pNetResource = (NETRESOURCE *)m_tree.GetItemData( hItemCur );//(item.lParam);
		if( pNetResource )
			{
			delete [] (pNetResource->lpLocalName);
			delete [] (pNetResource->lpRemoteName);
			delete [] (pNetResource->lpComment);
			delete [] (pNetResource->lpProvider);
			delete pNetResource;
			}

		m_tree.DeleteItem( hItemCur );
		hItemCur = NULL;

		// Determine which item is next to recieve the next focus
		if( hParentItem )
			hItemCur = hParentItem;
		if( hPrevSibItem )
			hItemCur = hPrevSibItem;
		if( hNextSibItem )
			hItemCur = hNextSibItem;
		}	
	
	CDialog::OnDestroy();	
}

void CDlgGetPath::OnRdblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 

{
	//P2N(_T("CDlgGetPath::OnRdblclkTree\r\n"));
	*pResult = 0;
}

void CDlgGetPath::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{

	OnOK();
	*pResult = 0;
}

void CDlgGetPath::OnSelchangingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//P2N(_T("CDlgGetPath::OnSelchangingTree\r\n"));

	*pResult = 0;
}

BOOL CDlgGetPath::OnEraseBkgnd(CDC* pDC)
{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
//	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgGetPath::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	HBRUSH hbr2;

	//P2N(_T("CSetup::OnCtlColor nCtlColor=%x dlgback=%x\r\n"), nCtlColor, dlgback);

	if(nCtlColor == CTLCOLOR_STATIC)
		{
		hbr2 = CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);
		}
	else if(nCtlColor == CTLCOLOR_EDIT)
		{
		//hbr2 = CreateSolidBrush(dlgback);
		hbr2 = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}
	else if(nCtlColor == CTLCOLOR_LISTBOX)
		{
		hbr2 = CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);

		}
	else if(nCtlColor == CTLCOLOR_DLG)
		{
		hbr2 = CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);
		}
	else if(nCtlColor == CTLCOLOR_BTN)
		{
		hbr2 = CreateSolidBrush(m_BackColor);
		pDC->SetBkColor(m_BackColor);
		}
	else
		{		
		hbr2 = CreateSolidBrush(m_BackColor);
		}
	// TODO:  Change any attributes of the DC here
	
	return hbr2;
	
	//return hbr;
}

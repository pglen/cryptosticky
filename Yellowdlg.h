
/* =====[ Yellowdlg.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Yellowdlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_YellowDlg_H__D2211722_06A9_11D3_B398_525400D994D7__INCLUDED_)
#define AFX_YellowDlg_H__D2211722_06A9_11D3_B398_525400D994D7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//////////////////////////////////////////////////////////////////////////
// YellowDlg.h : header file
//

#include <afxmt.h>

#include "fakewin.h"
#include "support.h"
#include "xButton.h"
#include "xEdit.h"
#include "CryptoSticky.h"
#include "ymemo.h"
#include "Speak.h"
#include "ChangeMem.h"
#include "XrayM.h"
#include "EditDlg.h"

#define ID_OPERATIONS_COPY              32960
#define ID_OPERATIONS_PASTE             32961
#define ID_OPERATIONS_SHOW              32962
#define ID_OPERATIONS_EDIT              32963
#define ID_OPERATIONS_DELETE            32964
#define ID_OPERATIONS_NEW               32965
#define ID_OPERATIONS_NEWNODE           32966

typedef void (*PFVC)(const TCHAR *);

#define MAX_SIMUL_ALARMS	 10

class MemoDetail

{
public:

	MemoDetail() { fname = new CString; checked = 0; }
	~MemoDetail() { delete fname; }

	HTREEITEM item;
	CString *fname;
	int checked;
};

/////////////////////////////////////////////////////////////////////////////
// YellowDlg dialog

class YellowDlg : public CDialog

{
	typedef  int (*YDCBRI)(YellowDlg *ptr, HTREEITEM hh, int parm);

	static	int	IteratorExport(YellowDlg *ptr, HTREEITEM hh, int parm);
	static	int	IteratorDup(YellowDlg *ptr, HTREEITEM hh, int parm);
	static	int	IteratorShow(YellowDlg *ptr, HTREEITEM hh, int parm);
	static  int	Iterator(YellowDlg *ptr, HTREEITEM hh, int parm);
	static  int	IteratorSel(YellowDlg *ptr, HTREEITEM hh, int parm);
	static	int	IteratorBcast(YellowDlg *ptr, HTREEITEM hh, int parm);
	static	int	IteratorGather(YellowDlg *ptr, HTREEITEM hh, int parm);

	static	void _MemoThreadProc(void *ptp);	
	static	void _CastThreadProc(void *ptp);
	static	void _mCastThreadProc(void *ptp);
	static	void _EditThreadProc(void *ptp);
	static	void _SendThreadProc(void *ptp);

public:

	HTREEITEM	Memo2Handle(CString &str);

	//CTreeCtrl *GetTreeCtrl() { return &m_tree; }

	int		m_BackColor;

	//void	RemoveAlarms();
	void	RemoveAlarm(const TCHAR *fname);

	void	WakeMachine();
	void	Decrypt_Clipboard(int ver = 3);
	void	Encrypt_Clipboard(int ver = 3);

	CChangeMem	chmem;
	HANDLE  event;
	
	void	SendFiles(const TCHAR *str, const TCHAR *nameto);
	void	SendFile(const TCHAR *str, const TCHAR *nameto);

	void	MenuAddNetList(CXrayM *rr);
	void	MenuAddFolderList(CXrayM *mm);

	void	xMoveMemo(HTREEITEM hfrom, HTREEITEM hto, int ignore = false);

	void	CopyDep(CString &from, CString &to);
	void	DelDep(CString &from);

	void	CleanYlist();

	void	DelMemobyHandle(HTREEITEM h1);
	void	ShowStatus(CString &str, int tout = 2000, int color = 0);
	void	ShowStatus(const TCHAR *str, int tout = 2000, int color = 0);

	int		ExportMemos(const TCHAR *str);
	CString	ImportMemo(const TCHAR *dir);
	void	Iterate_Exec(HTREEITEM currentItem, int parm, YDCBRI funct);

	CYmemo*	CreateMemoWin(HTREEITEM h1);
	
	void	Close();
	void	HideMe();

	void	DelMemo();
	void	Iterate_Uncheck(HTREEITEM currentItem, int hide = true);
	void	Decorate();

	YellowDlg(CWnd* pParent = NULL);   // standard constructor
	
	CEditDlg *ped;

	CStringList remlst;	
	CStringList chgfrom, chgto;	
		
	BOOL	m_hide,  m_auto;
	int		flag12;
	int		shutdown;
	CPtrList ylist, alist, detlist;

	CTreeCtrl	*GetTreeCtrl() {return &m_tree;}

	void	SendOne(const TCHAR *str, CNameEntry *entry);

	HTREEITEM GetMemoItem(const TCHAR *name) ;

	void ShowMissed();
	void HideMemos();
	void ShowMemos();
	void CreateBranch(int doleaf, const TCHAR *body = NULL);

	void FnameToSel(const TCHAR * fname, HTREEITEM *item);
	void ShowOne(HTREEITEM h1, int show = true, int ala = false,  int tmptop = false);
	void ArmNext();
	void ReadAllMemos(int erase,  int show, int tmptop, int force = false);
	void EditCurrent(int ro = false);
	void ShowAll();
	void HideAll();
	void Iterate_Checks(HTREEITEM currentItem );
	void OnShowMain();
	int	 IsDuplicate(const TCHAR *str);

	CYmemo *Handle2Memo(HTREEITEM h1);

	void	WrIterate_Checks(HTREEITEM currentItem );
	
	CString sendto;

	int		gatheroffsetx, gatheroffsety, gatherstep;

	DWORD	maxlist;

	void BuildTree(CString &str, HTREEITEM  h1  = NULL) ;
	//void GetNodeFilename(CString &str1, HTREEITEM  h1 = NULL);
	void GetNodePath(CString &str1, HTREEITEM  h1 = NULL);


protected:
	
	//int		cryptver;
	int		cboardtype;
	void	StopScreenSaver();

	SYSTEM_POWER_STATUS inisps;

	int		stat_color, stat_tout;
	CString	stat_str;

#ifdef SPEAK
	CSpeak	sp;
#endif
	
	int		memo_port, found;
	DWORD	lastsound;

	void	BCastFile(const TCHAR *str);

	DWORD		LastFolderTick;
	
	CTime sys;	
	
	int		al_done, num_alarms, load_complete;

	time_t	alarm_tt;

	
	HTREEITEM g_h1, dup;

	CString showthis;

	CPoint mouse;	
	HICON m_hIcon;
	FakeWin  *fakepar;

	MouseMsgHandlerPtr rBtn, lBtn, lsBtn; 

	// Worker functions for encryption

	int		Encrypt(CString &str);
	int		Decrypt(CString &str);

	//int xGuessCrypt(CString &str);
	//int	xEncrypt(CString &str, CString &pass);

	//int xDecrypt1(CString &str, CString &pass);
	//int xDecrypt2(CString &str, CString &pass);
	//int xDecrypt3(CString &str, CString &pass);

private:

	int		stable, reenter, prevwin, prevx, firstshow;
	
	CString alname[MAX_SIMUL_ALARMS];
	CString old_tmp2, old_tmp3;
	CString section;

	int	thhandle, thhid, lastactivated;

protected:

	void Iterate_Sel(HTREEITEM currentItem, int sel);
	int	 WriteStartup(int flag);
	void DelMemoFile(CString &fname, CString &memotree);

	void Iterate_fname(const TCHAR *fname, HTREEITEM currentItem, HTREEITEM *item);
	void Iterate_Show(HTREEITEM currentItem, int show = true);
	
	void ShowCode();
	LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam) ;
	void OnCancel() ;
	
	void	YellowDlg::xAddOneFolder(CXrayM *rr, HTREEITEM h0, HTREEITEM h1, int cntt);

	

protected:

// Dialog Data
	//{{AFX_DATA(YellowDlg)
	enum { IDD = IDD_DIALOG9 };
	CClicker	m_tell;
	CXButton	m_ok3;
	CLabel		m_status;
	CLabel		m_treelab;
	CXButton	m_ok2;
	CXButton	m_b35;
	CXButton	m_b40;
	CXButton	m_b34;
	CXButton	m_b1;
	CXButton	m_b17;
	CXButton	m_b5;
	CXButton	m_b41;
	CXButton	m_b42;
	CLabel		m_tstr2;
	CXButton	m_11;
	CXButton	m_10;
	CLabel		m_tstr;
	CXButton	m_9;
	CXButton	m_8;
	CXButton	m_7;
	CXButton	m_6;
	CXButton	m_4;
	CXButton	m_3;
	CXButton	m_2;
	CXButton	m_ok;
	//CMytree	m_tree;
	CTreeCtrl	m_tree;
	CxEdit		m_prog;
	CxEdit		m_check10;
	CxEdit		m_check1;
	CxEdit		m_check2;
	CxEdit		m_check3;
	CxEdit		m_check4;
	CxEdit		m_check5;
	CxEdit		m_check17;
	CxEdit		m_check18;
	
	BOOL	m_restart;
	BOOL	m_listen;
	BOOL	m_ignore;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(YellowDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(YellowDlg)
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton8();
	afx_msg void OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnOperationsCopy();
	afx_msg void OnOperationsEdit();
	afx_msg void OnOperationsPaste();
	afx_msg void OnOperationsShow();
	afx_msg void OnOperationsDelete();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnOperationsNew();
	afx_msg void OnOperationsNewnode();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	afx_msg void OnTaskbarmenuAbout();
	afx_msg void OnTaskbarmenuTogglesticky();
	afx_msg void OnButton9();
	afx_msg void OnTaskbarmenuHideselectedstickies();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnTaskBarCreated(WPARAM wp, LPARAM lp);
	afx_msg void OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpandedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButton10();
	afx_msg void OnButton11();
	afx_msg void OnRightShow();
	afx_msg void OnRightSearch();
	afx_msg void OnRightHide();
	afx_msg void OnRightCreatenewmeno();
	afx_msg void OnRightBroadcast();
	afx_msg void OnRightDelete();
	afx_msg void OnRight2Selectall();
	afx_msg void OnRight2BroadcastAll();	
	afx_msg void OnRightSelectall();
	afx_msg void OnRight2Hideallselectedmemos();
	afx_msg void OnRightEditmemo();
	afx_msg void OnRightSelectmemo();
	afx_msg void OnButton17();
	afx_msg void OnCheck3();
	afx_msg void OnRight2Refreshall();
	afx_msg void OnRight2Createnewmemo();
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	virtual void OnOK();
	afx_msg void OnRightRebuildmemotree();
	afx_msg void OnRightGather();
	afx_msg void OnCheck4();
	afx_msg void OnRight2Deselectall();
	afx_msg void OnRightClearselectonsubitems();
	afx_msg void OnCheck2();
	afx_msg void OnCheck1();
	afx_msg void OnButton34();
	afx_msg void OnOk2();
	afx_msg void OnButton35();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnKeydownTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheck10();
	afx_msg void OnCheck5();
	afx_msg void OnOk3();
	afx_msg void OnTell();
	afx_msg void OnTstr2();
	afx_msg void OnTstr();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnStnClickedTstr2();
	afx_msg void OnRight2Searchforsticky();
public:
	afx_msg void OnBnClickedButton40();
public:
	//afx_msg void OnPaint();
	afx_msg void OnBnClickedButton41();
	afx_msg void OnBnClickedButton42();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_YellowDlg_H__D2211722_06A9_11D3_B398_525400D994D7__INCLUDED_)

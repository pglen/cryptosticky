
/* =====[ EditDlg.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the EditDlg.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/3/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_EDITDLG_H__DAA2C590_81D4_4443_B53B_2C6F692D9866__INCLUDED_)
#define AFX_EDITDLG_H__DAA2C590_81D4_4443_B53B_2C6F692D9866__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// EditDlg.h : header file
//

#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "xbutton.h"
#include "PGedit.h"
#include "ymemo.h"
#include "waveedit.h"
#include "PlaySound.h"
#include "RecordSound.h"
#include "Speak.h"
#include "label.h"
#include "xedit.h"

#define CURR_MEMO_VER 3

/////////////////////////////////////////////////////////////////////////////
// CEditDlg dialog

class CEditDlg : public CDialog

{
	static void	CEditDlg::StaticSound(void *org, char *buff, int len);

public:
	
	CEditDlg(CWnd* pParent = NULL);   // standard constructor

	int		m_BackColor;
	int		Sound(char *buff, int len);
	
	CString memoroot, memorootp;
	CString caption, title, file, str;

	int		fresh, ro;

	CYmemo	*st;

	CRecordSound	recsnd;
	CPlaySound		playsound;
	HTREEITEM	newhandle;

protected:

#ifdef SPEAK
	CSpeak	sp;
#endif

	CString oldbutt;

	void	OnCancel();
	void	Prop_ampm();
	void	SetAla(CTime ct);
	void	UpdateAla();
	void	OnOK(void);
	void	LoadMemo();
	void	Decorate();

	CString m_sound, section12, btext, btext2;

	MemoSupp	memo;

	struct _stat old_sbuff;

	//CPGedit pged;
	
public:

// Dialog Data
	//{{AFX_DATA(CEditDlg)
	enum { IDD = IDD_DIALOG1 };
	CXButton	m_b19;
	CXButton	m_b35;
	CXButton	m_b34;
	CXButton	m_b17;
	CXButton	m_b9;
	CXButton	m_b5;
	CXButton	m_b8;
	CwaveEdit	m_wav;
	CPGedit		m_body;
	CXButton	m_b7;
	CXButton	m_b14;
	CXButton	m_b13;
	CXButton	m_b12;
	CXButton	m_b11;
	CXButton	m_b4;
	CxEdit		m_folderc;
	CPGedit		m_chead;
	CXButton	m_3;
	CXButton	m_2;
	CXButton	m_1;
	CXButton	m_can;
	CXButton	m_ok;
	CSpinButtonCtrl	m_spin2;
	CSpinButtonCtrl	m_spin1;
	BOOL	m_center;
	CString	m_hh;
	CString	m_mm;
	CString	m_alatime;
	CString	m_aladate;
	BOOL	m_ala;
	CString	m_us;
	BOOL	m_persist;
	BOOL	m_red;
	BOOL	m_soundon;
	CString	m_head;
	BOOL	m_folder;
	BOOL	m_beep;
	CString	m_ampm;
	BOOL	m_text;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit4();
	afx_msg void OnSelectMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheck1();
	afx_msg void OnCheck3();
	afx_msg void OnCheck4();
	afx_msg void OnButton3();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnCheck8();
	afx_msg void OnCheck5();
	afx_msg void OnButton4();
	afx_msg void OnButton11();
	afx_msg void OnButton12();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnButton13();
	afx_msg void OnButton14();
	afx_msg void OnButton7();
	afx_msg void OnCheck9();
	afx_msg void OnButton5();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnDestroy();
	afx_msg void OnButton17();
	afx_msg void OnButton34();
	afx_msg void OnButton35();
	afx_msg void OnCheck13();
	afx_msg void OnButton19();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CLabel m_titletext;
	CxEdit m_redcheck;
	CxEdit m_centertext;
	CxEdit m_persisttext;
	CxEdit m_autotext;
	CxEdit m_calframe;
	CxEdit m_alaframe;
	CxEdit m_alaframe2;
	CxEdit m_hourt;
	CxEdit m_mint;
	CxEdit m_ampmt;
	CxEdit m_alaht;
	CxEdit m_alamint;
	CxEdit m_alarmampmt;
	CxEdit m_check3t;
	CxEdit check4t;
	CxEdit m_check5t;
	CxEdit m_check13t;
	CxEdit m_cant;
	CxEdit m_can2t;
	CxEdit m_radio1;
	CxEdit m_radio2;
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	CXButton m_decr;
	afx_msg void OnBnClickedButton38();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITDLG_H__DAA2C590_81D4_4443_B53B_2C6F692D9866__INCLUDED_)

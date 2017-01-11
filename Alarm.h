
/* =====[ Alarm.h ]========================================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Alarm.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  11/24/2010  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_ALARM_H__D61A2E77_1C5B_4E30_A428_3F13074CC680__INCLUDED_)
#define AFX_ALARM_H__D61A2E77_1C5B_4E30_A428_3F13074CC680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Alarm.h : header file
//

#include "label.h"
#include "transdlg.h"
#include "xButton.h"
#include "Speak.h"
#include "ymemo.h"
#include "GrayDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAlarm dialog

class CAlarm : public CGrayDlg

{
	static void	CAlarm::_ThreadProc(void *ptp );

// Construction
public:

	void Show();
	void OnOK();
	CAlarm(CWnd* pParent = NULL);   // standard constructor

	int		final, alarmoffset, m_BackColor;
	CString fname;
	

	void CAlarm::Beep();

// Dialog Data
	//{{AFX_DATA(CAlarm)
	enum { IDD = IDD_DIALOG4 };
	CXButton	m_5;
	CLabel		m_alatitle;
	CXButton	m_4;
	CXButton	m_3;
	CXButton	m_2;
	CXButton	m_1;
	CLabel	m_alar;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarm)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	DWORD	createtime;
	CString title;
	RECT	rchead, closerect;

	int butcolor, hbutcolor;
	int	dlgcolor, hdlgcolor;
	int count, bigcount, snooze, delay;

#ifdef SPEAK
	CSpeak	sp;
#endif

	CYmemo* st;

	// Generated message map functions
	//{{AFX_MSG(CAlarm)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnButton3();
	afx_msg void OnButton1();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnButton2();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButton4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARM_H__D61A2E77_1C5B_4E30_A428_3F13074CC680__INCLUDED_)

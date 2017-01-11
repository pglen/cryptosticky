
/* =====[ PGedit.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the PGedit.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
// PGedit.cpp : implementation file
//


#include "stdafx.h"
#include "CryptoSticky.h"
#include "Yellowdlg.h"

#include "mxpad.h"
#include "PGedit.h"
#include "io.h"

#include "LevenStein.h"
#include "support.h"
#include "misc.h"

// Disable security warnings TODO for later
#pragma warning (disable : 4996)

#undef VERIFY
#define VERIFY(a) a

#ifdef _DEBUG
#undef THIS_FILE
//static TCHAR THIS_FILE[] = _T(__FILE__);
static const char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#define APSTUDIO_INVOKED
#undef APSTUDIO_READONLY_SYMBOLS
#include "resource.h"

#define ID_SPELL_WORD1		_APS_NEXT_COMMAND_VALUE+1
#define ID_SPELL_WORD2		ID_SPELL_WORD1+1
#define ID_SPELL_WORD3		ID_SPELL_WORD1+2
#define ID_SPELL_WORD4		ID_SPELL_WORD1+3
#define ID_SPELL_WORD5		ID_SPELL_WORD1+4
#define ID_SPELL_WORD6		ID_SPELL_WORD1+5
#define ID_SPELL_WORD7		ID_SPELL_WORD1+6
#define ID_SPELL_WORD8		ID_SPELL_WORD1+7
#define ID_SPELL_WORD9		ID_SPELL_WORD1+8
#define ID_SPELL_WORD10		ID_SPELL_WORD1+9

#define ID_SPELL_IGNORE		ID_SPELL_WORD1+11
#define ID_SPELL_ADD		ID_SPELL_WORD1+12
#define ID_SPELL_MORE		ID_SPELL_WORD1+13


#define ID_ENCRYPT			_APS_NEXT_COMMAND_VALUE + 100
#define ID_DECRYPT 			_APS_NEXT_COMMAND_VALUE + 101
#define ID_ENCRYPT2			_APS_NEXT_COMMAND_VALUE + 102
#define ID_DECRYPT2 		_APS_NEXT_COMMAND_VALUE + 103
#define	ID_EMPTYCLIP		_APS_NEXT_COMMAND_VALUE + 104


#undef APSTUDIO_INVOKED

//////////////////////////////////////////////////////////////////////////

int strdiff(const CString &str1, const CString &str2);
int strdiff2(const CString &str1, const CString &str2);
int strdist(const CString &str1, const CString &str2);

static	int comparestr(const void *elem1, const void *elem2 ) 

{
	CString *ptr1 = *((CString **)elem1);
	CString *ptr2 = *((CString **)elem2);

	//P2N(_T("Compare %s to %s\r\n"), *ptr1, *ptr2);

	return(ptr1->CompareNoCase(*ptr2));
}

/////////////////////////////////////////////////////////////////////////////
// CPGedit

// Static members of the spell box

int				CPGedit::dic_good = 0;
FILE			*CPGedit::fp1 = 0;
FILE			*CPGedit::fp3 = 0;
CDWordArray     CPGedit::widx;
		

CPGedit::CPGedit()

{
	in_spell = false;

	//fp1 = fp3 = 0;

	firsttime = 0;
	checkon = true;
	m_uiTimerID = 0;
	oldline = 0;
	cryptver = 3;
	m_Drawn = false;
}

CPGedit::~CPGedit()
{
}

BEGIN_MESSAGE_MAP(CPGedit, CEdit)
	//{{AFX_MSG_MAP(CPGedit)
	ON_WM_CHAR()
	ON_WM_PAINT()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_CONTROL_REFLECT(EN_VSCROLL, OnVscroll)
	ON_WM_HSCROLL()
	ON_WM_SHOWWINDOW()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_SPELL_WORD1, ID_SPELL_WORD10, ReplaceSpellingError)
	ON_COMMAND(ID_SPELL_ADD, AddWordToDic)
	ON_COMMAND(ID_SPELL_MORE, MoreSuggest)
	ON_COMMAND(ID_SPELL_IGNORE, IgnoreWord)
	ON_COMMAND(ID_EDIT_COPY, Copy)
	ON_COMMAND(ID_EDIT_PASTE, Paste)
	ON_COMMAND(ID_EDIT_CUT, Cut)
	ON_COMMAND(ID_EDIT_SELECT_ALL, SelAll)

	ON_COMMAND(ID_ENCRYPT, Encrypt)
	ON_COMMAND(ID_DECRYPT, Decrypt)
	ON_COMMAND(ID_ENCRYPT2, Encrypt2)
	ON_COMMAND(ID_DECRYPT2, Decrypt2)
	ON_COMMAND(ID_EMPTYCLIP, EmptyClip)
	
	
END_MESSAGE_MAP()


void CPGedit::AddWordToDic()

{
	//P2N(_T("Add Word To Dic %s\r\n"), currword);
	currword.MakeLower();

	fseek(fp3, 0, SEEK_END);
	char *ptr = NULL;
	misc.UnicodeToAnsi(currword, &ptr);
	fprintf(fp3, "%s\n", ptr);
	if (ptr) free(ptr);

	int ss,ee;
	GetSel(ss,ee);
	SetSel(ee,ee);

	m_Drawn = false;
	RedrawWindow();
	//m_dwLastTick = GetTickCount();
}


void CPGedit::IgnoreWord()

{
	//P2N(_T("Ignore Word %s\r\n"), currword);

	currword.MakeLower();
	ignore.AddTail(currword);
	
	int ss,ee;
	GetSel(ss,ee);
	SetSel(ee,ee);

	m_Drawn = false;
	RedrawWindow();

	//m_dwLastTick = GetTickCount();
}

void CPGedit::MoreSuggest()

{
	//P2N(_T("More Suggestions\r\n"));

	int offset;
	CStringList strlist;

	Distance dist;
	CString str2(currword);
	str2.MakeLower();

	// Figure out starting letter
    if(widx.GetSize() > (str2[0] - 'a'))
        offset =  widx.GetAt(str2[0] - 'a');

	//P2N(_T("Starting at buffer offset %d\r\n"), offset);
	// Look up in dictionary

	if(!fp1)
		return;

    fseek(fp1, offset, SEEK_SET);

	// Fill in selection
	while(TRUE)
		{
		if(!_fgetts(buffer, MAX_READ, fp1) )
			break;

		int len = _tcslen(buffer) - 1;
		buffer[len] = '\0';
		 
		for(int loop = 0; loop < len; loop++)
			{
			buffer[loop] = tolower(buffer[loop]);
			}

		//P2N(_T("Distance [%s] against [%s]\r\n'  "), str2, buffer);
		//if(cnt++ > 1000)
		//	break;

		// Is ascii?
		if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
			{
			if(tolower(buffer[0]) !=  str2[0])
				break;
			}

		int ddd = dist.LD(buffer, str2);

		if(ddd < 4 )
			{
			//P2N(_T(Suggested %s\r\n")), buffer);
			CString tmp; tmp.Format(_T("%d %s"), ddd, buffer);
			strlist.AddTail(tmp);
			//P2N(_T("more suggestions: %s\r\n"), tmp);
			}	
		}
}

/////////////////////////////////////////////////////////////////////////////
// CPGedit message handlers

void CPGedit::ReplaceSpellingError(UINT uid)

{
	// This is a hack, I do not know why and how
	uid -=2;

	//P2N(_T("ReplaceSpellingError iud=%d\r\n"), uid - ID_SPELL_WORD1);

	int idx = min(int(uid - ID_SPELL_WORD1), replace.GetSize());

	//P2N(_T("Clicked Range %d %s\r\n"), uid - ID_SPELL_WORD1, replace[idx]);

	// Propagate capitalisation
	if(isupper(currword[0]))
		replace[idx].SetAt(0, toupper(replace[idx][0]));

	ReplaceSel(replace[idx], true);
}

void CPGedit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N(_T("CPGedit WM_TCHAR %d\r\n"), nChar);

	//if(daysused > DEMO_DAYS && nChar != VK_TAB)
	//	{
		//Coco *cc = ((Coco *)AfxGetApp()->m_pMainWnd);
		////ASSERT(cc);
		//cc->m_prog.SetText(
		//AfxMessageBox(_T("Editing Disabed"));
		//MessageBox(_T("Editing Disabled."), _T("CryptoSticky Licensing"));
		//SetFocus();
		//return;
	//	}

	m_dwLastTick = ::GetTickCount() - 500;
	m_Drawn = false;

	if(isalnum(nChar) || nChar ==  VK_SPACE || nChar ==  8)
		{
		
		}

	switch(nChar)
		{
		case 1: // ctrl-a
			SetSel(0, -1);
			return;
			break;

		case 3: // ctrl-c
			//Copy();
			break;

		//case VK_SPACE:
		//	P2N(_T("Space in char\r\n"));
		//	m_dwLastTick = ::GetTickCount();
		//	m_Drawn = false;

		break;
		}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

//DEL void CPGedit::OnPaint() 
//DEL 
//DEL {
//DEL 	CPaintDC dc(this); // device context for painting
//DEL 	
//DEL 	// TODO: Add your message handler code here
//DEL 	
//DEL 	// Do not call CEdit::OnPaint() for painting messages
//DEL }

void CPGedit::OnPaint() 
{
	CEdit::OnPaint();

	//P2N(_T("PaintedCEdit\r\n"));

	CClientDC dc(this);
		
	CRect ClientRect;
	GetClientRect(ClientRect);

	//NOWAY CPaintDC dc(this); // device context for painting

	if(!checkon)
		return;
	
	if(m_Drawn)
		return;

	//////////////////////////////////////////////////////////////////////

	CFont* pFont = GetFont();
	CFont* pOldFont = NULL;
	CBrush* pNewBrush = NULL;
	CBrush* pOldBrush = NULL;
	CPen* pNewPen = NULL;
	CPen* pOldPen = NULL;
	
	pOldFont = dc.SelectObject(pFont);
	
	pNewBrush = new CBrush;	//ASSERT(pNewBrush);
	VERIFY(pNewBrush->CreateSolidBrush(RGB(255,0,0)));
	
	pNewPen = new CPen; //ASSERT(pNewPen);
	VERIFY(pNewPen->CreatePen(PS_SOLID, 1, RGB(255,150,150)));

	pOldBrush = dc.SelectObject(pNewBrush);
	pOldPen = dc.SelectObject(pNewPen);

	CRgn rgn;
	rgn.CreateRectRgn(ClientRect.left, ClientRect.top, ClientRect.right, ClientRect.bottom);
	dc.SelectClipRgn(&rgn);
	
	// CEdit
	int iline = GetFirstVisibleLine();
	int	tline = GetLineCount();

	// Determine current line:
	int start, cend;
	GetSel(start, cend);

	//P2N(_T("Start:%d end:%d\r\n"), start, cend);

	int currline = LineFromChar(start);

	CString str;
	GetLine(currline , str);
	ProcessLine(currline, str, ClientRect, dc);

#if 0
	// Painted every line
	int curr = 0, prev = 0;
	for(int loop = iline; loop <= tline + 1; loop++)
		{
		curr = LineIndex(loop);
		if((start <= curr) &&  (start >= prev))
			{
			//P2N(_T("Curr in line %d\r\n"), loop);
			CString str;
			GetLine(loop - 1, str);
			ProcessLine(loop - 1, str, ClientRect, dc);
			prev = curr;
			break;
			}
		}
#endif

	m_Drawn = TRUE;

	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);

	delete(pNewBrush);
	delete(pNewPen);
	
	//P2N(_T("Painted pgedit\r\n"));
}

///////////////////////////////////////////////////////////

int	CPGedit::ProcessLine(int line, CString str, CRect &rect, CDC &dc)

{
	CPoint pt;
	CSize ss;

	//P2N(_T("line='%s'\r\n"), str);
	//AfxMessageBox(str);

	CString word;
	int offs = 0, len = str.GetLength();

	for(int loop = 0; loop < len; loop++)
		{
		if(!isalpha(str[loop] & 0xff))
			{
			if(word != _T(""))
				{		
				ProcessWord(word, LineIndex(line) + offs, dc);
				}
			offs = loop + 1;
			word = _T("");
			}
		else
			{
			word += str[loop];
			}
		}
	// Output leftover 
	if(word != _T(""))
		{
		ProcessWord(word, LineIndex(line) + offs, dc);
		}

	//if(pt.y > rect.Height())
	//	return false;
	//else

		return true;
}


void	CPGedit::ProcessWord(CString &word, int pos, CDC &dc)

{
	CSize  ss = dc.GetTextExtent(word);
	CPoint pt = PosFromChar(pos);

	if(!SpellWord(word))
		{
		//P2N(_T(")LWord: %s x=%d y=%d ww=%d\r\n\r\n"), word, pt.x, pt.y, ss.cx);
		DrawSquigly(dc, pt.x, pt.y + ss.cy, ss.cx);
		}

	//Sleep(100);
}


//////////////////////////////////////////////////////////////////////////

void CPGedit::AttachEdit(CWnd *pWnd, UINT uiControlID)

{
	//P2N(_T("Attaching\r\n"));

	//ASSERT(!IsWindow(GetSafeHwnd()));

	//ASSERT(pWnd);
	//ASSERT(::IsWindow(pWnd->GetSafeHwnd()));

	CWnd* pEdit = pWnd->GetDlgItem(uiControlID);
	//ASSERT(pEdit);
	//ASSERT(::IsWindow(pEdit->GetSafeHwnd()));

	SubclassWindow(pEdit->GetSafeHwnd());
}

//////////////////////////////////////////////////////////////////////////

void CPGedit::DrawSquigly(CDC &dc, int iLeftX, int iY, int iWidth)

{
	//P2N(_T("Drawing squiggly xx=%d yy=%d ww=%d\r\n"), iLeftX, iY, iWidth);
	
	int	step = 2;
	int	vstep = 2;

	CRect ClientRect;
	GetClientRect(ClientRect);

	if(iLeftX   >= ClientRect.Width())
		return;

	if(iY + step >= ClientRect.Height())
		return;
	
	int iCurrentY = iY;
	int iCurrentX = iLeftX;

	//iCurrentX = max(iCurrentX, 0);
	//iWidth = min(ClientRect.Width() - 2 * step, iLeftX + iWidth);


	while (iCurrentX <= iLeftX + iWidth - 2 * step)
		{
		dc.MoveTo(iCurrentX, iY);

		dc.LineTo(iCurrentX + step , iY + vstep);
	
		dc.LineTo(iCurrentX + 2 * step, iY);
		
		iCurrentX += 2 * step;
		}
}

//////////////////////////////////////////////////////////////////////////

void CPGedit::GetLine(int idx, CString &str)

{
	int maxlen  = 5 * _MAX_PATH;

	TCHAR *mem = str.GetBuffer(maxlen);

	int len = CEdit::GetLine(idx, mem, maxlen);

	if(len < maxlen)
		mem[len] = 0;

	str.ReleaseBuffer();
}

BOOL CPGedit::PreTranslateMessage(MSG* pMsg) 

{
	// create our timer when appropriate
	if(m_uiTimerID == 0)
		{
		if (::IsWindow(GetSafeHwnd()))
			{
			//P2N(_T("initial timer ID\r\n"));
			m_uiTimerID = SetTimer(1, 10, NULL);
			}
		}
	return CEdit::PreTranslateMessage(pMsg);
}

void CPGedit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nChar)
	{
	case  VK_UP:case  VK_DOWN:
		CheckLineChange();
		break;
	}
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CPGedit::OnTimer(UINT nIDEvent) 
{

	//P2N(_T("Timer for spell\r\n"));

	KillTimer(nIDEvent);

	if(!firsttime)
		{
		firsttime = true;
		Init_Spellcheck();
		}

	// if timer event is the spell checker event, check it now
	if (nIDEvent == m_uiTimerID)
	{
		DWORD dwCount = GetTickCount() - m_dwLastTick;

		if (dwCount > 1000 && !m_Drawn)
			{
			//P2N(_T("Timer for spell %d\r\n"),m_uiTimerID);

			//RedrawSpellingErrors();
			RedrawWindow();
			//m_Drawn = TRUE;
			m_dwLastTick = GetTickCount();	
			}
	m_uiTimerID = SetTimer(m_uiTimerID, 100, NULL);
	}

	CEdit::OnTimer(nIDEvent);
}

void CPGedit::OnChange() 

{
	SetModify(true);
	m_dwLastTick = GetTickCount();
	//m_Drawn = false;	
}

void CPGedit::OnVscroll() 
{
	//P2N(_T("Vscroll\r\n"));	
}

void CPGedit::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	if(nSBCode == SB_ENDSCROLL)
		{
		//P2N(_T("Hscrollbar msg %d\r\n"), nSBCode);
		m_Drawn = false;
		RedrawWindow();
		}

	CEdit::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPGedit::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	//P2N(_T("Show window\r\n"));	

	CEdit::OnShowWindow(bShow, nStatus);
	
}

void CPGedit::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	//P2N(_T("Activate window\r\n"));	

	CEdit::OnActivate(nState, pWndOther, bMinimized);	
}

int CPGedit::indict(CString &str)
{
	return (cnt++ % 2 == 0);
}

void CPGedit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);

	CheckLineChange();
}

void CPGedit::CheckLineChange()

{
	int start, cend;
	GetSel(start, cend);

	int currline = LineFromChar(start);
	if(currline != oldline	)
		{
		//P2N(_T("line change\r\n"));
		oldline = currline;
		m_Drawn = false;
		m_dwLastTick = GetTickCount();
		}
}

void CPGedit::OnRButtonDown(UINT nFlags, CPoint point) 

{
	// Nothing to do here
	if(!checkon)
		{
		CEdit::OnRButtonDown(nFlags, point);
		return;
		}

	int ss,ee;
	GetSel(ss,ee);

	//P2N(_T("Current Selection %d %d\r\n"), ss, ee);

	CPoint cp(point);
	CMenu Menu;

	int pos = CharFromPos(point) & 0xffff;
	int line = CharFromPos(point) >> 16;

	//P2N(_T("Right click pos=%d line=%d\r\n"), pos, line);

	int idx = LineFromChar(pos);
	int linebeg = pos - LineIndex(idx);

	CString str;
	GetLine(idx, str);

	//P2N(_T("Line on click pos=%d idx=%d '%s'\r\n"), pos, idx, str);
	
	// Get Current word
	CString word, word2;
	int offs = 0, len = str.GetLength();

	int loop;
	for(loop = 0; loop < len; loop++)
		{
		if(!isalpha(str[loop] & 0xff) || (str[loop]  & 0xff) == '\'')
			{
			if(word != _T(""))
				{
				if((linebeg >= offs) && (linebeg <=loop))
					{
					word2 = word;
					word = _T(")");
					//P2N(_T("Got Word: '%s'\r\n"), word2);
					break;
					}
				}
			offs = loop + 1;
			word = _T("");
			}
		else
			{
			word += str[loop] ;
			}
		}
	// Output leftover 
	if(word != _T(")"))
		{
		if(linebeg >= offs && linebeg <= loop)
			{
			word2 = word;
			word = _T("");
			//P2N(_T(")Got Word: '%s'\r\n"), word2);	
			}
		}

	if(ss == ee)
		{
		SetSel(LineIndex(idx) + offs, LineIndex(idx) + loop);
		}

	currword = word2;

	int res = SpellWord(word2);

	Menu.CreatePopupMenu();

	if(res)
		{
		Menu.AppendMenu(MF_GRAYED, 0, _T("Correct Spelling"));
		}
	else
		{
		Menu.AppendMenu(MF_GRAYED, 0, _T("*** Spelling Mistake ***"));
		}

	Menu.AppendMenu(MF_SEPARATOR, 0, _T(""));
	Menu.AppendMenu(MF_STRING, ID_EDIT_COPY, _T("Copy"));
	Menu.AppendMenu(MF_STRING, ID_EDIT_PASTE, _T("Paste"));

	Menu.AppendMenu(MF_STRING, ID_EDIT_CUT, _T("Cut"));
	Menu.AppendMenu(MF_STRING, ID_EDIT_SELECT_ALL, _T("Select All"));

	Menu.AppendMenu(MF_SEPARATOR, 0, _T(""));
	Menu.AppendMenu(MF_STRING, ID_ENCRYPT, _T("Encrypt ClipBoard"));
	Menu.AppendMenu(MF_STRING, ID_DECRYPT, _T("Decrypt ClipBoard"));
	Menu.AppendMenu(MF_SEPARATOR, 0, _T(""));
	Menu.AppendMenu(MF_STRING, ID_ENCRYPT2, _T("Copy / Encrypt / Paste Selection"));
	Menu.AppendMenu(MF_STRING, ID_DECRYPT2, _T("Copy / Decrypt / Paste Selection"));
	Menu.AppendMenu(MF_SEPARATOR, 0, _T(""));
	Menu.AppendMenu(MF_STRING, ID_EMPTYCLIP, _T("Empty Clipboard"));
	Menu.AppendMenu(MF_SEPARATOR, 0, _T(""));
	
	if(!res)
		{
		Menu.AppendMenu(MF_SEPARATOR, 0, _T(""));
		Menu.AppendMenu(MF_STRING, ID_SPELL_ADD, _T("Add to Dictionary"));
		Menu.AppendMenu(MF_STRING, ID_SPELL_IGNORE, _T("Ignore Spelling"));		
		}

	Menu.AppendMenu(MF_SEPARATOR, 0, _T(""));

	// Suggested words:
	Suggest(Menu, word2);

	ClientToScreen(&cp);
	Menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,  cp.x, cp.y, this);
}

//////////////////////////////////////////////////////////////////////////

void CPGedit::Init_Spellcheck()

{
	int cnt = 0;
    //TCHAR buffer[MAX_READ];

	if(dic_good)
		return;

	dic_good = true;

	CWaitCursor wc;

    dicmain = approot; dicmain +=  _T("spell.txt");
    //P2N(_T("Dictionary: %s\r\n"), dicmain);
	
	//AfxMessageBox(dicmain);

//    message (_T("Started Spell Check"));
    if(_taccess(dicmain, 0))
        {
        CString num;
        num.Format(_T("Cannot open Dictionary: %s"), dicmain);
        AfxMessageBox(num);
        return;
        }

    // Check for index, recreate if needed
    dicidx = dataroot; dicidx +=  _T("spell.idx");
    if(_taccess(dicidx, 0) < 0)
        {
        CString num;
        num.Format(_T("Creating dictionary index ..."));
        //message(num);

        TCHAR    old = 0;
        int     found   = FALSE;
        
		//FILE    *fp, *fp2;

		int count = 0, count2 = 0;

        //fp1 = fopen(dicmain, _T("rt"));
		fp1 = _tfopen(dicmain, _T("rt"));
        if(!fp1) 
            {
            AfxMessageBox(_T("Cannot open dictionary file\n"));
            return;
            }
        FILE *fp2 = _tfopen(dicidx, _T("w+t"));
        //FILE *fp2; _tfopen_s(&fp2, dicidx, _T("w+t"));
        if(!fp2)
            {
            AfxMessageBox(_T("Cannot create index file\n"));
            return;
            }

		CString cont; GetWindowText(cont);

        while(TRUE)
            {
            TCHAR    *readd;
            long    fpos;

            fpos  = ftell(fp1);                    /* prev pos */
            readd = _fgetts(buffer, MAX_READ, fp1);

			count++;

			if(count % 10000 == 0)
				{
				CString str; str.Format(_T("Indexing dictionary (%d)\r\nPlease wait ..."), count);
				SetWindowText(str);
				support.YieldToWinEx();
				}

			// Is ascii?
			if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
				{	
				if(tolower(buffer[0]) != old)				/* new buffer */
					{
					_ftprintf(fp2, _T("%d %c\n"), fpos, tolower(buffer[0]));
					//P2N(_T("boundary word: %s -- %d\r\n"), buffer, fpos);
					old = tolower(buffer[0]);
					count2++;
					}
				}
			
			if(feof(fp1))
				break;

            if(!readd)
                break;
            }

        fclose(fp1); fclose(fp2);
		
		SetWindowText(cont); support.YieldToWinEx();

		//P2N(_T("Created %d index entries %d unique\r\n"), count, count2);
        }

    dicuser = dataroot; dicuser +=  _T("spell.cus");

    if(_taccess(dicuser, 0) < 0)
		{
		FILE *fp3 = _tfopen(dicuser, _T("w+t"));
        //FILE *fp3; _tfopen_s(&fp3, dicuser, _T("w+t"));
        if(!fp3)
            {
            AfxMessageBox(_T("Cannot create user dictionary file\n"));
            return;
            }
		fclose(fp3);
		}

	FILE *fp2 = _tfopen(dicidx, _T("rt"));
	//FILE *fp2; _tfopen_s(&fp2, dicidx, _T("rt"));

	if(!fp2)
        {
        AfxMessageBox(_T("Internal: Cannot open dictionary index file\n"));
		goto endd;
		}


	// Load index to memory
    while(TRUE)
        {
        TCHAR    *readd;
        int     pos;
		TCHAR	letter;

        readd = _fgetts(buffer, MAX_READ, fp2);

		if(feof(fp2))
			break;
			
        if(!readd)
            break;

        //_stscanf_s(buffer, _T("%d %c"), &pos, &letter);
		_stscanf(buffer, _T("%d %c"), &pos, &letter);

		//P2N(_T("index=%d letter=%c\r\n"), pos, letter);

		widx.SetAtGrow(cnt, pos);
        cnt++;
        }
    fclose(fp2);

	//P2N(_T("CPGedit::Init_Spellcheck Built index for array. Size=%d \r\n"), widx.GetSize());

	fp1 = _tfopen(dicmain, _T("rt"));
    //_tfopen_s(&fp1, dicmain, _T("rt"));
    if(!fp1)
        {
        AfxMessageBox(_T("Internal: Cannot open dictionary data file\n"));
        goto endd;
        }

    fp3 = _tfopen(dicuser, _T("r+t"));
    //_tfopen_s(&fp3, dicuser, "r+t");
    if(!fp3)
        {
        AfxMessageBox(_T("Internal: Cannot open custom dictionary file\n"));
        //fclose(fp1);  fclose(fp2);
        goto endd;
        }
endd:
		;

	//AfxMessageBox(_T("dic good"));
}

//////////////////////////////////////////////////////////////////////////

int		CPGedit::SpellWord(CString &str)


{
	int		offset = 0, ret = 0;
	int     cnt = 0, start = 0, end = 0;

	//P2N(_T("Spelling word: '%s'\r\n"), str);
	//return 1;

    in_spell = 0;

	if(str == _T(""))
		return true;

    if(in_spell)
        return true;

    in_spell = TRUE;	// Exit function via goto endd

	CString str2(str);

	str2.MakeLower();

	
	// Check if it is a hex str
	int len2 = str2.GetLength(), hex = true;
	for(int loop = 0; loop < len2; loop++)
		{
		TCHAR chh = str2.GetAt(loop);
		if(!_istxdigit(chh)) 
			hex = false;
		}

	// Check ignore list
	POSITION pos1 = ignore.GetHeadPosition();

	// If all hex, assume encrypted, say good
	if(hex) { ret = true; goto endd; }

	while(true)
		{
		if(!pos1)
			break;

		CString &xstr = ignore.GetNext(pos1);
		
		//P2N(_T("Ignore scan '%s' ==> '%s'\r\n"), xstr, str2);
		
		// Ignore this one as GOOD
		if(xstr == str2)
			{
			ret = true; goto endd;
			}
		}

	if(!fp3)  { goto endd; }

	// Check user dictionary
	fseek(fp3, 0, SEEK_SET);
	while(TRUE)
        {
        if(!_fgetts(buffer, MAX_READ, fp3) )
            break;

		buffer[_tcslen(buffer)-1] = '\0';

		//P2N(_T("usedic check [%s] against [%s]\r\n'  "), str2, buffer);

		// Ignore this one as GOOD
		if(_tcscmp(buffer, str2) == 0)
			{
			ret = true; goto endd;
			}
		}

	// Figure out starting letter, check bounds
	int idx = str2[0] - 'a';
	if(idx >=0 && idx < widx.GetSize())
        offset =  widx.GetAt(str2[0] - 'a');

	//P2N(_T("Starting at buffer offset %d\r\n"), offset);

	if(!fp1)
		return 1;

	// Look up in dictionary
    fseek(fp1, offset, SEEK_SET);
    while(TRUE)
        {
        if(!_fgetts(buffer, MAX_READ, fp1) )
            break;

        buffer[_tcslen(buffer)-1] = '\0';

        //P2N(_T("check [%s] against [%s]\r\n'  "), str2, buffer);

        if(!_tcsicmp(str2, buffer) )
            {
            //P2N(_T("Found: '%s' "), buffer);
            ret = true;
            break;
            }

		// Next letter, break

		// Is ascii?
		if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
			{
			if(tolower(buffer[0]) !=  str2[0])
				break;
			}
        }
    
	//CheckLine(v1, v1->row, flag);
	
    //fclose(fp1); fclose(fp3);

endd:

    in_spell = FALSE;

	if (!ret)
		{
		P2N(_T("Misspelled word '%s'\r\n"), str);
		}

	return ret;
}

void CPGedit::Suggest(CMenu &menu, CString str)

{
	int cnt = 0, offset = 0, command = ID_SPELL_WORD1;

	if(str == _T(""))
		return;

	replace.RemoveAll();

	CString str2(str);
	str2.MakeLower();

	//P2N(_T("Suggesting %s\r\n"), str);

	// Figure out starting letter
    if(widx.GetSize() > (str2[0] - 'a'))
        offset =  widx.GetAt(str2[0] - 'a');

	if(!fp1) return;

	//P2N(_T("Starting at buffer offset %d\r\n"), offset);
	// Look up in dictionary
    fseek(fp1, offset, SEEK_SET);

	CStringList strlist;

	Distance dist;

	// Fill in selection
	while(TRUE)
		{
		if(!_fgetts(buffer, MAX_READ, fp1) )
			break;

		int len = _tcslen(buffer) - 1;
		buffer[len] = '\0';
		 
		for(int loop = 0; loop < len; loop++)
			{
			buffer[loop] = tolower(buffer[loop]);
			}

		//P2N(_T("Distance [%s] against [%s]\r\n'  "), str2, buffer);
		//if(cnt++ > 1000)
		//	break;

		// Is ascii?
		if(tolower(buffer[0]) >= 'a' && tolower(buffer[0]) <= 'z')
			{
			if(tolower(buffer[0]) !=  str2[0])
				break;
			}

		int ddd = dist.LD(buffer, str2);		
		if(ddd < 3 ) // ||  eee < 3)
			{
			//P2N(_T("Suggested %s\r\n"), buffer);
			CString tmp; tmp.Format(_T("%d %s"), ddd, buffer);
			strlist.AddTail(tmp);
			}	
		}

	// Buld array
	CPtrArray  arr;

	POSITION pos1 = strlist.GetHeadPosition();
	while(true)
		{
		if(!pos1)
			break;
		CString &str = strlist.GetNext(pos1);
		arr.Add(&str);
		}

	// Verify array
	int size = arr.GetSize();

	//for(int loop = 0; loop < size; loop++)
	//	{
	//	CString *tmp = (CString *)arr.GetAt(loop);
	//	P2N(_T("PTR %s\r\n"), *tmp);
	//	}

	qsort(arr.GetData(), size, sizeof(void *), comparestr);

	int cnt2 = 0;
	for(int loop2 = 0; loop2 < size; loop2++)
		{
		CString *tmp = (CString *)arr.GetAt(loop2);
	
		if(cnt2 == 0)
			{
			CString tmp2; tmp2.Format(_T("Suggestions for: '%s'"), str);
			menu.AppendMenu(MF_GRAYED, 0, tmp2);
			menu.AppendMenu(MF_SEPARATOR, 0, _T(""));
			}

		*tmp = tmp->Mid(2);
		replace.Add(*tmp);

		//P2N(_T("Appending %d %s\r\n"), loop2, *tmp);
		menu.AppendMenu(MF_STRING, command + loop2, *tmp);

		if(++cnt2 >= 10) 
			break;
		}

	if(cnt2 == 0)
		{
		CString tmp2; tmp2.Format(_T("No suggestions for: '%s'"), str);
		menu.AppendMenu(MF_GRAYED, 0, tmp2);
		//menu.AppendMenu(MF_STRING, 0, str);
		}

	//menu.AppendMenu(MF_SEPARATOR, 0, _T(""));
	//menu.AppendMenu(MF_STRING, ID_SPELL_MORE, _T("More Suggestions ..."));
}

void CPGedit::OnDestroy() 

{
	CEdit::OnDestroy();
	
	//P2N(_T("Destroy PGedit %p\r\n"), this);	
	//if(fp1) fclose(fp1);
	//if(fp3) fclose(fp3);
}

void CPGedit::OnSetFocus(CWnd* pOldWnd) 

{
	CEdit::OnSetFocus(pOldWnd);
	m_Drawn = false;
	//P2N(_T("Setfocus\r\n"));	
}

void CPGedit::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 

{
	if(nSBCode == SB_ENDSCROLL)
		{
		//P2N(_T("Vscrollbar msg %d\r\n"), nSBCode);
		m_Drawn = false;
		RedrawWindow();
		}
	CEdit::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPGedit::SelAll()

{
	SetSel(0, -1);
	//Copy();
}

//////////////////////////////////////////////////////////////////////////


void CPGedit::Encrypt()

{
	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	//P2N(_T("void CPGedit::Encrypt()\r\n"));
	cc->Encrypt_Clipboard(cryptver);
}
	
//////////////////////////////////////////////////////////////////////////

void CPGedit::Decrypt()

{
	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;

	//P2N(_T("void CPGedit::Decrypt()\r\n"));
	cc->Decrypt_Clipboard(cryptver);
}

//////////////////////////////////////////////////////////////////////////

void CPGedit::Encrypt2()

{
	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;
	//P2N(_T("void CPGedit::Encrypt2()\r\n"));
	
	int ss,ee;	GetSel(ss,ee);
	//P2N(_T("ss %d ee %d\r\n"), ss, ee);
	if(ss == ee)
		{
		AfxMessageBox(_T("No text selected to encrypt."));
		return;
		}


	if(ee - ss < 4)
		{
		AfxMessageBox(_T("Must select at least four or more letters to encrypt."));
		return;
		}

	// CEdit
	CString text; GetWindowText(text);
	text = text.Mid(ss, ee - 1);

	//P2N(_T("Selected text '%s'\r\n"), text);

	OpenClipboard(); EmptyClipboard();	CloseClipboard();
	
	Copy();
	cc->Encrypt_Clipboard(cryptver);
	Paste();
	SetFocus();
}
	
//////////////////////////////////////////////////////////////////////////

void CPGedit::Decrypt2()

{
	YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;

	//P2N(_T("void CPGedit::Decrypt2()\r\n"));

	int ss,ee;	GetSel(ss,ee);
	//P2N(_T("ss %d ee %d\r\n"), ss, ee);
	if(ss == ee)
		{
		AfxMessageBox(_T("No text selected to decrypt."));
		return;
		}

	OpenClipboard(); EmptyClipboard();	CloseClipboard();

	Copy();
	cc->Decrypt_Clipboard(cryptver);
	Paste();
}


void CPGedit::EmptyClip()

{
	//YellowDlg *cc = (YellowDlg *)AfxGetApp()->m_pMainWnd;

	//P2N(_T("void CPGedit::EmptyClip()\r\n"));

	OpenClipboard(); EmptyClipboard();	CloseClipboard();
	
}

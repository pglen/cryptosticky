 
/* =====[ Yellowdlg.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Yellowdlg.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------    -----------  ----------------------------   
      0.00  6/4/2009    Peter Glen   Initial version.                         
      0.00  jan/4/2017  Peter Glen   Compiled for VC 2015
                                                                             
   ======================================================================= */

//_WIN32_WINNT_MAXVER

#include "stdafx.h"

#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <direct.h>
#include <mmsystem.h>
#include <io.h>
#include <afxmt.h>
#include <process.h>
#include <winsock2.h>

#include "datalog.h"
#include "clicense.h"

#include "CryptoSticky.h"

#include "Yellowdlg.h"
#include "mxpad.h"
#include "Popup.h"
#include "fakewin.h"
#include "gotoline.h"
#include "Alarm.h"
#include "ymemo.h"
#include "about.h"
#include "StatList.h"
#include "xArchive.h"
#include "logdlg.h"
#include "Setup.h"
#include "misc.h"
#include "bluepoint.h"
#include "pass.h"
#include "resolv.h"
#include "setuptab.h"
#include "SearchDLG.h"
#include "Banner.h"
			
#pragma warning (disable : 4996)

CString		pro(_T("Access"));
CString		keystr(_T("Software\\RobotMonkeySoftware\\CryptoSticky\\Alarms"));

CString		myURLstr  =_T("http://www.robotmonkeysoftware.com/cryptosticky/");
CString		buyURLstr = _T("http://www.robotmonkeysoftware.com/cryptosticky/buy.htm");

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
const char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

void	YellowDlg::_EditThreadProc(void *ptp )

{
	YellowDlg*  me = (YellowDlg*)ptp;

	// 
	AttachThreadInput(me->thhid, GetCurrentThreadId(), true);

	while(true)
		{
		WaitForSingleObject(me->event, INFINITE);

		P2N(_T("event\r\n"));
		me->EditCurrent();
		}
}

void	YellowDlg::_mCastThreadProc(void *ptp )

{
	YellowDlg*  me = (YellowDlg*)ptp;

	P2N(_T("void YellowDlg::OnRight2BroadcastAll()\r\n"));

	me->Iterate_Exec(NULL, 0, IteratorBcast);
}

void	YellowDlg::_CastThreadProc(void *ptp )

{
	YellowDlg*  me = (YellowDlg*)ptp;

	HTREEITEM h1 = me->m_tree.GetSelectedItem();
	if(!h1)
		return;
	CString str; me->GetNodePath(str, h1);
	
	//P2N(_T("OnRightBroadcast path='%s'\r\n"), str);

	me->BCastFile(str);
}

//////////////////////////////////////////////////////////////////////////

void	YellowDlg::_SendThreadProc(void *ptp )

{
	YellowDlg*  me = (YellowDlg*)ptp;

	HTREEITEM h1 = me->m_tree.GetSelectedItem();
	if(!h1)
		return;

	CString str; me->GetNodePath(str, h1);

	// Make a copy for sure
	CString sto = me->sendto;
	
	//P2N(_T("Sending file='%s' to=%s\r\n"), str, sto);

	me->SendFiles(str, sto);	
}

//////////////////////////////////////////////////////////////////////////

void	YellowDlg::_MemoThreadProc(void *ptp )

{
	char	buff[512];
	TCHAR	tmpname[_MAX_PATH + 1];

	YellowDlg*  me = (YellowDlg*)ptp;

	SOCKET	sock2; 
	
    //////////////////////////////////////////////////////////////////////
	// Listen for transmissions port xxx

    sock2 = socket(AF_INET, SOCK_STREAM, 0);
    
	if(sock2 == INVALID_SOCKET)
		{
		AP2N(_T("YellowDlg::MemoThreadProc -- Cannot create socket\r\n"));
		return;
		}

	//P2N(_T("YellowDlg::MemoThreadProc sock2=%d %s\r\n"), sock2, 
	//				mxpad.sockerr2str(WSAGetLastError()));

    //int ndlen  = sizeof(char), ret_val; TCHAR	nd;

	//ret_val = setsockopt (sock2, SOL_SOCKET, 
	//								SO_DONTLINGER, &nd, sizeof(int));
    
	SOCKADDR_IN rec_sin; int rec_sin_len = sizeof(rec_sin);

	rec_sin.sin_addr.s_addr = INADDR_ANY;
	rec_sin.sin_port = htons(me->memo_port);
	rec_sin.sin_family = AF_INET;	
	
    if (bind( sock2, (struct sockaddr FAR *) &rec_sin, sizeof(rec_sin))
        == SOCKET_ERROR)
        {
        //TRACE(_T("YellowDlg::AcceptThreadProc -- Cannot Bind socket\r\n"));
		AP2N(_T("YellowDlg::MemoThreadProc -- Cannot Bind socket\r\n"));

		//AfxMessageBox(_T("Cannot Bind socket, remote memos will not be available"));		
		return;
        }	
	
	if(listen(sock2, 1) == SOCKET_ERROR)
		{
		AP2N(_T("YellowDlg::MemoThreadProc -- Cannot listen on socket\r\n"));
		return;
		}

	// Get temp dir
	//_CRT_INSECURE_NO_DEPRECATE
	TCHAR *env = _tgetenv(_T("TEMP"));	 
	TCHAR *tmpname2 = _ttempnam(env, _T("sticky_"));
	_tcsncpy(tmpname,  tmpname2, _MAX_PATH);

	//TCHAR *env; size_t esize = _MAX_PATH; 
	//_tdupenv_s(&env, &esize,_T("TEMP"));
	//P2N(_T(")TEMP env '%s'\r\n"), env);
	//_tcsncpy_s(tmpname, _MAX_PATH, tmpname2, _TRUNCATE);
	//if(env) free(env);
	//if(tmpname2) free(tmpname2);

	//P2N(_T("Got tmp file %s\r\n"), tmpname);

	//TCHAR *tmpname =_T("tmlfile");
	
	while(true)
		{
		SOCKADDR_IN rec_sin2; int rec_sin_len2 = sizeof(rec_sin2);

		SOCKET sock3 = accept(sock2, (struct sockaddr FAR *) &rec_sin2, &rec_sin_len2);
		
		int  saveit = true;
		char *str = inet_ntoa( *((struct in_addr*) &rec_sin2.sin_addr.s_addr));

		//P2N(_T("YellowDlg::MemoThreadProc accept from %s %d saveit=%d\r\n"), str, rec_sin_len, saveit);

		int alen = resolver.namearr.GetSize();
		for(int loop = 0; loop < alen; loop++)
			{
			CNameEntry *val = (CNameEntry*)resolver.namearr.GetAt(loop);
			if(*val->ip ==  str)
				{
				CString tmp; 
				tmp.Format(_T("%s::%s::%s"), *val->machine, *val->username, _T("recv"));
				int val1 = AfxGetApp()->GetProfileInt(pro, tmp, 1);
				if(!val1)
					{
					//P2N(_T("no recv %s\r\n"), *val->machine);
					saveit = false;
					}
				}
			}

		CFile cf;
			
		if(saveit)
			{			
			//AP2N(_T("tmp file %s  %s\r\n"), env, tmpname);
		
			if(!cf.Open(tmpname, CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite))
				{
				AP2N(_T("Cannot create tmp file\r\n"));			
				}
			}

		char  ctype[13];

		// Get content type
		recv(sock3, ctype, sizeof(ctype) - 1, 0);
		ctype[sizeof(ctype) - 1] = 0;
		
		CString ext2 = support.GetExtension(ctype);

		//P2N(_T("Got type %s %s\r\n"), ctype, ext2);

		int total = 0;
		while(true)
			{
			int ret = 0;
			
			ret = recv(sock3, buff, sizeof(buff), 0);

			total += ret;

			//P2N(_T("YellowDlg::MemoThreadProc recv  len=%d %s\r\n"), ret, mxpad.sockerr2str(WSAGetLastError()));
				
			if(ret == 0)
				{
				break;
				}

			if(ret == SOCKET_ERROR)
				{
				//P2N(_T("YellowDlg::MemoThreadProc recv err %s\r\n"), mxpad.sockerr2str(WSAGetLastError()));
				break;
				}
			//buff[ret] = 0; //P2N(_T("%s"), buff);

			if(saveit)
				cf.Write(buff, ret);
			}

		closesocket(sock3);

		//P2N(_T("YellowDlg::MemoThreadProc closed recv %s\r\n"), tmpname);
		
		if(saveit)
			{
			cf.Close();
		
			if(ext2 ==_T("txt"))
				{
				MemoSupp memo; 
							
				CString changed = me->ImportMemo(tmpname);	

				_tunlink(tmpname); 
				//free(tmpname);

				if(changed != _T(""))
					{
					//CString str2; misc.AnsiToUnicode(str, str2);
					datalog.PrintToLog(_T("Received sticky from '%s' %d bytes\r\n"), str, total);

					CString chh2 = support.GetFileNameExt(changed);
					CString chh3 = support.GetFilename(changed);
					CString chh4 = support.GetFilename(ctype);
					
					//P2N(_T("Changed from='%s' to='%s'\r\n"), chh4, chh3);
					me->chmem.Add(chh4, &chh3);

					// Make sound
					if(me->m_listen)
						{
						//me->SetTimer(7, 20, NULL);

						//P2N(_T("time diff %d\r\n"), time(NULL) - me->lastsound);
						if(time(NULL) - me->lastsound > 5)
							{
							if(beepflag)
								{
								Beep(beepfreq, beeplen);
								}
							if(notifyflag)
								{
								PlaySound(notifysound, NULL, SND_FILENAME | SND_ASYNC);
								}
							if(voiceflag)
								{
								memo.Read(changed);
#ifdef SPEAK
								me->sp.Speak(memo.head);	
#endif
								}							
							me->lastsound = (int)time(NULL);
							}
						}
					}
				}
			else 
				{
				CString chh4 = support.GetFilename(ctype);
				CString changedto = me->chmem.Lookup(chh4);
				changedto +=_T("."); changedto += support.GetExtension(ctype);

				CString newfile(dataroot); 	newfile += changedto;

				if(_taccess(newfile, 0) >= 0)
					{
					//P2N(_T("Existing dependent %s\r\n"), newfile);

					// Is it the same size?
					struct _stat st1; memset(&st1, 0, sizeof(st1));
					struct _stat st2; memset(&st2, 0, sizeof(st2));					

					_tstat(newfile, &st1); _tstat(tmpname, &st2);

					//P2N(_T("stat %d %d\r\n"), st1.st_size, st2.st_size);
			
					if(st1.st_size !=  st2.st_size)
						{
						CString backupdir(dataroot); backupdir +=_T("backup\\");
						CString backupname(backupdir); backupname += changedto;		

						if(_taccess(backupdir, 0) < 0)
							{
							//P2N(_T("created dir %s\r\n"), backupdir);
							_tmkdir(backupdir);
							}

						// Create unique name
						CString newbackup(backupname);						
						if(_taccess(newbackup, 0) >= 0)
							{							
							int cnt = 1; 
							CString dir = support.GetDirName(backupname);
							CString name = support.GetFilename(backupname);
							CString ext = support.GetExtension(backupname);
															
							while(true)
								{																
								CString tmp; tmp.Format(_T("-%d"), cnt++);
								
								newbackup = dir +_T("\\") + name;
								newbackup += tmp +_T(".") + ext;

								if(_taccess(newbackup, 0) < 0)
									break;					
								}
							}
						//P2N(_T("unique backup name %s\r\n"), newbackup);
						CopyFile(newfile, newbackup, false);
						}
					}
				
				//P2N(_T("Save dependent %s\r\n"), newfile);
				CopyFile(tmpname, newfile, false);
				
				_tunlink(tmpname); 
				//free(tmpname);	
				
				if(me->m_listen)
					me->SetTimer(7, 20, NULL);
				}
			}
		
		if(me->shutdown)
			break;
		}

	//shutdown(sock2, 0);
	closesocket(sock2);

	//TRACE(_T("YellowDlg::MemoThreadProc - End thread.\r\n"));
}

//////////////////////////////////////////////////////////////////////////
//

int	YellowDlg::IteratorBcast(YellowDlg *ptr, HTREEITEM hh, int parm)

{
	CString str; ptr->GetNodePath(str, hh);
	ptr->BCastFile(str);
	return 0;
}

int	YellowDlg::IteratorExport(YellowDlg *ptr, HTREEITEM hh, int parm)

{
	CString str = ptr->m_tree.GetItemText(hh);
	CString str2 = (const TCHAR *)parm;

	support.RepairPath(str2);

	//P2N(_T("IteratorExport '%s'\r\n'%s'"), str2, str);
	
	CString fname; ptr->GetNodePath(fname, hh);
	CString name = support.GetFileNameExt(fname);
	
	CString target = str2 + name;

	int ret = CopyFile(fname, target, false);

	if(ret == 0)
		{
		//P2N(_T("Could not export '%s'\r\n"), fname);
		//ptr->found = -1;
		}

	//////////////////////////////////////////////////////////////////////////
	// Copy dependent files

	ptr->CopyDep(fname, target);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////

int	YellowDlg::IteratorDup(YellowDlg *ptr, HTREEITEM hh, int parm)

{
	//CString str = ptr->m_tree.GetItemText(hh);
	CString tmp; ptr->BuildTree(tmp, hh);

	const TCHAR *str2 = (const TCHAR *)parm;

	//P2N(_T("IteratorDup %s %s\r\n"),str2, tmp);
		
	if(tmp == str2)
		{
		ptr->found	= true;
		ptr->dup	= hh;
		}
	return 0;
}

int	YellowDlg::IteratorShow(YellowDlg *ptr, HTREEITEM hh, int parm)

{
	// Hide it anyway
	if(parm == 0)
		{
		ptr->ShowOne(hh, false);
		}
	else
		{
		if(ptr->m_tree.GetCheck(hh))
			{
			//CString str = ptr->m_tree.GetItemText(hh);
			//P2N(_T("YellowDlg::IteratorShow %s\r\n"), str);
			
			ptr->ShowOne(hh);
			}
		else
			{
			ptr->ShowOne(hh, false);
			}
		}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

int	YellowDlg::IteratorGather(YellowDlg *ptr, HTREEITEM hh, int parm)

{
	CRect	rc3; misc.WinMonitorRect((CWnd*)ptr, &rc3);
	
	int hstart =   1 * rc3.Width() / 16;
	int vstart =   1 * rc3.Height() / 16;
	int hend   =   15 * rc3.Width() / 16;
	int vend   =   15 * rc3.Height() / 16;

	//P2N(_T("Current mon %d %d %d %d\r\n"), 
	//						rc3.top, rc3.left, rc3.bottom, rc3.right);

	// Hide it anyway
	if(parm == false)
		{
		ptr->ShowOne(hh, false);
		}
	else
		{
		CYmemo *st = ptr->Handle2Memo(hh);

		if(st)
			{
			//ASSERT(st->magic == CYmemo_Magic);

			WINDOWPLACEMENT wp;	st->GetWindowPlacement(&wp);

			int ww = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
			int	hh = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;

			int hpos  =  rc3.left + hstart +  ptr->gatheroffsetx;
			int vpos  =  rc3.top  + vstart +  ptr->gatheroffsety;
			
			//P2N(_T("hpos=%d vpos=%d\r\n"), hpos, vpos);

			int modded = 0;
			
			if(hpos + ww > hend)
				{	
				ptr->gatheroffsety = rand() % 40 + 10;
				ptr->gatheroffsetx = rand() % 40 + 10;

				hpos  =  rc3.left + hstart +  ptr->gatheroffsetx;
				vpos  =  rc3.top  + vstart +  ptr->gatheroffsety;			

				ptr->gatherstep = 0;
				modded = true;
				}
			if(vpos + hh > vend)
				{
				ptr->gatheroffsety = 0;
				ptr->gatheroffsetx += ptr->gatherstep;

				hpos  =  rc3.left + hstart +  ptr->gatheroffsetx;
				vpos  =  rc3.top  + vstart +  ptr->gatheroffsety;			

				ptr->gatherstep = 0;
				modded = true;
				}
			
			if(!modded)
				{
				ptr->gatheroffsety += hh + 4;
				ptr->gatherstep = max(ptr->gatherstep, ww + 4);
				}			
			st->SetWindowPos(NULL, hpos, vpos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
			st->SavePos();
			}	

		if(ptr->m_tree.GetCheck(hh))
			{			
			ptr->ShowOne(hh);
			}
		else
			{
			ptr->ShowOne(hh, false);
			}
		}

	return 0;
}

//////////////////////////////////////////////////////////////////////////
//

int	YellowDlg::IteratorSel(YellowDlg *ptr, HTREEITEM hh, int parm)

{
	ptr->m_tree.SetCheck(hh, parm);

	if(ptr->m_tree.GetCheck(hh))
		{
		ptr->ShowOne(hh, true);
		}
	else
		{
		ptr->ShowOne(hh, false);
		}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int	YellowDlg::Iterator(YellowDlg *ptr, HTREEITEM hh, int parm)

{
	P2N(_T("iterate item %s\r\n"), ptr->m_tree.GetItemText(hh));

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::Iterate_Exec(HTREEITEM currentItem, int parm, YDCBRI funct)

{
	int stop = 0;

	if(currentItem == 0)
		currentItem = TVI_ROOT;

	HTREEITEM last = 0;
	HTREEITEM hChildItem = m_tree.GetChildItem(currentItem);

	while(hChildItem)
		{
		stop = funct(this, hChildItem, parm);

		if(stop)
			break;

		Iterate_Exec(hChildItem, parm, funct);

		hChildItem = m_tree.GetNextItem(hChildItem, TVGN_NEXT);
		}
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::Iterate_Sel(HTREEITEM currentItem, int sel)

{
	//P2N(_T("Iterate_Sel %d\r\n"), sel);

	Iterate_Exec(currentItem, sel, IteratorSel);
}

//////////////////////////////////////////////////////////////////////////
//

void YellowDlg::Iterate_Show(HTREEITEM currentItem, int show)

{
	Iterate_Exec(currentItem, show, IteratorShow);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::Iterate_Checks(HTREEITEM currentItem )

{
	unsigned int numSelectedFolder = 0;

	if(currentItem == 0)
		currentItem = TVI_ROOT;

	HTREEITEM last = 0;
	HTREEITEM hChildItem = m_tree.GetChildItem(currentItem);

	while(hChildItem)
		{
		// Reflect checked state
		CString str;   BuildTree(str, hChildItem);
		CString fname; GetNodePath(fname, hChildItem);
		CString title = m_tree.GetItemText(hChildItem);

		// Read YellowDlg registry
		//CString section(_T("Memos"));

		CString line = AfxGetApp()->GetProfileString(section, fname,_T("0"));
		//m_tree.SetCheck(hChildItem, _tstoi(line));
		m_tree.SetCheck(hChildItem, _tstoi(line));

		Iterate_Checks(hChildItem);

		//if(m_tree.GetCheck(hChildItem))
		//	{
		//	//CString str2 = m_tree.GetItemText(hChildItem);
		//	//P2N(_T("Terminal: %s\r\n"), str2);
		//	}

		hChildItem = m_tree.GetNextItem(hChildItem, TVGN_NEXT);
		}
}

///////////////////////////////////////////////////////////////////////////

void YellowDlg::WrIterate_Checks(HTREEITEM currentItem )

{
	unsigned int numSelectedFolder = 0;

	if(currentItem == 0)
		currentItem = TVI_ROOT;

	HTREEITEM last = 0;
	HTREEITEM hChildItem = m_tree.GetChildItem(currentItem);

	while(hChildItem)
		{
		CString str;   BuildTree(str, hChildItem);
		CString fname; GetNodePath(fname, hChildItem);
		CString title = m_tree.GetItemText(hChildItem);

		// Write YellowDlg registry
		//CString section(_T("Memos"));
		int checked = m_tree.GetCheck(hChildItem);

		CString tmp; tmp.Format(_T("%d"), checked);
		AfxGetApp()->WriteProfileString(section, fname, tmp);

		//P2N(_T("Wrote reg key: %s %s\r\n"), fname, tmp);

		WrIterate_Checks(hChildItem);

		//if(m_tree.GetCheck(hChildItem))
		//	{
		//	//CString str2 = m_tree.GetItemText(hChildItem);
		//	//P2N(_T("Terminal: %s\r\n"), str2);
		//	}

		hChildItem = m_tree.GetNextItem(hChildItem, TVGN_NEXT);
		}
}

/////////////////////////////////////////////////////////////////////////////
// YellowDlg dialog

YellowDlg::YellowDlg(CWnd* pParent /*=NULL*/)
    : CDialog(YellowDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(YellowDlg)
	m_restart = FALSE;
	m_listen = FALSE;
	m_hide = FALSE;
	m_auto = FALSE;
	m_ignore = FALSE;
	//}}AFX_DATA_INIT

	memo_port = YM_MEMO_PORT;

	m_BackColor = RGB(192, 192, 192);
	firstshow = 0;

	WSAData wsadat;	WSAStartup(MAKEWORD(2,0), &wsadat);

	shutdown = false;
	load_complete = false;
	prevwin = 60;
	prevx = 20;
	g_h1 = NULL;
	stable = false;
	section =_T("Memos");
	num_alarms = 0;
	LastFolderTick = GetTickCount();
	reenter = 0;
	lastsound = 0;
	gatherstep = gatheroffsetx =  gatheroffsety = 0;
	//cryptver = 3;
	ped = NULL;

	C2N;

	flag12 = 0;

	// Testing: Auto set alarm
	//alarm_tt = time(NULL);
	//struct tm *t2 = localtime(&alarm_tt);
	//t2->tm_year += 20;
	//alarm_tt = mktime(t2);

	alarm_tt = 0;

	//P2N(_T("In construct for YellowDlg init dialog\r\n"));

	thhid = GetCurrentThreadId();

	if(!thhandle)
		thhandle = _beginthread(_MemoThreadProc, 0, (void*)this);

	event = CreateEvent(NULL, 0, 0, NULL);
	_beginthread(_EditThreadProc, 0, (void*)this);

	// Assume no alarm for the time being
	al_done = true;
}

/////////////////////////////////////////////////////////////////////////////

void YellowDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(YellowDlg)
	DDX_Control(pDX, IDC_TELL, m_tell);
	DDX_Control(pDX, IDOK3, m_ok3);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Control(pDX, IDOK2, m_ok2);
	DDX_Control(pDX, IDC_BUTTON35, m_b35);
	DDX_Control(pDX, IDC_BUTTON34, m_b34);
	DDX_Control(pDX, IDC_BUTTON40, m_b40);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_Control(pDX, IDC_BUTTON17, m_b17);
	DDX_Control(pDX, IDC_BUTTON5, m_b5);
	DDX_Control(pDX, IDC_TSTR2, m_tstr2);
	DDX_Control(pDX, IDC_BUTTON11, m_11);
	DDX_Control(pDX, IDC_BUTTON10, m_10);
	DDX_Control(pDX, IDC_TSTR, m_tstr);
	DDX_Control(pDX, IDC_BUTTON9, m_9);
	DDX_Control(pDX, IDC_BUTTON8, m_8);
	DDX_Control(pDX, IDC_BUTTON7, m_7);
	DDX_Control(pDX, IDC_BUTTON6, m_6);
	DDX_Control(pDX, IDC_BUTTON4, m_4);
	DDX_Control(pDX, IDC_BUTTON3, m_3);
	DDX_Control(pDX, IDC_BUTTON2, m_2);
	DDX_Control(pDX, IDC_TREELABEL, m_treelab);
	DDX_Control(pDX, IDOK, m_ok);
    DDX_Control(pDX, IDC_EDIT1, m_prog);
	DDX_Control(pDX, IDC_CHECK10, m_check10);
	DDX_Control(pDX, IDC_CHECK1, m_check1);
	DDX_Control(pDX, IDC_CHECK2, m_check2);
	DDX_Control(pDX, IDC_CHECK3, m_check3);
	DDX_Control(pDX, IDC_CHECK4, m_check4);
	DDX_Control(pDX, IDC_CHECK5, m_check5);
	DDX_Control(pDX, IDC_CHECK17, m_check17);
	DDX_Control(pDX, IDC_CHECK18, m_check18);
	DDX_Control(pDX, IDC_BUTTON41, m_b41);
	DDX_Control(pDX, IDC_BUTTON42, m_b42);

	//DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	DDX_Check(pDX,   IDC_CHECK10, m_listen);
	DDX_Check(pDX,   IDC_CHECK5, m_ignore);
	
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(YellowDlg, CDialog)
    //{{AFX_MSG_MAP(YellowDlg)
    ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
    ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
    ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
    ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
    ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
    ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
    ON_NOTIFY(NM_RCLICK, IDC_TREE1, OnRclickTree1)
    ON_WM_MOUSEMOVE()
    ON_COMMAND(ID_OPERATIONS_COPY, OnOperationsCopy)
    ON_COMMAND(ID_OPERATIONS_EDIT, OnOperationsEdit)
    ON_COMMAND(ID_OPERATIONS_PASTE, OnOperationsPaste)
    ON_COMMAND(ID_OPERATIONS_SHOW, OnOperationsShow)
    ON_COMMAND(ID_OPERATIONS_DELETE, OnOperationsDelete)
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_COMMAND(ID_OPERATIONS_NEW, OnOperationsNew)
    ON_COMMAND(ID_OPERATIONS_NEWNODE, OnOperationsNewnode)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_COMMAND(ID_TASKBARMENU_ABOUT, OnTaskbarmenuAbout)
	ON_COMMAND(ID_TASKBARMENU_TOGGLESTICKY, OnTaskbarmenuTogglesticky)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_COMMAND(ID_TASKBARMENU_HIDESELECTEDSTICKIES, OnTaskbarmenuHideselectedstickies)
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(WM_TI_TASKBARCREATED, OnTaskBarCreated)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnClickTree1)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE1, OnSelchangingTree1)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE1, OnItemexpandedTree1)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnButton11)
	ON_COMMAND(ID_RIGHT_SHOW, OnRightShow)
	ON_COMMAND(ID_RIGHT_SEARCH, OnRightSearch)
	ON_COMMAND(ID_RIGHT_HIDE, OnRightHide)
	ON_COMMAND(ID_RIGHT_CREATENEWMENO, OnRightCreatenewmeno)
	ON_COMMAND(ID_RIGHT_BROADCASTMEMO, OnRightBroadcast)
	ON_COMMAND(ID_RIGHT_DELETE, OnRightDelete)
	ON_COMMAND(ID_RIGHT2_SELECTALL, OnRight2Selectall)
	ON_COMMAND(ID_RIGHT2_BROADCASTALL, OnRight2BroadcastAll)
	ON_COMMAND(ID_RIGHT_SHOWSELECTED, OnRightSelectall)
	ON_COMMAND(ID_RIGHT2_HIDEALLSELECTEDMEMOS, OnRight2Hideallselectedmemos)
	ON_COMMAND(ID_RIGHT_EDITMEMO, OnRightEditmemo)
	ON_COMMAND(ID_RIGHT_SELECTMEMO, OnRightSelectmemo)
	ON_BN_CLICKED(IDC_BUTTON17, OnButton17)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_COMMAND(ID_RIGHT2_REFRESHALL, OnRight2Refreshall)
	ON_COMMAND(ID_RIGHT2_CREATENEWMEMO, OnRight2Createnewmemo)
	ON_WM_SYSKEYDOWN()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_RIGHT_REBUILDMEMOTREE, OnRightRebuildmemotree)
	ON_COMMAND(ID_RIGHT_GATHERALLSTICKIESINTHECENTER, OnRightGather)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	ON_COMMAND(ID_RIGHT2_DESELECTALL, OnRight2Deselectall)
	ON_COMMAND(ID_RIGHT_CLEARSELECTONSUBITEMS, OnRightClearselectonsubitems)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_BUTTON34, OnButton34)
	ON_BN_CLICKED(IDOK2, OnOk2)
	ON_BN_CLICKED(IDC_BUTTON35, OnButton35)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE1, OnKeydownTree1)
	ON_BN_CLICKED(IDC_CHECK10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK5, OnCheck5)
	ON_BN_CLICKED(IDOK3, OnOk3)
	ON_BN_CLICKED(IDC_TELL, OnTell)
	ON_BN_CLICKED(IDC_TSTR2, OnTstr2)
	ON_BN_CLICKED(IDC_TSTR, OnTstr)
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_COMMAND(ID_RIGHT_UNSELECTALL, OnRight2Deselectall)	
	ON_REGISTERED_MESSAGE(WM_TI_NOTIFYICON, OnNotifyIcon)
	ON_COMMAND(IDR_TASKBAR_MENU_EXIT, OnCancel)
	ON_COMMAND(IDR_TASKBAR_MENU_SHOW, OnShowMain)
	//}}AFX_MSG_MAP
	//ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//ON_STN_CLICKED(IDC_TSTR2, &YellowDlg::OnStnClickedTstr2)
	//ON_COMMAND(ID_RIGHT2_SEARCHFORSTICKY, &YellowDlg::OnRight2Searchforsticky)
	ON_BN_CLICKED(IDC_BUTTON40, &YellowDlg::OnBnClickedButton40)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON41, &YellowDlg::OnBnClickedButton41)
	ON_BN_CLICKED(IDC_BUTTON42, &YellowDlg::OnBnClickedButton42)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnShowMain()

{
	//P2N(_T("Show window called\r\n"));

	// CWnd
	ShowWindow(SW_SHOW);	ShowWindow(SW_RESTORE);	
	BringWindowToTop();

	//::SetForegroundWindow(m_hWnd);

	Invalidate();
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnCancel()
{
	//P2N(_T("Cancel\r\n"));
	CDialog::OnCancel();
}

//////////////////////////////////////////////////////////////////////////

LRESULT YellowDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	m_pTrayIcon->OnNotifyIcon(wParam, lParam);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//  Get memo Item from name, create if needed

HTREEITEM YellowDlg::GetMemoItem(const TCHAR *fname)

{
	HTREEITEM hout = NULL, h1 = NULL;
	int ret = 0, changed = false, created = false;
	DWORD itemd = 0; 

	//P2N(_T("YellowDlg::GetMemoItem %s\r\n"), fname);

    CFile cf;

    CxArchive ar; //( &cf, CArchive::load);
	
	if(!ar.Open(fname, CFile::modeRead | CFile::shareDenyNone))
		{		
		P2N(_T("YellowDlg::GetMemoItem - no file '%s'\r\n"), fname);
		return NULL;
		}

	CString buff(_T(""));
	ar.ReadString(buff);

	//P2N(_T("Read String %d bytes '%s'\r\n"), buff.GetLength(), (const char*)buff);

	if(buff == _T(""))
		{
		datalog.PrintToLog(_T("Invalid (empty) sticky '%s'\r\n"), fname);	
		//P2N(_T("YellowDlg::GetMemoItem Empty string on %s\r\n"), fname);
		return NULL;
		}

	// Invalid memo, log it
	if(buff[0] != '\\')  
		{
		datalog.PrintToLog(_T("Invalid or damaged sticky %s\r\n"), fname);	

		P2N(_T("YellowDlg::GetMemoItem damaged sticky '%s'\r\n"), fname);
		//P2N(_T("YellowDlg::GetMemoItem invalid line '%s'\r\n"), buff);

		return NULL;
		}

	// Create copy to operate on
	CString buff2(buff);

	//P2N(_T("File '%s' Initial Line:'%s'\r\n"), name, buff);

	while(true)
		{
		int cc = buff2.Find('\\');
		//P2N(_T("found in '%s' %d \r\n"), buff, cc);
		if(cc == -1)
			{
			//P2N(_T("fcut at '%s'\r\n"), buff);
			break;
			}
		if(cc)
			{
			CString curr = buff2.Mid(0, cc);

			//P2N(_T("cut at %d '%s'\r\n"), cc, curr);

			// See if that sub branch exists already:
			HTREEITEM h2 = m_tree.GetChildItem(h1);
			while(1)
				{
				if(!h2) break;

				changed = false;					
				if(m_tree.GetItemText(h2) == curr)
					{
					//hout = 
					h1 = h2;
					break;
					}
				h2 = m_tree.GetNextItem(h2, TVGN_NEXT);
				}

			if(!h2)
				{
				if(buff.Mid(0, 9) !=_T("\\Deleted\\") || 
						((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck()
					)
					{
					//None there, create it
					h1 = m_tree.InsertItem(curr, h1);
					
					//P2N(_T("New Item inserted: %d '%s'\r\n"), h1, curr);

					created = true;
					hout = h1;
					// Check it (if needed)
					if(buff.Mid(0, 9) !=_T("\\Deleted\\"))
						{
						m_tree.SetCheck(h1, true);
						}
					}
				}
			}
		buff2 = buff2.Right(buff2.GetLength() - cc - 1);
		}
	
	if(created)
		return hout;
	else
		return h1;
}

/////////////////////////////////////////////////////////////////////////////

void YellowDlg::DelMemo()

{
	CString str, str2, str3, str4;
	
	HTREEITEM h1 = m_tree.GetSelectedItem();

    // Anything selected ?
    if(!h1)
       {
       AfxMessageBox(_T("Please select a Sticky to delete first."));
       return;
       }

    // Any Children ?
    HTREEITEM h2 = m_tree.GetNextItem(h1, TVGN_CHILD);
    if(h2)
       {
       AfxMessageBox(_T("This Sticky has leaf (child) items, cannot delete."));
       return;		
       }
	
	str3 = m_tree.GetItemText(h1);

	BuildTree(str4, h1);

	if(str4 ==_T("\\Deleted\\"))
		{
		AfxMessageBox(_T("Cannot delete the 'Deleted' folder"));
		return;
		}

	if(str4.Mid(0, 9) ==_T("L\\Deleted\\"))
		str2.Format(_T("About to PERMANENTLY delete the Sticky titled:  '%s'      \r\n")
							 _T("       Are you sure?"), str3);
	else
		str2.Format(_T("About to delete the Sticky titled:  '%s'      \r\n")
							 _T("       Are you sure?"), str3);

	if(AfxMessageBox(str2, MB_YESNO) == IDNO)
		return;
		
	DelMemobyHandle(h1);
}

/////////////////////////////////////////////////////////////////////////////
// Delete item
// void YellowDlg::OnButton3()

void YellowDlg::OnButton3()

{
	DelMemo();
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnButton4()

void YellowDlg::OnButton4()

{
    CreateBranch(TRUE);
}

/////////////////////////////////////////////////////////////////////////////

void YellowDlg::CreateBranch(int doleaf, const TCHAR *body)

{
 	//CEditDlg gt(this); gt.m_BackColor = dlgback;
	ped = new CEditDlg(this); ped->m_BackColor = dlgback;

	HTREEITEM   h1, h1p, h2;

    h1 = m_tree.GetSelectedItem(); 
	h1p = m_tree.GetNextItem(h1, TVGN_PARENT);

	int ret = 0;

	CString name = AfxGetApp()->GetProfileString(_T("License"),_T("Name"),_T("DEMO"));

	if(name ==_T("DEMO") && daysused > DEMO_DAYS)
		{
		ret = AfxMessageBox(
		_T("New Sticky Creation is disabled after the CryptoSticky evaluation period.   \r\n")
		_T("To activate all CryptoSticky's features, Please purchase a license, \r\n")
		_T("which is available at our web site.\r\n")
		_T("\r\n")
		_T("Would you like to go to our web site [www.RobotMonkeySoftware.com] now?\r\n"),  MB_YESNO);
		
		if(ret == IDYES)
			{
			extern CString myURLstr;
			ShellExecute(NULL,_T("open"), buyURLstr, NULL, NULL, SW_SHOWMAXIMIZED);	
			return;
			}
		return;
		}

	if(_taccess(dataroot, 02) < 0)
		{
		AfxMessageBox(_T("Cannot create new Sticky. (No write access to the Memo Data Directory)     "),  
			MB_OK | MB_ICONERROR);
		return;
		}

	BuildTree(ped->memoroot);

	// Take exception to root parent (parent of root is self)
	if(h1p)
		BuildTree(ped->memorootp, h1p);
	else
		ped->memorootp =_T("\\");
	
    ped->caption =_T("Create Sticky");

	CTime ct = CTime::GetCurrentTime();

	ped->m_head = ct.Format(_T("Untitled at %Y/%m/%d %H:%M:%S"));

	if(body)
		{
		ped->str = body;
		}    

	//P2N(_T("gt.memoroot=%d\r\n"), gt.memoroot);

    if(ped->DoModal() == IDCANCEL)
		{
		delete ped; ped = NULL;
		return;
		}
 	
	if(ped->m_folder)
		{
		h2 = m_tree.InsertItem(ped->m_head, h1);
		}
	else
		{
		h2 = m_tree.InsertItem(ped->m_head, h1p);
		}

	delete ped; ped = NULL;
		
	// Select item, display it:
    m_tree.Select(h2, TVGN_CARET );
	m_tree.SetCheck(h2);
	WrIterate_Checks(NULL );

	CYmemo *st = Handle2Memo(h2);
	if(st)
		st->AutoSize();

	ShowOne(h2);

	SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	ShowStatus(_T("Created new Sticky"));
}

/////////////////////////////////////////////////////////////////////////////
// Paste a new memo

void YellowDlg::OnButton5()

{
    int ucount = 0;

	CString text, head;

	OpenClipboard();

	HANDLE htext = GetClipboardData(CF_UNICODETEXT);

	if (htext)
		{
		LPVOID ptr = GlobalLock(htext);
		//ASSERT(ptr);
		text = (const TCHAR *)ptr;
		GlobalUnlock(htext);
		}

	CloseClipboard();

	if(text == _T(""))
		{
		AfxMessageBox(_T("No text on clipboard.\r\n"));
		return;
		}
	
	//P2N(_T("Got clipboard text=%s\r\n"), text);
	
	CreateBranch(false, text);
}

/////////////////////////////////////////////////////////////////////////////
// Copy from holding tree to clipboard

void YellowDlg::OnButton6()

{
    int ucount = 0;
    HTREEITEM h1;

    if(!(h1 = m_tree.GetSelectedItem() ))
		{
		AfxMessageBox(_T("Please select item to copy first."));
		return;
		}

	CString fname; GetNodePath(fname, h1);

	MemoSupp	memo;

	//if(!memosup.ReadMemoFile(fname, initial, &flags, head, body))
	if(!memo.Read(fname))
		{
		AfxMessageBox(_T("Cannot copy to clipboard, missing Sticky data file."));
		return;
		}

	//P2N(_T("body '%s'\r\n"), memo.body);

	// Put it to clipboard
	int bigmemlen = memo.body.GetLength() * sizeof(TCHAR) + sizeof(TCHAR);

	// Alloc it, Assemble it
	HANDLE	bighandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, bigmemlen + 1);
    if(!bighandle)
		{
		AfxMessageBox(_T("Cannot allocate memory for clipboard text selection"));
		return;
		}

	TCHAR *bigptr = (TCHAR *)GlobalLock(bighandle);
	
	if(!bigptr)
		{
		AfxMessageBox(_T("Cannot lock memory for text selection"));
		return;
		}
	
	memcpy(bigptr, memo.body, bigmemlen);
	bigptr[bigmemlen] = 0;

	GlobalUnlock(bigptr);

	if (::OpenClipboard(AfxGetApp()->m_pMainWnd->GetSafeHwnd()))
        {
        //if(::EmptyClipboard())
            {
			EmptyClipboard();
			//::SetClipboardData(CF_TEXT, bighandle);
			::SetClipboardData(CF_UNICODETEXT, bighandle);
			}
		CloseClipboard();

		CString tmp; tmp.Format(_T("Copied '%s' to clipboard"), memo.head);
		ShowStatus(tmp);
		} 
}

/////////////////////////////////////////////////////////////////////////////
// Build tree

void YellowDlg::BuildTree(CString &str, HTREEITEM  h1)

{
	//P2N(_T("Building tree from %s\r\n"), m_tree.GetItemText(h1) );

	HTREEITEM org = h1;
	// Init to root
	str = _T("");

    CStringList list;
    
    if(!h1)
		h1 = m_tree.GetSelectedItem();

	// Assume root if nothing selected
	if(!h1)
		{
		str =_T("\\"); return;
		}
	
	// Create a list of parents
	list.AddTail( m_tree.GetItemText(h1));

    while((h1 = m_tree.GetNextItem(h1, TVGN_PARENT)) )
       list.AddTail( m_tree.GetItemText(h1) );

	// Assemble the list backwards
    POSITION pos;
    pos = list.GetTailPosition();
    while(1)
       {
	   str +=_T("\\"); 

	   if(!pos)
         break;

	   str += list.GetPrev(pos);
       }

	//if(str == _T(""))
	//	str =_T("\\");

	//P2N(_T("Building tree from '%s' to '%s'\r\n"), m_tree.GetItemText(org), str);
}

/////////////////////////////////////////////////////////////////////////////
// Edit YellowDlg item:

void YellowDlg::OnButton7()

{
	EditCurrent();
}

/////////////////////////////////////////////////////////////////////////////
//

void YellowDlg::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	CPoint mouse, hittest;	GetCursorPos(&mouse);
	hittest = mouse;

	m_tree.ScreenToClient(&hittest);

	unsigned int flags = 0;
	HTREEITEM h1 = m_tree.HitTest(hittest, &flags);
	if(h1)
		{
		//P2N(_T("Double click on %x\r\n"), h1);
		m_tree.Select(h1, TVGN_CARET);

		// Call Edit subroutine
		EditCurrent();
		}
    *pResult = 1;
}

/////////////////////////////////////////////////////////////////////////////
// Get node file name


void YellowDlg::GetNodePath(CString &str1, HTREEITEM  h1)

{
   // Build YellowDlg tree
	MemoSupp memo; BuildTree(memo.initial, h1);
    
	str1 = memo.MakeFileName();

 	//P2N(_T("NodeFileName=%s nodestring=%s\r\n"), str1, memo.initial);
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::ShowCode()

void YellowDlg::ShowCode()

{
    //ViewText vt;
    CString fname; GetNodePath(fname);

    //P2N(_T("Clicking tree at %s\r\n"), fname);

    // Read YellowDlg file
    CFile cf;
    if(cf.Open(fname, CFile::modeRead))
        {
        CString line;
        CxArchive ar; //( &cf, CArchive::load);

        // Read initial line
		ar.ReadString(line);
		
		//P2N(_T("initial line: %s"), line + "\r\n");

        while(ar.ReadString(line))
            {
            //vt.m_str += line +_T("\r\n");
			//P2N(_T("line: %s"), line +_T("\r\n"));
            }
        }
	else
		{
		P2N(_T("Cannot open file to read\r\n"));
		}
    //vt.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// Load menu

void YellowDlg::OnRclickTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	unsigned int flags = 0;

	CPoint mouse, hittest;	GetCursorPos(&mouse);
	hittest = mouse; m_tree.ScreenToClient(&hittest);

	int menu = IDR_MENU1;
	HTREEITEM h1 = m_tree.HitTest(hittest, &flags);
	if(h1)
		{
		// Select clicked on (current) item
		m_tree.Select(h1, TVGN_CARET);
		}
	else
		{
		menu = IDR_MENU2;
		}

	CXrayM *mm = new CXrayM(IDD_MENUBASE);

	mm->LoadMenu(menu, 1);

	// Add folder list to menu
	//MenuAddFolderList(mm);

	// Add network list to menu
	//MenuAddNetList(mm);	
	
	POINT scr(mouse); mm->Show(scr.x, scr.y);
	 
    *pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnMouseMove(UINT nFlags, CPoint point)

void YellowDlg::OnMouseMove(UINT nFlags, CPoint point)

{
    CDialog::OnMouseMove(nFlags, point);
	//P2N(_T("moved x=%d y=%d\r\n"), point.x, point.y);
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnOperationsCopy()

void YellowDlg::OnOperationsCopy()

{
    OnButton5();
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnOperationsEdit()

void YellowDlg::OnOperationsEdit()

{
    EditCurrent();
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnOperationsPaste()

void YellowDlg::OnOperationsPaste()

{
    OnButton6();
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnOperationsShow()

void YellowDlg::OnOperationsShow()

{
    OnButton8();
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnOperationsDelete()

void YellowDlg::OnOperationsDelete()

{
    OnButton3();
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)

void YellowDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    //P2N(_T("OnChar %c\r\n"), nChar);
    CDialog::OnChar(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)

void YellowDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)

{
	P2N(_T("YellowDlg::OnKeyDown %d TCHAR"), nChar);

    CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnOperationsNew()

void YellowDlg::OnOperationsNew()
{
    OnButton4();
}

/////////////////////////////////////////////////////////////////////////////
// void YellowDlg::OnOperationsNewnode()

void YellowDlg::OnOperationsNewnode()
{
	
	P2N(_T("YellowDlg::OnOperationsNewnode()\r\n"));
   // OnButton1();
}

//////////////////////////////////////////////////////////////////////////

BOOL YellowDlg::OnInitDialog()

{
	//P2N(_T("In init dialog\r\n"));
	CDialog::OnInitDialog();

	SetTimer(9, 20, NULL);

	GetSystemPowerStatus(&inisps);

	//CString  port(_T("port"));
	//memo_port = AfxGetApp()->GetProfileInt(port,_T("memo_port"), YM_MEMO_PORT);
	
	if (bAlpha < 255)
		{
        //  Set layered style for the dialog
        //SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

        //  Call it with 255 as alpha - opacity
		//if(g_pSetLayeredWindowAttributes)
		//	g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
		}

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_tree.DeleteAllItems();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	MouseMsgHandlerPtr *aHandler = new MouseMsgHandlerPtr [3];

	rBtn = new CRightMouseClickMsgHandler(WM_RBUTTONDOWN);
	lBtn = new CLeftMouseDblClickMsgHandler();
	lsBtn = new CLeftMouseClickMsgHandler(WM_LBUTTONDOWN);

	aHandler[0] = lBtn;
	aHandler[1] = rBtn;
	aHandler[2] = lsBtn;

	m_pTrayIcon = new CTrayIcon(m_hWnd, IDR_MAINFRAME, m_hIcon,_T("CryptoSticky"), aHandler, 3);
	m_pTrayIcon->AddIcon();

	ArmNext();

	SetTimer(1, 1000, NULL);

	fakepar = new FakeWin();
	fakepar->cocodlg = (void*) this;
	
	//	fakepar->Create(IDD_DIALOG2, this); 
	fakepar->Create(IDD_DIALOG2, GetDesktopWindow());

	fakepar->SetIcon(AfxGetApp()->LoadIcon(IDI_ICON1), true);
	
	//fakepar->ModifyStyleEx(WS_EX_APPWINDOW, 0);

	// Get hide entry to/from the registry
	CString section(_T("Setup"));
	
	flag12 = AfxGetApp()->GetProfileInt(section,_T("flag12"), 0);

	m_hide		= AfxGetApp()->GetProfileInt(section,_T("Hide"), 0);
	m_auto		= AfxGetApp()->GetProfileInt(section,_T("Auto"), 0);
	m_restart	= AfxGetApp()->GetProfileInt(section,_T("Restart"), 0);
	m_listen	= AfxGetApp()->GetProfileInt(section,_T("Listen"), 1);
	m_ignore	= AfxGetApp()->GetProfileInt(section,_T("Ignore"), 1);
	
	UpdateData(false);

	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(m_restart);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(m_auto);
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(m_hide);

	// Get color sceme from the registry
	section =_T("Colors");
	confcol.memocolor 	= AfxGetApp()->GetProfileInt(section,_T("memocolor"),  RGB(0xff, 0xff, 0x80));
	confcol.memorcolor 	= AfxGetApp()->GetProfileInt(section,_T("memorcolor"), RGB(0xff, 0xa0, 0x80));
	confcol.memohead 	= AfxGetApp()->GetProfileInt(section,_T("memohead"),   RGB(0xf0, 0xf0, 0x00));
	confcol.memorhead	= AfxGetApp()->GetProfileInt(section,_T("memorhead"),  RGB(0xf0, 0xb0, 0x00));

	confcol.alarmcolor = AfxGetApp()->GetProfileInt(section,_T("alarmcolor"),		RGB(0xf0, 0xf0, 0xc0));
	confcol.alarmhcolor = AfxGetApp()->GetProfileInt(section,_T("alarmhcolor"),	RGB(0xc0, 0xc0, 0xa0));
	confcol.alarmbutton = AfxGetApp()->GetProfileInt(section,_T("alarmbutton"),	RGB(255,255,0));
	confcol.alarmhbutton = AfxGetApp()->GetProfileInt(section,_T("alarmhbutton"),  RGB(255,255,100));

	ShowStatus(_T("CryptoSticky Ready"), 4000);

	// Restore old placement
	WINDOWPLACEMENT wp2; GetWindowPlacement(&wp2);

	CString section2(_T("Dynamic"));
	CString str = AfxGetApp()->GetProfileString(section2,_T("MainWinPos"), _T(""));

	if(str != _T(""))
		{
		//_stscanf_s(str,_T("%d %d %d %d"),
		//	&wp2.rcNormalPosition.top, &wp2.rcNormalPosition.left,
		//		&wp2.rcNormalPosition.bottom,  &wp2.rcNormalPosition.right);
		_stscanf(str,_T("%d %d %d %d"), 
			&wp2.rcNormalPosition.top, &wp2.rcNormalPosition.left,
				&wp2.rcNormalPosition.bottom,  &wp2.rcNormalPosition.right);
		}

	SetWindowPos(NULL, wp2.rcNormalPosition.left, wp2.rcNormalPosition.top, 0, 0, SWP_NOSIZE  | SWP_NOZORDER );

	Decorate();
	
	LOGFONT lf;	CFont *ff = m_tstr2.GetFont();
	//P2N("font %s\r\n", ff);
	if (!ff)
		::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(lf), &lf);
	else		
		ff->GetLogFont(&lf);

	//P2N("font ww=%d hh=%d %s\r\n", lf.lfWidth, lf.lfHeight, lf.lfFaceName);	
	long dbu = GetDialogBaseUnits();
	//P2N("GetDialogBaseUnits %x hh=%d ww=%d \r\n", dbu, dbu >> 16, dbu & 0xffff);

	m_tstr.SetText(_T(""));	m_tstr.SetFontBold(true);
	m_tstr.SetTrans();	m_tstr.CenterText(); m_tstr.SetFontSize(12);

	int ss = 88; //ss *= dbu >> 16; ss /= abs(lf.lfHeight);
	m_tstr2.SetText(_T("")); m_tstr2.SetFontBold(true);
	m_tstr2.SetTrans();	m_tstr2.CenterText(); m_tstr2.SetFontSize(ss);
	
	//////////////////////////////////////////////////////////////////////
	// CWnd CDialog
	
	CString name = AfxGetApp()->GetProfileString(_T("License"),_T("Name"),_T("DEMO"));
	CString tmp; 
	
	if(name ==_T("DEMO"))
		tmp.Format(_T("CryptoSticky Pro (Evaluation - Day %d)"), daysused + 1);
	else
		{
		int check;
		CString key  = AfxGetApp()->GetProfileString(_T("License"),_T("Key"), _T(""));
		CString all;  all += name; all += key;
		int	res = CheckLic(all, &check);

		if(res && ((check % LIC_MODULUS)  == 0) )
			{
			tmp.Format(_T("CryptoSticky Pro - Licensed to %s "), name);		
			}
		else
			{
			tmp.Format(_T("CryptoSticky Pro - Invalid License"));		
			}
		}

	SetWindowText(tmp);

#ifdef SPEAK
	sp.PostHere(m_hWnd);
#endif

	SetTimer(0, 600, NULL);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////

void YellowDlg::OnButton2()

{
	HideMe();
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::HideMe()

{
	// Write hide entry to the registry
	CString section(_T("Setup"));
	AfxGetApp()->WriteProfileInt(section,_T("Hide"), m_hide);

	//P2N(_T("Hide to tray reg_hide = %d\r\n"), m_hide);

	ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////

void YellowDlg::OnTaskbarmenuAbout()
{
	//CAboutDlg dlgAbout;
	//dlgAbout.DoModal();

	CreateBranch(TRUE);
}


void YellowDlg::OnTaskbarmenuTogglesticky()

{
	OnButton8();
}

void YellowDlg::OnButton8()

{
	WrIterate_Checks(NULL);

	//P2N(_T("YellowDlg::OnButton8()\r\n"));
	// Make sure we are still top of the chain
	//SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//SetFocus();
	
	Iterate_Show(NULL);

	// See if we have a stale window for a while

	toggle = true;
}

void YellowDlg::OnButton9()

{
	if(misc.IsShiftDown())
		{
		//P2N(_T("Shift deactivate \r\n"));
		Iterate_Uncheck(NULL);
		}
	else
		{
		WrIterate_Checks(NULL);

		//st.Hide();
		POSITION pos = ylist.GetHeadPosition();
		while(true)
			{
			if(!pos)
				break;

			CYmemo *st = (CYmemo *) ylist.GetNext(pos);
			st->Hide();
			//delete st;
			}
		//list.RemoveAll();
		toggle = false;
		}
}

void YellowDlg::OnTaskbarmenuHideselectedstickies()

{
	OnButton9();
}

void YellowDlg::ShowMemos()

{
	OnButton8();
}

void YellowDlg::HideMemos()

{
	OnButton9();
}


LRESULT YellowDlg::OnTaskBarCreated(WPARAM wParam, LPARAM lParam)

{
    return (true);
	//return m_pTrayIcon->OnTaskBarCreated(wParam, lParam);	
}

//////////////////////////////////////////////////////////////////////////
//

CYmemo* YellowDlg::Handle2Memo(HTREEITEM h1)

{
	if(!h1) return NULL;

	CYmemo *ret = NULL;
	CString fname; GetNodePath(fname, h1);

	// Find the one with the same file name:
	POSITION pos = ylist.GetHeadPosition();
	while(true)
		{
		if(!pos)
			break;

		CYmemo *st = (CYmemo *) ylist.GetNext(pos);

		//P2N(_T("scan => '%s'\r\n"), st->memo.head);

		if(st->memofile == fname)
			{
			ret = st;
			break;	
			}
		}

	// Not shown, create one
	if(ret == NULL)
		{
		ret = CreateMemoWin(h1);

		if(ret)
			ret->LoadContent(true);
		else
			P2N(_T("could not create memo window\r\n"));
		}

	//P2N(_T("YellowDlg::Handle2Memo %d %p\r\n"), h1, ret);

	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Find if we have an entry like this already

int	 YellowDlg::IsDuplicate(const TCHAR *str)

{
	//P2N(_T("YellowDlg::IsDuplicate %s\r\n"), str);

	found = 0;
	Iterate_Exec(NULL, (int)str, IteratorDup);

	return found;
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::ShowOne(HTREEITEM h1, int show, int ala, int tmptop)

{
	CString str;   BuildTree(str, h1);
	CString fname; GetNodePath(fname, h1);

	CString title = m_tree.GetItemText(h1);

	SetForegroundWindow();

	//P2N(_T("ShowOne show=%d  ala=%d  tmptop=%d  %s\r\n"), show, ala, tmptop, str); 
	
	// Never Show deleted root:
	if(str ==_T("\\Deleted\\"))
		{
		//P2N(_T("Not Showing '\\Deleted\\'\r\n"));
		return;
		}

	// Find the one with the same file name:
	POSITION pos = ylist.GetHeadPosition();
	while(true)
		{
		if(!pos)
			break;

		CYmemo *st = (CYmemo *) ylist.GetNext(pos);

		//P2N(_T("ShowOne ylist iterate  %d '%s'\r\n"), show, st->memo.head);

		if(st->memofile == fname)
			{
			if(show)
				{
				//P2N(_T("Showing %s\r\n"), fname);

				st->tmptop = tmptop;
				st->alshow = ala;
				//st->timer = 7;
				st->SetTransparency(main_trans);
				st->Show();	
				st->timeout = (int)memo_timeout;
			
				st->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
				
				if(show & 0x80)
					st->SetFocus();
				}
			else
				{
				st->Hide();
				}			
			return;
			}
		}

	// Stop here: we are hiding memos
	if(!show)
		return;

	CYmemo *st = CreateMemoWin(h1);
	if(st)
		st->Show();

	// GetDesktopWindow()
	//SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//SetActiveWindow();

	support.YieldToWinEx();
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnTimer(UINT nIDEvent)

{
	//P2N(_T("Timer tick id=%d\r\n"), nIDEvent);

	//////////////////////////////////////////////////////////////////////
	// Initial timer

	if(nIDEvent == 0)
		{
		KillTimer(nIDEvent);

		ReadAllMemos(false, false, false);
		Iterate_Checks(NULL );

		support.YieldToWinEx();

		if(m_auto)
			Iterate_Show(NULL);

		load_complete = true;

		support.YieldToWinEx();

		if(m_hide)
			{
			ShowWindow(SW_HIDE);
			}
		else
			{
			SetFocus();
			}

		StopScreenSaver();

		// Initiate license check timer
		SetTimer(4, 300, NULL);

		support.YieldToWinEx();
		stable = true;
		}

	//////////////////////////////////////////////////////////////////////
	// Timer event, calculate alarm, check refresh folder
	
	if(nIDEvent == 1)
		{
		if(load_complete)
			{
			if(folder_refresh)
				{
				DWORD dwCount = GetTickCount();

	#ifdef _DEBUG
			if(dwCount  - LastFolderTick > (DWORD)folder_refresh * 1000 * 4)
	#else		
			if(dwCount  - LastFolderTick > (DWORD)folder_refresh * (1000 * 60) )
	#endif			
					{
					LastFolderTick = dwCount;
					}
				}
			}
		time_t tme = time(NULL);

		// Display running time/date on dialog
		time_t tt= time(NULL);
		CTime ctme(tt);

		CString tmp2; tmp2 = ctme.Format(_T("%a, %b %d (%m/%d/%Y) (Week %W)"));
		// Changed?
		if(old_tmp2 != tmp2)
			{
			m_tstr.SetText(tmp2); old_tmp2 = tmp2;
			}

		// Changed?
		CString tmp3; 
		
		if(lastactivated)
			{
			if(flag12)
				tmp3 = ctme.Format(_T("%I:%M:%S %p"));	
			else
				tmp3 = ctme.Format(_T("%H:%M:%S"));
			}
		else
			{
			if(flag12)
				tmp3 = ctme.Format(_T("%I:%M %p"));	
			else
				tmp3 = ctme.Format(_T("%H:%M"));
			}

		if(old_tmp3 != tmp3)
			{
			m_tstr2.SetText(tmp3); old_tmp3 = tmp3;
			}

		// Is it time for popping alarm?
		if(tme > alarm_tt)
			{
			if(!al_done)
				{
				al_done = true;

				// Pop up all that we have
				for(int loop = 0; loop < num_alarms; loop++)
					{
					CAlarm *al = new CAlarm(this); 
					al->m_BackColor = dlgback;
					alist.AddHead(al);

					//al->m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);
					al->fname = alname[loop];
					
					//memosup.ReadTitle(al->fname, al->title);

					//P2N(_T("Popping alarm alpha=%d\r\n"), ala_trans);

					al->alarmoffset = 10 * loop;
					al->SetAlpha(ala_trans);
					al->Show();
					al->SetForegroundWindow();

					al->Invalidate();
					support.YieldToWinEx();
					}

				num_alarms = 0;
				alarm_tt = 0;

				// Re-arm with pending alarms
				ArmNext();
				}
			}

		///////////////////////////////////////////////////////////
		/// Remove stale alarm dialogs

		POSITION oldpos2, pos2 = alist.GetHeadPosition();
		while(true)
			{
			if(!pos2)
				break;

			oldpos2 = pos2;
			CAlarm *al = (CAlarm *) alist.GetNext(pos2);
			//ASSERT(al);
						
			if(al->final)
				{
				//P2N(_T("Removed alarm dialog from %s\r\n"), al->title);
				alist.RemoveAt(oldpos2);
				delete al;
				}
			}
		}

	//////////////////////////////////////////////////////////////////////
	// Selection mouse click
		
	if(nIDEvent == 2)
		{
		KillTimer(nIDEvent);

		if(g_h1)
			{
			//P2N(_T("Clicked/keyed on item '%s' checked=%d\r\n"), m_tree.GetItemText(g_h1), m_tree.GetCheck(g_h1) );

			CString fname; GetNodePath(fname, g_h1);
			CString title = m_tree.GetItemText(g_h1);

			// Read YellowDlg registry
			CString line = AfxGetApp()->GetProfileString(section, fname, _T(""));
			int checked = m_tree.GetCheck(g_h1);

			if(checked != _tstoi(line))
				{
				//P2N(_T("Changed check value on %s\r\n"), title);
				CString tmp; tmp.Format(_T("%d"), checked);
				AfxGetApp()->WriteProfileString(section, fname, tmp);
				ShowOne(g_h1, checked);
				}
			//ShowOne(g_h1, checked);
			}
		SetFocus();
		return;
		}

	//////////////////////////////////////////////////////////////////////
	else if(nIDEvent == 4)
		{
		KillTimer(nIDEvent);

		CString name = AfxGetApp()->GetProfileString(_T("License"),_T("Name"),_T("DEMO"));
		
		if(name ==_T("DEMO"))
			{
			if(daysused > DEMO_DAYS)
				{
				AfxMessageBox(
				_T("CryptoSticky Licensing: Evaluation Period expired, new Sticky creation is disabled.\r\n\r\n")
				_T("To register CryptoSticky, please obtain a License Key from us, and then goto the 'About' dialog\r\n")
				_T("by clicking on the 'About' button, and then click on 'Register'"));
				}
			else if(daysused > DEMO_DAYS - 4)
				{
				AfxMessageBox(_T("CryptoSticky Licensing: Less than three days left for evaluation."));
				}
			else if(daysused > DEMO_DAYS - 7)
				{
				AfxMessageBox(_T("CryptoSticky Licensing: Less than seven days left for evaluation."));
				}
			}
		
		if(remlst.GetCount() > 0)
			{
			CString tmp; 

			if(remlst.GetCount() == 1)
				tmp.Format(_T("There is %d missed alarm"), 
								remlst.GetCount());
			else
				tmp.Format(_T("There are %d missed alarms"), 
								remlst.GetCount());
			
			ShowStatus(tmp, 5000, RGB(255, 0, 0));
				
			datalog.PrintToLog(_T("Reported missed alarms to user\r\n"));
			
			if(!m_ignore)
				{
				tmp +=_T("\r\nTo clear missed alarms list,  goto Configure -> press 'Clear Missed Alarms'");
				tmp +=_T("\r\nOr check 'Ignore Missed Alarms' on the main pane");
				AfxMessageBox(tmp);
				}
			}
		}	

	if(nIDEvent == 6)
		{
		KillTimer(nIDEvent);
		m_status.SetText(_T("Ready"));
		}

	// Refresh one memo
	if(nIDEvent == 7)
		{
		KillTimer(nIDEvent);

		HTREEITEM hh = GetMemoItem(showthis);
		if(hh == NULL)
			{
			// Try original file name
			POSITION pos3 = detlist.GetHeadPosition();
			while(true)
				{
				if(!pos3)break;
				MemoDetail *md = (MemoDetail *) detlist.GetNext(pos3);
				if(*md->fname == showthis)
					{
					P2N(_T("showthis %s\r\n"), showthis);
					//ShowOne(
					}
				}
			}
		if(auto_netpop)
			{
			//memo.Read(changed);
			
			P2N(_T("Auto netpop %s\r\n"), showthis);
			//me->showthis = changed;	
			//me->SetTimer(7, 20, NULL);
			ShowOne(hh, true);
			}
		
		
#if 0
		CYmemo *st = Handle2Memo(hh);
		if(st)
			{
			st->LoadContent(true);
			st->AutoSize();
			}
		ShowOne(hh, true);
#endif

		}

	// Show status
	if(nIDEvent == 8)
		{
		KillTimer(nIDEvent);

		m_status.SetTextColor(stat_color);
		m_status.SetText(stat_str);
		SetTimer(6, stat_tout, NULL);
		}

	// Show banner
	if(nIDEvent == 9)
		{
		KillTimer(nIDEvent);

		//P2N(_T("Showing banner\r\n"));

		CBanner banner(this); 
		banner.m_BackColor = dlgback; banner.DoModal();
		}

	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
//

void YellowDlg::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	UINT Flags;

	//WINDOWPLACEMENT	wp;
	//m_tree.GetWindowPlacement(&wp);

	//cWnd cTreeCtrl
	//mouse.x -= wp.rcNormalPosition.left;
	//mouse.y -= wp.rcNormalPosition.top;

	g_h1 = m_tree.HitTest(mouse, &Flags);

	if(!g_h1)
		{
		//P2N(_T("No hit\r\n"));
		m_tree.SelectItem(NULL);
		return;
		}

	SetTimer(2, 10, NULL);

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnSelchangingTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//P2N(_T("YellowDlg::OnSelchangingTree1\r\n"));


	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnDestroy()

{
	UpdateData();

	shutdown = true;

	//P2N(_T("OnDestroy for YellowDlg\r\n"));

	if(load_complete)
		{
		WrIterate_Checks(NULL);
		}
	else
		{
		//P2N(_T("Incomplete load, not saving\r\n"));
		}


	// Write hide entry to the registry
	CString section(_T("Setup"));
	AfxGetApp()->WriteProfileInt(section,_T("Hide"), m_hide);
	AfxGetApp()->WriteProfileInt(section,_T("Auto"), m_auto);
	AfxGetApp()->WriteProfileInt(section,_T("Restart"), m_restart);
	AfxGetApp()->WriteProfileInt(section,_T("Listen"), m_listen);
	AfxGetApp()->WriteProfileInt(section,_T("Ignore"), m_ignore);

	// Write color entry to the registry
	section =_T("Colors");
	AfxGetApp()->WriteProfileInt(section,_T("memocolor"),  confcol.memocolor);
	AfxGetApp()->WriteProfileInt(section,_T("memorcolor"), confcol.memorcolor);
	AfxGetApp()->WriteProfileInt(section,_T("memohead"),   confcol.memohead);
	AfxGetApp()->WriteProfileInt(section,_T("memorhead"),  confcol.memorhead);

	AfxGetApp()->WriteProfileInt(section,_T("alarmcolor"),   confcol.alarmcolor);
	AfxGetApp()->WriteProfileInt(section,_T("alarmhcolor"),  confcol.alarmhcolor);
	AfxGetApp()->WriteProfileInt(section,_T("alarmbutton"),  confcol.alarmbutton);
	AfxGetApp()->WriteProfileInt(section,_T("alarmhbutton"), confcol.alarmhbutton);

	//P2N(_T("Write tray reg_hide = %d\r\n"), m_hide);

	// Save position
	WINDOWPLACEMENT wp2;
	GetWindowPlacement(&wp2);

	CString tmp; tmp.Format(_T("%d %d %d %d"),
		wp2.rcNormalPosition.top, wp2.rcNormalPosition.left,
				wp2.rcNormalPosition.bottom,  wp2.rcNormalPosition.right);

	CString section2(_T("Dynamic"));
	AfxGetApp()->WriteProfileString(section2,_T("MainWinPos"), tmp);

	// Dump ylist
	//POSITION posd = ylist.GetHeadPosition();
	//while(true)
	//	{
	//	if(!posd)
	//		break;
	//	CYmemo *st = (CYmemo *) ylist.GetNext(posd);
	//	//ASSERT(st); //ASSERT(st->magic == CYmemo_Magic);
	//	//P2N(_T("Ylist items: %s\r\n"), st->memo.head);
	//	}

	/// Remove memo dialogs
	POSITION pos = ylist.GetHeadPosition();
	while(true)
		{
		if(!pos)
			break;

		CYmemo *st = (CYmemo *) ylist.GetNext(pos);
		//ASSERT(st); //ASSERT(st->magic == CYmemo_Magic);

		//st->Hide();

		delete st;
		}
	ylist.RemoveAll();

	///////////////////////////////////////////////////////////
	/// Remove alarm dialogs

	POSITION pos2 = alist.GetHeadPosition();
	while(true)
		{
		if(!pos2)
			break;

		CAlarm *al = (CAlarm *) alist.GetNext(pos2);
		//ASSERT(al);
		delete al;
		}
	alist.RemoveAll();

	// Remove memo details
	POSITION pos3 = detlist.GetHeadPosition();
	while(true)
		{
		if(!pos3)break;
		MemoDetail *md = (MemoDetail *) detlist.GetNext(pos3);
		delete md;
		}
	detlist.RemoveAll();
	
	if(fakepar)
		delete(fakepar);

	if(rBtn)
		delete(rBtn);

	if(lBtn)
		delete(lBtn);

	if(lsBtn)
		delete(lsBtn);

	delete m_pTrayIcon;

	CDialog::OnDestroy();
}


void YellowDlg::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//P2N(_T("sel changed to %s\r\n"), m_tree.GetItemText(m_tree.GetSelectedItem()));

	*pResult = 0;
}

void YellowDlg::OnItemexpandedTree1(NMHDR* pNMHDR, LRESULT* pResult)

{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	//P2N(_T("item expanded %s\r\n"), m_tree.GetItemText(m_tree.GetSelectedItem()) );

	*pResult = 0;
}

//DEL void YellowDlg::OnMove(int x, int y)
//DEL 
//DEL {
//DEL 	CDialog::OnMove(x, y);
//DEL }

//////////////////////////////////////////////////////////////////////////

BOOL YellowDlg::PreTranslateMessage(MSG* pMsg)

{
	//if(pMsg->message == WM_COMMAND)
	//	{
	//	P2N(_T("Command = lo=%d hi=%d %d %d\r\n"), 
	//			LOWORD(pMsg->wParam), HIWORD(pMsg->wParam),
	//					LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));
	//
	//		}
	
	if(pMsg->message == WM_MOUSEMOVE)
		{
		//P2N(_T("Mouse Move x=%d y=%d\r\n"), LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));

		mouse.x = LOWORD(pMsg->lParam);
		mouse.y = HIWORD(pMsg->lParam);
		}

	// Keycode ESC
	if(pMsg->message == WM_KEYDOWN)
		{
		//P2N(_T("Key Down W=%d LpLO=%d LpHI=%d\r\n"), pMsg->wParam, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));

		if(pMsg->wParam == VK_ESCAPE)
			{
			return true;
			}
		}

	// Speak Messages
	if(pMsg->message == WM_USER+1)
		{
		//P2N(_T("Text 2 Speach message W=%d LpLO=%d LpHI=%d\r\n"), 
		//		pMsg->wParam, LOWORD(pMsg->lParam), HIWORD(pMsg->lParam));
		}

	if(pMsg->message == WM_USER+10)
		{
		P2N(_T("Message from old app to reset UI\r\n"));
		//SetWindowText(_T("Reset UI"));
		if(ped)
			ped->EndModalLoop(0);

		SetForegroundWindow();
		}
	
	return CDialog::PreTranslateMessage(pMsg);
}


void YellowDlg::OnButton10()

{
	//AfxMessageBox(_T("Setup"));

	CSetup su; su.m_BackColor = dlgback;

	su.DoModal();
}

//////////////////////////////////////////////////////////////////////////
// Read registry key for alarms

void YellowDlg::ArmNext()

{
	time_t tt =	time(NULL);
	
	remlst.RemoveAll();

	num_alarms = 0;
	
	CString tmp(_T("CryptoSticky")), 
	tmp2(_T("No Alarm is set ... click on time or date to speak it."));;

	//P2N(_T("Scanning for next alarm at '%s'\r\n"), support.time_t2str(tt));

	HKEY key;
	LONG ret = RegOpenKey(HKEY_CURRENT_USER, keystr, &key);

	//P2N(_T("opened reg key, handle %d\r\n"), key);
 	////ASSERT(ret == ERROR_SUCCESS);

	TCHAR kname[_MAX_PATH]; 
	TCHAR kval[_MAX_PATH];

	int idx = 0;
	alarm_tt = 0;

	while(true)
		{
		DWORD ksize = _MAX_PATH, kvsize = _MAX_PATH, ktype;

		// Fresh start
		kval[0] = '\0'; kname[0] = '\0'; 

		LONG ret = RegEnumValue(key, idx, kname, &ksize, 0, &ktype, (LPBYTE)kval, &kvsize);
		
		//if(ret != ERROR_SUCCESS && ret != ERROR_NO_MORE_ITEMS)
		//	break;

		if(ret != ERROR_SUCCESS)
			break;

		//if(ktype == REG_DWORD)
		if(ktype == REG_SZ)
			{
			//P2N(_T("Reg enumeration %s, %s\r\n"), kname, support.time_t2str(att));
			//P2N(_T("\r\nReg enumeration '%s' '%s'\r\n"), kname, kval);

			struct tm stm; memset(&stm, 0, sizeof(stm));
					
			// Decompile date from reg
			//_stscanf_s(kval,_T("%02d/%02d/%04d %02d:%02d"), 
			//	&stm.tm_mon, &stm.tm_mday, &stm.tm_year, &stm.tm_hour,
			//			&stm.tm_min);
			
			_stscanf(kval,_T("%02d/%02d/%04d %02d:%02d"), 
				&stm.tm_mon, &stm.tm_mday, &stm.tm_year, &stm.tm_hour,
						&stm.tm_min);

			//P2N(_T("Decomp date %02d-%02d-%04d  %02d:%02d\r\n"), 
			//	stm.tm_mday, stm.tm_mon, stm.tm_year, stm.tm_hour, stm.tm_min);

			// Fix tm structure
			stm.tm_year -= 1900; stm.tm_mon -= 1; stm.tm_hour -= 1;			
			time_t att =  mktime(&stm);

			 //P2N(_T("Unconvert '%s'\r\n"), support.time_t2str(att));

				// Alarm in future?
				if(att >= tt)
					{					
					P2N(_T("Alarm set: %s, %s\r\n"), kname, support.time_t2str(att));

					// Is it less than current or unset?
					if(att <= alarm_tt ||  alarm_tt == 0)
						{
						//P2N(_T("Setting alarm to %s %s\r\n"), kname, support.time_t2str(att));
						
						alarm_tt = att;
						alname[num_alarms] = kname;

						al_done = false;
						
						if(num_alarms < MAX_SIMUL_ALARMS - 1)
							num_alarms++;

						WakeMachine();
						}
					}
				else
					{
					if(kname[0] != '\0')
						{
						//P2N(_T("Remove this one '%s' '%s'\r\n"), kname, support.time_t2str(att));
						remlst.AddTail(kname);
						}
					}
				}

		idx++;

		if(ret == ERROR_NO_MORE_ITEMS)
			{
			//P2N(_T("No more Items\r\n"));
			break;
			}
		}

	CString title;

	if(num_alarms == 0)
		{
		alname[0] = _T("");
		tmp =_T("CryptoSticky Application");
		}
	else
		{		
		MemoSupp	memo; memo.Read(alname[0]);

		title = memo.head;
		support.ShortenStrL(tmp, 22);

		CString multi(_T("Next Alarm"));

		if(num_alarms == 1)
			{
			CString title2(title); support.ShortenStr(title2, 32);

			tmp.Format(_T("CryptoSticky Alarm\r\nat: %s\r\nTitle: '%s'"),
							support.time_t2tstr(alarm_tt), title2);
						
			tmp2.Format(_T("Alarm at: %s Title: '%s'"), 
							support.time_t2tstr(alarm_tt), title2);		
			}
		else
			{
			tmp2 = tmp =_T("Multiple Alarms");
			}
		
		}

	m_pTrayIcon->SetTooltip(tmp);
	m_prog.SetWindowText(tmp2);	

	RegCloseKey(key);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnButton11()

{
	//ArmNext();

	if(num_alarms == 0)
		{
		AfxMessageBox(_T("No CryptoSticky Alarm is set."));
		return;
		}

	P2N(_T("Next alarm is '%s' at %d %d alarms\r\n"), alname[0], alarm_tt, num_alarms);

	HTREEITEM item = 0;

	if(num_alarms > 0)
		{
		HideMemos();

		// Show all that we have now
		for(int loop = 0; loop < num_alarms; loop++)
			{
			FnameToSel(alname[loop], &item);
			if(item)
				{
				m_tree.SelectItem(item);	
				ShowOne(item, true, true, true);
				}
			}
		}
	else
		{
		AfxMessageBox(_T("No Alarm is set"));
		}
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::ShowAll()

{
	OnButton8();
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::HideAll()

{
	Iterate_Show(NULL, false);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::FnameToSel(const TCHAR *fname, HTREEITEM *item)

{
	HTREEITEM iii = NULL;
	*item = NULL;

	if(fname[0] == 0)
		return;

	Iterate_fname(fname, iii, item);

	P2N(_T("YellowDlg::FnameToSel %s %x\r\n"), fname, *item);

}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::Iterate_fname(const TCHAR *oname, HTREEITEM currentItem, HTREEITEM *item)

{
	//P2N(_T("YellowDlg::Iterate_fname %s\r\n"), oname);

	unsigned int numSelectedFolder = 0;

	if(currentItem == 0)
		currentItem = TVI_ROOT;

	HTREEITEM last = 0;
	HTREEITEM hChildItem = m_tree.GetChildItem(currentItem);

	while(hChildItem)
		{
		//CString str;   BuildTree(str, hChildItem);
		CString fname; GetNodePath(fname, hChildItem);
		//CString title = m_tree.GetItemText(hChildItem);

		//P2N(_T("YellowDlg::Iterate_fname %s -- %s\r\n"), fname, oname);

		if(_tcscmp(fname, oname) == 0)
			{
			//P2N(_T("Found File name %s\r\n"), fname);
			*item = hChildItem;
			break;
			}

		Iterate_fname(oname, hChildItem, item);

		hChildItem = m_tree.GetNextItem(hChildItem, TVGN_NEXT);
		}
}

void YellowDlg::OnRightSearch() 

{
	//P2N(_T("YellowDlg::OnRightSearch\r\n"));

	SearchDLG sdlg(this); sdlg.m_BackColor = dlgback;

	sdlg.DoModal();
}


//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnRightShow() 

{
	HTREEITEM h1 = m_tree.GetSelectedItem();
	if(!h1)
		return;

	ShowOne(h1, true | 0x80);
}


void YellowDlg::OnRightHide() 

{
	HTREEITEM h1 = m_tree.GetSelectedItem();
	if(!h1)
		return;

	ShowOne(h1, false);	
}

// Create new memo

void YellowDlg::OnRightCreatenewmeno() 

{
	HTREEITEM h1 = m_tree.GetSelectedItem();
	if(!h1)
		return;

	CreateBranch(TRUE);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnRightBroadcast() 

{
	_beginthread(_CastThreadProc, 0, (void*)this);
}

//////////////////////////////////////////////////////////////////////////

// Delete current memo

void YellowDlg::OnRightDelete() 

{
	HTREEITEM h1 = m_tree.GetSelectedItem();
	if(!h1)
		return;

	OnButton3();
}

void YellowDlg::OnRightSelectall() 

{
	WrIterate_Checks(NULL);
	Iterate_Show(NULL);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnRight2BroadcastAll() 

{
	_beginthread(_mCastThreadProc, 0, (void*)this);
}

void YellowDlg::OnRight2Selectall() 

{
	Iterate_Sel(NULL, true);
	WrIterate_Checks(NULL);
}

void YellowDlg::OnRight2Hideallselectedmemos() 

{
	HTREEITEM h1 = m_tree.GetSelectedItem();
	if(!h1)
		return;

	OnButton9();
}

void YellowDlg::OnRightEditmemo() 

{
	HTREEITEM h1 = m_tree.GetSelectedItem();
	if(!h1)
		return;

	EditCurrent();	
}

void YellowDlg::OnRightSelectmemo() 

{
	HTREEITEM h1 = m_tree.GetSelectedItem();
	if(!h1)
		return;

	m_tree.SetCheck(h1, !m_tree.GetCheck(h1));	

	ShowOne(h1, m_tree.GetCheck(h1));

	WrIterate_Checks(NULL);
}

//////////////////////////////////////////////////////////////////////////
// Use this button for testing also

void YellowDlg::OnButton17() 

{
	CAboutDlg ca; //ca.SetAlpha(255);
	ca.m_BackColor = dlgback;
	ca.DoModal();	
}

//////////////////////////////////////////////////////////////////////////
// 

void YellowDlg::OnCheck3() 

{
	//UpdateData();

	m_auto = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();
	
	// Write hide entry to the registry
	CString section(_T("Setup"));

	AfxGetApp()->WriteProfileInt(section,_T("Auto"), m_auto);
	int new_auto = AfxGetApp()->GetProfileInt(section,_T("Auto"), 0);
	
	if(m_auto != new_auto)
		{
		AfxMessageBox(_T("Cannot save configuration to registry, ")
		_T("Make sure your security settings allow registry operations."));
		}

	if(m_auto)	
		{
		//P2N(_T("Auto on\r\n"));
		OnButton8();
		}
	CString tmp; tmp.Format(_T("Auto show Sticky setting changed (%d)"), m_auto); 
	ShowStatus(tmp);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::ReadAllMemos(int erase, int show, int tmptop, int force)

{
	//return;

	if(reenter)
		return;
	
	support.MarkPerf();	

	//P2N(_T("ReadAllMemos erase=%d show=%d tmptop=%d\r\n"), erase, show, tmptop);

	reenter = true;

	if(erase)
		m_tree.DeleteAllItems();

    // Create file
    CFile cf; CString fname;
    int count = 0, err = 0, ret = 0;
    long hFile;
    struct _tfinddata_t c_file;

    fname.Format(_T("%s*.txt"), dataroot);

    hFile = _tfindfirst(fname, &c_file);

    while(TRUE)
        {
        if(hFile == -1 || ret)
            break;

		//P2N(_T("\r\nYellowDlg::ReadAllMemos scanning %s\r\n"), c_file.name);

		//Sleep(20);
		if(count % 5 == 0)
			support.YieldToWinEx();

		CString fname;  fname.Format(_T("%s%s"), dataroot, c_file.name);
		HTREEITEM hout = GetMemoItem(fname);	

		if(hout)
			{			
			// Mark it in detail buffer
			MemoDetail *det = new MemoDetail; //ASSERT(det);
			*det->fname = fname; det->item = hout;
			detlist.AddTail(det);
			//P2N(_T("adding 0x%x '%s'\r\n"), det->item, *det->fname);

			if(show)
				{
				ShowOne(hout, show, false, tmptop);			
				}	
			if(force)
				{
				CYmemo *st = Handle2Memo(hout);
				if(st)
					{
					st->LoadContent(true);
					}
				}
			}
		else
		{
			err++;
			P2N(_T("Cannot load %s\r\n"), fname);
		}
		count++;		
		//if(count > 5)  // just for testing
		//	break;
        ret = _tfindnext( hFile, &c_file );
        }	
	P2N(_T("Loaded %d memos %d errors\r\n"), count, err);
	reenter = false;

	P2N(_T("Readall %d memos exec time=%d ms \r\n"), count, support.QueryPerf());	
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnRight2Refreshall() 

{
	ReadAllMemos(false, true, false);	
}

void YellowDlg::OnRight2Createnewmemo() 

{
	CreateBranch(TRUE);	
}

void YellowDlg::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 

{
	//P2N(_T("%d\r\n"), nChar);

	if(nChar == 89)
		{
		m_tree.SetFocus();
		}
	CDialog::OnSysKeyDown(nChar, nRepCnt, nFlags);
}


void YellowDlg::OnSize(UINT nType, int cx, int cy) 

{
	CDialog::OnSize(nType, cx, cy);
	
	if(nType == SIZE_MINIMIZED)
		{
		//P2N(_T("Minimized window\r\n"));
		OnButton2();
		}
}

void YellowDlg::OnClose() 

{	
	//P2N(_T("YellowDlg::OnClose\r\n"));

	CString section(_T("")), pkey(_T("Prompt_Close"));
	int ret = AfxGetApp()->GetProfileInt(section, pkey, 0);
	
	if(ret == 0)
		{	
		ret = AfxMessageBox(
		_T("Closing CryptoSticky, Alarms will be inactive   \r\n")
		_T("If you want alarms to stay active, minimze to System Tray instead   \r\n")
		_T("\r\n")
		_T(" Press  'YES'  to Close CryptoSticky   \r\n")
		_T(" Press  'NO'   to minimize CryptoSticky to System Tray   \r\n")
		_T("\r\n")
		_T("Hold Down Shift Key while answering 'Yes or No' to make this answer permanent.\r\n")
			, MB_YESNO);
		
		SHORT ks   = GetAsyncKeyState (VK_SHIFT);

		if(ks & 0x8000)
			{
			//P2N(_T("Save perm answer %d\r\n"), ret);
			AfxGetApp()->WriteProfileInt(section, pkey, ret);
			}
		}

	if(ret == IDNO)
		{
		ShowWindow(SW_MINIMIZE);
		return;
		}

	// Redirct to
	OnOK();

	//CDialog::OnClose();
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnOK() 

{			
	CString section(_T("")), pkey(_T("Prompt_OK"));
	int ret = AfxGetApp()->GetProfileInt(section, pkey, 0);
	
	if(ret == 0)
		{	
		ret = AfxMessageBox(
		_T("Closing CryptoSticky, Alarms will be inactive\r\n")
		_T("Hold Down Shift Key while pressing 'OK' to skip this prompt permenantly.")
			, MB_OK);
		
		SHORT ks   = GetAsyncKeyState (VK_SHIFT);

		if(ks & 0x8000)
			{
			//P2N(_T("Save perm answer %d\r\n"), ret);
			AfxGetApp()->WriteProfileInt(section, pkey, ret);
			}
		}

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnRightRebuildmemotree() 

{
	//P2N(_T("YellowDlg::OnRightRebuildmemotree() \r\n"));

	HideMemos();
	ReadAllMemos(true, false, false, true);	
	Iterate_Checks(NULL );	
	ShowAll();
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnRightGather() 

{
	gatherstep = gatheroffsetx = gatheroffsety = 0;

	Iterate_Checks(NULL);	
	Iterate_Exec(NULL, true, IteratorGather);
	//Iterate_Checks(NULL );
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::DelMemoFile(CString &fname, CString &memotree)

{
	//P2N(_T("Delete Memo: %s\r\n"), fname);

	if(memotree.Mid(0, 9) !=_T("\\Deleted\\"))
		{
		// Move it to 'deleted' folder		
		MemoSupp memo2;

		if(memo2.Read(fname))
			{
			memo2.initial =_T("\\Deleted") + memo2.initial;
			
			CString newfname = memo2.MakeFileName();

			memo2.Write(newfname);			
			
			//CString name = support.GetFileNameExt(newfname);
			
			//P2N(_T("Got filename '%s'\r\n"), newfname);
			
			// Copy Dependent files as well
			CString dir = support.GetDirName(newfname);
			CString xname = support.GetFilename(newfname);
			CString wavname = dir +_T("\\") + xname +_T(".wav");
			
			CString diro = support.GetDirName(fname);
			CString xnameo = support.GetFilename(fname);
			CString wavnameo = diro +_T("\\") + xnameo +_T(".wav");
			
			//P2N(_T("Got old Wavename %s\r\n"), wavnameo);
			//P2N(_T("Got Wavename %s\r\n"), wavname);

			_tunlink(wavname);
			_trename(wavnameo, wavname);
			
			// Copy Dependent files as well
			CString dirg = support.GetDirName(newfname);
			CString xnameg = support.GetFilename(newfname);
			CString gesname = dir +_T("\\") + xname +_T(".ymd");
			
			CString dirgo = support.GetDirName(fname);
			CString xnamego = support.GetFilename(fname);
			CString gesnameo = diro +_T("\\") + xnamego +_T(".ymd");
			
			//P2N(_T("Got old gesname %s\r\n"), gesnameo);
			//P2N(_T("Got gesname %s\r\n"), gesname);

			_tunlink(gesname);
			_trename(gesnameo, gesname);			
			}
		else
			{
			P2N(_T("YellowDlg::DelMemo - no file %s"), fname);			
			}
		}
	else
		{
		// Unlink dependent files

		CString dir = support.GetDirName(fname);
		CString xname = support.GetFilename(fname);
		CString wavname = dir +_T("\\") + xname +_T(".wav");	
		_tunlink(wavname);

		CString gesname = dir +_T("\\") + xname +_T(".ymd");
		_tunlink(gesname);
		}
 
	// Just nuke the main file either way
	if(_tunlink(fname))
		{
		P2N(_T("Cannot delete memo %s\r\n"), fname);
		//AfxMessageBox(_T("Info: Cannot find the file of the memo. ")
		//_T("(possibly 'Shared Memo' removed by the other user)"));
		}

	
	ShowStatus(_T("Deleted Sticky"));
}

void YellowDlg::OnCheck4() 

{
	ShowStatus(_T("Changing Show/Hide Trash (The 'Deleted' Folder)"));
	OnRightRebuildmemotree();
	ShowStatus(_T("Changed Show/Hide Trash (The 'Deleted' Folder)"));
}

void YellowDlg::OnRight2Deselectall() 

{
	Iterate_Sel(NULL, false);
	WrIterate_Checks(NULL);
}

void YellowDlg::OnRightClearselectonsubitems() 

{
	HTREEITEM h1;

    if(!(h1 = m_tree.GetSelectedItem() ))
	    {
		AfxMessageBox(_T("Please select a Sticky to clear selection of."));
		return;
		}
	Iterate_Sel(h1, false);
	WrIterate_Checks(NULL);
}

void YellowDlg::EditCurrent(int ro)

{
	 HTREEITEM h1;

    if(!(h1 = m_tree.GetSelectedItem() ))
	    {
		AfxMessageBox(_T("Please select a Sticky to edit first."));
		return;
		}

    CString str;   BuildTree(str);
    CString fname; GetNodePath(fname);

	//P2N(_T("Editing '%s'\r\n"), str);

	if(str ==_T("\\Deleted\\"))
		{
		AfxMessageBox(_T("Cannot edit the 'Deleted' folder"));
		return;
		}

	//CEditDlg ed(this); ed.m_BackColor = dlgback;
	ped = new CEditDlg(this); ped->m_BackColor = dlgback;

	ped->ro = ro;

	HTREEITEM   h1x, h1p;

    h1x = m_tree.GetSelectedItem(); 
	h1p = m_tree.GetNextItem(h1, TVGN_PARENT);

	BuildTree(ped->memoroot, h1x);

	// Take exception to root parent (parent of root is self)
	if(h1p)
		BuildTree(ped->memorootp, h1p);
	else
		ped->memorootp =_T("\\");

	ped->file = fname;
	ped->title = str;
	ped->st = Handle2Memo(h1x);

	//P2N(_T("Editing memo %p\r\n"), ed.st);

	ped->newhandle = h1x;
	if(ped->DoModal() != IDOK)
		{
		delete ped; ped = NULL;
		return;
		}

	ShowOne(ped->newhandle, true, false, true);

	delete ped; ped = NULL;

	ShowStatus(_T("Edited Sticky")); 
}

void YellowDlg::OnCheck2() 
{

	m_restart = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	WriteStartup(m_restart);

	CString tmp; tmp.Format(_T("AutoStart setting changed (%d)"), m_restart);
	ShowStatus(tmp);
}

void YellowDlg::OnCheck1() 

{
	m_hide = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();

	// Write hide entry to the registry
	CString section(_T("Setup"));
	AfxGetApp()->WriteProfileInt(section,_T("Hide"), m_hide);
	int new_hide = AfxGetApp()->GetProfileInt(section,_T("Hide"), 0);
	
	if(m_hide != new_hide)
		{
		AfxMessageBox(
		_T("Cannot save configuration to registry, make sure your security settings allow registry operations."));
		}

	P2N(_T("Hide to tray new_hide=%d m_hide=%d\r\n"), new_hide, m_hide);

	CString tmp; tmp.Format(_T("CryptoSticky auto hide setting changed (%d)"), m_hide); 
	ShowStatus(tmp);
}

//////////////////////////////////////////////////////////////////////////

int		YellowDlg::WriteStartup(int flag)

{
	//P2N(_T("YellowDlg::WriteStartup(int flag=%d)\r\n"), flag);

	CString section(_T("Setup")), appname(_T("CryptoSticky"));
	int restart2 = AfxGetApp()->GetProfileInt(section,_T("Restart"), 0);

	// Write to startup registry, if changed
	if(flag != restart2)
		{
		//P2N(_T("Writing new registry value\r\n"));

		AfxGetApp()->WriteProfileInt(section,_T("Restart"), flag);

		if(flag)
			{
			HKEY key;
			LONG ret = RegCreateKey(HKEY_CURRENT_USER,
					_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), &key);

			CString appr, revapp;
			TCHAR *buff = appr.GetBuffer(_MAX_PATH);
			GetModuleFileName(AfxGetInstanceHandle(), buff, MAX_PATH);
			
			LONG ret2 = RegSetValueEx(key, appname,  0, REG_SZ,
								(const BYTE *)buff, 
										_tcslen(buff) * sizeof(TCHAR));

			//P2N(_T("Writing app '%s'\r\n"), buff);

			appr.ReleaseBuffer();

			
			DWORD	revlen = _MAX_PATH, typeval;
			TCHAR	*ptr = revapp.GetBuffer(revlen); //ASSERT(ptr);

			LONG ret3 = RegQueryValueEx(key, appname,  0, &typeval, (BYTE*)ptr, &revlen);

			revapp.ReleaseBuffer();

			RegCloseKey(key);

			//P2N(_T("revapp='%s'\r\nappr='%s'\r\n"), revapp, appr);

			if(revapp != appr)
				{
				AfxMessageBox(
				_T("Cannot save configuration to registry, make sure your security settings allow registry operations."));
				}
			
			}
		else
			{
			HKEY key;
			LONG ret = RegCreateKey(HKEY_CURRENT_USER,
					_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), &key);

			LONG ret2 = RegDeleteValue(key, appname);

			if(ret2 !=  ERROR_SUCCESS)
				AfxMessageBox(
				_T("Cannot remove configuration from registry, make sure your security settings allow registry operations."));
			}
		}
	return 0;

}

void YellowDlg::OnButton34() 

{
	AfxGetApp()->WinHelp(0);
	ShowStatus(_T("Displaying Help ..."));
}


void YellowDlg::OnOk2() 

{
	Encrypt_Clipboard();
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnButton35() 

{
	if(misc.IsShiftDown())
		{
		P2N(_T("YellowDlg::OnDoubleclickedButton11\r\n"));
		CAlarm *al = new CAlarm; //ASSERT(al);
		al->m_BackColor = dlgback;
					
		alist.AddHead(al);

		al->Show();
		}
	else
		{
		ShowMissed();
		}
}


void YellowDlg::ShowMissed() 

{
	//YellowDlg *cc = ((YellowDlg *)AfxGetApp()->m_pMainWnd);
	////ASSERT(cc);

	ArmNext();

	if(remlst.GetCount() == 0)
		{
		CString tmp;
		tmp.Format(_T("No missed alarms")); // as of %s", support.time_t2str(time(NULL)) );

		ShowStatus(tmp); 
		//AfxMessageBox(tmp);
		return;
		}
	
	HideMemos();

	// Remove all that is in the past
	POSITION pos = remlst.GetHeadPosition();

	while(true)
		{
		if(!pos)
			break;

		CString str = remlst.GetNext(pos);
		
		//P2N(_T("Stale regvalues: '%s'\r\n"), str);

		HTREEITEM iii; FnameToSel(str, &iii);

		if(iii)
			ShowOne(iii, true);
		}		

	CString tmp;
	tmp.Format(_T("Showing %d missed alarm(s)"), remlst.GetCount());

	ShowStatus(tmp); 
	//AfxMessageBox(tmp);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnShowWindow(BOOL bShow, UINT nStatus) 

{
	if(firstshow == 0)
		{
		//m_prog.SetWindowTextA(_T("hello"));
		//m_prog.setSetActiveWindow
		m_prog.m_BackColor = dlgback;
		
		m_check1.m_BackColor = dlgback;
		m_check2.m_BackColor = dlgback;
		m_check3.m_BackColor = dlgback;
		m_check4.m_BackColor = dlgback;

		m_check10.m_BackColor = dlgback;
		
		firstshow = 1;
		}

	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
		OnButton8();

	//P2N(_T("YellowDlg::OnShowWindow %d\r\n"), bShow);	
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnKeydownTree1(NMHDR* pNMHDR, LRESULT* pResult) 

{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

	//P2N(_T("YellowDlg::OnKeydownTree1 %d\r\n"), pTVKeyDown->wVKey);

	g_h1 = m_tree.GetSelectedItem();

	switch(pTVKeyDown->wVKey)
		{
		case VK_SPACE:
			//P2N(_T("OnKeydownTree1 Space timer1\r\n"));
			SetTimer(2, 10, NULL);
		break;

		case VK_DELETE:
			DelMemo();
		break;

		case VK_F10:
			CRect rc; GetWindowRect(&rc);

			P2N(_T("OnKeydownTree1 f10\r\n"));			

			CXrayM   *mm = new CXrayM(IDD_MENUBASE);
			
			//mm->sendto = this;
			mm->LoadMenu(IDR_MENU1, 1);

			CPoint scr(rc.left + rc.Width() / 2 - 24, 
					rc.top + rc.Height() / 2 - 24);  
			mm->Show(scr.x, scr.y);

		break;
}

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::Iterate_Uncheck(HTREEITEM currentItem, int hide)

{
	unsigned int numSelectedFolder = 0;

	if(currentItem == 0)
		currentItem = TVI_ROOT;

	HTREEITEM last = 0;
	HTREEITEM hChildItem = m_tree.GetChildItem(currentItem);

	while(hChildItem)
		{
		ShowOne(hChildItem, false);

		m_tree.SetCheck(hChildItem, false);	

		//ShowOne(h1, m_tree.GetCheck(h1));

		Iterate_Uncheck(hChildItem);

		hChildItem = m_tree.GetNextItem(hChildItem, TVGN_NEXT);
		}

	//WrIterate_Checks(NULL);
}

void YellowDlg::Close()
{
	//P2N(_T("YellowDlg::Close\r\n"));
	OnOK();
}

//////////////////////////////////////////////////////////////////////////
// Decorate tooltips

void YellowDlg::Decorate()

{
	m_2.SetToolTipText(_T("Hide this Panel to System Tray\r\nTo autohide on start check the 'AutoHide' option below"));
	m_3.SetToolTipText(_T("Delete currently selcted Sticky\r\nWill show a confirmation dialog before delete"));
	m_4.SetToolTipText(_T("Create Memo in the current selection level\r\nTo create a sub memo, select the parent memo to create it under"));
	m_b5.SetToolTipText(_T("Create a new memo from clipboard\r\nWill open up memo creation dialog"));
	m_6.SetToolTipText(_T("Copy contents of the selected Sticky to clipboard"));
	m_7.SetToolTipText(_T("Edit currently selected Sticky"));
	m_8.SetToolTipText(_T("Show Stickies that have their 'CheckBox' selected\r\nTo show an idividual Sticky, click on its checkbox"));
	m_9.SetToolTipText(_T("Hide All visible memos\r\nHold shift to uncheck all memos from main window"));
	m_11.SetToolTipText(_T("Show next Alarm in the Alarm Queue"));
	m_10.SetToolTipText(_T("Set up CryptoSticky Colors, Sounds and Data Directory"));
	m_b17.SetToolTipText(_T("Show the About Box and the Registration Dialog"));
	m_b34.SetToolTipText(_T("Show CryptoSticky help. Will open up a web browser window"));
	m_b35.SetToolTipText(_T("Show memos with missed alarms"));
	
	m_ok.SetToolTipText(_T("Close all Stickies and the CryptoSticky application"));
	m_ok2.SetToolTipText(_T("Encrypt the text contents of the clipboard"));
	m_ok3.SetToolTipText(_T("Decrypt the text contents of the clipboard"));
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnCheck10() 

{
	// Write hide entry to the registry
	CString section(_T("Setup"));

	m_listen = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();
	
	AfxGetApp()->WriteProfileInt(section,_T("Listen"), m_listen);
	int new_listen = AfxGetApp()->GetProfileInt(section,_T("Listen"), 0);
	
	if(m_listen != new_listen)
		{
		AfxMessageBox(
		_T("Cannot save configuration to registry, make sure your security settings allow registry operations."));
		}

	ShowStatus(_T("Listening for remote memos setting changed"));
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnCheck5() 

{
	// Write hide entry to the registry
	CString section(_T("Setup"));

	m_ignore = ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();
	
	AfxGetApp()->WriteProfileInt(section,_T("Ignore"), m_ignore);
	int new_ignore = AfxGetApp()->GetProfileInt(section,_T("Ignore"), 0);
	
	if(m_ignore != new_ignore)
		{
		AfxMessageBox(
		_T("Cannot save configuration to registry, make sure your security settings allow registry operations."));
		}

	ShowStatus(_T("Ignore missing alarms setting changed")); 
}

//////////////////////////////////////////////////////////////////////////

int YellowDlg::Encrypt(CString &str)

{	
	P2N(_T("YellowDlg::Encrypt '%s'\r\n"), str);

	CPass passdlg(this); passdlg.m_BackColor = dlgback;

	passdlg.title =_T("Encryption Key / Pass");

	if(passdlg.DoModal() != IDOK)
		return 0;

	if (passdlg.pass.GetLength() == 0)
	{
		AfxMessageBox(_T("Cannot use empty pass."));
		return 1;
	}

	P2N(_T("CYmemo::Encrypt pass '%s'\r\n"), passdlg.pass);

	bluepoint.encrypthex(str, passdlg.pass);

	// Destroy pass
	bluepoint.killpass(passdlg.pass);

	CString tmp; tmp.Format(_T("Encrypted %d bytes"), str.GetLength());
	ShowStatus(tmp);

	return 1;
}

//////////////////////////////////////////////////////////////////////////

int YellowDlg::Decrypt(CString &str)

{
	P2N(_T("YellowDlg::Decrypt str:'%s'\r\n"), str);

	CPass passdlg(this);  passdlg.m_BackColor = dlgback;
	passdlg.title =_T("Decyption Key / Pass");

	if(passdlg.DoModal() != IDOK)
		return false;
	
	P2N(_T("CYmemo::OnDecrypt pass '%s'\r\n"), passdlg.pass);

	bluepoint.verbose = true;
	int ret = bluepoint.decrypthex(str, passdlg.pass);
	if (ret == CRYPT_EMPTY)
	{
		AfxMessageBox(_T("Must have content to decrypt"));
		ShowStatus(_T("Incomplete selection"));
		return false;
	}
	
	P2N(_T("CYmemo::OnDecrypt ret=%d\r\n"), ret);
	if (ret == CRYPT_NOTAIL || ret == CRYPT_NOHEAD)
		{
		AfxMessageBox(_T("Must select text with #CRYPT and ")
			_T("#XCRYPT in it."));

		ShowStatus(_T("Incomplete selection"));
		return false;
		}

	if (ret == CRYPT_BADPASS)
	{
		AfxMessageBox(_T("Mismatching hash due to invalid password, ")
			_T("or damaged cyphertext."));

		ShowStatus(_T("Invalid password"));
		return false;
	}


	CString tmp; tmp.Format(_T("Decrypted clipboard %d bytes"), str.GetLength());
	ShowStatus(tmp);
	
	// Destroy pass
	bluepoint.killpass(passdlg.pass);

	return true;
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnOk3() 

{
	Decrypt_Clipboard();
}

//////////////////////////////////////////////////////////////////////////

int	YellowDlg::ExportMemos(const TCHAR *str) 

{
	found = 0;

	//P2N(_T("YellowDlg::ExportMemos %s\r\n"), str);
	Iterate_Exec(NULL, (int)str, IteratorExport);

	return found;
}

//////////////////////////////////////////////////////////////////////////
// Return changed name

CString		YellowDlg::ImportMemo(const TCHAR *str) 

{
	CString	ret_val;

	//P2N(_T("YellowDlg::ImportMemo '%s'\r\n"), str); 

	MemoSupp	memo;

	// Read YellowDlg file
	int ret = memo.Read(str);

	if(!ret)
		{
		AfxMessageBox(_T("Cannot read Sticky for import"));
		return ret_val;
		}
	
	//P2N(_T("Read memo '%s' '%s'\r\n"), memo.head, memo.initial);
	
	// Find if it is a duplicate
	int cnt = 1;
	CString str2 = memo.initial;
	CString str3 = memo.initial;
	CString str4 = memo.head;

	int idx = memo.initial.ReverseFind('\\');
	if(idx >= 0)
		str2 = memo.initial.Left(idx);

	int idx2 = memo.initial.ReverseFind('#');
	if(idx2 >= 0)
		str2 = memo.initial.Left(idx2);
	
	while(true)
		{
		if(IsDuplicate(str3))
			{			
			CString fname; GetNodePath(fname, dup);

			//P2N(_T("Duplicate memo fname='%s' str='%s'\r\n"), fname, str);

			// If identical, take no action
			MemoSupp	memo2;	memo2.Read(fname);
			if(memo.body == memo2.body)
				{
				//P2N(_T("Duplicate import fname='%s' str='%s'\r\n"), fname, str);
				return ret_val; 
				}

			CString tmp; tmp.Format(_T("#%d"), cnt++);
			str3 = str2 + tmp +_T("\\");
			str4 = memo.head + tmp;		
			}
		else
			{
			break;
			}
		}
		
	memo.initial = str3; memo.head = str4;
	
    // Create Path name
    CString tmp2 = memo.MakeFileName();

	ret_val = tmp2;
	
	//P2N(_T("New file  '%s'\r\n"), tmp2);
	//P2N(_T("New initial  '%s'\r\n"), memo.initial);

	// Commit to new memo settings
	memo.Write(tmp2);

	CString source(str);

	// Copy dependent files
	CopyDep(source, tmp2);

	datalog.PrintToLog(_T("Imported Sticky '%s'\r\n"), memo.head);

	//CString fname = support.GetFileNameExt(tmp2);
	//showthis = fname;
	showthis = tmp2;

	//DWORD pid; GetWindowThreadProcessId(m_hWnd, &pid);
	//P2N(_T("Import wthread=%x curr=%x\r\n"), pid, GetCurrentThreadId());

	//if(m_listen)
		{
		CString tmp4; tmp4.Format(_T("Imported Sticky '%s'"), memo.head);
		ShowStatus(tmp4); 
		//m_status.SetText(tmp4);
		}

	return ret_val;
}

//////////////////////////////////////////////////////////////////////////

void	YellowDlg::SendOne(const TCHAR *str, CNameEntry *entry)

{
	char buff[512]; int ret = 0;

	CString fname = support.GetFileNameExt(str);
	
	MemoSupp memo;	memo.Read(str);
	
	CString  strtmp; 
	strtmp.Format(_T("%s::%s"), *entry->machine, *entry->username);

	CFile cf;  
	if(cf.Open(str, CFile::modeRead) == 0)
		{
		P2N(_T("YellowDlg::OnRightBroadcast cannot open %s\r\n"), str);
		return;
		}

	SOCKADDR_IN     local_sin;	
	SOCKET sock = socket( AF_INET, SOCK_STREAM, 0);

	char *ipstr = NULL; 
	misc.UnicodeToAnsi(*entry->ip, &ipstr);
		
	//if (sizeof(TCHAR) == 1)
	//	{
	//	}
	//else
	//	{
	//	misc.UnicodeToAnsi(*entry->ip, &ipstr);
	//	}

	local_sin.sin_addr.s_addr = inet_addr(ipstr);
	local_sin.sin_port = htons(memo_port);
	local_sin.sin_family = AF_INET;
	int acc_sin_len = sizeof(local_sin);

	int ret_val = connect(sock, (SOCKADDR *) &local_sin, sizeof(local_sin));

	if(ret_val != SOCKET_ERROR)
		{		
		datalog.PrintToLog(_T("Sending sticky '%s' to '%s'\r\n"), memo.head, strtmp);

		// Send file identifier
		char *astr;	misc.UnicodeToAnsi(fname, &astr);
		send(sock, astr, 12, 0);
		free(astr);

		while(ret = cf.Read(buff, sizeof(buff)))
			{
			//AP2N(_T("Sending %s\r\n"), line);
			int ret_val = send(sock, buff, ret, 0);
			}

		// Wait for a short period to trceive an event
		//WSAEVENT   WSACreateEvent();
		// Wait for acknowledgement
		//int ret_val2 = recv(sock, buff, sizeof(buff), 0);		
		//P2N(_T("ack from pier %d %s\r\n"), ret_val2, buff);
		}
	else
		{
		datalog.PrintToLog(_T("Could not send sticky file '%s' to '%s'\r\n"), str, ipstr);

		// Ignore siltently 
		//CString tmp; tmp.Format(_T("Cannot open connection: %d"), WSAGetLastError());
		//AfxMessageBox(tmp);
		//P2N(_T("%s\r\n"), mxpad.sockerr2str(WSAGetLastError()) );
		//return;
		}

	cf.Close();

	closesocket(sock);
}

//////////////////////////////////////////////////////////////////////////

void	YellowDlg::SendFile(const TCHAR *str, const TCHAR *nameto)

{
	CString name2 = support.GetExtension(str);
	
	int alen = resolver.namearr.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		CNameEntry *val = (CNameEntry*)resolver.namearr.GetAt(loop);
		
		CString tmp; tmp.Format(_T("%s::%s"), *val->machine, *val->username);

		if(nameto == tmp)
			{
			SendOne(str, val);
			break;
			}
		}	
}

//////////////////////////////////////////////////////////////////////////
// Send all 3 files

void	YellowDlg::SendFiles(const TCHAR *str, const TCHAR *nameto)

{
	CString ext2 = support.GetExtension(str);
	CString name2 = support.GetFilename(str);
	CString dir2 = support.GetDirName(str);
	
	P2N(_T("SendFiles '%s' '%s' '%s'\r\n"), dir2, name2, ext2);

	CString txt = dir2 +_T("\\") + name2 +_T(".") + ext2;

	if(_taccess(txt, 0) >= 0)
		{
		//P2N(_T("SendFile '%s'\r\n"), txt);
		SendFile(txt, nameto);
		}

	CString  wav = dir2 + _T("\\") + name2 +_T(".wav");
	if(_taccess(wav, 0) >= 0)
		{
		//P2N(_T("SendFile '%s'\r\n"), wav);
		SendFile(wav, nameto);		
		}

	CString  ymd = dir2 + _T("\\") + name2 +_T(".ymd");
	if(_taccess(ymd, 0) >= 0)
		{
		//P2N(_T("SendFile '%s'\r\n"), ymd);
		SendFile(ymd, nameto);		
		}
	
}

//////////////////////////////////////////////////////////////////////////

void	YellowDlg::BCastFile(const TCHAR *str)

{
	//P2N(_T("YellowDlg::BCastFile str=%s\r\n"), str);

	TCHAR machine[MAX_COMPUTERNAME_LENGTH + 1]; 
	DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName((TCHAR*)&machine, &len);
	
	CString pro(_T("Access"));

	int alen = resolver.namearr.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		CNameEntry *val = (CNameEntry*)resolver.namearr.GetAt(loop);

		//P2N(_T("Sending to %s %s\r\n"), *val->machine, *val->ip);

		if(*val->machine == machine)
			{
			//P2N(_T("Self %s %s\r\n"), machine, *val->machine);
			continue;
			}
		
		CString tmp; 
		tmp.Format(_T("%s::%s::%s"), *val->machine, *val->username,_T("send"));
		int val1 = AfxGetApp()->GetProfileInt(pro, tmp, 1);
		if(!val1)
			{
			P2N(_T("No send %s\r\n"), *val->machine);
			continue;
			}
		CString strs; strs.Format(_T("%s::%s"), *val->machine, *val->username);
				
		SendFiles(str, strs);
		}
	}

//////////////////////////////////////////////////////////////////////////
// Not found, create

CYmemo*		YellowDlg::CreateMemoWin(HTREEITEM h1)

{
	CString fname; GetNodePath(fname, h1);
	CString title = m_tree.GetItemText(h1);
	CString strtree;   BuildTree(strtree, h1);
		
	//P2N(_T("YellowDlg::CreateMemoWin tree='%s'\r\n"), strtree); 
	//P2N(_T("YellowDlg::CreateMemoWin fname='%s'\r\n"), fname); 
	//P2N(_T("YellowDlg::CreateMemoWin handle 0x%x\r\n"), h1); 
	//P2N(_T("YellowDlg::CreateMemoWin title '%s'\r\n"), title); 

	// No tree presence - do not create
	if(title == _T(""))
		return(NULL);

	CYmemo *st = new CYmemo(fakepar); //ASSERT(st);
	//ASSERT(st->magic == CYmemo_Magic);

	st->tmptop = false;
	st->title = title;
	st->memofile  = fname;

	if(!support.IsFile(fname))
		{
		//P2N(_T("YellowDlg::CreateMemoWin fixing up file name for 0x%x \r\n"), h1);
		
		// Try original file name
		POSITION pos3 = detlist.GetHeadPosition();
		while(true)
			{
			if(!pos3) break;
			MemoDetail *md = (MemoDetail *) detlist.GetNext(pos3);
			//P2N(_T("YellowDlg::CreateMemoWin looking %x %s\r\n"), md->item, *md->fname);
				
			if(md->item == h1)
				{
				//P2N(_T("YellowDlg::CreateMemoWin Found: %s\r\n"), *md->fname);
				st->memofile  = *md->fname;
				break;
				}
			}
		}

	st->timeout = (int)memo_timeout;
	st->SetTransparency(main_trans);
	st->Create();

	CDC     DC;	CDC     *pDC = &DC;
	HDC		hdc = ::GetDC(HWND_DESKTOP);

    pDC->FromHandle(hdc);

    int     xsize =  GetDeviceCaps(hdc, HORZRES);
    int     ysize =  GetDeviceCaps(hdc, VERTRES);

	// Restore old placement, if any
	WINDOWPLACEMENT wp2; GetWindowPlacement(&wp2);

	CString section(_T("Positions"));
	CString str2 = AfxGetApp()->GetProfileString(section, fname, _T(""));

	if(str2 != _T(""))
		{
		_stscanf_s(str2,_T("%d %d %d %d"),
			&wp2.rcNormalPosition.top, &wp2.rcNormalPosition.left,
				&wp2.rcNormalPosition.bottom,  &wp2.rcNormalPosition.right);

		int	ww  = wp2.rcNormalPosition.right - wp2.rcNormalPosition.left;
		int	hh  = wp2.rcNormalPosition.bottom - wp2.rcNormalPosition.top;
		int	ll  = wp2.rcNormalPosition.left;
		int tt  = wp2.rcNormalPosition.top;

		st->SetWindowPos(NULL, ll, tt, ww, hh, SWP_NOZORDER);
		}
	else 
		{
		// Get lastly created window dims
		int ww = 0, hh = 0;
		CYmemo *st2  = NULL;
		WINDOWPLACEMENT wp;	
			
		if(!ylist.IsEmpty())
			{	
			st2 = (CYmemo *) ylist.GetHead();
			}
		if(st2)
			{
			st2->GetWindowPlacement(&wp);	
			}
		else
			{
			st->GetWindowPlacement(&wp);				
			}
		
		ww  = wp.rcNormalPosition.right - wp.rcNormalPosition.left;
		hh  = wp.rcNormalPosition.bottom - wp.rcNormalPosition.top;

		//P2N(_T("Last window dimentions ww=%d hh=%d\r\n"), ww, hh);
		
		if(prevwin < ysize - 300)
			prevwin += hh + 2;
		else
			prevwin += st->GetHeadHeight() + 2; 

		if(prevwin > ysize - 60)
			{
			//AfxMessageBox(_T("Overflow"));
			prevx += 40;
			prevwin = 60;
			}

		//P2N(_T("initialpos x-%d y=%d w=%d h=%d\r\n"), xsize  - 300 - prevx,
		//						prevwin, 100, 100);

		st->SetWindowPos(NULL, xsize  - 300 - prevx, prevwin,
									 100, 100,  SWP_NOZORDER);

		st->wantauto = true;
		}

	// Maintain a list of windows
	ylist.AddHead(st);

	// Dump window list
	//POSITION pos2 = ylist.GetHeadPosition();
	//while(true)
	//	{
	//	if(!pos2)
	//		break;
	//	CYmemo *st2 = (CYmemo *) ylist.GetNext(pos2);
	//	P2N(_T("List head: '%s'\r\n"), st2->memo.head);
	//	}

	return st;
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::ShowStatus(const TCHAR *str, int tout, int color)

{
	CString sstr(str);
	ShowStatus(sstr, tout, color);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::ShowStatus(CString &str, int tout, int color)

{
	KillTimer(6); KillTimer(8);

	stat_color = color;
	stat_tout = tout;
	stat_str = str;

	SetTimer(8, 20, NULL);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::CleanYlist()

{
	//P2N(_T("Dumpint late flag\r\n"));

	POSITION oldpos, pos = ylist.GetHeadPosition();
	DWORD  tick = GetTickCount();
	while(true)
		{
		oldpos = pos;

		if(!pos)
			break;

		CYmemo *st = (CYmemo *) ylist.GetNext(pos);
		//ASSERT(st);

		if(!st->showstate)
			{
			P2N(_T("Window hidden for %d %d (%d)  %s\r\n"), tick, st->lasthide,
				 tick -  st->lasthide, st->title);

			if(st->lasthide -  tick > 1000 * 10)
				{
				ylist.RemoveAt(oldpos);
				delete st;
				break;
				}
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void	YellowDlg::DelMemobyHandle(HTREEITEM h1)

{
	CString str, str4;

	//ASSERT(h1);

	// Unselect and hide the item
	m_tree.SetCheck(h1, false);
	ShowOne(h1, false);
	WrIterate_Checks(NULL);

	BuildTree(str4, h1);
	GetNodePath(str);

	// Delete from YellowDlg tree
    m_tree.DeleteItem(h1);

	AfxGetApp()->WriteProfileString(_T("Positions"), str, _T(""));	

	//P2N(_T("DelMemobyHandle str=%s --> str4=%s\r\n"), str, str4);
	//P2N(_T("DelMemobyHandle str4='%s'\r\n"), str4);

	DelMemoFile(str, str4);

	// Remove from ylist
	POSITION oldpos, pos = ylist.GetHeadPosition();
	while(true)
		{
		oldpos = pos;

		if(!pos)
			break;

		CYmemo *st = (CYmemo *) ylist.GetNext(pos);

		//ASSERT(st); //ASSERT(st->magic == CYmemo_Magic);
	
		if(st->memofile == str)
			{
			//P2N(_T("Nuking %s\r\n"), st->memo.head);
			ylist.RemoveAt(oldpos);
			delete st;
			}		
		}
	datalog.PrintToLog(_T("Deleted Sticky '%s'\r\n"), str4);

	// Arm next alarm in line
	ArmNext();

	m_tree.SetFocus();
}

//////////////////////////////////////////////////////////////////////////
//
// Copy dependent files

void YellowDlg::CopyDep(CString &from, CString &to)

{
	//P2N(_T("Copydep %s %d\r\n"), from, to);

	CString name2 = support.GetFilename(from);
	CString name3 = support.GetFilename(to);
	
	CString dir2 = support.GetDirName(from);
	CString dir3 = support.GetDirName(to);
	
	CString newname2  = dir2 +_T("\\") + name2 +_T(".ymd");
	CString newname3  = dir3 +_T("\\") + name3 +_T(".ymd");

	//P2N(_T("Export: %s\r\n%s\r\n"), newname2, newname3);
	CopyFile(newname2, newname3,	false);

	CString newname2a = dir2 +_T("\\") + name2 +_T(".wav");
	CString newname3a = dir3 +_T("\\") + name3 +_T(".wav");

	//P2N(_T("Export: %s\r\n%s\r\n"), newname2a, newname3a);
	CopyFile(newname2a, newname3a,	false);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::DelDep(CString &from)

{
	CString name2 = support.GetFilename(from);	
	CString dir2 = support.GetDirName(from);
	
	CString newname2  = dir2 +_T("\\") + name2 +_T(".ymd");
	_tunlink(newname2);

	CString newname2a = dir2 +_T("\\") + name2 +_T(".wav");
	_tunlink(newname2a);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnTell() 

{
	CString str2 =_T("The time is: "), str; m_tstr2.GetText(str);

	if(str == _T(""))
		return;

	// Cut the seconds off
	if(str.GetLength() > 5)
		{
		int idx = str.ReverseFind(':');
		str2 += str.Left(idx);
		}
	else
		{
		str2 += str;
		}

	int idx2 = str.Find(_T("AM"));
	if(idx2 >= 0)
		str2 +=_T(" AM");

	int idx3 = str.Find(_T("PM"));
	if(idx3 >= 0)
		str2 +=_T(" PM");

#ifdef SPEAK
	sp.PostHere(m_hWnd);
	sp.Speak(str2);
#endif
	//AfxMessageBox(_T("Tel"));	
}

void YellowDlg::OnTstr2() 
{
	OnTell();	
}

void YellowDlg::OnTstr() 

{
	CString str2 =_T("The date is: "), str; m_tstr.GetText(str);

	int idx = str.Find('(');
	int idx2 = str.Find(')');

	str2 += str.Mid(idx, idx2 - idx);

#ifdef SPEAK
	sp.Speak(str2);
#endif

}

//////////////////////////////////////////////////////////////////////////

BOOL YellowDlg::OnCommand(WPARAM wParam, LPARAM lParam) 

{
	DWORD maxcom = resolver.namearr.GetSize();

	//////////////////////////////////////////////////////////////////////
	if(wParam >= ID_RIGHT_SENDNETWORKSTICKYTO_FIRST && 
					wParam < ID_RIGHT_SENDNETWORKSTICKYTO_FIRST + maxcom)
		{
		// Figure out the machine to send to
		CXrayM *mm = (CXrayM*)FromHandlePermanent((HWND)lParam);

		//ASSERT(mm); //ASSERT(mm->magic == MENU_MAGIC);

		//mm->DumpMenu(mm);

		CString str;
		mm->GetMenuItemText(wParam - ID_RIGHT_SENDNETWORKSTICKYTO_FIRST, str);

		P2N(_T("YellowDlg::OnCommand sendto %d %s\r\n"), 
						wParam - ID_RIGHT_SENDNETWORKSTICKYTO_FIRST, str);

		if(str != _T(""))
			{
			sendto = str;
			_beginthread(_SendThreadProc, 0, (void*)this);
			}	
		}

	//////////////////////////////////////////////////////////////////////
	if(wParam >= ID_RIGHT_FOLDESTICKYTO_FIRST && 
					wParam < ID_RIGHT_FOLDESTICKYTO_FIRST + maxlist)
		{
		// Figure out the folder to copy to 
		CXrayM *mm = (CXrayM*)FromHandlePermanent((HWND)lParam);

		//ASSERT(mm); //ASSERT(mm->magic == MENU_MAGIC);

		//mm->DumpMenu(mm);

		int idx = mm->FindMenuItem(wParam);		
		CString str; mm->GetMenuItemText(idx, str);

		HTREEITEM h1 = m_tree.GetSelectedItem();
		if(h1)
			{
			HTREEITEM h2 = (HTREEITEM)mm->GetMenuItemInt(idx);

			CString curr = m_tree.GetItemText(h1);
			CString nnn  = m_tree.GetItemText(h2);
			//P2N(_T("YellowDlg::OnCommand move '%s' => '%s' (%s) \r\n"), curr, str, nnn);
			
			xMoveMemo(h1, h2);
			}
		}
	
	//////////////////////////////////////////////////////////////////////
	if(wParam == ID_RIGHT_FOLDESTICKYTO_PARENT)
		{
		// Figure out the folder to copy to 
		CXrayM *mm = (CXrayM*)FromHandlePermanent((HWND)lParam);

		//ASSERT(mm); //ASSERT(mm->magic == MENU_MAGIC);

		//mm->DumpMenu(mm);

		int idx = mm->FindMenuItem(wParam);		
		CString str; mm->GetMenuItemText(idx, str);

		HTREEITEM h1 = m_tree.GetSelectedItem();
		if(h1)
			{
			HTREEITEM h2 = (HTREEITEM)mm->GetMenuItemInt(idx);
			HTREEITEM h3 = m_tree.GetNextItem(h2, TVGN_PARENT);

			CString ccc  = m_tree.GetItemText(h1);
			CString nnn  = m_tree.GetItemText(h2);
			CString xxx  = m_tree.GetItemText(h3);

			P2N(_T("YellowDlg::OnCommand move parent '%s' => '%s'  (%s)\r\n")
								, ccc, nnn, xxx);

			xMoveMemo(h1, h3, true);
			}
		}

	return CDialog::OnCommand(wParam, lParam);
}


#if 0
   1.
      inline void ASSERT_GUI_THREAD()
   2.
      {
   3.
      const CWnd * pWnd = AfxGetMainWnd();
   4.
      const HWND hWnd = pWnd->GetSafeHwnd();
   5.
      const DWORD WindowThreadId = GetWindowThreadProcessId( hWnd, 0 );
   6.
      const DWORD thisThreadId = AfxGetThread()->m_nThreadID;
   7.
      // const bool bGuiThread = IsGUIThread(FALSE);
   8.
      // ctrlTree.InvokeRequired == false // this might work in .NET but not MFC.
   9.
       
  10.
      bool isGuiThread =
  11.
      pWnd != NULL &&
  12.
      hWnd != NULL &&
  13.
      thisThreadId == WindowThreadId;
  14.
       
  15.
      //ASSERT( isGuiThread );
  16.
       
  17.
      }


   AttachThreadInput(
    GetWindowThreadProcessId(
        ::GetForegroundWindow(),NULL),
    GetCurrentThreadId(),TRUE);

//Do our stuff here ;-)

SetForegroundWindow();
SetFocus(); //Just playing safe


//Detach the attached thread

AttachThreadInput(
    GetWindowThreadProcessId(
        ::GetForegroundWindow(),NULL),
    GetCurrentThreadId(),FALSE);

#endif


#if 0
					// Append
					
					CFile fp; 
					if(fp.Open(tmpname, CFile::modeRead))
						{
						CFile fp2; 
						if(fp2.Open(newfile, CFile::modeReadWrite))
							{
							fp2.Seek(0, SEEK_END);

							while(true)
								{
								int ret = fp.Read(buff, sizeof(buff));
								fp2.Write(buff, ret);
								if(ret < sizeof(buff))
									break;
								}		
							fp2.Close(); 
							}							
						}					
					fp.Close();
#endif


//////////////////////////////////////////////////////////////////////////
//
										
void	YellowDlg::MenuAddFolderList(CXrayM *mm)

{
	CXrayM *rr = mm->FindSubmenu(_T("Move to")); 

	if(!rr)
		return;											//oops

	rr->Clear();

	int cnt = 0;

	HTREEITEM currentItem = m_tree.GetSelectedItem();
	HTREEITEM h2 = NULL, h1 = currentItem;
	
	// Seek back to beginning
	while(true)
		{
		if(!h1)
			break;
		
		h2 = h1;	// remember
		h1 = m_tree.GetNextItem(h1, TVGN_PREVIOUS );
		}
	
	// Walk current 
	while(true)
		{
		if(!h2)
			break;

		if(h2 != currentItem)
			xAddOneFolder(rr, currentItem, h2, cnt);

		h2 = m_tree.GetNextItem(h2, TVGN_NEXT );
		cnt++;			
		}

	if(!cnt)
		{
		int idx = rr->AppendMenuItem(_T("No subfolders found on this leve"), ID_RIGHT_FOLDESTICKYTO_FIRST );
		rr->SetItemStatus(idx, MF_GRAYED);			
		}

	h1 = m_tree.GetNextItem(currentItem, TVGN_PARENT);
	if(h1)
		{
		rr->AppendMenuItem(_T(""));
		int idx = rr->AppendMenuItem(_T("Move to parent"), ID_RIGHT_FOLDESTICKYTO_PARENT);

		CString strz = m_tree.GetItemText(currentItem);
		CString str3; str3.Format(_T("Move '%s' to -> Parent folder"), strz);
		rr->SetToolTip(idx, str3);
		rr->SetItemInt(idx, (int)h1);
		}
	maxlist = cnt;
}

//////////////////////////////////////////////////////////////////////////

void	YellowDlg::xAddOneFolder(CXrayM *rr, HTREEITEM h0, HTREEITEM h1, int cntt)

{	
	CString strs = m_tree.GetItemText(h1);

	//P2N(_T("YellowDlg::xAddOneFolder '%s'\r\n"), strs);

	int idx = rr->AppendMenuItem(strs, ID_RIGHT_FOLDESTICKYTO_FIRST + cntt);

	CString strz = m_tree.GetItemText(h0);
	CString str3; str3.Format(_T("Move '%s' to -> '%s'"), strz, strs);
	rr->SetToolTip(idx, str3);
	rr->SetItemInt(idx, (int)h1);

}
	
//////////////////////////////////////////////////////////////////////////

void	YellowDlg::MenuAddNetList(CXrayM *mm)

{
	CXrayM *rr = mm->FindSubmenu(_T("Send Network")); 

	CString machine = support.Machine();

	// If this is the right menu to go for ...
	if(rr)
		{
		rr->Clear();
		
		// CResolv	
		int loop, cnt = 0, alen = resolver.namearr.GetSize();
		
		for(loop = 0; loop < alen; loop++)
			{
			CNameEntry *val = (CNameEntry*)resolver.namearr.GetAt(loop);

			// Do not show self
			if(*val->machine != machine)
				{
				CString strs; strs.Format(_T("%s::%s"), *val->machine, *val->username);
				//P2N(_T("%s %s\r\n"), *val->machine, *val->username);
				int idx = rr->AppendMenuItem(strs, ID_RIGHT_SENDNETWORKSTICKYTO_FIRST + cnt);

				CString str3; str3.Format(_T("Send this sticky to %s"), strs);
				rr->SetToolTip(idx, str3);

				cnt++;
				}
			}

		if(!cnt)
			{
			int idx = rr->AppendMenuItem(_T("No network peers at this time"), ID_RIGHT_SENDNETWORKSTICKYTO_FIRST + loop);
			rr->SetItemStatus(idx, MF_GRAYED);
			}
		}
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::Encrypt_Clipboard(int ver)

{
	//P2N(_T("YellowDlg::Encrypt_Clipboard\r\n"));

	//cryptver = ver;

	CString text, head;
	OpenClipboard();

	HANDLE htext = GetClipboardData(CF_UNICODETEXT);

	if (htext)
		{
		cboardtype  = CF_UNICODETEXT;
	
		LPVOID ptr = GlobalLock(htext);	//ASSERT(ptr);
		text = (const TCHAR *)ptr;
		GlobalUnlock(htext);
		}
	if(text == _T(""))
		{
		char *textA = NULL; int len = 0;

		// Try regulartext
		HANDLE htext2 = GetClipboardData(CF_UNICODETEXT);
		if (htext2)
			{
			cboardtype  = CF_TEXT;	
			
			LPVOID ptr = GlobalLock(htext2);	//ASSERT(ptr);
			textA = (char *)ptr;
			GlobalUnlock(htext2);

			if(textA != NULL)
				len = strlen(textA);
			}

		// Any of the fail conditions present?
		if(textA == NULL || len == 0 || htext2 == 0)
			{
			AfxMessageBox(_T("No text on clipboard."));
			CloseClipboard();
			return;
			}

		//TCHAR *out;	
		//misc.AnsiToUnicode(textA, &out);
		//P2N(_T("Converted clipboard to '%s'\r\n"), out);
		}

	CloseClipboard();

	//D2N(text, 16);
	//P2N(_T("YellowDlg::Encrypt_Clipboard '%s'\r\n"), text);

	if(!Encrypt(text))
		return;

	if(!OpenClipboard())
		{
		AfxMessageBox(_T("Cannot open clipboard for writing"));
		return;
		}

	// Alloc it, Assemble it
	int biglen = text.GetLength() * sizeof(TCHAR) + sizeof(TCHAR);
	HANDLE bighandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, biglen);
	if(!bighandle)
		{
		AfxMessageBox(_T("Cannot allocate memory for clipboard contents."));
		return;
		}

	TCHAR *bigptr = (TCHAR *)GlobalLock(bighandle);
	if(!bigptr)
		{
		AfxMessageBox(_T("Cannot lock memory for clipboard contents."));
		return;
		}

	EmptyClipboard();

	memcpy(bigptr, text, biglen);

	::GlobalUnlock(bighandle);	
	//::SetClipboardData(CF_TEXT, bighandle);
	::SetClipboardData(CF_UNICODETEXT, bighandle);

	CloseClipboard();	
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::Decrypt_Clipboard(int ver)

{
	//P2N(_T("YellowDlg::Decrypt_Clipboard \r\n"));

	//cryptver = ver;
	CString text, head;
	OpenClipboard();

	HANDLE htext = GetClipboardData(CF_UNICODETEXT);

	if (htext)
		{
		LPVOID ptr = GlobalLock(htext);	//ASSERT(ptr);
		text = (const TCHAR *)ptr;
		GlobalUnlock(htext);
		}

	EmptyClipboard();
	CloseClipboard();

	if(text == _T(""))
		{
		AfxMessageBox(_T("No text on clipboard.\r\n"));
		return;
		}

	//P2N(_T("YellowDlg::Decrypt_Clipboard '%s'\r\n"), text);

	if(!Decrypt(text))
		return;

	if(!OpenClipboard())
		{
		AfxMessageBox(_T("Cannot open clipboard"));
		return;
		}

	// Alloc it, Assemble it
	int biglen = text.GetLength() * sizeof(TCHAR) + sizeof(TCHAR);
	HANDLE bighandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, biglen);
	if(!bighandle)
		{
		AfxMessageBox(_T("Cannot allocate memory for clipboard contents"));
		return;
		}

	TCHAR *bigptr = (TCHAR *)GlobalLock(bighandle);
	if(!bigptr)
		{
		AfxMessageBox(_T("Cannot lock memory for clipboard contents"));
		return;
		}

	memcpy(bigptr, text, biglen);

	EmptyClipboard();

	::GlobalUnlock(bighandle);	
	::SetClipboardData(CF_UNICODETEXT, bighandle);

	CloseClipboard();

	//cryptver = 3;
}

//////////////////////////////////////////////////////////////////////////

LRESULT YellowDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 

{
	if(WM_POWERBROADCAST == message)
		{
		//P2N(_T("Power event %d\r\n"), wParam);
	
		if(wParam == PBT_APMRESUMEAUTOMATIC)
			{
			//P2N(_T("Power broadcast event PBT_APMRESUMEAUTOMATIC\r\n"));
			
			// Do something for system wake up
			StopScreenSaver();
			}
		
		if(wParam == PBT_APMPOWERSTATUSCHANGE)
			{
			SYSTEM_POWER_STATUS sps;
			GetSystemPowerStatus(&sps);

			P2N(_T("Power broadcast event PBT_APMPOWERSTATUSCHANGE \r\n"));
	
			// Change in power status
			if(sps.ACLineStatus != inisps.ACLineStatus)
				{
				inisps.ACLineStatus = sps.ACLineStatus;

				if(sps.ACLineStatus == 1)
					{
					ShowStatus(_T("AC LINE CONNECTED"));
					//P2N(_T("\r\nAC LINE CONNECTED\r\n"));
					}
				else
					{
					ShowStatus(_T("AC LINE DISCONNECTED"));
					//P2N(_T("\r\nAC LINE DICONNECTED\r\n"));
					}
				}
			}

		if(wParam == PBT_APMQUERYSUSPEND)
			{			
			if(stop_standby)
				{
				//P2N(_T("CryptoSticky prevented the system to enter standby.\r\n"));
				ShowStatus(_T("CryptoSticky prevented the system to enter standby."));
				return BROADCAST_QUERY_DENY;
				}
			else
				{
				//P2N(_T("CryptoSticky allowed the system to enter standby.\r\n"));
				return true;
				}
			}

		if(wParam == PBT_APMBATTERYLOW)
			{
			// Battery low, aborting copy proress
			ShowStatus(_T("Battery low ...."));
			//stop = true;
			}
		}	

	return CDialog::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnSetFocus(CWnd* pOldWnd) 

{
	CDialog::OnSetFocus(pOldWnd);
	//P2N(_T("YellowDlg::OnSetFocus"));	
}

//////////////////////////////////////////////////////////////////////////

void YellowDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 

{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	//P2N(_T("YellowDlg::OnActivate %d"), nState);
	lastactivated = nState;
}

//////////////////////////////////////////////////////////////////////////
// Make it alertable 15 seconds before alarm

void YellowDlg::WakeMachine()

{
	// Create an unnamed waitable timer.
	HANDLE hh = CreateWaitableTimer(NULL, false,_T("CryptoSticky"));

	// Initate wakeup timer  CTime		
	CTime tm(alarm_tt);
	CTimeSpan ts( 0, 0, 0, 15);
	tm -= ts;

	SYSTEMTIME st; tm.GetAsSystemTime(st); 		
	FILETIME ft, ft2; SystemTimeToFileTime(&st, &ft);
	LocalFileTimeToFileTime(&ft, &ft2);
	
	// Just checking ...
	//CTime tm2(ft2);
	//P2N(_T("Waking the computer up at UTC '%s'\r\n"), tm2.Format(_T("%c")));

	BOOL ret = SetWaitableTimer(hh, (LARGE_INTEGER *)&ft2, 0, NULL, NULL, true);

	//P2N(_T("SetWaitableTimer ret=%d (%d)\r\n"), ret, GetLastError());
	
	// Testing again
	//WaitForSingleObject(hh, INFINITE);
	//P2N(_T("Waiting done\r\n"));
}

//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK KillScreenSaverFunc(HWND hwnd, LPARAM lParam) 

{ 
	if(IsWindowVisible(hwnd)) 
		{
		TCHAR str[_MAX_PATH];
		TCHAR str2[_MAX_PATH];
		
		::GetWindowText(hwnd, str, sizeof(str));
		::GetClassName(hwnd, str2, sizeof(str2));


		WNDCLASSEX  cl;
		int aaaa = GetClassLong(hwnd, GCW_ATOM);
		
		//int ret = GetClassInfo(AfxGetInstanceHandle(), (const TCHAR*)aaaa, &cl);
		int ret = GetClassInfoEx(NULL, str2, &cl);

		P2N(_T("Iterated window '%s' class='%s'\r\n"), str, str2);

		//PostMessage(hwnd, WM_CLOSE, 0, 0); 
		}

	return TRUE; 
} 

//////////////////////////////////////////////////////////////////////////

BOOL CALLBACK EnumDesktopProc(
		LPTSTR lpszDesktop,			// name of a desktop
			LPARAM lParam				// value specified in EnumDesktops call
)

{
	P2N(_T("Desktop to enum: '%s'\r\n"), lpszDesktop);

	HDESK hdesk; 
	hdesk = OpenDesktop(lpszDesktop, 0, FALSE, DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS); 

	EnumDesktopWindows(hdesk, KillScreenSaverFunc, 0);
	return 0;
}
 
//////////////////////////////////////////////////////////////////////////

void	YellowDlg::StopScreenSaver()

{
	//P2N(_T("YellowDlg::StopScreenSaver\r\n"));

	SetThreadExecutionState(ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
}

#if 0		

	//POINT pp;	GetCursorPos(&pp);	SetCursorPos(pp.x + 10, pp.y + 10);
	//EnumDesktops(GetProcessWindowStation(), EnumDesktopProc, 0);

	//HANDLE    hDevice = CreateFile(_T("\\\\.\\Display"), 
    //    0, FILE_SHARE_READ | FILE_SHARE_WRITE, 
    //    NULL, OPEN_EXISTING, 0, NULL); 
 
	//P2N(_T("handle=%d\r\n"),hDevice);
    //if (hDevice != NULL) 
	//	{
	//	
	//	//RequestDeviceWakeup (hDevice);
    // 	}
 
	HDESK hdesk; 
	hdesk = OpenDesktop(TEXT(_T("Screen-saver")), 0, FALSE, DESKTOP_READOBJECTS | DESKTOP_WRITEOBJECTS); 

	if (hdesk) 
		{ 
		P2N(_T("Got desktop\r\n"));
		EnumDesktopWindows(hdesk, KillScreenSaverFunc, 0);
		CloseDesktop(hdesk); 
		} 
	else 
		{ 
		P2N(_T("No screen saver desktop\r\n"));
		
		// Windows 2000 and later: 
		// If there is no screen saver desktop, the screen saver 
		// is on the default desktop. Close it by sending a WM_CLOSE. 

		EnumDesktopWindows(hdesk, KillScreenSaverFunc, 0);
		
		::PostMessage(GetForegroundWindow()->m_hWnd, WM_CLOSE, 0, 0L); 
		}	 
	
	keybd_event(VK_SHIFT, 0, 0, 0);
	support.YieldToWinEx();
	keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);

	BOOL var;
	BOOL ret = SystemParametersInfo(SPI_GETSCREENSAVEACTIVE, 0, &var, 0);

	P2N(_T("Screen saver was running %d\r\n"), var);
	
	BOOL ret2 = SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 0, 0, 0);

	KEYBDINPUT ki; 
	ki.wVk = 10;
	ki.time = time(NULL);
	ki.wScan = 10;
	ki.dwFlags = 0;

	INPUT in; in.type = INPUT_KEYBOARD;
	in.ki = ki;

	SendInput(1, & in, sizeof(in)); 

#endif

//////////////////////////////////////////////////////////////////////////

void YellowDlg::xMoveMemo(HTREEITEM hfrom, HTREEITEM hto, int ignore)

{
	CString str3, str4, str5;
	
	//HTREEITEM h1 = m_tree.GetSelectedItem();

    // Anything selected ?
    if(!hfrom)
       {
       AfxMessageBox(_T("Please select a Sticky to move first."));
       return;
       }

	//if(!hto)
    //  {
    //   //AfxMessageBox(_T("Please select a folder to move to first."));
    //   //return;
    //  }

    // Any Children ?
    HTREEITEM h2 = m_tree.GetNextItem(hfrom, TVGN_CHILD);
    if(h2)
       {
       AfxMessageBox(_T("This Sticky has leaf (child) items, cannot move."));
       return;		
       }

	CString curr = m_tree.GetItemText(hfrom);
	CString nnn  = m_tree.GetItemText(hto);
			
	//P2N(_T("YellowDlg::MoveMemo move '%s' => '%s'\r\n"), curr, nnn);
			
	BuildTree(str3, hfrom);	BuildTree(str4, hto);
	
	//P2N(_T("YellowDlg::MoveMemo move from '%s'\r\n"), str3);
	//P2N(_T("YellowDlg::MoveMemo move to   '%s'\r\n"), str4);
	
	// Create memo location from last level
	int idx = str3.ReverseFind('\\');
	if(idx >= 0)
		{
		str3 = str3.Left(idx);

		//P2N(_T("YellowDlg::MoveMemo move newx  %d '%s'\r\n"), idx, str4);

		int idx2 = str3.ReverseFind('\\');
		if(idx2 >= 0)
			{
			str3 = str3.Mid(idx2 + 1);
			}

		if(hto)
			{
			str5 = str4; str5 += str3; str5 +=_T("\\");
			}
		else
			{
			str5 =_T("\\"); str5 += str3; str5 +=_T("\\");  // Copy to root
			}
		}
		
	P2N(_T("YellowDlg::MoveMemo move newy '%s'\r\n"), str5);
	
	CString fname1, fname2;
	GetNodePath(fname1, hfrom);	GetNodePath(fname2, hto);

	//P2N(_T("YellowDlg::MoveMemo move from '%s'\r\n"), fname1);
	//P2N(_T("YellowDlg::MoveMemo move to   '%s'\r\n"), fname2);
	
	MemoSupp memo; 
	
	if(!memo.Read(fname1))
		{
		AfxMessageBox(_T("Cannot read source memo."));
		return;
		}

	RemoveAlarm(fname1);

	memo.initial = str5; 
	CString newname = memo.MakeFileName();

	//P2N(_T("YellowDlg::MoveMemo newname   '%s'\r\n"), newname);
	
	if(_taccess(newname, 0) >= 0)
		{
		AfxMessageBox(_T("Target memo already exist, cannot rename."));
		return;
		}

	if(!memo.Write(newname))
		{
		AfxMessageBox(_T("Cannot write target memo."));
		return;
		}

	// Dependents also
	CopyDep(fname1, newname);

	// Remove current ones
	_tunlink(fname1);	DelDep(fname1);

	// Hide, and remove old from tree
	ShowOne(hfrom, false);	m_tree.DeleteItem(hfrom);

	// Add the new one in the new location
	m_tree.InsertItem(memo.head, hto);

	memo.CheckAlarm();

	// Let the user know
	CString tmp; tmp.Format(_T("Moved memo '%s' to '%s'"), curr, nnn);
	ShowStatus(tmp);

	ArmNext();

	return;
}

//////////////////////////////////////////////////////////////////////////

void	YellowDlg::RemoveAlarm(const TCHAR *fname)

{
	HKEY	key;
	LONG	ret = RegOpenKey(HKEY_CURRENT_USER, keystr, &key);

	// Remove registry entry from alarm list
	TCHAR kname[_MAX_PATH]; TCHAR kval[_MAX_PATH];

	int idx = 0;

	while(true)
		{
		DWORD ksize = _MAX_PATH, kvsize = _MAX_PATH, ktype;

		kname[0] = '\0'; kval[0]  = '\0';

		LONG ret = RegEnumValue(key, idx, kname, &ksize, 0, &ktype, 
			(BYTE *)kval, &kvsize);

		//P2N(_T("Reg key %s val = %d -> ret = %d\r\n"), kname, ret);

		if(ret != ERROR_SUCCESS && ret != ERROR_NO_MORE_ITEMS)
			break;
		
		//time_t att =  (*(time_t*)kvname);

		if(ktype == REG_SZ)
			{
			//P2N(_T("Reg delete %s - %s\r\n"), kname, support.time_t2str(att));
		
			if(_tcscmp(kname, fname) == 0)
				{
				LONG ret = RegDeleteValue(key, kname);
				}
			else if(_taccess(kname, 0) < 0)
				{
				LONG ret = RegDeleteValue(key, kname);
				}
			}

		idx++;

		if(ret == ERROR_NO_MORE_ITEMS)
			{
			//P2N(_T("No more Items\r\n"));
			break;
			}
		}
}

HBRUSH YellowDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)

{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);


	//P2N(_T("YellowDlg::OnCtlColor nCtlColor=%x dlgback=%x\r\n"), nCtlColor, dlgback);

	//hbr = CreateSolidBrush(dlgback);

	if(nCtlColor == CTLCOLOR_STATIC)
		{
		hbr = CreateSolidBrush(dlgback);
		}
	else if(nCtlColor == CTLCOLOR_EDIT)
		{
		hbr = CreateSolidBrush(dlgback);
		}
	else if(nCtlColor == CTLCOLOR_LISTBOX)
		{
		hbr = CreateSolidBrush(dlgback);
		}
	else if(nCtlColor == CTLCOLOR_DLG)
		{
		hbr = CreateSolidBrush(dlgback);
		}
	else
		{		
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}

	return hbr;
}

int YellowDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL YellowDlg::OnEraseBkgnd(CDC* pDC)

{
	CRect rec; GetClientRect(rec);
	
	pDC->FillSolidRect(rec, m_BackColor);

	return true;
	//return CDialog::OnEraseBkgnd(pDC);
}

void YellowDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonDown(nFlags, point);
}

void YellowDlg::OnStnClickedTstr2()
{
	// TODO: Add your control notification handler code here
}

void YellowDlg::OnRight2Searchforsticky()
{
	P2N(_T("Search for sticky\r\n"));

	SearchDLG sdlg(this); sdlg.m_BackColor = dlgback;

	sdlg.DoModal();
}


//////////////////////////////////////////////////////////////////////////

HTREEITEM	YellowDlg::Memo2Handle(CString &str)

{
	HTREEITEM hh = NULL;

	// Try original file name
	POSITION pos3 = detlist.GetHeadPosition();
	while(true)
		{
		if(!pos3) break;

		MemoDetail *md = (MemoDetail *) detlist.GetNext(pos3);
		//P2N(_T("YellowDlg::Memo2Handle looking %x %s\r\n"), md->item, *md->fname);
			
		if(md->fname->Compare(str) == 0)
			{
			//P2N(_T("YellowDlg::CreateMemoWin Found: %x %s\r\n"), md->item, *md->fname);
			hh = md->item;
			break;
			}
		}
	return hh;
}


void YellowDlg::OnBnClickedButton40()
{
	// TODO: Add your control notification handler code here
	P2N(_T("Search for str\r\n"));
	OnRightSearch();
}

#if 0

void YellowDlg::OnPaint()
{

#if 0
	
	//CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
#endif

	CDialog::OnPaint();
}

#endif

void YellowDlg::OnBnClickedButton41()
{
	P2N(_T("butt 41"));
}


void YellowDlg::OnBnClickedButton42()
{
	P2N(_T("butt 42"));
}

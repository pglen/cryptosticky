
/* =====[ MemoSupp.cpp ]================================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the 
					MemoSupp.cpp. Automatic unicode hehaviour.
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE         BY            DESCRIPTION                       
      ----  --------	-----------  ----------------------------   
      0.00  6/3/2009	Peter Glen   Initial version.                         
      0.00  apr/6/2016  Peter Glen   Updated auto MBCS file handling
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include <direct.h>
#include <io.h>

#include "CryptoSticky.h"
#include "xArchive.h"

#include "support.h"
#include "mxpad.h"

// Disable security warnings TODO for later
#pragma warning (disable : 4996)

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[] = _T(__FILE__);
#define new DEBUG_NEW
#endif



#define REMOVECRLF(idxx, strx)					\
	int idxx = strx.Find('\r');					\
	if(idxx >= 0) strx = strx.Left(idxx)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MemoSupp::MemoSupp()

{
	flags.ver			= 2;	
	flags.enabled		= 1;	
	flags.centered		= 0;	
	flags.red			= 0;	
	flags.ala			= 0;	
	flags.bugme			= 0;	
	flags.soundon   	= 0;	
	flags.persist   	= 0;	
	
	flags.date			= "";	
	flags.time			= "";	
	flags.sound			= "";

	flags.encrypt		= 0;
	flags.textcol		= 0;
	flags.headcol		= 0;
	flags.textsize		= 0;
	flags.headsize		= 0;

	flags.headfontcol	=	0;
	flags.textfontcol	=	0;

}

MemoSupp::~MemoSupp()

{

}

//////////////////////////////////////////////////////////////////////////
// Ret number of characters written

int		MemoSupp::WriteFlags(CxArchive &ar, MemoSupp::_MemoData  *pflags)

{
	int ret = 0;

	CString str;

	if(pflags->date == "") pflags->date = "none";
	if(pflags->time == "") pflags->time = "none";
	if(pflags->sound == "") pflags->sound = "none";

	Escape_Space(pflags->date);
	Escape_Space(pflags->time);
	Escape_Space(pflags->sound);

	str.Format(
			_T("%d %d %d %d %d %d %d %d %s %s %s %d %d %d %d %d %d %d %d %d\r\n"),
				pflags->ver, pflags->enabled,
					pflags->centered, pflags->red, pflags->ala,
						pflags->bugme, pflags->soundon, pflags->persist,
							pflags->date, pflags->time, pflags->sound, 							
				pflags->encrypt, pflags->textcol, pflags->headcol,
					pflags->textsize, pflags->headsize,  
							pflags->beep, pflags->readt,
							// New in version 3
							pflags->headfontcol, pflags->textfontcol);							
					
	//P2N(_T("MemoSupp::WriteFlags %s\r\n"), str);

	ar.WriteString(str);

	//DumpFlags(&flags);

	return ret;
}

//////////////////////////////////////////////////////////////////////////
//

int		MemoSupp::ReadFlags(CxArchive &ar, MemoSupp::_MemoData  *pflags)

{
	int ret = 0;

	CString str;

	ar.ReadString(str);

	//P2N(_T("MemoSupp::ReadFlags '%s'\r\n"), str);

	_stscanf(str,
			_T("%d %d %d %d %d %d %d %d %s %s %s %d %d %d %d %d %d %d"),
				&pflags->ver, &pflags->enabled,
					&pflags->centered, &pflags->red, &pflags->ala,
						&pflags->bugme, &pflags->soundon, &pflags->persist,
							pflags->date.GetBuffer(_MAX_PATH),
								pflags->time.GetBuffer(_MAX_PATH),
									pflags->sound.GetBuffer(_MAX_PATH), 
			&pflags->encrypt, &pflags->textcol, &pflags->headcol,
				&pflags->textsize, &pflags->headsize, &pflags->beep, &pflags->readt);

#if 0
	_stscanf_s(str,
			"%d %d %d %d %d %d %d %d %s %s %s %d %d %d %d %d %d %d %d %d",
				&pflags->ver, &pflags->enabled,
					&pflags->centered, &pflags->red, &pflags->ala,
						&pflags->bugme, &pflags->soundon, &pflags->persist,
							pflags->date.GetBuffer(_MAX_PATH), _MAX_PATH,
								pflags->time.GetBuffer(_MAX_PATH), _MAX_PATH,
									pflags->sound.GetBuffer(_MAX_PATH), _MAX_PATH, 
			&pflags->encrypt, &pflags->textcol, &pflags->headcol,
				&pflags->textsize, &pflags->headsize, &pflags->beep, &pflags->readt,
					// New in version 3
					&pflags->headfontcol, &pflags->textfontcol							
					);
#endif

	pflags->date.ReleaseBuffer();
	pflags->time.ReleaseBuffer();
	pflags->sound.ReleaseBuffer();

	UnEscape_Space(pflags->date);
	UnEscape_Space(pflags->time);
	UnEscape_Space(pflags->sound);

	return ret;
}

//////////////////////////////////////////////////////////////////////////
// Create file name
	
CString	MemoSupp::MakeFileName()

{
	CString fname;
	
	fname.Format(_T("%s%08x.txt"), dataroot, support.HashString(initial));

	return fname;
}
		
//////////////////////////////////////////////////////////////////////////

int		MemoSupp::Write(const TCHAR *fname)

{	
	int ucode = false;
#ifdef _MBCS 
		ucode = true;
#endif
	
	P2N(_T("MemoSupp::Write %s ucode=%d size=%d\r\n"), fname, ucode, sizeof(TCHAR));
	//Dump();

	int perm = CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite;
	//CFile cf;
	CxArchive ar;

	//if(_taccess(fname, 0) < 0)
	//	{
	//	// Force new ones to reflect program options
	//	ar.multibyte = ucode;
	//	}

	if(!ar.Open(fname, perm, ucode))
		{
		//TCHAR errstr[_MAX_PATH];	strerror_s(errstr, _MAX_PATH, errno);
		TCHAR * errstr = _tcserror(errno);

		CString str; str.Format(
				_T("Cannot save Sticky file onto permanent storage. (%s)"),
						errstr);
	    AfxMessageBox(str);

		return false;
		}
	
	CString crlf(_T("\r\n"));
    
    ar.WriteString(initial); ar.WriteString(crlf);

	WriteFlags(ar, &flags);
	
	ar.WriteString(head); ar.WriteString(crlf);

	P2N(_T("MemoSupp::Write body '%s'\r\n"), body);
	ar.WriteString(body); 
	
	return true;
}

//////////////////////////////////////////////////////////////////////////

int		MemoSupp::Read(const TCHAR *fname)

{
	int ret = 0;

	//filename = fname;

	//P2N(_T("MemoSupp::Read %s\r\n"), fname);

    //CFile cf;  
    //if(cf.Open(fname, CFile::modeRead) == 0)
	//	{
	//	//P2N(_T("MemoSupp::Read cannot read %s\r\n"), fname);
	//	return false;
	//	}
	
	initial = head = body = "";

	CxArchive ar; // (&cf, CArchive::load);
	
	if(!ar.Open(fname, CFile::modeRead | CFile::shareDenyNone))
		{
		P2N(_T("YellowDlg::GetMemoItem - no file '%s'\r\n"), fname);
		return false;
		}

	// Read initial line, flags and header
	ar.ReadString(initial); REMOVECRLF(idx2, initial);
	
	ReadFlags(ar, &flags);	
	
	ar.ReadString(head);  REMOVECRLF(idx3, head);
	
	//P2N(_T("MemoSupp::Read Head '%s'\r\n"), head);

	CString line;
	while(ar.ReadString(line))
		{
        body += line; // not removing CRLF // "\r\n";
		}	
	//P2N(_T("MemoSupp::Read Body '%s'\r\n"), body);
		
	//ar.Close();	

	//DumpFlags(&flags);
	//Dump();

	return true;
} 

//////////////////////////////////////////////////////////////////////////

void	MemoSupp::Dump()

{
	P2N(_T("---- Initial:\r\n%s\r\n---- end Initial\r\n"), initial);
	
	DumpFlags(&flags);

	P2N(_T("---- Head:\r\n%s\r\n----- end Head\r\n"), head);
	P2N(_T("---- Body:\r\n%s\r\n----- end Body\r\n"), body);
}

//////////////////////////////////////////////////////////////////////////

void	MemoSupp::DumpFlags( MemoSupp::_MemoData  *pflags)

{
	//P2N(_T("%s\r\n"), head);
	P2N(_T("ver=%d enabled=%d centered=%d red=%d ala=%d bugme=%d ")
					_T("soundon=%d persist=%d date=%s time=%s \r\n")
						_T("sound=%s encrypt=%d textcol=%d headcol=%d textsize=%d headsize=%d\r\n")
							_T("headfontcol=%d textfontcol=%d\r\n"), 
				pflags->ver, pflags->enabled,
					pflags->centered, pflags->red, pflags->ala,
						pflags->bugme, pflags->soundon, pflags->persist,
							pflags->date, pflags->time, pflags->sound, 							
				pflags->encrypt, pflags->textcol, pflags->headcol,
					pflags->textsize, pflags->headsize,
					pflags->headfontcol, pflags->textfontcol);		
					
}

//////////////////////////////////////////////////////////////////////////
// Replace " " with %20

void	MemoSupp::Escape_Space(CString &str)

{
	CString nstr;

	TCHAR *ptr = str.GetBuffer(str.GetLength());

	while(true)
		{
		TCHAR chh = *ptr++;
		if(!chh) break;

		switch (chh)
			{
			case ' ' :
				nstr += "%20"; break;

			case '%' :
				nstr += "%%"; break;

			default:
				nstr += chh; break;
			}
		}
	str.ReleaseBuffer();

	str = nstr;
}

//////////////////////////////////////////////////////////////////////////
// Replace " " with %20

void	MemoSupp::UnEscape_Space(CString &str)

{
	CString nstr;

	TCHAR *ptr = str.GetBuffer(str.GetLength());

	while(true)
		{
		TCHAR chh = *ptr++;
		if(!chh) break;

		switch (chh)
			{
			case '%' :
				{
				if(*ptr == '%')
					{
					nstr += "%"; ptr++; break;
					}
				else if(_tstoi(ptr) == 20)
					{
					nstr += ' '; ptr += sizeof(char); break;
					}
				}

			default:
				nstr += chh; break;
			}
		}
	str.ReleaseBuffer();

	str = nstr;
}

//////////////////////////////////////////////////////////////////////////

void	MemoSupp::CheckAlarm()

{
	// Save it to the registry
	int Day = 0, Month = 0, Year = 0, Hour = 0, Minute = 0;

	//_stscanf_s(flags.date, "%02d-%02d-%04d", &Month, &Day, &Year);
	//_stscanf_s(flags.time, "%02d:%02d", &Hour, &Minute);
	_stscanf(flags.date, _T("%02d-%02d-%04d"), &Month, &Day, &Year);
	_stscanf(flags.time, _T("%02d:%02d"), &Hour, &Minute);
	
	CTime tme;

	if(BETWEEN_DATES(Year, 0, 2038) && BETWEEN_DATES(Day, 0, 32) && BETWEEN_DATES(Month, 0, 13))
		{
		//P2N(_T("Valid date %02d-%02d-%04d  %02d:%02d\r\n"), Day, Month, Year, Hour, Minute);
		CTime tme2(Year, Month, Day, Hour, Minute, 0);
		tme = tme2;
		}
	else
		{
		//P2N(_T("OnOK invalid date %02d-%02d-%04d  %02d:%02d\r\n"), Day, Month, Year, Hour, Minute);
		
		time_t tt = time(NULL); tt -= 2;
		CTime tme2(tt);
		tme = tme2;
		}
	
	// Override no alarm with date/time in the past
	if(!flags.ala)
		{
		//P2N(_T("No alarm set\r\n"));
		time_t tt = time(NULL); tt -= 2;
		CTime tme2(tt);
		tme = tme2;
		}

	// Is the alarm in the future? -> Write it up
	struct tm stm; 	tme.GetLocalTm(&stm);
	time_t ttt = mktime(&stm), tnow = time(NULL);
	
	if(ttt > tnow)
		{
		CString newfile = MakeFileName();
		//P2N(_T("Saving alarm '%s' %02d-%02d-%04d  %02d:%02d\r\n"), 
		//									newfile, Day, Month, Year, Hour, Minute);		

		CString tttstr = support.time_t2str(ttt);
		//P2N(_T("Saving alarm2 '%s'\r\n"), tttstr);

		CString section(_T("Alarms"));

		AfxGetApp()->WriteProfileString(section, newfile, tttstr);
		//AfxGetApp()->WriteProfileInt(section, newfile, (int)ttt);

		}
	else
		{
		//P2N(_T("NOT saving %02d-%02d-%04d  %02d:%02d\r\n"), Day, Month, Year, Hour, Minute);		
		}

}
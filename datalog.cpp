
/* =====[ datalog.cpp ]========================================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the datalog.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/17/2010  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

//////////////////////////////////////////////////////////////////////////
// Logging subsystem

#include "stdafx.h"
#include "CryptoSticky.h"
#include "datalog.h"

// Disable security warnings TODO for later
#pragma warning (disable : 4996)

static 	TCHAR    szOutString[1000];

CDataLog datalog;

CDataLog::CDataLog() 

{
}

CDataLog::~CDataLog()

{ 
	if(datalog.m_hFile  != CFile::hFileNull)
		{
		P2N(_T("CDataLog::~CDataLog Closing log\r\n"));
		datalog.Close();
		}
};

//////////////////////////////////////////////////////////////////////////

void	CDataLog::delete_datalog()

{
	close_datalog();

	_tunlink(logfile);	
	
	open_datalog();
}

void	CDataLog::open_datalog()

{
	if(logfile == "")
		{
		CString str(appdata); 
		logfile = str + "activity.txt";
		}

	//P2N(_T("CDataLog::open_datalog '%s' \r\n"), logfile);

	if(!datalog.Open(logfile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite | CFile::shareDenyNone))
		{
		//P2N(_T("Cannot open 'datalog' log file, Logging will not be available.\r\n"));
		AfxMessageBox(_T("Cannot open 'datalog' log file, Logging will not be available.\r\n"));
		}
	else
		{
		//P2N(_T("Opened DataLog handle %x  %s\r\n"), datalog.m_hFile, logfile);
		datalog.Seek(0, CFile::end );
		}
}

void	CDataLog::close_datalog()

{
	PrintToLog(_T("Ended CryptoSticky Application\r\n"));

	//P2N(_T("CDataLog::close_datalog Closing log\r\n"));
		
	datalog.Close();
}

//////////////////////////////////////////////////////////////////////////

int     CDataLog::PrintToLog(const TCHAR *Format, ...)

{
    TCHAR    *asc = NULL;

	// Do nothing if log is not opened
	if(datalog.m_hFile == CFile::hFileNull )
		{
		P2N(_T("Log attempted to log to NULL file\r\n"));
		return 0;
		}

	CTime ct = CTime::GetCurrentTime();
	CString datestr = ct.Format(_T("%A, %B %d, %Y - %I:%M %p "));

	//P2N(_T("DateStr %s\r\n"), datestr);	
	
	datalog.Write(datestr, TXT2BYTE(datestr.GetLength()));

	va_list ArgList;  va_start(ArgList, Format);
    //int len = _vsntprintf_s(szOutString, TSIZEOF(szOutString), _TRUNCATE, Format, ArgList);
	int len = _vstprintf(szOutString, Format, ArgList);
	datalog.Write(szOutString, TXT2BYTE(len));

    return(0);
}

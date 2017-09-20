
/* =====[ xArchive.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the xArchive.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// xArchive.h: interface for the CxArchive class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XARCHIVE_H__F6D54744_2946_412C_A874_11A32B8798F3__INCLUDED_)
#define AFX_XARCHIVE_H__F6D54744_2946_412C_A874_11A32B8798F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ARCHIVE_BUFFSIZE 4096;

class CxArchive

{
public:
	
	CxArchive();
	virtual ~CxArchive();

	int		Open(const TCHAR *fname, int mode, int uniflag = false);
	void	Close();

	int		ReadString(CString &str);
	int		WriteString(CString &str);
	int		Rewind();
	int		multibyte;

private:

	int		head, tail;
	char	*contents;
	TCHAR	*contents2;
	int		prog, bufflen;

	CString filename;
	CFile cfile;

	//TCHAR *buff;
};

#endif // !defined(AFX_XARCHIVE_H__F6D54744_2946_412C_A874_11A32B8798F3__INCLUDED_)

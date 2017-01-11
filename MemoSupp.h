
/* =====[ MemoSupp.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the MemoSupp.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// MemoSupp.h: interface for the MemoSupp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMOSUPP_H__A4D1CA00_997B_497F_8B9D_DB19BD32ECC7__INCLUDED_)
#define AFX_MEMOSUPP_H__A4D1CA00_997B_497F_8B9D_DB19BD32ECC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xArchive.h"

#define		BETWEEN_DATES(val, low, up)  (((val < up) && (val > low)) ? 1 : 0)
#undef BETWEEN


#define	INIT_FLAGS(flags)		\
	flags.ver			= 1;	\
	flags.enabled		= 1;	\
	flags.centered		= 0;	\
	flags.red			= 0;	\
	flags.ala			= 0;	\
	flags.bugme			= 0;	\
	flags.soundon   	= 0;	\
	flags.persist   	= 0;	\
	flags.date			= "";	\
	flags.time			= "";	\
	flags.sound			= "";



class MemoSupp
{

//////////////////////////////////////////////////////////////////////////
typedef struct _MemoData

{
	int		ver;
	int		enabled, centered;
	int		red;
	int		ala;
	int		bugme; 
	int		soundon;
	int		persist;
	int		beep;
	int		readt;

	CString date;
	CString time;
	CString sound;

	int		encrypt;
	int		textcol;
	int		headcol;
	int		textsize;
	int		headsize;

	// New in version 3
	int		headfontcol;
	int		textfontcol;

	CString image;


} MemoData;

public:

	MemoSupp();
	virtual ~MemoSupp();
	
	void	Dump();
	void	DumpFlags( MemoSupp::_MemoData  *pflags);
	int		Read(const TCHAR *fname);
	int		Write(const TCHAR *fname);
	void	CheckAlarm();

	CString	MakeFileName();
	//CString	filename;

	MemoData	flags;
	CString		initial, head, body;

	
protected:
	
	int		WriteFlags(CxArchive &ar, MemoSupp::_MemoData *flags);
	int		ReadFlags(CxArchive &ar,  MemoSupp::_MemoData *flags);

	void	Escape_Space(CString &str);
	void	UnEscape_Space(CString &str);
};

#endif // !defined(AFX_MEMOSUPP_H__A4D1CA00_997B_497F_8B9D_DB19BD32ECC7__INCLUDED_)

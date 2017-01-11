
/* =====[ Resolv.h ]========================================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Resolv.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/15/2010  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#if !defined(AFX_RESOLV_H__1600312F_29C4_4C29_A7C2_364BB0C58A7B__INCLUDED_)
#define AFX_RESOLV_H__1600312F_29C4_4C29_A7C2_364BB0C58A7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock2.h"

#define		YM_RES_PORT		10000
#define		YM_DATA_PORT	10001
#define		YM_MEMO_PORT	10003

class CNameEntry

{  
public:

	int		regtime;

	int		res_port, data_port;

	CString *machine;
	CString *username;
	CString *ip;

	CNameEntry::CNameEntry()
		{
		machine = new CString;
		username = new CString;
		ip = new CString;
		}

	CNameEntry::~CNameEntry()
		{
		delete machine, delete username; delete ip;
		}
};

#define	CResolv_Magic	0xee333422


class CResolv  

{
	static	void		_AcceptThreadProc(void *ptp);
	static	void		_DataThreadProc(void *ptp);

	static	CStringA	cachestr, datastr;

public:

	int		data_port, res_port;

	CPtrArray	namearr;

	CResolv();
	virtual ~CResolv();

	int		Announce();
	int		Denounce();

	int		SendStr(const char *str, int len);
	int		Respond(const char *str, int len, const char *to);

	int		magic;

protected:

	int		thhandle, thhandle2, shutdown;

	void	GetCompStr(CStringA &str);
	void	UpdateCache(const char *buff, const char *str);

	//SOCKADDR_IN     dest_sin, local_sin;	

	int		nd, ndlen, connected, resolved; 

};

#endif // !defined(AFX_RESOLV_H__1600312F_29C4_4C29_A7C2_364BB0C58A7B__INCLUDED_)


/* =====[ Resolv.cpp ]========================================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the Resolv.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  5/29/2010  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */


#include "stdafx.h"
#include <afxmt.h>
#include <process.h>

#include "Resolv.h"
#include "mxpad.h"
#include "misc.h"

#ifdef _DEBUG
#undef THIS_FILE
//static TCHAR THIS_FILE[]=_T(__FILE__);
static const char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib, "ws2_32.lib")

CStringA	CResolv::cachestr = "cache://";
CStringA	CResolv::datastr = "data://";

//////////////////////////////////////////////////////////////////////////

void	CResolv::_DataThreadProc(void *ptp )

{
	char	buff[1280];

	CResolv*  me = (CResolv*)ptp;

	int		ndlen  = sizeof(char), ret_val;
	char	nd;
	
	SOCKET	sock2; 
	
    //////////////////////////////////////////////////////////////////////
	// Listen for broadcasts on port xxx

    sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    
	if(sock2 == INVALID_SOCKET)
		{
		AP2N(_T("CResolv::_DataThreadProc -- Cannot create socket\r\n"));
		return;
		}

	//P2N(_T("CResolv::_DataThreadProc sock2=%d %s\r\n"), sock2, 
	//				mxpad.sockerr2str(WSAGetLastError()));

    ret_val = setsockopt (sock2, SOL_SOCKET, 
									SO_DONTLINGER, &nd, sizeof(int));
    
	SOCKADDR_IN rec_sin; int rec_sin_len = sizeof(rec_sin);

	rec_sin.sin_addr.s_addr = INADDR_ANY;
	rec_sin.sin_port = htons(me->data_port);
	rec_sin.sin_family = AF_INET;
	
    if (bind( sock2, (struct sockaddr FAR *) &rec_sin, sizeof(rec_sin))
        == SOCKET_ERROR)
        {
        //TRACE(_T("CResolv::_AcceptThreadProc -- Cannot Bind socket\r\n"));
		AP2N(_T("CResolv::_DataThreadProc -- Cannot Bind socket\r\n"));

		return;
		//Sleep(1000);
        //exit(0);
        }
		
	while(true)
		{	    
		int ret = recvfrom (sock2, buff, sizeof(buff), 0, (struct sockaddr *)&rec_sin, &rec_sin_len);

		if(ret == 0)
			break;

		if(me->shutdown)
			break;

		char *str = (char*)inet_ntoa( *((struct in_addr*) &rec_sin.sin_addr.s_addr));

		//P2N(_T("CResolv::_DataThreadProc recfrom '%s' '%s'\r\n"), buff, str);

		if(strncmp(buff, me->cachestr, me->cachestr.GetLength() ) == 0)
			{
			me->UpdateCache(buff + 8, str);
			}	
		}
	closesocket(sock2);

	TRACE(_T("CResolv::_AcceptThreadProc - End thread.\r\n"));
}

/*
 * Only one of these will run, no access control needed.
 */

void	CResolv::_AcceptThreadProc(void *ptp )

{
	char	buff[128];

	CResolv*  me = (CResolv*)ptp;

	int		ndlen  = sizeof(char), ret_val;
	char	*str, nd  = 1;
	
	SOCKET	sock2; 
	
    //////////////////////////////////////////////////////////////////////
	// Listen for broadcasts on port xxx

    sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    
	if(sock2 == INVALID_SOCKET)
		{
		AP2N(_T("CResolv::_AcceptThreadProc -- Cannot create socket\r\n"));
		return;
		}

	//P2N(_T("CResolv::_AcceptThreadProc sock2=%d %s\r\n"), sock2, 
	//				mxpad.sockerr2str(WSAGetLastError()));

    ret_val = setsockopt (sock2, SOL_SOCKET, 
									SO_DONTLINGER, &nd, sizeof(int));
    
	SOCKADDR_IN rec_sin; int rec_sin_len = sizeof(rec_sin);

	rec_sin.sin_addr.s_addr = INADDR_ANY;
	rec_sin.sin_port = htons(me->res_port);
	rec_sin.sin_family = AF_INET;
	
    if (bind( sock2, (struct sockaddr FAR *) &rec_sin, sizeof(rec_sin))
        == SOCKET_ERROR)
        {
        //TRACE(_T("CResolv::_AcceptThreadProc -- Cannot Bind socket\r\n"));
		AP2N(_T("CResolv::_AcceptThreadProc -- Cannot Bind socket\r\n"));

		//Sleep(1000);
        return;
        }
		
	while(true)
		{	    
		int ret = recvfrom (sock2, buff, 128, 0, (struct sockaddr *)&rec_sin, &rec_sin_len);
	
		if(me->shutdown)
			break;

		//P2N(_T("CResolv::_AcceptThreadProc recfrom '%s'\r\n"), buff);

		str = inet_ntoa( *((struct in_addr*) &rec_sin.sin_addr.s_addr));

		//P2N(_T("CResolv::_AcceptThreadProc got address '%s'\r\n"), str);

		if(ret >= 0)
			{
			me->UpdateCache(buff, str);

			// Respond with our ID
			CStringA response(me->cachestr);
			CStringA compstr; me->GetCompStr(compstr);
			response += compstr;
			me->Respond(response, response.GetLength() + 1, str);
			}
		}
	
	closesocket(sock2);

    TRACE(_T("CResolv::_AcceptThreadProc - End thread.\r\n"));
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResolv::CResolv()

{
	WSAData wsadat;	WSAStartup(MAKEWORD(2,0), &wsadat);

	shutdown = false;

	int		res2 = 0;
	DWORD	res = 0;
					
	CString portx(_T("port"));
	
	//data_port = AfxGetApp()->GetProfileInt(portx, "data_port", YM_DATA_PORT);
	//res_port  = AfxGetApp()->GetProfileInt(portx, "res_port", YM_RES_PORT);
	
	data_port = YM_DATA_PORT;
	res_port  = YM_RES_PORT;
	
	magic = CResolv_Magic;

#if 0
	if(!thhandle)
		thhandle = _beginthread(_AcceptThreadProc, 0, (void*)this);
	
	if(!thhandle2)
		thhandle2 = _beginthread(_DataThreadProc, 0, (void*)this);
#endif
}

//////////////////////////////////////////////////////////////////////////

CResolv::~CResolv()

{
	TRACE(_T("CResolv::~CResolv\r\n"));

	shutdown = true;

	int alen = namearr.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		CNameEntry *val = (CNameEntry*) namearr.GetAt(loop);
		delete val;
 		}

	//TRACE(_T("CResolv::~CResolv2\r\n"));
}

//////////////////////////////////////////////////////////////////////////
// Get Computer ID compname::user
// Forced ANSI for network operation
//

void	CResolv::GetCompStr(CStringA &str)

{
	DWORD len = MAX_COMPUTERNAME_LENGTH + 1;
	TCHAR cname[MAX_COMPUTERNAME_LENGTH + 1]; 
	
	GetComputerName((TCHAR*)&cname, &len);
	char *astr; misc.UnicodeToAnsi(cname, &astr);
	str = astr;	str += "::"; free(astr); 

	DWORD ulen = MAX_COMPUTERNAME_LENGTH + 1;
	TCHAR uname[MAX_COMPUTERNAME_LENGTH + 1]; 
	
	GetUserName((TCHAR*)&uname, &ulen);
	char *bstr;	misc.UnicodeToAnsi(uname, &bstr);	
	str += bstr; free (bstr);
}

//////////////////////////////////////////////////////////////////////////

int CResolv::Denounce()

{
	shutdown = true;
	//return 0;

	CStringA str; GetCompStr(str);
	CStringA str2 = "~"; str2 += str;
	SendStr(str2, str2.GetLength()+1);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int CResolv::Announce()

{
	CStringA str; GetCompStr(str);
	SendStr(str, str.GetLength()+1);

	return 0;
}

//////////////////////////////////////////////////////////////////////////

int		CResolv::SendStr(const char *str, int len)

{
	char	bc  = 1;
	int		bclen  = sizeof(char);
	int     res2 = 0;

	//P2N(_T("CResolv::Resolve()\r\n"));

	SOCKADDR_IN		local_sin;	
	SOCKET sock = socket( AF_INET, SOCK_DGRAM, 0);

	int ret_val = setsockopt(sock, SOL_SOCKET, 
						SO_BROADCAST, &bc, sizeof(char));

	local_sin.sin_addr.s_addr = INADDR_BROADCAST;
	local_sin.sin_port = htons(res_port);
	local_sin.sin_family = AF_INET;
	int acc_sin_len = sizeof(local_sin);


	ret_val = sendto(sock, str,  len, 
					0, (struct sockaddr *)&local_sin, acc_sin_len);

	//P2N(_T("Resolv::SendStr ret_val=%d\r\n"), ret_val);

	return resolved;
}

//////////////////////////////////////////////////////////////////////////
//

int		CResolv::Respond(const char *str, int len, const char *to)

{
	//P2N(_T("CResolv::Respond(str=%s len=%d, to=%s)\r\n"), str, len, to);

	SOCKADDR_IN     local_sin;	
	SOCKET sock = socket( AF_INET, SOCK_DGRAM, 0);

	local_sin.sin_addr.s_addr = inet_addr(to);
	local_sin.sin_port = htons(data_port);
	local_sin.sin_family = AF_INET;
	int acc_sin_len = sizeof(local_sin);

	int ret_val = sendto(sock, str,  len, 
					0, (struct sockaddr *)&local_sin, acc_sin_len);

	//P2N(_T("Resolv::Respond ret_val=%d\r\n"), ret_val);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Update cached data

void	CResolv::UpdateCache(const char *buff, const char *str)

{
	CNameEntry *ne = NULL;

	// See if delete request
	if(buff[0] == '~')
		{		
		int alens = namearr.GetSize();
		for(int loops = 0; loops < alens; loops++)
			{
			CNameEntry *val = (CNameEntry*) namearr.GetAt(loops);

			if(*val->ip == str)
				{
				namearr.RemoveAt(loops);
				delete val;
				}
			}			
		}
	else
		{
		// Find if there is an entry
		int alens = namearr.GetSize();
		for(int loops = 0; loops < alens; loops++)
			{
			CNameEntry *val = (CNameEntry*) namearr.GetAt(loops);

			if(*val->ip == str)
				{
				ne = val; break;
				}
			}

		// Not found, create one
		if(!ne)
			{
			ne = new CNameEntry();

			*ne->ip = str;

			CString str2 = buff; int idx = str2.Find(_T("::"));

			if(idx >= 0)
				{
				*ne->machine = str2.Mid(0, idx);
				*ne->username = str2.Mid(idx + 2);
				}
			else
				{
				*ne->machine = str;
				}				
			namearr.Add(ne);
			}

		// Update timer in every case
		ne->regtime = (int)time(NULL);
		}		

	// Dump name array
	//P2N(_T("Dump name Array\r\n"));

	int alen = namearr.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		CNameEntry *val = (CNameEntry*) namearr.GetAt(loop);

		CTime tme((time_t)val->regtime);
		CString ttt = tme.Format(_T("%d %b %Y %H:%M:%S"));

		//P2N(_T("'%s' '%s' '%s' %s\r\n"), *val->machine, *val->username, *val->ip, ttt);
		}

	//P2N(_T("Dump end.\r\n"));
}


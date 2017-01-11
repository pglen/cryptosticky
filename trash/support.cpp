/////////////////////////////////////////////////////////////////////////////
// support.cpp implementation file


#include "stdafx.h"

#include <direct.h>
#include <io.h>

#include "YellowMemo.h"
#include "support.h"
#include "mxpad.h"


//////////////////////////////////////////////////////////////////////////

CString time_t2str(time_t tme)

{
	CString str;

	CTime ct(tme);
	str = ct.Format("%m/%d/%Y %H:%M");
	return str;
}

//////////////////////////////////////////////////////////////////////////

CString time_t2tstr(time_t tme)

{
	CString str;

	CTime ct(tme);
	str = ct.Format("%d/%m/%y - %H:%M");
	return str;
}


//////////////////////////////////////////////////////////////////////////

void	ShortenStr(CString &str, int len)

{
	int slen = str.GetLength();

	// Nothing to do
	if(slen <= len)
		return;

	CString lstr = str.Left ((len-5)/2);
	CString rstr = str.Right((len-5)/2);
	str = lstr + " ... " + rstr;
}

//////////////////////////////////////////////////////////////////////////

void	ShortenStrL(CString &str, int len)

{
	CString lstr;
	int slen = str.GetLength();

	// Nothing to do
	if(slen <= len)
		return;

	// Find space right of
	int idx	= str.Find(" ", len);

	if(idx >= 0)
		{
		lstr = str.Left(idx);
		}
	else
		{
		lstr = str.Left(len-5);
		}

	str = lstr + " ...";
}


/////////////////////////////////////////////////////////////////////////////
// int		HashString(const char *name)

int		HashString(const char *name)

{
	unsigned int ret_val = 0;

    while(*name != '\0')
        {
        ret_val ^= (int)*name;
        ret_val  = ROTATE_LONG_RIGHT(ret_val, 3);          /* rotate right */
        *name++;
        }
    return((int)ret_val);
}

///////////////////////////////////////////////////////////////////////////
// YieldToWinEx
// Let every accumulated message go through
// Return 1 if ESC | Ctrl-Break pressed -- 2 if EXITING

int     YieldToWinEx()

{
    int ret_val = 0;
    while(TRUE)
        {
        MSG msg;
        if(!PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
           break;

        // Ctrl-Break
        if(msg.message == WM_KEYDOWN)
            {
            if(msg.wParam == VK_CANCEL)
                {
                ret_val = 1; break;
                }
            }
        // Esc
        if(msg.message == WM_CHAR)
            {
            if(msg.wParam == VK_ESCAPE)
                {
                ret_val = 1; break;
                }
            }
        // App exit
        if(msg.message == WM_QUIT)
                {
                //wait_exit = TRUE;
                ret_val = 2; break;
                }
        // App kill
        if(msg.message == WM_DESTROY)
                {
                //wait_exit = TRUE;
                ret_val = 2; break;
                }
        // Every other message is processed
        TranslateMessage(&msg); DispatchMessage(&msg);
        }
    return(ret_val);
}


//////////////////////////////////////////////////////////////////////////

int		GetLocMachineInt(const char *str, const char *key)

{
	HKEY	Result;
	DWORD	oldtime = 0;
	DWORD	plen = sizeof(DWORD);
	DWORD	Type = 0;

	LONG ret2 = RegOpenKey(HKEY_LOCAL_MACHINE, str, &Result);

	if(ret2 == ERROR_SUCCESS)
		{
		RegQueryValueEx(Result, key, NULL, &Type, 
							(unsigned char*)&oldtime, &plen);
		RegCloseKey(Result);
		}
	return oldtime;
}

void	SetLocMachineInt(const char *str, const char *key, int val)

{
	HKEY	Result;
	DWORD	oldtime = 0;
	DWORD	plen = sizeof(DWORD);

	LONG ret2 = RegOpenKey(HKEY_LOCAL_MACHINE, str, &Result);

	if(ret2 != ERROR_SUCCESS)
		{
		/// Create key
		ret2 = RegCreateKeyEx(HKEY_LOCAL_MACHINE, str, 0,
									NULL, 0, KEY_ALL_ACCESS, NULL, &Result, NULL);
		}

	if(ret2 == ERROR_SUCCESS)
		{
		RegSetValueEx(Result, key, NULL, REG_DWORD, 
								(unsigned char*)&val, sizeof(DWORD));
		RegCloseKey(Result);
		}
}

void	PathToFname(CString &docname)

{
	int idx;
	CString filename;

	if((idx = docname.ReverseFind('\\')) != -1)
		{
		filename = docname.Right(docname.GetLength() - (idx + 1));
		docname = filename;
		}
}

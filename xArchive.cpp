
/* =====[ xArchive.cpp ]==================================================

   Description:     project,
   
   xArchive.cpp: implementation of the CxArchive class.

*/

//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CryptoSticky.h"
#include "xArchive.h"
#include "datalog.h"

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "mxpad.h"
#include "misc.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[]=_T(__FILE__);
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Not derived from Carchive any more

CxArchive::CxArchive()
{
	head = tail = prog = 0;
	multibyte = false;
	contents = NULL; contents2 = NULL;
}

CxArchive::~CxArchive()

{
	// is CFile open?
	//P2N(_T("Destructor: Closing %s\r\n"), filename); 
	if (cfile.m_hFile != CFile::hFileNull)
		cfile.Close();
	if (contents != NULL)
		free(contents);
	if (contents2 != NULL)
		free(contents2);
}

//////////////////////////////////////////////////////////////////////////

void	CxArchive::Close()
{
	// is CFile open?
	if(cfile.m_hFile != CFile::hFileNull)
		{
		//P2N(_T("Regular Closing %s\r\n"), filename); 
		cfile.Close();
		cfile.m_hFile = CFile::hFileNull;
		}
	if (contents != NULL)
		free(contents);
	contents = NULL;

	if (contents2 != NULL)
		free(contents2);
	contents2 = NULL;
}

//////////////////////////////////////////////////////////////////////////
// Open archive
// CFile permissions apply
// pass true for uniflag to create multibyte files
// CFile::modeCreate | CFile::modeNoTruncate 
// CFile::modeWrite | CFile::shareDenyNone

int CxArchive::Open(const TCHAR *fname, int mode, int uniflag)

{
	multibyte = uniflag;

	// Remember file name for error messages
	filename = fname;
	prog = 0;

	if (contents != NULL)
		free(contents);
	if (contents2 != NULL)
		free(contents2);

	//P2N(_T("CxArchive::Open %s\r\n"), fname);

	if(!cfile.Open(fname, mode))
		{
		//CString tmp; 
		//if(mode & CFile::modeCreate)
		//	tmp.Format(_T("Cannot create file '%s'"), fname);
		//else
		//	tmp.Format(_T("Cannot open file '%s'"), fname);
		// MBCS
		//AfxMessageBox(tmp);

		P2N(_T("CxArchive::Open error on %s\r\n"), fname);
		return false;
		}

		if ((mode & CFile::modeCreate) && (cfile.GetLength() == 0))
		{
			//P2N(_T("Creating new sticky ... multibyte %d\r\n"), uniflag);

			//if(uniflag)
			// Tag if newly created to multibyte
			if (sizeof(TCHAR) > 1)
			{
				multibyte = true;
				unsigned char tbuff[3];
				tbuff[0] = 0xef; tbuff[1] = 0xbb; tbuff[2] = 0xbf;
				cfile.Write(tbuff, 3);
			}			
		}
		else			// READ in existing
		{	
			// See if it is multibyte
			unsigned char tbuff[3]; tbuff[0] = '\0'; tbuff[1] = '\0'; tbuff[2] = '\0';
			int tret = cfile.Read(tbuff, 3);
			//D2N(tbuff, 3);
			//P2N(_T("%x %x %x\r\n"), tbuff[0], tbuff[1], tbuff[2]);
			if ((tbuff[0] == 0xef && tbuff[1] == 0xbb && tbuff[2] == 0xbf) ||
					(tbuff[0] == 0xff && tbuff[1] == 0xfe)
					)
			{
			// Seek back one if old file
			if (tbuff[0] == 0xff && tbuff[1] == 0xfe)
			{ 
				//cfile.Seek(-1, CFile::current);
				cfile.Seek(2, CFile::begin);
			}

			//P2N(_T("Multibyte file %s\r\n"), fname);
			multibyte = true;
			bufflen = 0;
				// Read in the whole file, decode, deal
				int flen = (int)cfile.GetLength() - 3;
				if (flen > 100000 || flen < 0)
				{
					P2N(_T("File too large %d"), flen);
					//str = _T("File too large");
					//return str.GetLength();
					return false;
				}
				contents = (char*)malloc(flen);
				if (!contents)
				{
					P2N(_T("Cannot alloc mem  %d"), flen);
					//str = _T("Cannot allocate memory");
					//return str.GetLength();
					return false;
				}
				int lret = cfile.Read(contents, flen);
				//P2N(_T("read %d %d\r\n"), flen, lret);
				if (lret != flen)
				{
					P2N(_T("Cannot read file"));
					//str = _T("Cannot read file");
					//return str.GetLength();
					return false;
				}
				contents2 = (TCHAR*)malloc(flen * 4);
				if (!contents2)
				{
					P2N(_T("Cannot alloc (decodoing) mem  %d"), flen * 4);
					//str = _T("Cannot allocate memory");
					//return str.GetLength();
					return false;
				}
				// Decode

				// Seek back one if old file
				if (tbuff[0] == 0xff && tbuff[1] == 0xfe)
				{
					// Nothing, just copy
					memcpy(contents2, contents, flen);
					contents2[flen/2] = 0;
					bufflen = flen/2;

				}
				else
				{
					int ret = MultiByteToWideChar(CP_UTF8, 0, contents, flen, contents2, flen * 4);
					if (ret < flen * 2 && ret > 0)
					{
						contents2[ret] = 0;
						bufflen = ret;
					}
					else
					{
						P2N(_T("Unterminated buffer %s\r\n"), filename);
						return false;
					}
				}
				
				//P2N(_T("Got file %s\r\n"), contents2);
			}
			else
			{
				// Seek to beginning on ansi
				//P2N(_T("ASCII file %s\r\n"), fname);
				multibyte = 0;
				ULONGLONG seekto = cfile.Seek(0, CFile::begin);
				//P2N(_T("Seek returned:%I64u\r\n"), seekto);
			}
		}
		return true;
}

// Read in a string ... branch to ascii or multibyte

int CxArchive::ReadString(CString &str)

{
	int ret = 0, eol = false;

	// is CFile open?
	if (cfile.m_hFile == CFile::hFileNull)
	{
		P2N(_T("CxArchive::ReadString: must open file first\r\n"));
		return ret;
	}

	str = "";	// Start with empty string

	if(multibyte) 
		{
		if (contents2 == NULL)
			return str.GetLength();

		while (true)
		{			
			if (prog >= bufflen)
				break;

			if (eol)
				break;

			TCHAR chht = contents2[prog++];
			//P2N(_T("%c"), chhtt);
			switch (chht)
			{
			case '\r':	str += chht; break;				// ignore
			case '\n': 	str += chht; eol = true; break;
			default: 	str += chht; break;
			}
		}

		//P2N(_T("Read multibyte '%s'"), str);
		ret = str.GetLength();
	}
	else
	{
		// Convert ascii to Unicode
		char chht[2]; CString str2 = _T("");
		//P2N(_T("Reading ASCII"));
		while (true)
		{
			if (eol)
				break;
			chht[1] = 0;
			int len = cfile.Read(&chht[0], sizeof(char));
			if (len != sizeof(char))
				break;
			misc.AnsiToUnicode(chht, str2);
			//P2N(_T("%c"), chht);
			switch (chht[0])
				{
				case '\r':	str += chht; break;				// ignore
				case '\n': 	str += chht; eol = true; break;
				default: 	str += str2; break;
				}
		}		
	}

	ret = str.GetLength();
	//P2N(_T("CxArchive::ReadString multibyte = %d ret = %d str='%s'\r\n"), multibyte, ret, str);
	return ret;
}

int CxArchive::Rewind()
{
	cfile.Seek(0, CFile::begin);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// Write string to archive

int CxArchive::WriteString(CString &str)

{
	int ret = 0; 

	// is CFile open?
	if(cfile.m_hFile == CFile::hFileNull)
		{
		P2N(_T("CxArchive::WriteString() - Must open file first.\r\n"));
		return ret;
		}

	//printf("Writing '%s' %d\r\n", (const char*)str, str.GetLength());
	//P2N("Writing '%s' %d\r\n", (const char*)str, str.GetLength());

	static char szBuf[1024];
	ret = WideCharToMultiByte(CP_UTF8, 0, str, str.GetLength(), szBuf, sizeof(szBuf), NULL, NULL);
		
	cfile.Write(szBuf, ret);
	//cfile.Write(str, str.GetLength());

	return ret;
}


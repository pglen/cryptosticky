// ChangeMem.cpp: implementation of the CChangeMem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChangeMem.h"
#include "mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[]=_T(__FILE__);
#undef DEBUG_NEW
#define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChangeMem::CChangeMem()

{

}

CChangeMem::~CChangeMem()

{
	int alen = arr.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		CChangeData *val = (CChangeData *)arr.GetAt(loop);
		delete val;
 		}
}

//////////////////////////////////////////////////////////////////////////

void CChangeMem::Add(const TCHAR *from, CString *to)

{
	CChangeData *newentry = NULL;

	CString fromstr(from);
	
	//P2N(_T("CChangeMem::Add fromstr=%s to=%s\r\n"), fromstr, *to);

	// Is this already in the mix?
	int alen = arr.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		CChangeData *val = (CChangeData *)arr.GetAt(loop);

		if(*val->from == fromstr)
			{
			//P2N(_T("CChangeMem::Add Found %s\r\n"), *val->from);
			newentry = val;
			break;
			}
		}

	if(!newentry)
		{
		newentry = new CChangeData(); //ASSERT(newentry);
		arr.Add(newentry);
		}

	*newentry->from = fromstr;	*newentry->to = *to;
	newentry->chtime = (int)time(NULL);	

	// Clear out long lasting entries
	int alen2 = arr.GetSize();
	for(int loop2 = alen2 - 1; loop2 >= 0; loop2--)
		{
		CChangeData *val = (CChangeData *)arr.GetAt(loop2);
		
		if(time(NULL) - val->chtime > 100)
			{
			//P2N(_T("Stale Entry %s\r\n"), *val->from);
			arr.RemoveAt(loop2); delete val;
			}
		}
}

//////////////////////////////////////////////////////////////////////////
//

CString  CChangeMem::Lookup(const TCHAR *from)

{
	CString fromstr(from), ret(from);

	//P2N(_T("CChangeMem::Lookup fromstr=%s\r\n"), fromstr);
	//Dump();

	int alen = arr.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		CChangeData *val = (CChangeData *)arr.GetAt(loop);

		if(fromstr == *val->from)
			{
			//P2N(_T("CChangeMem::Lookup Found %s\r\n"), *val->from);		
			ret = *val->to; break;
			}
		}
	return ret;
}	

//////////////////////////////////////////////////////////////////////////
//

void CChangeMem::Dump()

{
	int alen = arr.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		CChangeData *val = (CChangeData *)arr.GetAt(loop);

		P2N(_T("CChangeMem::Dump '%s' -> '%s'\r\n"), *val->from, *val->to);
		}
}

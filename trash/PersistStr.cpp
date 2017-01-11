// PersistStr.cpp: implementation of the PersistStr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "yellowmemo.h"
#include "mxpad.h"
#include "PersistStr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PersistStr::PersistStr()

{
	TRACE("PersistStr::PersistStr\r\n");
}

PersistStr::PersistStr(const char *_dir, const char *_name, const char * _def)

{
	TRACE("PersistStr::PersistStr 3\r\n");

	initr = initw = 0;

	dir			= _dir;	
	name		= _name;
	val = def	= _def;	

	glob.Add((void*)this);
}

PersistStr::~PersistStr()

{

}

PersistStr::Load()

{
	int alen = glob.GetSize();
	for(int loop = 0; loop < alen; loop++)
		{
		PersistStr *ptr =  (PersistStr*)glob[loop];
		ptr->val = AfxGetApp()->GetProfileString(ptr->dir, ptr->name, ptr->def);
		P2N("PersistStr::Load %s\r\n", ptr->val);
 		}
}

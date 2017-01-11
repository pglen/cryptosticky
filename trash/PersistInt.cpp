
/* =====[ PersistInt.cpp ]========================================== 
                                                                             
   Description:     The yellowmemo project, implementation of the PersistInt.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/3/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "stdafx.h"
#include "yellowmemo.h"
#include "PersistInt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PersistInt::PersistInt(const char *_dir, const char *_name, int _def)

{
	initr = initw = 0;

	dir			= _dir;	
	name		= _name;
	val= def	= _def;
}

PersistInt::~PersistInt()

{

}

void	PersistInt::Default()

{
	val = def;
}


/* =====[ Clicense.cpp ]========================================================== 
                                                                             
   Description:     The serials project, implementation of the Clicense.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/15/2010  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#include "mxpad.h"

///////////////////////////////////////////////////////////////////////////////
// Return sum of string

static	int		SumStr(const TCHAR *str)

{
    CString str2 = str;

	int sum = 0, loop, lim = str2.GetLength();

	str2.MakeUpper();

	for(loop = 0; loop < lim; loop++)
		{
		TCHAR chh = str2.GetAt(loop);

		if(_istalnum(chh))
			{
			//P2N(L"%c ", chh);
			sum += chh;
			}
		}

	//P2N(L"\r\nSumStr '%s' %d\r\n", str, sum);
	return(sum);
}

///////////////////////////////////////////////////////////////////////////////
// Return TRUE if OK

int		CheckLic(const TCHAR *str, int *res)

{
	int sum2; 

	if(res)
		*res = 0;
	
	sum2 = SumStr(str);

	if(*str == 0)
		return(false);

	if(res)
		*res = sum2;
	
	// Terminal condition
	if((sum2 % LIC_MODULUS) == 0 && sum2 != 0)
		{
		return true;
		}
	else
		{
		return false;
		}			
}



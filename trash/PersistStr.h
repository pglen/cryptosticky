
/* =====[ PersistStr.h ]========================================== 
                                                                             
   Description:     The yellowmemo project, implementation of the PersistStr.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// PersistStr.h: interface for the PersistStr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERSISTSTR_H__603CEC1D_891F_4920_9A16_3BA674D79F5B__INCLUDED_)
#define AFX_PERSISTSTR_H__603CEC1D_891F_4920_9A16_3BA674D79F5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class PersistStr  
{

public:
	
	static	CPtrArray	glob;
	static	Load();						// Call this at init app time

	PersistStr();
	PersistStr(const char *_dir, const char *_name, const char * _def = 0);

	virtual ~PersistStr();

	operator const char * ()
		{
		//P2N("PersistStr operator const char *\r\n");
	
		if(!initr)
			{
			initr = true;
			val = AfxGetApp()->GetProfileString(dir, name, def);
			}

		//return (const char *)val;
		return val;
		}

	operator CString ()
		{
		//P2N("PersistStr operator CString \r\n");
	
		if(!initr)
			{
			initr = true;
			val = AfxGetApp()->GetProfileString(dir, name, def);
			}
		return val;
		}

	CString & operator = (CString ss)
		{
		//P2N("PersistStr operator = PersistStr\r\n");
		val = ss;
		AfxGetApp()->WriteProfileString(dir, name, val);

		return val;
		}

	CString & operator += (CString ss)
		{
		//P2N("PersistStr operator = PersistStr\r\n");
		val += ss;
		AfxGetApp()->WriteProfileString(dir, name, val);

		return val;
		}

protected:

	CString	val, name, dir, def;

private:
	BOOL	initr, initw;
};

#endif // !defined(AFX_PERSISTSTR_H__603CEC1D_891F_4920_9A16_3BA674D79F5B__INCLUDED_)

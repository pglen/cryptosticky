
/* =====[ StatList.h ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the StatList.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// StatList.h: interface for the CStatList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATLIST_H__533D76DA_D25C_458B_8C2D_590B95FB44E8__INCLUDED_)
#define AFX_STATLIST_H__533D76DA_D25C_458B_8C2D_590B95FB44E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CStatList  

{

typedef struct _statfile

{
	DWORD		mtime;
	int			present;
	char		fname[_MAX_PATH];
	HTREEITEM	item;
		
} statfile;

public:

	CStatList();
	virtual ~CStatList();
	
	
	void	Dump();
	void	ClearHits();
	void	ShowHits();
	int		CompareList(const char *filename, HTREEITEM item);
	int		Delete(const char *filename);

protected:

	CPtrList  list;

};

#endif // !defined(AFX_STATLIST_H__533D76DA_D25C_458B_8C2D_590B95FB44E8__INCLUDED_)

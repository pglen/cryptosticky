
/* =====[ StatList.cpp ]========================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the StatList.cpp                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/4/2009  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

// StatList.cpp: implementation of the CStatList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "CryptoSticky.h"
#include "Yellowdlg.h"

#include "StatList.h"
#include "mxpad.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CStatList	statlist;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatList::CStatList()
{

}

CStatList::~CStatList()

{
	//P2N("Destroy CStatList\r\n");

	POSITION pos = list.GetHeadPosition();
	while(true)
		{
		if(list.IsEmpty())
			break;

		void *ptr = list.RemoveTail();
		ASSERT(ptr);
		//P2N("Removing CStatList:: %s\r\n", ((statfile*)ptr)->fname);
		delete ptr;
		}
}

//////////////////////////////////////////////////////////////////////////
// Compare file in list, add if not present 
// Return diff between old and new
//

int CStatList::CompareList(const char *filename, HTREEITEM item)

{
	int ret = 0, found = 0;
	statfile *ptr;

	CString fname;
    fname.Format("%s%s", dataroot, filename);

	//P2N("CStatList::CompareList %d %s %s\r\n", item, filename, fname);

	struct _stat sbuff;
	memset(&sbuff, 0, sizeof(sbuff));

	if(_stat(fname, &sbuff) < 0)
		{
		P2N("No file with this name '%s'\r\n", fname);
		return 0;
		}

	POSITION pos = list.GetHeadPosition();
	while(true)
		{
		if(!pos)
			break;

		ptr = (statfile *)list.GetNext(pos);

		if(strcmp(ptr->fname, fname) == 0)
			{
			//P2N("Found stat item %d - %d %s\r\n", ptr->mtime, sbuff.st_mtime, ptr->fname);
		
			found = true;
			ret = sbuff.st_mtime - ptr->mtime;
			ptr->present = true;
			ptr->item = item;

			// Remember new status
			ptr->mtime = sbuff.st_mtime;
			break;
			}
		}

	if(!found)
		{
		//P2N("Adding to statbuf: %s\r\n", fname);

		statfile *ptr2 = new statfile;
		strncpy(ptr2->fname, fname, _MAX_PATH); 
		ptr2->mtime = sbuff.st_mtime;
		ptr2->item = item;
		list.AddTail(ptr2);

		ret = sbuff.st_mtime;
		}

	//P2N("Comparelist ret=%d\r\n", ret);

	return ret;
}

//////////////////////////////////////////////////////////////////////////

int CStatList::Delete(const char *filename)

{
	statfile *ptr;
	
	//P2N("Bfore delete\r\n");
	//Dump();

	POSITION pos = list.GetHeadPosition();
	while(true)
		{
		if(!pos) 
			break;

		POSITION oldpos = pos;
		ptr = (statfile *)list.GetNext(pos);
		if(strcmp(ptr->fname, filename) == 0)
			{
			if(!list.IsEmpty())
				{
				list.RemoveAt(oldpos);
				free(ptr);
				break;
				}
			}
		}

	//P2N("After delete\r\n");
	//Dump();

	return 0;
}

//////////////////////////////////////////////////////////////////////////

void	CStatList::ClearHits()

{
	statfile *ptr;
	
	POSITION pos = list.GetHeadPosition();
	while(true)
		{
		if(!pos)
			break;

		ptr = (statfile *)list.GetNext(pos);
		ptr->present = false;
		}
}

void	CStatList::Dump()

{
	statfile *ptr;
	
	POSITION pos = list.GetHeadPosition();
	while(true)
		{
		if(!pos)
			break;

		ptr = (statfile *)list.GetNext(pos);
		//P2N("Dump: Item=%d  File: '%s'\r\n", ptr->item, ptr->fname);
		}
}

//////////////////////////////////////////////////////////////////////////

void	CStatList::ShowHits()

{
	statfile *ptr;
	
	POSITION pos = list.GetHeadPosition();
	while(true)
		{
		if(!pos)
			break;

		POSITION oldpos = pos;
		ptr = (statfile *)list.GetNext(pos);
		if(ptr->present == false)
			{
			P2N("Missing Hit: %s %d\r\n", ptr->fname, ptr->item);
			if(ptr->item !=  0)
				{
				((YellowDlg * )AfxGetMainWnd())->ShowOne(ptr->item, false, false, false);
				((YellowDlg * )AfxGetMainWnd())->GetTreeCtrl()->DeleteItem(ptr->item);

				// Delete from statlist
				list.RemoveAt(oldpos);
				}
			}
		}
}


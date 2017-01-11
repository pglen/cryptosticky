// ChangeMem.h: interface for the CChangeMem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANGEMEM_H__FD33173A_DD93_40C1_ABFE_8A934E548D26__INCLUDED_)
#define AFX_CHANGEMEM_H__FD33173A_DD93_40C1_ABFE_8A934E548D26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CChangeData_Magic 0x34298765

class CChangeMem  

{
	class CChangeData
		{
	public:
		CString *from, *to;
		int chtime, magic;

		CChangeData()	{
						from = new CString; to = new CString;
						chtime = 0;
						magic = CChangeData_Magic;
						}

		~CChangeData()	{ 
						//ASSERT(magic == CChangeData_Magic);
						delete from; delete to;
						}		
		};

public:
	void Dump();

	void	Add(const TCHAR *from, CString *to);
	CString	Lookup(const TCHAR *from);

	CChangeMem();
	virtual ~CChangeMem();

protected:
	CPtrArray arr;

};

#endif // !defined(AFX_CHANGEMEM_H__FD33173A_DD93_40C1_ABFE_8A934E548D26__INCLUDED_)

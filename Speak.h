// Speak.h: interface for the CSpeak class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPEAK_H__9BFA87CF_FA95_47D9_9739_FBD43581E5F1__INCLUDED_)
#define AFX_SPEAK_H__9BFA87CF_FA95_47D9_9739_FBD43581E5F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef SPEAK
#include "sapi.h"
#endif

#include <afxmt.h>
#include <process.h>

//#ifdef SPEAK

class CSpeak  

{
	static	void  _Thread(void *lpParam);
	
public:

	static	int	  reenter;
	
	void	Skip(int num);
	
	CSpeak();
	virtual ~CSpeak();

	void	PostHere(HWND wnd) {postto = wnd;}
	void	Speak(const TCHAR *str);
	void	Speak(CString &str);
	void	Stop();

	int		Prop();

protected:

	HWND	postto;

#ifdef SPEAK
	ISpVoice * pVoice;
#endif

	void	xCleanUp();

	unsigned short  outp[10240];

	// Thread related
	HANDLE  thhevent; int thhend, thhandle;
};

//#endif

#endif // !defined(AFX_SPEAK_H__9BFA87CF_FA95_47D9_9739_FBD43581E5F1__INCLUDED_)

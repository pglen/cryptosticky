// Speak.cpp: implementation of the CSpeak class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Speak.h"
#include "mxpad.h"

#ifdef _DEBUG
#undef THIS_FILE
static TCHAR THIS_FILE[]=_T(__FILE__);
#define new DEBUG_NEW
#endif

int	  CSpeak::reenter = 0;
	
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpeak::CSpeak()

{
	postto = 0;

#ifdef SPEAK
	pVoice = NULL;
#endif
	thhevent = CreateEvent(NULL, 0, 0, NULL);	//ASSERT(thhevent);
	thhandle = _beginthread(_Thread, 0, this); //ASSERT(thhandle);
}

CSpeak::~CSpeak()

{
	xCleanUp();
	//::CoUninitialize();
}

//////////////////////////////////////////////////////////////////////////

void	CSpeak::Speak(const TCHAR *str)

{
	CString cstr(str);
	Speak(cstr);
}


void	CSpeak::Speak(CString &str)

{
	reenter = true;

	//P2N(_T("Speak '%s'\r\n"), str);
	
	memset(outp, 0, sizeof(outp));

#ifndef _UNICODE
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str), (LPWSTR)outp, sizeof(outp));
#else
	TCHAR *buff = str.GetBuffer();
	memcpy(outp, buff, min(str.GetLength() * sizeof(TCHAR), 
						sizeof(outp) / sizeof(TCHAR)));
#endif

	//P2N(_T("CSpeak::Speak '%s'\r\n"), outp);

	SetEvent(thhevent);
}

int	CSpeak::Prop()

{
	HRESULT hr = S_OK;
	int fSupported = false;

#ifdef SPEAK
    // display properties UI for the current TTS engine
    hr = pVoice->IsUISupported(SPDUI_EngineProperties, NULL, NULL, &fSupported);
    // Check hr

	P2N(_T("DisplayUI supp=%d hr=%d %d\r\n"), fSupported, hr, S_OK);
    	
    if(fSupported == TRUE)
		{
		hr = pVoice->DisplayUI(AfxGetMainWnd()->m_hWnd, 
					(L"Voice"), SPDUI_EngineProperties, NULL, NULL);

		P2N(_T("DisplayUI hr=%d %d\r\n"), hr, FAILED(hr));
    	}
#endif
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void	CSpeak::Stop()

{
	TerminateThread((HANDLE)thhandle, 0);
	
	xCleanUp();

	reenter = false;

	// Post stop status
	if(postto)
		{
#ifdef SPEAK
		::PostMessage(postto, WM_USER + 1, SPRS_DONE, 0);												
#endif
		}

	// Restart the thead and init the COM interface again
	thhandle = _beginthread(_Thread, 0, this); //ASSERT(thhandle);
}

//////////////////////////////////////////////////////////////////////////
// initialze TTS engine in the thread

void	CSpeak::_Thread(void *lpParam)

{
	CSpeak *ts = (CSpeak*)lpParam;

	// Make sure you call this in you app first
	if (FAILED(::CoInitialize(NULL)))
		{
		TRACE(_T("Could not init COM\r\n"));
		}

#ifdef SPEAK
	HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, 
						(void **)&ts->pVoice);

	if( SUCCEEDED( hr ) )
		{
		//P2N(_T("Got voice subsystem\r\n"));	
		}
#endif
		
	//////////////////////////////////////////////////////////////////////////

	while(true)
		{
		WaitForSingleObject(ts->thhevent, INFINITE);

#ifdef SPEAK
		if(ts->pVoice)
			{
			//TRACE(_T("_Thread event sound: '%S'\r\n"), ts->outp);		
			
			HRESULT hr = ts->pVoice->Speak((WCHAR*)ts->outp, SPF_ASYNC, NULL);

			SPVOICESTATUS ss;

			DWORD	ulInputWordLen = -1, ulInputWordPos = -1;

			while(true)
				{
				// Object vanished (async close)
				if(!ts->pVoice)
					break;

				ts->pVoice->GetStatus(&ss, NULL);

				//P2N(_T("status run=%d pos=%d len=%d\r\n"), 
				//			ss.dwRunningState, ss.ulInputWordPos, 
				//				ss.ulInputWordLen);

				if(ts->postto)
					{	
					// Changed?
					
					//P2N(_T("Posting message %d\r\n"), ss.dwRunningState);


					if(ulInputWordPos != ss.ulInputWordPos || 
							ulInputWordLen != ss.ulInputWordLen)
						{
						::PostMessage(ts->postto, WM_USER + 1, ss.dwRunningState,						
								(ss.ulInputWordPos << 16) + ss.ulInputWordLen);
						}
					ulInputWordPos = ss.ulInputWordPos;
					ulInputWordLen = ss.ulInputWordLen;
					}
			
				if(ss.dwRunningState  == SPRS_DONE)
					{
					reenter = false;

					if(ts->postto)
						{
						// Always post final one
						//P2N(_T("Posting done message %d\r\n"), ss.dwRunningState);

						::PostMessage(ts->postto, WM_USER + 1, ss.dwRunningState,						
								(ss.ulInputWordPos << 16) + ss.ulInputWordLen);
						}
					break;
					}

				// Make the processor happy
				Sleep(100);
				}
			}
#endif
		}
}

//////////////////////////////////////////////////////////////////////////

void CSpeak::xCleanUp()

{
#ifdef SPEAK
	if(pVoice)
		{
		pVoice->Release();	pVoice = NULL;	
		}
#endif
}

//////////////////////////////////////////////////////////////////////////

void CSpeak::Skip(int num)

{
	
#ifdef SPEAK
	DWORD nnn;
	pVoice->Skip((L"SENTENCE"), num, &nnn);
#endif

}

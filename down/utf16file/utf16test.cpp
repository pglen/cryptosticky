// utf16test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "UTF16File.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

// Removed by JW (sloppy)
// using namespace std;

int _tmain(int /*argc*/, TCHAR* /*argv*/[], TCHAR* /*envp*/[])
{
	int nRetCode = 0;
	
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		std::cerr << _T("Fatal Error: MFC initialization failed") << std::endl;
		nRetCode = 1;
	}
	else
	{
		
        TRY
		{
            ///////////////////////////////////////////////
            ///////////////////////////////////////////////
            //
            // Write Tests
            //
            ///////////////////////////////////////////////
            ///////////////////////////////////////////////
			CString strLen;

			OutputDebugString(_T("Creating test files\n\n"));
			
            CUTF16File output1(_T("unicode_write.txt"), CFile::modeWrite | CFile::modeCreate);
			strLen.Format(_T("Before write, unicode_write.txt: %d bytes\n"), output1.GetLength());OutputDebugString(strLen);
			
			output1.WriteString(_T("SOME_KEY=Курсив\n"), TRUE);
			output1.WriteString(_T("Hello World from Unicode land!"), TRUE);
			strLen.Format(_T("After write,  unicode_write.txt: %d bytes\n\n"), output1.GetLength());OutputDebugString(strLen);
			
            output1.Close();
			
            CUTF16File output2;
			output2.Open(_T("ansi_write.txt"), CFile::modeWrite | CFile::modeCreate);
			strLen.Format(_T("Before write, ansi_write.txt: %d bytes\n"), output2.GetLength());OutputDebugString(strLen);
			
            output2.WriteString(_T("Hello World from ANSI land!"));
			strLen.Format(_T("After write,  ansi_write.txt: %d bytes\n\n"), output2.GetLength());OutputDebugString(strLen);
            
			output2.Close();


            ///////////////////////////////////////////////
            ///////////////////////////////////////////////
            //
            // Read Tests
            //
            ///////////////////////////////////////////////
            ///////////////////////////////////////////////

            CString szInput;

            CUTF16File input1(_T("unicode_write.txt"), CFile::modeRead);        

            input1.ReadString(szInput);

            OutputDebugString(_T("Unicode read test:\n"));
            OutputDebugString(szInput);
            OutputDebugString(_T("\n\n"));

            input1.Close();

            CUTF16File input2;
            
            input2.Open( _T("ansi_write.txt"), CFile::modeRead);        
            input2.ReadString(szInput);

            OutputDebugString(_T("ANSI read test:\n"));
            OutputDebugString(szInput);
            OutputDebugString(_T("\n\n"));

            input2.Close();
        }
		CATCH(CFileException, e)
		{
            #ifdef _DEBUG
                afxDump << _T("File error: ") << e->m_cause << _T("\n");
            #endif
				e->Delete();
        }
		END_CATCH
	}

	return nRetCode;
}



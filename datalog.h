
/* =====[ datalog.h ]========================================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the datalog.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/17/2010  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

class CDataLog

{
public:

	CDataLog();
	~CDataLog();

	void	open_datalog();
	void	close_datalog();
	void	delete_datalog();

	int     PrintToLog(const TCHAR *Format, ...);

protected:

	CString logfile;
	CFile	datalog;

};

// So it is accesscable just by including 

extern CDataLog datalog;
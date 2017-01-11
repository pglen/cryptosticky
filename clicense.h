
/* =====[ clicense.h ]========================================================== 
                                                                             
   Description:     The CryptoSticky project, implementation of the clicense.h                
                                                                             
                    Defines the behavior for the application.          
                                                                             
   Compiled:        MS-VC 6.00                                               
                                                                             
   Notes:           <Empty Notes>                                            
                                                                             
   Revisions:                                                                
                                                                             
      REV     DATE        BY            DESCRIPTION                       
      ----  --------  -----------  ----------------------------   
      0.00  6/15/2010  Peter Glen   Initial version.                         
                                                                             
   ======================================================================= */

#define LIC_MODULUS 123

extern int		CheckLic(const TCHAR *str, int *res);

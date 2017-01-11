///////////////////////////////////////////////////////////////////////////
// Suppor header files


#ifndef ROTATE_LONG_RIGHT
#define     ROTATE_LONG_LEFT(x, n)  (((x) << (n))  | ((x) >> (32 - (n))))
#define     ROTATE_LONG_RIGHT(x, n) (((x) >> (n))  | ((x) << (32 - (n))))
#endif

extern	int     YieldToWinEx();

CString time_t2str(time_t tme);
CString time_t2tstr(time_t tme);

void	ShortenStr(CString &str, int len);
void	ShortenStrL(CString &str, int len);

int		HashString(const char *name);
int     YieldToWinEx();

void	PathToFname(CString &docname);


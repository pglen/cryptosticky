cd ..
nmake -f cryptosticky.mak CFG="CryptoSticky - Win32 Release"
cd install
"C:\Program Files\NSIS\makensisw.exe" cryptosticky.nsi


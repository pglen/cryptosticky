;///////////////////////////////////////////////////////////////////////////
;//

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

	!insertmacro MUI_DEFAULT MUI_UNWELCOMEFINISHPAGE_BITMAP "setup.bmp"
	!insertmacro MUI_DEFAULT MUI_WELCOMEFINISHPAGE_BITMAP "setup.bmp"

;--------------------------------
;Name and file

  Name "CryptoSticky"
  outfile "bin\crsetup2.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\CryptoSticky2"

  BrandingText " CryptoSticky Pro "

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\CryptoSticky2" ""

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !define 	 	MUI_FINISHPAGE_RUN $PROGRAMFILES\CryptoSticky2\CryptoSticky.exe
    !insertmacro MUI_PAGE_FINISH
	
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

    icon ym.ico
    uninstallicon ym.ico

	;-------------------------------
; Test if Visual Studio Redistributables 2005+ SP1 installed
; Returns -1 if there is no VC redistributables intstalled
;Function CheckVCRedist
;   Push $R0
;   
;   ClearErrors
;   ReadRegDword $R0 HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{7299052b-02a4-4627-81f2-1818da5d550d}" "Version";;

   ; if VS 2005+ redist SP1 not installed, install it
   ;IfErrors 0 VSRedistInstalled
   ;StrCpy $R0 "-1"

;VSRedistInstalled:
;   Exch $R0 
   
 ;FunctionEnd
   
;--------------------------------
;Installer Sections

Section "Install"

;  !error "exit"

    SetShellVarContext all

    SetOutPath "$INSTDIR"

    ; Add your own files here...
    File  /oname=CryptoSticky2.exe ..\release\CryptoSticky.exe
	File  /oname=crstart2.exe  ..\crstart\release\crstart.exe
	
    File  ..\wav\*.wav

    File  readme.txt
    File  revisions.txt
    File  license.txt
	File  vcredist_x86sp1x.exe
    File  ..\Debug\spell.txt
    ; File  ..\Debug\spell.idx

    File  /r /x ".svn" /x "Debug\html" /x "doc" /x "old" /x "tmp" /x "cpt" /x "*.cpt" /x "*.xcpt" /x "_autosave" "..\html"

	;call CheckVCRedist
	
	;${If} $0 ==  "-1"              
	#ExecWait "vcredist_x86sp1x.exe /q"
	;${EndIf}
	
	; Nuke it when we are done
	#Delete "vcredist_x86sp1x.exe"
	
	;SetOutPath "$SYSDIR"

	;SetOverwrite off
    ;File  /nonfatal .\dlls\*
    ;SetOverwrite on
	
    ; ---------------------------------------------------------------------
    ;CreateDirectory "C:\CryptoSticky"
    ;SetOutPath "C:\CryptoSticky"

    ; Copy it ...
    ; File /r "c:\CryptoSticky-org\"

    ; ---------------------------------------------------------------------
    SetOutPath $INSTDIR

    CreateDirectory $SMPROGRAMS\CryptoSticky2
	
    CreateShortCut "$SMPROGRAMS\CryptoSticky2\CryptoSticky2.lnk" "$INSTDIR\CryptoSticky2.exe" "" \
                    "$INSTDIR\CryptoSticky.exe" 0  SW_SHOWNORMAL ""

    ;CreateShortCut "$SMPROGRAMS\CryptoSticky2\CryptoSticky.lnk" "$INSTDIR\CryptoSticky2.exe" "" \
    ;               "$INSTDIR\CryptoSticky.exe" 0  SW_SHOWNORMAL "ALT|CONTROL|S"

	CreateShortCut "$SMPROGRAMS\CryptoSticky2\CryptoSticky.lnk" "$INSTDIR\CryptoSticky2.exe" "" \
                    "$INSTDIR\CryptoSticky.exe" 0  SW_SHOWNORMAL ""

	CreateShortCut "$SMPROGRAMS\CryptoSticky2\Readme.lnk"      "$INSTDIR\readme.txt"

    CreateShortCut "$QUICKLAUNCH\CryptoSticky2.lnk" "$INSTDIR\CryptoSticky2.exe"

    ;CreateShortCut "$SENDTO\CryptoSticky2.lnk" "$INSTDIR\CryptoSticky.exe"

    CreateShortCut "$SMPROGRAMS\CryptoSticky2\CryptoSticky Web Site.lnk" "http://www.robotmonkeysoftware.com/cryptosticky/" "" \
                    "" 0  SW_SHOWNORMAL

    CreateShortCut "$SMPROGRAMS\CryptoSticky\HTML Documentation.lnk" "$INSTDIR\html\index.html"

    CreateDirectory $SMPROGRAMS\CryptoSticky2\Tools
	
	CreateShortCut "$SMPROGRAMS\CryptoSticky\Tools\crstart.lnk" "$INSTDIR\crstart.exe" "" \
                    "$INSTDIR\CryptoSticky2.exe" 0  SW_SHOWNORMAL "CONTROL|ALT|S"

    CreateShortCut "$SMPROGRAMS\CryptoSticky2\Tools\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    CreateShortCut "$SMPROGRAMS\CryptoSticky2\Tools\License.lnk" "$INSTDIR\License.txt"
    CreateShortCut "$DESKTOP\CryptoSticky.lnk" "$INSTDIR\CryptoSticky.exe"

	GetFileTime    "$DESKTOP\CryptoSticky.lnk" $R0 $R1

    ;Store installation folder
    WriteRegStr HKCU "Software\CryptoSticky2" "" $INSTDIR

    ;Store installation timestamp
    WriteRegStr HKCU "Software\CryptoSticky2" "FirstRun1" $R0
    WriteRegStr HKCU "Software\CryptoSticky2" "FirstRun2" $R1

    ;Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"

    ; Create control panel unistaller
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CryptoSticky2"  "DisplayName" "CryptoSticky "
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CryptoSticky2"  "UninstallString" $INSTDIR\Uninstall.exe
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CryptoSticky2"  "DisplayIcon" $INSTDIR\CryptoSticky.exe,0"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CryptoSticky2"  "DisplayVersion" "Version 2.0"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CryptoSticky2"  "HelpLink" "http://www.robotmonkeysoftware.com/cryptosticky/"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CryptoSticky2"  "NoMod	y" 1
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CryptoSticky2"  "NoRepair" 1

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

    SetShellVarContext all

    Delete "$QUICKLAUNCH\CryptoSticky.lnk"
    Delete "$DESKTOP\CryptoSticky.lnk"

	DeleteRegValue	HKCU "Software\Microsoft\Windows\CurrentVersion\Run" "CryptoSticky2"
    DeleteRegKey 	HKCU "Software\CryptoSticky"
    DeleteRegKey 	HKCR ".CryptoSticky"
    DeleteRegKey 	HKCR "CryptoSticky.FileHandler"
    DeleteRegKey 	HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\CryptoSticky2"

    RMDir /r "$SMPROGRAMS\CryptoSticky\"
    RMDir /r "$INSTDIR"


SectionEnd



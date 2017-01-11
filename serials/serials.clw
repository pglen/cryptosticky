; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLogDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "serials.h"

ClassCount=4
Class1=CSerialsApp
Class2=CSerialsDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_SERIALS_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDD_DIALOG3
Class4=CLogDlg
Resource5=IDR_ACCELERATOR1

[CLS:CSerialsApp]
Type=0
HeaderFile=serials.h
ImplementationFile=serials.cpp
Filter=N

[CLS:CSerialsDlg]
Type=0
HeaderFile=serialsDlg.h
ImplementationFile=serialsDlg.cpp
Filter=D
LastObject=IDC_BUTTON1
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=serialsDlg.h
ImplementationFile=serialsDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SERIALS_DIALOG]
Type=1
Class=CSerialsDlg
ControlCount=20
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_BUTTON4,button,1342242816
Control4=IDC_STATIC,static,1476526080
Control5=IDC_EDIT2,edit,1484849280
Control6=IDC_BUTTON5,button,1476460544
Control7=IDC_BUTTON1,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT3,edit,1350631552
Control10=IDC_BUTTON6,button,1342242816
Control11=IDC_BUTTON2,button,1342242816
Control12=IDC_BUTTON3,button,1342242816
Control13=IDOK,button,1342242817
Control14=IDC_BUTTON7,button,1342242816
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,static,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_BUTTON8,button,1342242816

[ACL:IDR_ACCELERATOR1]
Type=1
Class=?
Command1=IDOK
CommandCount=1

[DLG:IDD_DIALOG3]
Type=1
Class=CLogDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1208025088
Control3=IDC_EDIT1,edit,1353779204

[CLS:CLogDlg]
Type=0
HeaderFile=LogDlg.h
ImplementationFile=LogDlg.cpp
BaseClass=CDialog
LastObject=1002


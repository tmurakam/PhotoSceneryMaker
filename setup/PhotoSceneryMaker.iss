; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=Photo Scenery Maker
AppVerName=Photo Scenery Maker 1.2.3
AppPublisher=Takuya Murakami
AppPublisherURL=http://scenerytools.sourceforge.net
AppSupportURL=http://scenerytools.sourceforge.net
AppUpdatesURL=http://scenerytools.sourceforge.net
DefaultDirName={pf}\Photo Scenery Maker
DefaultGroupName=Photo Scenery Maker
LicenseFile=D:\USR\FS.SceneryTools\PhotoSceneryMaker\setup\license.txt
DetectLanguageUsingLocale=yes
ShowLanguageDialog=yes

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "jp"; MessagesFile: "compiler:Japanese.isl"

[Tasks]
; NOTE: The following entry contains English phrases ("Create a desktop icon" and "Additional icons"). You are free to translate them into another language if required.
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Files]
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\PhotoSceneryMaker.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\default.po"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\doc\*.html"; DestDir: "{app}\doc"; Flags: ignoreversion
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\doc\*.png"; DestDir: "{app}\doc"; Flags: ignoreversion
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\doc\*.gif"; DestDir: "{app}\doc"; Flags: ignoreversion
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\doc\*.css"; DestDir: "{app}\doc"; Flags: ignoreversion
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\locale\ja_JP\LC_MESSAGES\default.*o"; DestDir: "{app}\locale\ja_JP\LC_MESSAGES"; Flags: ignoreversion

Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\setup\vcl60.bpl"; DestDir: "{sys}"; Flags: restartreplace uninsneveruninstall
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\setup\BORLNDMM.DLL"; DestDir: "{sys}"; Flags: restartreplace uninsneveruninstall
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\setup\cc3260mt.dll"; DestDir: "{sys}"; Flags: restartreplace uninsneveruninstall
Source: "D:\USR\FS.SceneryTools\PhotoSceneryMaker\setup\rtl60.bpl"; DestDir: "{sys}"; Flags: restartreplace uninsneveruninstall
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Photo Scenery Maker"; Filename: "{app}\PhotoSceneryMaker.exe"
Name: "{userdesktop}\Photo Scenery Maker"; Filename: "{app}\PhotoSceneryMaker.exe"; Tasks: desktopicon

[Run]
; NOTE: The following entry contains an English phrase ("Launch"). You are free to translate it into another language if required.
Filename: "{app}\PhotoSceneryMaker.exe"; Description: "Launch Photo Scenery Maker"; Flags: nowait postinstall skipifsilent

[Registry]
Root: HKCR; Subkey: ".psm"; ValueType: string; ValueName: ""; ValueData: "PhotoSceneryMakerProject"; Flags: uninsdeletevalue 

Root: HKCR; Subkey: "PhotoSceneryMakerProject"; ValueType: string; ValueName: ""; ValueData: "Photo Scenery Maker Project"; Flags: uninsdeletekey 

Root: HKCR; Subkey: "PhotoSceneryMakerProject\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\PhotoSceneryMaker.exe,0" 

Root: HKCR; Subkey: "PhotoSceneryMakerProject\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\PhotoSceneryMaker.exe"" ""%1"""

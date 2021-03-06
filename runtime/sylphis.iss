; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=Sylphis SDK
AppVerName=Sylphis Release 0.92
AppPublisher=Sylphis Dev Team
AppPublisherURL=http://www.sylphis3d.com
DefaultDirName=c:\Sylphis
DefaultGroupName=Sylphis
LicenseFile=LICENSE.txt
AppCopyright=Copyright � 2002, 2003, 2004, 2005 Harry Kalogirou
Compression=bzip/9
; uncomment the following line if you want your installation to run on NT 3.51 too.
; MinVersion=4,3.51

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; MinVersion: 4,4

[Files]
Source: "sylphis.exe"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "msvcr71.dll"; DestDir: "{sys}"; Flags: onlyifdoesntexist uninsneveruninstall
Source: "openal32.dll"; DestDir: "{sys}"; Flags: onlyifdoesntexist uninsneveruninstall
Source: "python22.dll"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "stdlib\*"; DestDir: "{app}\stdlib"; CopyMode: alwaysoverwrite
Source: "stdlib\wxPython\*"; DestDir: "{app}\stdlib\wxPython"; CopyMode: alwaysoverwrite
Source: "stdlib\wxPython\lib\*"; DestDir: "{app}\stdlib\wxPython\lib"; CopyMode: alwaysoverwrite
Source: "stdlib\xml\*"; DestDir: "{app}\stdlib\xml"; CopyMode: alwaysoverwrite
Source: "stdlib\xml\parsers\*"; DestDir: "{app}\stdlib\xml\parsers"; CopyMode: alwaysoverwrite
Source: "system\*"; DestDir: "{app}\system"; CopyMode: alwaysoverwrite
Source: "readme.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite
Source: "LICENSE.txt"; DestDir: "{app}"; CopyMode: alwaysoverwrite

Source: "base\base0.col"; DestDir: "{app}\base"; CopyMode: alwaysoverwrite
Source: "base\base1.col"; DestDir: "{app}\base"; CopyMode: alwaysoverwrite
;Source: "base\core\*"; DestDir: "{app}\base\core"; CopyMode: alwaysoverwrite
;Source: "base\core\Debugger\*"; DestDir: "{app}\base\core\Debugger"; CopyMode: alwaysoverwrite
;Source: "base\devenv\*"; DestDir: "{app}\base\devenv"; CopyMode: alwaysoverwrite
Source: "base\devenv\Images\Debug\*"; DestDir: "{app}\base\devenv\Images\Debug"; CopyMode: alwaysoverwrite
;Source: "base\game\*"; DestDir: "{app}\base\game"; CopyMode: alwaysoverwrite
;Source: "base\game\actors\*"; DestDir: "{app}\base\game\actors"; CopyMode: alwaysoverwrite
;Source: "base\pyui\*"; DestDir: "{app}\base\pyui"; CopyMode: alwaysoverwrite
;Source: "base\pyui\renderers\*"; DestDir: "{app}\base\pyui\renderers"; CopyMode: alwaysoverwrite
;Source: "base\pyui\themes\*"; DestDir: "{app}\base\pyui\themes"; CopyMode: alwaysoverwrite
;Source: "base\ui\*"; DestDir: "{app}\base\ui"; CopyMode: alwaysoverwrite

Source: "base\system.cfg"; DestDir: "{app}\base"; CopyMode: alwaysoverwrite

;Source: "base\fonts\*"; DestDir: "{app}\base\fonts"; CopyMode: alwaysoverwrite
;Source: "base\lights\*"; DestDir: "{app}\base\lights"; CopyMode: alwaysoverwrite
;Source: "base\scripts\*"; DestDir: "{app}\base\scripts"; CopyMode: alwaysoverwrite
;Source: "base\textures\*"; DestDir: "{app}\base\textures"; CopyMode: alwaysoverwrite
;Source: "base\textures\common\*"; DestDir: "{app}\base\textures\common"; CopyMode: alwaysoverwrite

Source: "tools\mc.exe"; DestDir: "{app}\tools"; CopyMode: alwaysoverwrite
Source: "tools\QuArK"; Flags: recursesubdirs; DestDir: "{app}\tools"; CopyMode: alwaysoverwrite

[Icons]
Name: "{group}\Sylphis"; Filename: "{app}\sylphis.exe"; Parameters: "-map last.cmap"; WorkingDir: "{app}"
Name: "{group}\Readme.txt"; Filename: "{app}\readme.txt"; WorkingDir: "{app}"
Name: "{group}\Uninstall Sylphis"; Filename: "{app}\unins000.exe";
Name: "{userdesktop}\Sylphis"; Filename: "{app}\sylphis.exe"; Parameters: "-map last.cmap"; WorkingDir: "{app}"; MinVersion: 4,4; Tasks: desktopicon

[Run]
Filename: "{app}\readme.txt"; Description: "View Readme.txt"; Flags: shellexec postinstall skipifsilent
Filename: "{app}\sylphis.exe"; Parameters: "-map last.cmap"; Description: "Launch Sylphis"; Flags: nowait postinstall skipifsilent unchecked


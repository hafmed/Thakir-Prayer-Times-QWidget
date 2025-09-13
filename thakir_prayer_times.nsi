Name "thakir_prayer_times HAFIANE Mohamed"
OutFile "thakir_prayer_times_win64_install_2.0.1.exe"

Icon "thakir_prayer_times.ico"
UninstallIcon "thakir_prayer_times.ico"


DirText "Close and Uninstall previous version if exist and Choose directory for installing (thakir_prayer_times) Ver 2.0.1 du 5-8-2022"

InstallDir "$PROGRAMFILES\thakir_prayer_times"

Section "Install"
  SetOutPath $INSTDIR
  File "C:\Qt\6.3.1\mingw_64\bin\libgcc_s_seh-1.dll"
  File "C:\Qt\6.3.1\mingw_64\bin\libstdc++-6.dll"
  File "C:\Qt\6.3.1\mingw_64\bin\libwinpthread-1.dll"
  File "C:\Qt\6.3.1\mingw_64\bin\Qt6Core.dll"
  File "C:\Qt\6.3.1\mingw_64\bin\Qt6Gui.dll"
  ;File "C:\Qt\6.3.1\mingw_64\bin\Qt6PrintSupport.dll"
  File "C:\Qt\6.3.1\mingw_64\bin\Qt6Widgets.dll"
  File "C:\Qt\6.3.1\mingw_64\bin\Qt6Multimedia.dll"
  File "C:\Qt\6.3.1\mingw_64\bin\Qt6Network.dll"
CreateDirectory $INSTDIR\platforms
SetOutPath $INSTDIR\platforms
  File "C:\Qt\6.3.1\mingw_64\plugins\platforms\qwindows.dll"
SetOutPath $INSTDIR 
 File "..\CompilationWin\release\thakir_prayer_times.exe"
 File "thakir_prayer_times.ico"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\thakir_prayer_times" "DisplayName" "thakir_prayer_times"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\thakir_prayer_times" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteUninstaller "uninstall.exe"
SectionEnd

Section "Shortcuts"
  CreateDirectory "$SMPROGRAMS\thakir_prayer_times"
  SetOutPath $INSTDIR
  CreateShortCut "$SMPROGRAMS\thakir_prayer_times\thakir_prayer_times.lnk""$INSTDIR\thakir_prayer_times.exe"
  SetOutPath $INSTDIR
  CreateShortCut "$DESKTOP\thakir_prayer_times.lnk""$INSTDIR\thakir_prayer_times.exe"
  CreateShortCut "$QUICKLAUNCH\thakir_prayer_times.lnk""$INSTDIR\thakir_prayer_times.exe"
  CreateShortCut "$SMPROGRAMS\thakir_prayer_times\uninstall_thakir_prayer_times.lnk""$INSTDIR\uninstall.exe"
SectionEnd

UninstallText "Close thakir_prayer_times if running ; This will uninstall thakir_prayer_times from your system"

Section "Uninstall"
DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\thakir_prayer_times"
Delete "$SMPROGRAMS\thakir_prayer_times\thakir_prayer_times.lnk"
Delete "$SMPROGRAMS\thakir_prayer_times\uninstall_thakir_prayer_times.lnk"
RmDir "$SMPROGRAMS\thakir_prayer_times"
Delete "$DESKTOP\thakir_prayer_times.lnk"
Delete "$QUICKLAUNCH\thakir_prayer_times.lnk"
Delete "$INSTDIR\libgcc_s_seh-1.dll"
Delete "$INSTDIR\libstdc++-6.dll"
Delete "$INSTDIR\libwinpthread-1.dll"
Delete "$INSTDIR\Qt6Core.dll"
Delete "$INSTDIR\Qt6Gui.dll"
;Delete "$INSTDIR\Qt6PrintSupport.dll"
Delete "$INSTDIR\Qt6Widgets.dll"
Delete "$INSTDIR\Qt6Multimedia.dll"
Delete "$INSTDIR\Qt6Network.dll"
Delete "$INSTDIR\platforms\qwindows.dll"
RMDir $INSTDIR\platforms
Delete "$INSTDIR\thakir_prayer_times.exe"
Delete "$INSTDIR\thakir_prayer_times.ico"
Delete "$INSTDIR\uninstall.exe"
RmDir "$INSTDIR"
SectionEnd

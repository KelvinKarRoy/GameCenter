# Microsoft Developer Studio Project File - Name="Game" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Game - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Game.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Game.mak" CFG="Game - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Game - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Game - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Game - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Game - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Game - Win32 Release"
# Name "Game - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bitmap.rc
# End Source File
# Begin Source File

SOURCE=.\WinMain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\bitmap.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap_2048.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_20482.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_back.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_back2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_bg1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_bg2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell1024.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell128.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell16.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell2048.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell256.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell32.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell4.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell512.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell64.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_cell8.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_chooseBlack.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_chooseBlack2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_chooseWhite.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_chooseWhite2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_heibaiqi.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_heibaiqi2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_home.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_lose1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_lose2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_start.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_tie.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_undo.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_undo2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_win1.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_win2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_wiziBlack.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_wuzi.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_wuzi2.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap_wuziWhite.bmp
# End Source File
# Begin Source File

SOURCE=.\chooseBG.bmp
# End Source File
# Begin Source File

SOURCE=.\icon.ico
# End Source File
# End Group
# End Target
# End Project

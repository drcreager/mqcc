# Microsoft Developer Studio Project File - Name="komqput" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=komqput - Win32 NTDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "komqput.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "komqput.mak" CFG="komqput - Win32 NTDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "komqput - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "komqput - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "komqput - Win32 NTDebug" (based on "Win32 (x86) Console Application")
!MESSAGE "komqput - Win32 NTRelease" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "komqput - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PutRelease"
# PROP Intermediate_Dir "PutRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Od /I "..\\" /I "..\Common" /I "..\komqlib" /I "..\komqapi" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\komqlib\release\komqlib.lib ..\komqapi\release\komqapic.lib ..\common\release\common.lib /nologo /subsystem:console /machine:I386 /out:"PutRelease/komqputc.exe" /libpath:"c:\mqclient\tools\lib" /libpath:"..\komqapi\release" /libpath:"..\common\release" /libpath:"..\komqlib\release"

!ELSEIF  "$(CFG)" == "komqput - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PutDebug"
# PROP Intermediate_Dir "PutDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\Common" /I "..\komqlib" /I "..\komqapi" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\komqlib\debug\komqlib.lib ..\komqapi\debug\komqapic.lib ..\common\debug\common.lib /nologo /subsystem:console /debug /machine:I386 /out:"PutDebug\komqputc.exe" /pdbtype:sept /libpath:"c:\mqclient\tools\lib" /libpath:"..\komqapi\debug" /libpath:"..\common\debug" /libpath:"..\komqlib\debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "komqput - Win32 NTDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "komqput_"
# PROP BASE Intermediate_Dir "komqput_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PutNTDebug"
# PROP Intermediate_Dir "PutNTDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /I "..\\" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\Common" /I "..\komqlib" /I "..\komqapi" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\mqapi\debug\komqapic.lib /nologo /subsystem:console /debug /machine:I386 /out:"Put Debug/komqputc.exe" /pdbtype:sept
# ADD LINK32 ..\komqapi\NTDebug\komqapi.lib ..\common\debug\common.lib ..\komqlib\NTDebug\komqlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"c:\mqclient\tools\lib"

!ELSEIF  "$(CFG)" == "komqput - Win32 NTRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "komqput0"
# PROP BASE Intermediate_Dir "komqput0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "PutNTRelease"
# PROP Intermediate_Dir "PutNTRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "..\\" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\\" /I "c:\mqclient\tools\c\include" /I "..\Common" /I "..\komqlib" /I "..\komqapi" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\mqapi\release\komqapic.lib /nologo /subsystem:console /machine:I386 /out:"Release/komqputc.exe"
# ADD LINK32 ..\komqapi\NTRelease\komqapi.lib ..\common\release\common.lib ..\komqlib\NTRelease\komqlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /libpath:"c:\mqclient\tools\lib"

!ENDIF 

# Begin Target

# Name "komqput - Win32 Release"
# Name "komqput - Win32 Debug"
# Name "komqput - Win32 NTDebug"
# Name "komqput - Win32 NTRelease"
# Begin Source File

SOURCE=.\komqput.cpp
# End Source File
# Begin Source File

SOURCE=.\putversion.rc
# End Source File
# End Target
# End Project

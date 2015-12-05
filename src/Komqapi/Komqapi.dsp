# Microsoft Developer Studio Project File - Name="Komqapi" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Komqapi - Win32 NTDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Komqapi.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Komqapi.mak" CFG="Komqapi - Win32 NTDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Komqapi - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Komqapi - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Komqapi - Win32 NTDebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Komqapi - Win32 NTRelease" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Komqapi - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\\" /I "c:\mqclient\tools\c\include" /I "..\Common" /I "..\Komqlib" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mqic32.lib ..\common\release\common.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBC" /out:"Release/Komqapic.dll" /libpath:"c:\mqclient\tools\lib" /libpath:"..\Common\Release"

!ELSEIF  "$(CFG)" == "Komqapi - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\" /I "c:\mqclient\tools\c\include" /I "..\Common" /I "..\Komqlib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mqic32.lib ..\common\debug\common.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"Debug/Komqapic.dll" /pdbtype:sept /libpath:"c:\mqclient\tools\lib" /libpath:"..\Common\Debug"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "Komqapi - Win32 NTDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Komqapi_"
# PROP BASE Intermediate_Dir "Komqapi_"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "NTDebug"
# PROP Intermediate_Dir "NTDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\\" /I "c:\mqclient\tools\c\include" /I "..\Common" /I "..\Komqlib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\" /I "c:\mqclient\tools\c\include" /I "..\Common" /I "..\Komqlib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 mqic32.lib ..\common\debug\common.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/Komqapic.dll" /pdbtype:sept /libpath:"c:\mqclient\tools\lib" /libpath:"..\Common\Debug"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 mqm.lib ..\common\debug\common.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"LIBC" /pdbtype:sept /libpath:"c:\mqclient\tools\lib" /libpath:"..\Common\Debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Komqapi - Win32 NTRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Komqapi0"
# PROP BASE Intermediate_Dir "Komqapi0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "NTRelease"
# PROP Intermediate_Dir "NTRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\\" /I "c:\mqclient\tools\c\include" /I "..\Common" /I "..\Komqlib" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\\" /I "c:\mqclient\tools\c\include" /I "..\Common" /I "..\Komqlib" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mqic32.lib ..\common\release\common.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/Komqapic.dll" /libpath:"c:\mqclient\tools\lib" /libpath:"..\Common\Release"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mqm.lib ..\common\release\common.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"LIBC" /libpath:"c:\mqclient\tools\lib" /libpath:"..\Common\Release"

!ENDIF 

# Begin Target

# Name "Komqapi - Win32 Release"
# Name "Komqapi - Win32 Debug"
# Name "Komqapi - Win32 NTDebug"
# Name "Komqapi - Win32 NTRelease"
# Begin Source File

SOURCE=.\apiversion.rc
# End Source File
# Begin Source File

SOURCE=.\CAPIParmSet.cpp
# End Source File
# Begin Source File

SOURCE=.\CAPIParmSet.h
# End Source File
# Begin Source File

SOURCE=.\CCorrelationField.cpp
# End Source File
# Begin Source File

SOURCE=.\CCorrelationField.h
# End Source File
# Begin Source File

SOURCE=.\Cerror.cpp
# End Source File
# Begin Source File

SOURCE=.\Cerror.h
# End Source File
# Begin Source File

SOURCE=.\CKeyField.cpp
# End Source File
# Begin Source File

SOURCE=.\CKeyField.h
# End Source File
# Begin Source File

SOURCE=.\Cqueue.cpp
# End Source File
# Begin Source File

SOURCE=.\Cqueue.h
# End Source File
# Begin Source File

SOURCE=.\CQueueMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CQueueMgr.h
# End Source File
# Begin Source File

SOURCE=.\Cqueues.cpp
# End Source File
# Begin Source File

SOURCE=.\Cqueues.h
# End Source File
# Begin Source File

SOURCE=.\Dllmain.cpp
# End Source File
# Begin Source File

SOURCE=.\Dllmain.h
# End Source File
# Begin Source File

SOURCE=.\Komqapic.def
# End Source File
# End Target
# End Project

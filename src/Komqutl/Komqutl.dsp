# Microsoft Developer Studio Project File - Name="Komqutl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Komqutl - Win32 NTDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Komqutl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Komqutl.mak" CFG="Komqutl - Win32 NTDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Komqutl - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Komqutl - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "Komqutl - Win32 NTRelease" (based on "Win32 (x86) Console Application")
!MESSAGE "Komqutl - Win32 NTDebug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Komqutl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UtlRelease"
# PROP Intermediate_Dir "UtlRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /Od /I "..\komqapi" /I "..\Common" /I "..\\" /I "..\komqlib" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /FAs /Fa"UtlRelease/Komqutlc.lst" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\komqlib\release\komqlib.lib ..\komqapi\release\komqapic.lib ..\common\release\common.lib /nologo /subsystem:console /machine:I386 /out:"UtlRelease/Komqutlc.exe"
# SUBTRACT LINK32 /pdb:none /map /debug

!ELSEIF  "$(CFG)" == "Komqutl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UtlDebug"
# PROP Intermediate_Dir "UtlDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\Common" /I "..\komqlib" /I "..\komqapi" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\komqlib\debug\komqlib.lib ..\komqapi\debug\komqapic.lib ..\common\debug\common.lib /nologo /subsystem:console /debug /machine:I386 /out:"UtlDebug\Komqutlc.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Komqutl - Win32 NTRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Komqutl___Win32_NTRelease"
# PROP BASE Intermediate_Dir "Komqutl___Win32_NTRelease"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UtlNTRelease"
# PROP Intermediate_Dir "UtlNTRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /I "..\\" /I "..\Common" /I "..\komqlib" /I "..\komqapi" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /FAs /Fa"UtlRelease/Komqutlc.lst" /YX /FD /c
# ADD CPP /nologo /W3 /GX /Zi /O2 /I "..\\" /I "..\Common" /I "..\komqlib" /I "..\komqapi" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /FAs /Fa"UtlRelease/Komqutlc.lst" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\komqlib\release\komqlib.lib ..\komqapi\release\komqapic.lib ..\common\release\common.lib /nologo /subsystem:console /pdb:"UtlRelease/Komqutlc.pdb" /map:"UtlRelease/Komqutlc.map" /machine:I386 /out:"UtlRelease/Komqutlc.exe"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 ..\komqapi\NTRelease\komqapi.lib ..\common\release\common.lib ..\komqlib\NTRelease\komqlib.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"UtlNTRelease\Komqutlc.pdb" /machine:I386
# SUBTRACT LINK32 /pdb:none /map /debug

!ELSEIF  "$(CFG)" == "Komqutl - Win32 NTDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Komqutl___Win32_NTDebug"
# PROP BASE Intermediate_Dir "Komqutl___Win32_NTDebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "NTDebug"
# PROP Intermediate_Dir "NTDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\Common" /I "..\komqlib" /I "..\komqapi" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\Common" /I "..\komqlib" /I "..\komqapi" /I "c:\mqclient\tools\c\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\komqlib\debug\komqlib.lib ..\komqapi\debug\komqapic.lib ..\common\debug\common.lib /nologo /subsystem:console /debug /machine:I386 /out:"UtlDebug\Komqutlc.exe" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\komqlib\debug\komqlib.lib ..\komqapi\debug\komqapic.lib ..\common\debug\common.lib /nologo /subsystem:console /debug /machine:I386 /out:"UtlDebug\Komqutlc.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Komqutl - Win32 Release"
# Name "Komqutl - Win32 Debug"
# Name "Komqutl - Win32 NTRelease"
# Name "Komqutl - Win32 NTDebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\komqutl.cpp
# End Source File
# Begin Source File

SOURCE=.\utlversion.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=".\Dev Log.txt"

!IF  "$(CFG)" == "Komqutl - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Komqutl - Win32 Debug"

!ELSEIF  "$(CFG)" == "Komqutl - Win32 NTRelease"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Komqutl - Win32 NTDebug"

!ENDIF 

# End Source File
# End Target
# End Project

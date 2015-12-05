# Microsoft Developer Studio Project File - Name="KOMQLIB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=KOMQLIB - Win32 NTDebug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Komqlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Komqlib.mak" CFG="KOMQLIB - Win32 NTDebug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "KOMQLIB - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "KOMQLIB - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "KOMQLIB - Win32 NTDebug" (based on "Win32 (x86) Static Library")
!MESSAGE "KOMQLIB - Win32 NTRelease" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "KOMQLIB - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "c:\mqclient\tools\c\include" /I "..\\" /I "..\Common" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /I "c:\mqclient\tools\c\include" /I "..\\" /I "..\Common" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /D "WIN32" /YX /FD /c
# SUBTRACT CPP /O<none>
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTDebug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "KOMQLIB_"
# PROP BASE Intermediate_Dir "KOMQLIB_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "NTDebug"
# PROP Intermediate_Dir "NTDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Zi /Od /I "c:\mqclient\tools\c\include" /I "..\\" /I "..\Common" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /D "WIN32" /YX /FD /c
# ADD CPP /nologo /W3 /GX /ZI /Od /I "c:\mqclient\tools\c\include" /I "..\\" /I "..\Common" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_POSIX_" /D "WIN32" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTRelease"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "KOMQLIB0"
# PROP BASE Intermediate_Dir "KOMQLIB0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "NTRelease"
# PROP Intermediate_Dir "NTRelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /I "c:\mqclient\tools\c\include" /I "..\\" /I "..\Common" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "c:\mqclient\tools\c\include" /I "..\\" /I "..\Common" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /D "_POSIX_" /YX /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "KOMQLIB - Win32 Release"
# Name "KOMQLIB - Win32 Debug"
# Name "KOMQLIB - Win32 NTDebug"
# Name "KOMQLIB - Win32 NTRelease"
# Begin Source File

SOURCE=.\Any2.cpp
# End Source File
# Begin Source File

SOURCE=.\Any2.h
# End Source File
# Begin Source File

SOURCE=.\Auto_ptr.h
# End Source File
# Begin Source File

SOURCE=.\BatchList.cpp
# End Source File
# Begin Source File

SOURCE=.\BatchList.h
# End Source File
# Begin Source File

SOURCE=.\BlockingSrvc.cpp

!IF  "$(CFG)" == "KOMQLIB - Win32 Release"

# ADD CPP /FAcs /Fa"Release\CBlockingSrvc.lst"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 Debug"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTDebug"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTRelease"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\BlockingSrvc.h
# End Source File
# Begin Source File

SOURCE=.\BufferList.cpp
# End Source File
# Begin Source File

SOURCE=.\BufferList.h
# End Source File
# Begin Source File

SOURCE=.\CmdInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdInterface.h
# End Source File
# Begin Source File

SOURCE=.\Cmdline.cpp
# End Source File
# Begin Source File

SOURCE=.\Cmdline.h
# End Source File
# Begin Source File

SOURCE=.\Ctlfile.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctlfile.h
# End Source File
# Begin Source File

SOURCE=.\Ctlque.cpp
# End Source File
# Begin Source File

SOURCE=.\Ctlque.h
# End Source File
# Begin Source File

SOURCE=.\DataHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\DataHeader.h
# End Source File
# Begin Source File

SOURCE=.\Dataque.cpp
# End Source File
# Begin Source File

SOURCE=.\Dataque.h
# End Source File
# Begin Source File

SOURCE=.\DeadBatch.cpp
# End Source File
# Begin Source File

SOURCE=.\DeadBatch.h
# End Source File
# Begin Source File

SOURCE=.\DeadBatches.cpp
# End Source File
# Begin Source File

SOURCE=.\DeadBatches.h
# End Source File
# Begin Source File

SOURCE=.\DeadLetter.cpp
# End Source File
# Begin Source File

SOURCE=.\DeadLetter.h
# End Source File
# Begin Source File

SOURCE=.\DeadReport.cpp
# End Source File
# Begin Source File

SOURCE=.\DeadReport.h
# End Source File
# Begin Source File

SOURCE=.\DeadSingle.cpp
# End Source File
# Begin Source File

SOURCE=.\DeadSingle.h
# End Source File
# Begin Source File

SOURCE=.\Error.cpp
# End Source File
# Begin Source File

SOURCE=.\Error.h
# End Source File
# Begin Source File

SOURCE=.\Errors.cpp
# End Source File
# Begin Source File

SOURCE=.\Errors.h
# End Source File
# Begin Source File

SOURCE=.\Exceptions.h
# End Source File
# Begin Source File

SOURCE=.\ExtServices.cpp

!IF  "$(CFG)" == "KOMQLIB - Win32 Release"

# ADD CPP /FAcs /Fa"Release\CExtServices.lst"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 Debug"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTDebug"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTRelease"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExtServices.h
# End Source File
# Begin Source File

SOURCE=.\ExtSrvcsParamFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtSrvcsParamFile.h
# End Source File
# Begin Source File

SOURCE=.\FileModificationSrvc.cpp

!IF  "$(CFG)" == "KOMQLIB - Win32 Release"

# ADD CPP /FAcs /Fa"Release\CFileModificationSrvc.lst"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 Debug"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTDebug"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTRelease"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FileModificationSrvc.h
# End Source File
# Begin Source File

SOURCE=.\FileNameSrvc.cpp
# End Source File
# Begin Source File

SOURCE=.\FileNameSrvc.h
# End Source File
# Begin Source File

SOURCE=.\Get.cpp
# End Source File
# Begin Source File

SOURCE=.\Get.h
# End Source File
# Begin Source File

SOURCE=.\Getbatch.cpp
# End Source File
# Begin Source File

SOURCE=.\Getbatch.h
# End Source File
# Begin Source File

SOURCE=.\GetFilesSrvc.cpp
# End Source File
# Begin Source File

SOURCE=.\GetFilesSrvc.h
# End Source File
# Begin Source File

SOURCE=.\GetSingle.cpp
# End Source File
# Begin Source File

SOURCE=.\GetSingle.h
# End Source File
# Begin Source File

SOURCE=.\Isdec.cpp
# End Source File
# Begin Source File

SOURCE=.\Isdec.h
# End Source File
# Begin Source File

SOURCE=.\KOConstants.h
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\MonitoringSrvc.cpp

!IF  "$(CFG)" == "KOMQLIB - Win32 Release"

# ADD CPP /FAcs /Fa"Release\CMonitoringSrvc.lst"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 Debug"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTDebug"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTRelease"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MonitoringSrvc.h
# End Source File
# Begin Source File

SOURCE=.\MsgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MsgFile.h
# End Source File
# Begin Source File

SOURCE=.\ParamFile.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamFile.h
# End Source File
# Begin Source File

SOURCE=.\Put.cpp
# End Source File
# Begin Source File

SOURCE=.\Put.h
# End Source File
# Begin Source File

SOURCE=.\Putbatch.cpp
# End Source File
# Begin Source File

SOURCE=.\Putbatch.h
# End Source File
# Begin Source File

SOURCE=.\PutFilesSrvc.cpp
# End Source File
# Begin Source File

SOURCE=.\PutFilesSrvc.h
# End Source File
# Begin Source File

SOURCE=.\PutSingle.cpp
# End Source File
# Begin Source File

SOURCE=.\PutSingle.h
# End Source File
# Begin Source File

SOURCE=.\Que.cpp
# End Source File
# Begin Source File

SOURCE=.\Que.h
# End Source File
# Begin Source File

SOURCE=.\Quemgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Quemgr.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RespBatch.cpp

!IF  "$(CFG)" == "KOMQLIB - Win32 Release"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 Debug"

# SUBTRACT CPP /YX

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTDebug"

!ELSEIF  "$(CFG)" == "KOMQLIB - Win32 NTRelease"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RespBatch.h
# End Source File
# Begin Source File

SOURCE=.\RespData.cpp
# End Source File
# Begin Source File

SOURCE=.\RespData.h
# End Source File
# Begin Source File

SOURCE=.\RestartFile.cpp
# End Source File
# Begin Source File

SOURCE=.\RestartFile.h
# End Source File
# Begin Source File

SOURCE=.\Service.cpp
# End Source File
# Begin Source File

SOURCE=.\Service.h
# End Source File
# Begin Source File

SOURCE=.\StartupParamFile.cpp
# End Source File
# Begin Source File

SOURCE=.\StartupParamFile.h
# End Source File
# Begin Source File

SOURCE=.\Transfer.cpp
# End Source File
# Begin Source File

SOURCE=.\Transfer.h
# End Source File
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="Common" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Common - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Common.mak" CFG="Common - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Common - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Common - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Common - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_POSIX_" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Common - Win32 Debug"

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
# ADD CPP /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_POSIX_" /FD /c
# SUBTRACT CPP /nologo /YX
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

# Name "Common - Win32 Release"
# Name "Common - Win32 Debug"
# Begin Source File

SOURCE=.\CAny.cpp
# End Source File
# Begin Source File

SOURCE=.\CAny.h
# End Source File
# Begin Source File

SOURCE=.\CBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\CBuffer.h
# End Source File
# Begin Source File

SOURCE=.\CCollection.cpp

!IF  "$(CFG)" == "Common - Win32 Release"

# SUBTRACT CPP /Z<none>

!ELSEIF  "$(CFG)" == "Common - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CCollection.h
# End Source File
# Begin Source File

SOURCE=.\CCursor.cpp
# End Source File
# Begin Source File

SOURCE=.\CCursor.h
# End Source File
# Begin Source File

SOURCE=.\CFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CFile.h
# End Source File
# Begin Source File

SOURCE=.\CFileCatalog.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileCatalog.h
# End Source File
# Begin Source File

SOURCE=.\CFileList.cpp
# End Source File
# Begin Source File

SOURCE=.\CFileList.h
# End Source File
# Begin Source File

SOURCE=.\CItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CItem.h
# End Source File
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\CString.cpp
# End Source File
# Begin Source File

SOURCE=.\CString.h
# End Source File
# Begin Source File

SOURCE=.\CStringList.cpp
# End Source File
# Begin Source File

SOURCE=.\CStringList.h
# End Source File
# Begin Source File

SOURCE=.\CSysCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\CSysCmd.h
# End Source File
# End Target
# End Project

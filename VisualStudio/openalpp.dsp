# Microsoft Developer Studio Project File - Name="openalpp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=openalpp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "openalpp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "openalpp.mak" CFG="openalpp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "openalpp - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "openalpp - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "openalpp - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I "../include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib\openalpp.lib"

!ELSEIF  "$(CFG)" == "openalpp - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /ZI /Od /I "../include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib\openalppd.lib"

!ENDIF 

# Begin Target

# Name "openalpp - Win32 Release"
# Name "openalpp - Win32 Debug"
# Begin Group "Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\audiobase.cpp
# End Source File
# Begin Source File

SOURCE=..\src\audioconvert.cpp
# End Source File
# Begin Source File

SOURCE=..\src\audioenvironment.cpp
# End Source File
# Begin Source File

SOURCE=..\src\deviceupdater.cpp
# End Source File
# Begin Source File

SOURCE=..\src\error.cpp
# End Source File
# Begin Source File

SOURCE=..\src\groupsource.cpp
# End Source File
# Begin Source File

SOURCE=..\src\inputdevice.cpp
# End Source File
# Begin Source File

SOURCE=..\src\listener.cpp
# End Source File
# Begin Source File

SOURCE=..\src\netstream.cpp
# End Source File
# Begin Source File

SOURCE=..\src\netupdater.cpp
# End Source File
# Begin Source File

SOURCE=..\src\sample.cpp
# End Source File
# Begin Source File

SOURCE=..\src\sounddata.cpp
# End Source File
# Begin Source File

SOURCE=..\src\source.cpp
# End Source File
# Begin Source File

SOURCE=..\src\sourcebase.cpp
# End Source File
# Begin Source File

SOURCE=..\src\stream.cpp
# End Source File
# Begin Source File

SOURCE=..\src\streamupdater.cpp
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\include\openalpp\alpp.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\audiobase.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\audioconvert.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\audioenvironment.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\deviceupdater.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\error.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\groupsource.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\inputdevice.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\listener.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\netstream.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\netupdater.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\positionedobject.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\sample.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\sounddata.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\source.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\sourcebase.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\stream.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\streamupdater.h
# End Source File
# Begin Source File

SOURCE=..\include\openalpp\windowsstuff.h
# End Source File
# End Group
# End Target
# End Project

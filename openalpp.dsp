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
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x41d /d "NDEBUG"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x41d /d "_DEBUG"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "openalpp - Win32 Release"
# Name "openalpp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\audiobase.cpp
# End Source File
# Begin Source File

SOURCE=.\audioenviroment.cpp
# End Source File
# Begin Source File

SOURCE=.\deviceupdater.cpp
# End Source File
# Begin Source File

SOURCE=.\error.cpp
# End Source File
# Begin Source File

SOURCE=.\groupsource.cpp
# End Source File
# Begin Source File

SOURCE=.\inputdevice.cpp
# End Source File
# Begin Source File

SOURCE=.\listener.cpp
# End Source File
# Begin Source File

SOURCE=.\netstream.cpp
# End Source File
# Begin Source File

SOURCE=.\netupdater.cpp
# End Source File
# Begin Source File

SOURCE=.\sample.cpp
# End Source File
# Begin Source File

SOURCE=.\sounddata.cpp
# End Source File
# Begin Source File

SOURCE=.\source.cpp
# End Source File
# Begin Source File

SOURCE=.\sourcebase.cpp
# End Source File
# Begin Source File

SOURCE=.\stream.cpp
# End Source File
# Begin Source File

SOURCE=.\streamupdater.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\audiobase.h
# End Source File
# Begin Source File

SOURCE=.\audioenviroment.h
# End Source File
# Begin Source File

SOURCE=.\deviceupdater.h
# End Source File
# Begin Source File

SOURCE=.\error.h
# End Source File
# Begin Source File

SOURCE=.\groupsource.h
# End Source File
# Begin Source File

SOURCE=.\inputdevice.h
# End Source File
# Begin Source File

SOURCE=.\listener.h
# End Source File
# Begin Source File

SOURCE=.\netstream.h
# End Source File
# Begin Source File

SOURCE=.\netupdater.h
# End Source File
# Begin Source File

SOURCE=.\positionedobject.h
# End Source File
# Begin Source File

SOURCE=.\sample.h
# End Source File
# Begin Source File

SOURCE=.\sounddata.h
# End Source File
# Begin Source File

SOURCE=.\source.h
# End Source File
# Begin Source File

SOURCE=.\sourcebase.h
# End Source File
# Begin Source File

SOURCE=.\stream.h
# End Source File
# Begin Source File

SOURCE=.\streamupdater.h
# End Source File
# Begin Source File

SOURCE=.\windowsstuff.h
# End Source File
# End Group
# End Target
# End Project

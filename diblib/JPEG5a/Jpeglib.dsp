# Microsoft Developer Studio Project File - Name="Jpeglib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Jpeglib - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Jpeglib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Jpeglib.mak" CFG="Jpeglib - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Jpeglib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Jpeglib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Win/Graphics/DibLib/JPEG", FFAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Jpeglib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\WinRel"
# PROP BASE Intermediate_Dir ".\WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\LIB\JPEG.LIB"

!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\WinDebug"
# PROP BASE Intermediate_Dir ".\WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /Gf /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fr /FD /c
# SUBTRACT CPP /Gy /YX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\LIB\JPEGD.LIB"

!ENDIF 

# Begin Target

# Name "Jpeglib - Win32 Release"
# Name "Jpeglib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Djpeg.c
# End Source File
# Begin Source File

SOURCE=.\JCAPI.C
# End Source File
# Begin Source File

SOURCE=.\JCCOEFCT.C
# End Source File
# Begin Source File

SOURCE=.\JCCOLOR.C
# End Source File
# Begin Source File

SOURCE=.\JCDCTMGR.C
# End Source File
# Begin Source File

SOURCE=.\JCHUFF.C
# End Source File
# Begin Source File

SOURCE=.\JCMAINCT.C
# End Source File
# Begin Source File

SOURCE=.\JCMARKER.C
# End Source File
# Begin Source File

SOURCE=.\JCMASTER.C
# End Source File
# Begin Source File

SOURCE=.\JCOMAPI.C
# End Source File
# Begin Source File

SOURCE=.\JCPARAM.C
# End Source File
# Begin Source File

SOURCE=.\JCPREPCT.C
# End Source File
# Begin Source File

SOURCE=.\JCSAMPLE.C
# End Source File
# Begin Source File

SOURCE=.\JDAPI.C
# End Source File
# Begin Source File

SOURCE=.\JDATADST.C
# End Source File
# Begin Source File

SOURCE=.\JDATASRC.C
# End Source File
# Begin Source File

SOURCE=.\JDCOEFCT.C
# End Source File
# Begin Source File

SOURCE=.\JDCOLOR.C
# End Source File
# Begin Source File

SOURCE=.\JDDCTMGR.C
# End Source File
# Begin Source File

SOURCE=.\JDHUFF.C
# End Source File
# Begin Source File

SOURCE=.\JDMAINCT.C
# End Source File
# Begin Source File

SOURCE=.\JDMARKER.C
# End Source File
# Begin Source File

SOURCE=.\JDMASTER.C
# End Source File
# Begin Source File

SOURCE=.\JDMERGE.C
# End Source File
# Begin Source File

SOURCE=.\JDPOSTCT.C
# End Source File
# Begin Source File

SOURCE=.\JDSAMPLE.C
# End Source File
# Begin Source File

SOURCE=.\JERROR.C
# End Source File
# Begin Source File

SOURCE=.\JFDCTFLT.C
# End Source File
# Begin Source File

SOURCE=.\JFDCTFST.C
# End Source File
# Begin Source File

SOURCE=.\JFDCTINT.C
# End Source File
# Begin Source File

SOURCE=.\JIDCTFLT.C
# End Source File
# Begin Source File

SOURCE=.\JIDCTFST.C
# End Source File
# Begin Source File

SOURCE=.\JIDCTINT.C
# End Source File
# Begin Source File

SOURCE=.\JIDCTRED.C
# End Source File
# Begin Source File

SOURCE=.\JMEMMGR.C
# End Source File
# Begin Source File

SOURCE=.\JMEMNOBS.C
# End Source File
# Begin Source File

SOURCE=.\JQUANT1.C
# End Source File
# Begin Source File

SOURCE=.\JQUANT2.C
# End Source File
# Begin Source File

SOURCE=.\JUTILS.C
# End Source File
# Begin Source File

SOURCE=.\RDCOLMAP.C
# End Source File
# Begin Source File

SOURCE=.\Winmem.c

!IF  "$(CFG)" == "Jpeglib - Win32 Release"

!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\WRBMP.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\Cderror.h
# End Source File
# Begin Source File

SOURCE=.\Cdjpeg.h
# End Source File
# Begin Source File

SOURCE=.\djpeg.h
# End Source File
# Begin Source File

SOURCE=.\Jconfig.h
# End Source File
# Begin Source File

SOURCE=.\Jdct.h
# End Source File
# Begin Source File

SOURCE=.\Jerror.h
# End Source File
# Begin Source File

SOURCE=.\Jinclude.h
# End Source File
# Begin Source File

SOURCE=.\Jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\Jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\Jpegint.h
# End Source File
# Begin Source File

SOURCE=.\Jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\Jversion.h
# End Source File
# Begin Source File

SOURCE=.\stdwin.h
# End Source File
# Begin Source File

SOURCE=.\Winmem.h
# End Source File
# Begin Source File

SOURCE=.\Wrbmp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

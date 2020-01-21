# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=Jpeglib - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Jpeglib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Jpeglib - Win32 Release" && "$(CFG)" !=\
 "Jpeglib - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Jpeglib.mak" CFG="Jpeglib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Jpeglib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Jpeglib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Jpeglib - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "Jpeglib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
OUTDIR=.\Release
INTDIR=.\Release

ALL : "..\..\..\LIB\JPEG.LIB"

CLEAN : 
	-@erase "$(INTDIR)\Djpeg.obj"
	-@erase "$(INTDIR)\JCAPI.OBJ"
	-@erase "$(INTDIR)\JCCOEFCT.OBJ"
	-@erase "$(INTDIR)\JCCOLOR.OBJ"
	-@erase "$(INTDIR)\JCDCTMGR.OBJ"
	-@erase "$(INTDIR)\JCHUFF.OBJ"
	-@erase "$(INTDIR)\JCMAINCT.OBJ"
	-@erase "$(INTDIR)\JCMARKER.OBJ"
	-@erase "$(INTDIR)\JCMASTER.OBJ"
	-@erase "$(INTDIR)\JCOMAPI.OBJ"
	-@erase "$(INTDIR)\JCPARAM.OBJ"
	-@erase "$(INTDIR)\JCPREPCT.OBJ"
	-@erase "$(INTDIR)\JCSAMPLE.OBJ"
	-@erase "$(INTDIR)\JDAPI.OBJ"
	-@erase "$(INTDIR)\JDATADST.OBJ"
	-@erase "$(INTDIR)\JDATASRC.OBJ"
	-@erase "$(INTDIR)\JDCOEFCT.OBJ"
	-@erase "$(INTDIR)\JDCOLOR.OBJ"
	-@erase "$(INTDIR)\JDDCTMGR.OBJ"
	-@erase "$(INTDIR)\JDHUFF.OBJ"
	-@erase "$(INTDIR)\JDMAINCT.OBJ"
	-@erase "$(INTDIR)\JDMARKER.OBJ"
	-@erase "$(INTDIR)\JDMASTER.OBJ"
	-@erase "$(INTDIR)\JDMERGE.OBJ"
	-@erase "$(INTDIR)\JDPOSTCT.OBJ"
	-@erase "$(INTDIR)\JDSAMPLE.OBJ"
	-@erase "$(INTDIR)\JERROR.OBJ"
	-@erase "$(INTDIR)\JFDCTFLT.OBJ"
	-@erase "$(INTDIR)\JFDCTFST.OBJ"
	-@erase "$(INTDIR)\JFDCTINT.OBJ"
	-@erase "$(INTDIR)\JIDCTFLT.OBJ"
	-@erase "$(INTDIR)\JIDCTFST.OBJ"
	-@erase "$(INTDIR)\JIDCTINT.OBJ"
	-@erase "$(INTDIR)\JIDCTRED.OBJ"
	-@erase "$(INTDIR)\JMEMMGR.OBJ"
	-@erase "$(INTDIR)\JMEMNOBS.OBJ"
	-@erase "$(INTDIR)\JQUANT1.OBJ"
	-@erase "$(INTDIR)\JQUANT2.OBJ"
	-@erase "$(INTDIR)\JUTILS.OBJ"
	-@erase "$(INTDIR)\RDCOLMAP.OBJ"
	-@erase "$(INTDIR)\Winmem.obj"
	-@erase "$(INTDIR)\WRBMP.OBJ"
	-@erase "..\..\..\LIB\JPEG.LIB"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

$(OUTDIR)/JPEGLIB.bsc : $(OUTDIR)  $(BSC32_SBRS)
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /c
# SUBTRACT CPP /Fr /YX
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Jpeglib.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"D:\SRC\WIN\LIB\JPEG.LIB"
LIB32_FLAGS=/nologo /out:"D:\SRC\WIN\LIB\JPEG.LIB" 
LIB32_OBJS= \
	"$(INTDIR)\Djpeg.obj" \
	"$(INTDIR)\JCAPI.OBJ" \
	"$(INTDIR)\JCCOEFCT.OBJ" \
	"$(INTDIR)\JCCOLOR.OBJ" \
	"$(INTDIR)\JCDCTMGR.OBJ" \
	"$(INTDIR)\JCHUFF.OBJ" \
	"$(INTDIR)\JCMAINCT.OBJ" \
	"$(INTDIR)\JCMARKER.OBJ" \
	"$(INTDIR)\JCMASTER.OBJ" \
	"$(INTDIR)\JCOMAPI.OBJ" \
	"$(INTDIR)\JCPARAM.OBJ" \
	"$(INTDIR)\JCPREPCT.OBJ" \
	"$(INTDIR)\JCSAMPLE.OBJ" \
	"$(INTDIR)\JDAPI.OBJ" \
	"$(INTDIR)\JDATADST.OBJ" \
	"$(INTDIR)\JDATASRC.OBJ" \
	"$(INTDIR)\JDCOEFCT.OBJ" \
	"$(INTDIR)\JDCOLOR.OBJ" \
	"$(INTDIR)\JDDCTMGR.OBJ" \
	"$(INTDIR)\JDHUFF.OBJ" \
	"$(INTDIR)\JDMAINCT.OBJ" \
	"$(INTDIR)\JDMARKER.OBJ" \
	"$(INTDIR)\JDMASTER.OBJ" \
	"$(INTDIR)\JDMERGE.OBJ" \
	"$(INTDIR)\JDPOSTCT.OBJ" \
	"$(INTDIR)\JDSAMPLE.OBJ" \
	"$(INTDIR)\JERROR.OBJ" \
	"$(INTDIR)\JFDCTFLT.OBJ" \
	"$(INTDIR)\JFDCTFST.OBJ" \
	"$(INTDIR)\JFDCTINT.OBJ" \
	"$(INTDIR)\JIDCTFLT.OBJ" \
	"$(INTDIR)\JIDCTFST.OBJ" \
	"$(INTDIR)\JIDCTINT.OBJ" \
	"$(INTDIR)\JIDCTRED.OBJ" \
	"$(INTDIR)\JMEMMGR.OBJ" \
	"$(INTDIR)\JMEMNOBS.OBJ" \
	"$(INTDIR)\JQUANT1.OBJ" \
	"$(INTDIR)\JQUANT2.OBJ" \
	"$(INTDIR)\JUTILS.OBJ" \
	"$(INTDIR)\RDCOLMAP.OBJ" \
	"$(INTDIR)\Winmem.obj" \
	"$(INTDIR)\WRBMP.OBJ"

"..\..\..\LIB\JPEG.LIB" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "..\..\..\LIB\JPEGD.LIB" "$(OUTDIR)\Jpeglib.bsc"

CLEAN : 
	-@erase "$(INTDIR)\Djpeg.obj"
	-@erase "$(INTDIR)\Djpeg.sbr"
	-@erase "$(INTDIR)\JCAPI.OBJ"
	-@erase "$(INTDIR)\JCAPI.SBR"
	-@erase "$(INTDIR)\JCCOEFCT.OBJ"
	-@erase "$(INTDIR)\JCCOEFCT.SBR"
	-@erase "$(INTDIR)\JCCOLOR.OBJ"
	-@erase "$(INTDIR)\JCCOLOR.SBR"
	-@erase "$(INTDIR)\JCDCTMGR.OBJ"
	-@erase "$(INTDIR)\JCDCTMGR.SBR"
	-@erase "$(INTDIR)\JCHUFF.OBJ"
	-@erase "$(INTDIR)\JCHUFF.SBR"
	-@erase "$(INTDIR)\JCMAINCT.OBJ"
	-@erase "$(INTDIR)\JCMAINCT.SBR"
	-@erase "$(INTDIR)\JCMARKER.OBJ"
	-@erase "$(INTDIR)\JCMARKER.SBR"
	-@erase "$(INTDIR)\JCMASTER.OBJ"
	-@erase "$(INTDIR)\JCMASTER.SBR"
	-@erase "$(INTDIR)\JCOMAPI.OBJ"
	-@erase "$(INTDIR)\JCOMAPI.SBR"
	-@erase "$(INTDIR)\JCPARAM.OBJ"
	-@erase "$(INTDIR)\JCPARAM.SBR"
	-@erase "$(INTDIR)\JCPREPCT.OBJ"
	-@erase "$(INTDIR)\JCPREPCT.SBR"
	-@erase "$(INTDIR)\JCSAMPLE.OBJ"
	-@erase "$(INTDIR)\JCSAMPLE.SBR"
	-@erase "$(INTDIR)\JDAPI.OBJ"
	-@erase "$(INTDIR)\JDAPI.SBR"
	-@erase "$(INTDIR)\JDATADST.OBJ"
	-@erase "$(INTDIR)\JDATADST.SBR"
	-@erase "$(INTDIR)\JDATASRC.OBJ"
	-@erase "$(INTDIR)\JDATASRC.SBR"
	-@erase "$(INTDIR)\JDCOEFCT.OBJ"
	-@erase "$(INTDIR)\JDCOEFCT.SBR"
	-@erase "$(INTDIR)\JDCOLOR.OBJ"
	-@erase "$(INTDIR)\JDCOLOR.SBR"
	-@erase "$(INTDIR)\JDDCTMGR.OBJ"
	-@erase "$(INTDIR)\JDDCTMGR.SBR"
	-@erase "$(INTDIR)\JDHUFF.OBJ"
	-@erase "$(INTDIR)\JDHUFF.SBR"
	-@erase "$(INTDIR)\JDMAINCT.OBJ"
	-@erase "$(INTDIR)\JDMAINCT.SBR"
	-@erase "$(INTDIR)\JDMARKER.OBJ"
	-@erase "$(INTDIR)\JDMARKER.SBR"
	-@erase "$(INTDIR)\JDMASTER.OBJ"
	-@erase "$(INTDIR)\JDMASTER.SBR"
	-@erase "$(INTDIR)\JDMERGE.OBJ"
	-@erase "$(INTDIR)\JDMERGE.SBR"
	-@erase "$(INTDIR)\JDPOSTCT.OBJ"
	-@erase "$(INTDIR)\JDPOSTCT.SBR"
	-@erase "$(INTDIR)\JDSAMPLE.OBJ"
	-@erase "$(INTDIR)\JDSAMPLE.SBR"
	-@erase "$(INTDIR)\JERROR.OBJ"
	-@erase "$(INTDIR)\JERROR.SBR"
	-@erase "$(INTDIR)\JFDCTFLT.OBJ"
	-@erase "$(INTDIR)\JFDCTFLT.SBR"
	-@erase "$(INTDIR)\JFDCTFST.OBJ"
	-@erase "$(INTDIR)\JFDCTFST.SBR"
	-@erase "$(INTDIR)\JFDCTINT.OBJ"
	-@erase "$(INTDIR)\JFDCTINT.SBR"
	-@erase "$(INTDIR)\JIDCTFLT.OBJ"
	-@erase "$(INTDIR)\JIDCTFLT.SBR"
	-@erase "$(INTDIR)\JIDCTFST.OBJ"
	-@erase "$(INTDIR)\JIDCTFST.SBR"
	-@erase "$(INTDIR)\JIDCTINT.OBJ"
	-@erase "$(INTDIR)\JIDCTINT.SBR"
	-@erase "$(INTDIR)\JIDCTRED.OBJ"
	-@erase "$(INTDIR)\JIDCTRED.SBR"
	-@erase "$(INTDIR)\JMEMMGR.OBJ"
	-@erase "$(INTDIR)\JMEMMGR.SBR"
	-@erase "$(INTDIR)\JMEMNOBS.OBJ"
	-@erase "$(INTDIR)\JMEMNOBS.SBR"
	-@erase "$(INTDIR)\JQUANT1.OBJ"
	-@erase "$(INTDIR)\JQUANT1.SBR"
	-@erase "$(INTDIR)\JQUANT2.OBJ"
	-@erase "$(INTDIR)\JQUANT2.SBR"
	-@erase "$(INTDIR)\JUTILS.OBJ"
	-@erase "$(INTDIR)\JUTILS.SBR"
	-@erase "$(INTDIR)\RDCOLMAP.OBJ"
	-@erase "$(INTDIR)\RDCOLMAP.SBR"
	-@erase "$(INTDIR)\Winmem.obj"
	-@erase "$(INTDIR)\Winmem.sbr"
	-@erase "$(INTDIR)\WRBMP.OBJ"
	-@erase "$(INTDIR)\WRBMP.SBR"
	-@erase "$(OUTDIR)\Jpeglib.bsc"
	-@erase "..\..\..\LIB\JPEGD.LIB"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /Gf /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Fr /c
# SUBTRACT CPP /Gy /YX
CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /Gf /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fr"$(INTDIR)/" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Jpeglib.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\Djpeg.sbr" \
	"$(INTDIR)\JCAPI.SBR" \
	"$(INTDIR)\JCCOEFCT.SBR" \
	"$(INTDIR)\JCCOLOR.SBR" \
	"$(INTDIR)\JCDCTMGR.SBR" \
	"$(INTDIR)\JCHUFF.SBR" \
	"$(INTDIR)\JCMAINCT.SBR" \
	"$(INTDIR)\JCMARKER.SBR" \
	"$(INTDIR)\JCMASTER.SBR" \
	"$(INTDIR)\JCOMAPI.SBR" \
	"$(INTDIR)\JCPARAM.SBR" \
	"$(INTDIR)\JCPREPCT.SBR" \
	"$(INTDIR)\JCSAMPLE.SBR" \
	"$(INTDIR)\JDAPI.SBR" \
	"$(INTDIR)\JDATADST.SBR" \
	"$(INTDIR)\JDATASRC.SBR" \
	"$(INTDIR)\JDCOEFCT.SBR" \
	"$(INTDIR)\JDCOLOR.SBR" \
	"$(INTDIR)\JDDCTMGR.SBR" \
	"$(INTDIR)\JDHUFF.SBR" \
	"$(INTDIR)\JDMAINCT.SBR" \
	"$(INTDIR)\JDMARKER.SBR" \
	"$(INTDIR)\JDMASTER.SBR" \
	"$(INTDIR)\JDMERGE.SBR" \
	"$(INTDIR)\JDPOSTCT.SBR" \
	"$(INTDIR)\JDSAMPLE.SBR" \
	"$(INTDIR)\JERROR.SBR" \
	"$(INTDIR)\JFDCTFLT.SBR" \
	"$(INTDIR)\JFDCTFST.SBR" \
	"$(INTDIR)\JFDCTINT.SBR" \
	"$(INTDIR)\JIDCTFLT.SBR" \
	"$(INTDIR)\JIDCTFST.SBR" \
	"$(INTDIR)\JIDCTINT.SBR" \
	"$(INTDIR)\JIDCTRED.SBR" \
	"$(INTDIR)\JMEMMGR.SBR" \
	"$(INTDIR)\JMEMNOBS.SBR" \
	"$(INTDIR)\JQUANT1.SBR" \
	"$(INTDIR)\JQUANT2.SBR" \
	"$(INTDIR)\JUTILS.SBR" \
	"$(INTDIR)\RDCOLMAP.SBR" \
	"$(INTDIR)\Winmem.sbr" \
	"$(INTDIR)\WRBMP.SBR"

"$(OUTDIR)\Jpeglib.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"D:\SRC\WIN\LIB\JPEGD.LIB"
LIB32_FLAGS=/nologo /out:"D:\SRC\WIN\LIB\JPEGD.LIB" 
LIB32_OBJS= \
	"$(INTDIR)\Djpeg.obj" \
	"$(INTDIR)\JCAPI.OBJ" \
	"$(INTDIR)\JCCOEFCT.OBJ" \
	"$(INTDIR)\JCCOLOR.OBJ" \
	"$(INTDIR)\JCDCTMGR.OBJ" \
	"$(INTDIR)\JCHUFF.OBJ" \
	"$(INTDIR)\JCMAINCT.OBJ" \
	"$(INTDIR)\JCMARKER.OBJ" \
	"$(INTDIR)\JCMASTER.OBJ" \
	"$(INTDIR)\JCOMAPI.OBJ" \
	"$(INTDIR)\JCPARAM.OBJ" \
	"$(INTDIR)\JCPREPCT.OBJ" \
	"$(INTDIR)\JCSAMPLE.OBJ" \
	"$(INTDIR)\JDAPI.OBJ" \
	"$(INTDIR)\JDATADST.OBJ" \
	"$(INTDIR)\JDATASRC.OBJ" \
	"$(INTDIR)\JDCOEFCT.OBJ" \
	"$(INTDIR)\JDCOLOR.OBJ" \
	"$(INTDIR)\JDDCTMGR.OBJ" \
	"$(INTDIR)\JDHUFF.OBJ" \
	"$(INTDIR)\JDMAINCT.OBJ" \
	"$(INTDIR)\JDMARKER.OBJ" \
	"$(INTDIR)\JDMASTER.OBJ" \
	"$(INTDIR)\JDMERGE.OBJ" \
	"$(INTDIR)\JDPOSTCT.OBJ" \
	"$(INTDIR)\JDSAMPLE.OBJ" \
	"$(INTDIR)\JERROR.OBJ" \
	"$(INTDIR)\JFDCTFLT.OBJ" \
	"$(INTDIR)\JFDCTFST.OBJ" \
	"$(INTDIR)\JFDCTINT.OBJ" \
	"$(INTDIR)\JIDCTFLT.OBJ" \
	"$(INTDIR)\JIDCTFST.OBJ" \
	"$(INTDIR)\JIDCTINT.OBJ" \
	"$(INTDIR)\JIDCTRED.OBJ" \
	"$(INTDIR)\JMEMMGR.OBJ" \
	"$(INTDIR)\JMEMNOBS.OBJ" \
	"$(INTDIR)\JQUANT1.OBJ" \
	"$(INTDIR)\JQUANT2.OBJ" \
	"$(INTDIR)\JUTILS.OBJ" \
	"$(INTDIR)\RDCOLMAP.OBJ" \
	"$(INTDIR)\Winmem.obj" \
	"$(INTDIR)\WRBMP.OBJ"

"..\..\..\LIB\JPEGD.LIB" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "Jpeglib - Win32 Release"
# Name "Jpeglib - Win32 Debug"

!IF  "$(CFG)" == "Jpeglib - Win32 Release"

!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\JDHUFF.C
DEP_CPP_JDHUF=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDHUFF.OBJ" : $(SOURCE) $(DEP_CPP_JDHUF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDHUFF.OBJ" : $(SOURCE) $(DEP_CPP_JDHUF) "$(INTDIR)"

"$(INTDIR)\JDHUFF.SBR" : $(SOURCE) $(DEP_CPP_JDHUF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JFDCTINT.C
DEP_CPP_JFDCT=\
	".\Jconfig.h"\
	".\Jdct.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JFDCTINT.OBJ" : $(SOURCE) $(DEP_CPP_JFDCT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JFDCTINT.OBJ" : $(SOURCE) $(DEP_CPP_JFDCT) "$(INTDIR)"

"$(INTDIR)\JFDCTINT.SBR" : $(SOURCE) $(DEP_CPP_JFDCT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDAPI.C
DEP_CPP_JDAPI=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDAPI.OBJ" : $(SOURCE) $(DEP_CPP_JDAPI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDAPI.OBJ" : $(SOURCE) $(DEP_CPP_JDAPI) "$(INTDIR)"

"$(INTDIR)\JDAPI.SBR" : $(SOURCE) $(DEP_CPP_JDAPI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JERROR.C
DEP_CPP_JERRO=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	".\Jversion.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JERROR.OBJ" : $(SOURCE) $(DEP_CPP_JERRO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JERROR.OBJ" : $(SOURCE) $(DEP_CPP_JERRO) "$(INTDIR)"

"$(INTDIR)\JERROR.SBR" : $(SOURCE) $(DEP_CPP_JERRO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDATASRC.C
DEP_CPP_JDATA=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDATASRC.OBJ" : $(SOURCE) $(DEP_CPP_JDATA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDATASRC.OBJ" : $(SOURCE) $(DEP_CPP_JDATA) "$(INTDIR)"

"$(INTDIR)\JDATASRC.SBR" : $(SOURCE) $(DEP_CPP_JDATA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCSAMPLE.C
DEP_CPP_JCSAM=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCSAMPLE.OBJ" : $(SOURCE) $(DEP_CPP_JCSAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCSAMPLE.OBJ" : $(SOURCE) $(DEP_CPP_JCSAM) "$(INTDIR)"

"$(INTDIR)\JCSAMPLE.SBR" : $(SOURCE) $(DEP_CPP_JCSAM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDCOEFCT.C
DEP_CPP_JDCOE=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDCOEFCT.OBJ" : $(SOURCE) $(DEP_CPP_JDCOE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDCOEFCT.OBJ" : $(SOURCE) $(DEP_CPP_JDCOE) "$(INTDIR)"

"$(INTDIR)\JDCOEFCT.SBR" : $(SOURCE) $(DEP_CPP_JDCOE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JFDCTFLT.C
DEP_CPP_JFDCTF=\
	".\Jconfig.h"\
	".\Jdct.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JFDCTFLT.OBJ" : $(SOURCE) $(DEP_CPP_JFDCTF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JFDCTFLT.OBJ" : $(SOURCE) $(DEP_CPP_JFDCTF) "$(INTDIR)"

"$(INTDIR)\JFDCTFLT.SBR" : $(SOURCE) $(DEP_CPP_JFDCTF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCPARAM.C
DEP_CPP_JCPAR=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCPARAM.OBJ" : $(SOURCE) $(DEP_CPP_JCPAR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCPARAM.OBJ" : $(SOURCE) $(DEP_CPP_JCPAR) "$(INTDIR)"

"$(INTDIR)\JCPARAM.SBR" : $(SOURCE) $(DEP_CPP_JCPAR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JMEMMGR.C
DEP_CPP_JMEMM=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmemsys.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JMEMMGR.OBJ" : $(SOURCE) $(DEP_CPP_JMEMM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JMEMMGR.OBJ" : $(SOURCE) $(DEP_CPP_JMEMM) "$(INTDIR)"

"$(INTDIR)\JMEMMGR.SBR" : $(SOURCE) $(DEP_CPP_JMEMM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCMARKER.C
DEP_CPP_JCMAR=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCMARKER.OBJ" : $(SOURCE) $(DEP_CPP_JCMAR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCMARKER.OBJ" : $(SOURCE) $(DEP_CPP_JCMAR) "$(INTDIR)"

"$(INTDIR)\JCMARKER.SBR" : $(SOURCE) $(DEP_CPP_JCMAR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCHUFF.C
DEP_CPP_JCHUF=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCHUFF.OBJ" : $(SOURCE) $(DEP_CPP_JCHUF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCHUFF.OBJ" : $(SOURCE) $(DEP_CPP_JCHUF) "$(INTDIR)"

"$(INTDIR)\JCHUFF.SBR" : $(SOURCE) $(DEP_CPP_JCHUF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDMARKER.C
DEP_CPP_JDMAR=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDMARKER.OBJ" : $(SOURCE) $(DEP_CPP_JDMAR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDMARKER.OBJ" : $(SOURCE) $(DEP_CPP_JDMAR) "$(INTDIR)"

"$(INTDIR)\JDMARKER.SBR" : $(SOURCE) $(DEP_CPP_JDMAR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JQUANT1.C
DEP_CPP_JQUAN=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JQUANT1.OBJ" : $(SOURCE) $(DEP_CPP_JQUAN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JQUANT1.OBJ" : $(SOURCE) $(DEP_CPP_JQUAN) "$(INTDIR)"

"$(INTDIR)\JQUANT1.SBR" : $(SOURCE) $(DEP_CPP_JQUAN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDCOLOR.C
DEP_CPP_JDCOL=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDCOLOR.OBJ" : $(SOURCE) $(DEP_CPP_JDCOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDCOLOR.OBJ" : $(SOURCE) $(DEP_CPP_JDCOL) "$(INTDIR)"

"$(INTDIR)\JDCOLOR.SBR" : $(SOURCE) $(DEP_CPP_JDCOL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JIDCTRED.C
DEP_CPP_JIDCT=\
	".\Jconfig.h"\
	".\Jdct.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JIDCTRED.OBJ" : $(SOURCE) $(DEP_CPP_JIDCT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JIDCTRED.OBJ" : $(SOURCE) $(DEP_CPP_JIDCT) "$(INTDIR)"

"$(INTDIR)\JIDCTRED.SBR" : $(SOURCE) $(DEP_CPP_JIDCT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDPOSTCT.C
DEP_CPP_JDPOS=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDPOSTCT.OBJ" : $(SOURCE) $(DEP_CPP_JDPOS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDPOSTCT.OBJ" : $(SOURCE) $(DEP_CPP_JDPOS) "$(INTDIR)"

"$(INTDIR)\JDPOSTCT.SBR" : $(SOURCE) $(DEP_CPP_JDPOS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCCOLOR.C
DEP_CPP_JCCOL=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCCOLOR.OBJ" : $(SOURCE) $(DEP_CPP_JCCOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCCOLOR.OBJ" : $(SOURCE) $(DEP_CPP_JCCOL) "$(INTDIR)"

"$(INTDIR)\JCCOLOR.SBR" : $(SOURCE) $(DEP_CPP_JCCOL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JIDCTINT.C
DEP_CPP_JIDCTI=\
	".\Jconfig.h"\
	".\Jdct.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JIDCTINT.OBJ" : $(SOURCE) $(DEP_CPP_JIDCTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JIDCTINT.OBJ" : $(SOURCE) $(DEP_CPP_JIDCTI) "$(INTDIR)"

"$(INTDIR)\JIDCTINT.SBR" : $(SOURCE) $(DEP_CPP_JIDCTI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCAPI.C
DEP_CPP_JCAPI=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCAPI.OBJ" : $(SOURCE) $(DEP_CPP_JCAPI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCAPI.OBJ" : $(SOURCE) $(DEP_CPP_JCAPI) "$(INTDIR)"

"$(INTDIR)\JCAPI.SBR" : $(SOURCE) $(DEP_CPP_JCAPI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDMERGE.C
DEP_CPP_JDMER=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDMERGE.OBJ" : $(SOURCE) $(DEP_CPP_JDMER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDMERGE.OBJ" : $(SOURCE) $(DEP_CPP_JDMER) "$(INTDIR)"

"$(INTDIR)\JDMERGE.SBR" : $(SOURCE) $(DEP_CPP_JDMER) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JQUANT2.C
DEP_CPP_JQUANT=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JQUANT2.OBJ" : $(SOURCE) $(DEP_CPP_JQUANT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JQUANT2.OBJ" : $(SOURCE) $(DEP_CPP_JQUANT) "$(INTDIR)"

"$(INTDIR)\JQUANT2.SBR" : $(SOURCE) $(DEP_CPP_JQUANT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCCOEFCT.C
DEP_CPP_JCCOE=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCCOEFCT.OBJ" : $(SOURCE) $(DEP_CPP_JCCOE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCCOEFCT.OBJ" : $(SOURCE) $(DEP_CPP_JCCOE) "$(INTDIR)"

"$(INTDIR)\JCCOEFCT.SBR" : $(SOURCE) $(DEP_CPP_JCCOE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JIDCTFLT.C
DEP_CPP_JIDCTF=\
	".\Jconfig.h"\
	".\Jdct.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JIDCTFLT.OBJ" : $(SOURCE) $(DEP_CPP_JIDCTF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JIDCTFLT.OBJ" : $(SOURCE) $(DEP_CPP_JIDCTF) "$(INTDIR)"

"$(INTDIR)\JIDCTFLT.SBR" : $(SOURCE) $(DEP_CPP_JIDCTF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCPREPCT.C
DEP_CPP_JCPRE=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCPREPCT.OBJ" : $(SOURCE) $(DEP_CPP_JCPRE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCPREPCT.OBJ" : $(SOURCE) $(DEP_CPP_JCPRE) "$(INTDIR)"

"$(INTDIR)\JCPREPCT.SBR" : $(SOURCE) $(DEP_CPP_JCPRE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCDCTMGR.C
DEP_CPP_JCDCT=\
	".\Jconfig.h"\
	".\Jdct.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCDCTMGR.OBJ" : $(SOURCE) $(DEP_CPP_JCDCT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCDCTMGR.OBJ" : $(SOURCE) $(DEP_CPP_JCDCT) "$(INTDIR)"

"$(INTDIR)\JCDCTMGR.SBR" : $(SOURCE) $(DEP_CPP_JCDCT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDDCTMGR.C
DEP_CPP_JDDCT=\
	".\Jconfig.h"\
	".\Jdct.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDDCTMGR.OBJ" : $(SOURCE) $(DEP_CPP_JDDCT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDDCTMGR.OBJ" : $(SOURCE) $(DEP_CPP_JDDCT) "$(INTDIR)"

"$(INTDIR)\JDDCTMGR.SBR" : $(SOURCE) $(DEP_CPP_JDDCT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JFDCTFST.C
DEP_CPP_JFDCTFS=\
	".\Jconfig.h"\
	".\Jdct.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JFDCTFST.OBJ" : $(SOURCE) $(DEP_CPP_JFDCTFS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JFDCTFST.OBJ" : $(SOURCE) $(DEP_CPP_JFDCTFS) "$(INTDIR)"

"$(INTDIR)\JFDCTFST.SBR" : $(SOURCE) $(DEP_CPP_JFDCTFS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDATADST.C
DEP_CPP_JDATAD=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDATADST.OBJ" : $(SOURCE) $(DEP_CPP_JDATAD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDATADST.OBJ" : $(SOURCE) $(DEP_CPP_JDATAD) "$(INTDIR)"

"$(INTDIR)\JDATADST.SBR" : $(SOURCE) $(DEP_CPP_JDATAD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCMAINCT.C
DEP_CPP_JCMAI=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCMAINCT.OBJ" : $(SOURCE) $(DEP_CPP_JCMAI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCMAINCT.OBJ" : $(SOURCE) $(DEP_CPP_JCMAI) "$(INTDIR)"

"$(INTDIR)\JCMAINCT.SBR" : $(SOURCE) $(DEP_CPP_JCMAI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDMAINCT.C
DEP_CPP_JDMAI=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDMAINCT.OBJ" : $(SOURCE) $(DEP_CPP_JDMAI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDMAINCT.OBJ" : $(SOURCE) $(DEP_CPP_JDMAI) "$(INTDIR)"

"$(INTDIR)\JDMAINCT.SBR" : $(SOURCE) $(DEP_CPP_JDMAI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDSAMPLE.C
DEP_CPP_JDSAM=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDSAMPLE.OBJ" : $(SOURCE) $(DEP_CPP_JDSAM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDSAMPLE.OBJ" : $(SOURCE) $(DEP_CPP_JDSAM) "$(INTDIR)"

"$(INTDIR)\JDSAMPLE.SBR" : $(SOURCE) $(DEP_CPP_JDSAM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCMASTER.C
DEP_CPP_JCMAS=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCMASTER.OBJ" : $(SOURCE) $(DEP_CPP_JCMAS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCMASTER.OBJ" : $(SOURCE) $(DEP_CPP_JCMAS) "$(INTDIR)"

"$(INTDIR)\JCMASTER.SBR" : $(SOURCE) $(DEP_CPP_JCMAS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JDMASTER.C
DEP_CPP_JDMAS=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JDMASTER.OBJ" : $(SOURCE) $(DEP_CPP_JDMAS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JDMASTER.OBJ" : $(SOURCE) $(DEP_CPP_JDMAS) "$(INTDIR)"

"$(INTDIR)\JDMASTER.SBR" : $(SOURCE) $(DEP_CPP_JDMAS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JCOMAPI.C
DEP_CPP_JCOMA=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JCOMAPI.OBJ" : $(SOURCE) $(DEP_CPP_JCOMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JCOMAPI.OBJ" : $(SOURCE) $(DEP_CPP_JCOMA) "$(INTDIR)"

"$(INTDIR)\JCOMAPI.SBR" : $(SOURCE) $(DEP_CPP_JCOMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JUTILS.C
DEP_CPP_JUTIL=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JUTILS.OBJ" : $(SOURCE) $(DEP_CPP_JUTIL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JUTILS.OBJ" : $(SOURCE) $(DEP_CPP_JUTIL) "$(INTDIR)"

"$(INTDIR)\JUTILS.SBR" : $(SOURCE) $(DEP_CPP_JUTIL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JIDCTFST.C
DEP_CPP_JIDCTFS=\
	".\Jconfig.h"\
	".\Jdct.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JIDCTFST.OBJ" : $(SOURCE) $(DEP_CPP_JIDCTFS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JIDCTFST.OBJ" : $(SOURCE) $(DEP_CPP_JIDCTFS) "$(INTDIR)"

"$(INTDIR)\JIDCTFST.SBR" : $(SOURCE) $(DEP_CPP_JIDCTFS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\RDCOLMAP.C
DEP_CPP_RDCOL=\
	".\Cderror.h"\
	".\Cdjpeg.h"\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\RDCOLMAP.OBJ" : $(SOURCE) $(DEP_CPP_RDCOL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\RDCOLMAP.OBJ" : $(SOURCE) $(DEP_CPP_RDCOL) "$(INTDIR)"

"$(INTDIR)\RDCOLMAP.SBR" : $(SOURCE) $(DEP_CPP_RDCOL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\WRBMP.C
DEP_CPP_WRBMP=\
	".\Cderror.h"\
	".\Cdjpeg.h"\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	".\Wrbmp.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\WRBMP.OBJ" : $(SOURCE) $(DEP_CPP_WRBMP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\WRBMP.OBJ" : $(SOURCE) $(DEP_CPP_WRBMP) "$(INTDIR)"

"$(INTDIR)\WRBMP.SBR" : $(SOURCE) $(DEP_CPP_WRBMP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Djpeg.c
DEP_CPP_DJPEG=\
	".\Cderror.h"\
	".\Cdjpeg.h"\
	".\Djpeg.h"\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	".\Jversion.h"\
	".\Winmem.h"\
	".\Wrbmp.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\Djpeg.obj" : $(SOURCE) $(DEP_CPP_DJPEG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\Djpeg.obj" : $(SOURCE) $(DEP_CPP_DJPEG) "$(INTDIR)"

"$(INTDIR)\Djpeg.sbr" : $(SOURCE) $(DEP_CPP_DJPEG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\JMEMNOBS.C
DEP_CPP_JMEMN=\
	".\Jconfig.h"\
	".\Jerror.h"\
	".\Jinclude.h"\
	".\Jmemsys.h"\
	".\Jmorecfg.h"\
	".\Jpegint.h"\
	".\Jpeglib.h"\
	{$(INCLUDE)}"\sys\Types.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\JMEMNOBS.OBJ" : $(SOURCE) $(DEP_CPP_JMEMN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"


"$(INTDIR)\JMEMNOBS.OBJ" : $(SOURCE) $(DEP_CPP_JMEMN) "$(INTDIR)"

"$(INTDIR)\JMEMNOBS.SBR" : $(SOURCE) $(DEP_CPP_JMEMN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Winmem.c
DEP_CPP_WINME=\
	".\stdwin.h"\
	".\Winmem.h"\
	

!IF  "$(CFG)" == "Jpeglib - Win32 Release"


"$(INTDIR)\Winmem.obj" : $(SOURCE) $(DEP_CPP_WINME) "$(INTDIR)"
   $(CPP) /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Jpeglib - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

BuildCmds= \
	$(CPP) /nologo /MTd /W3 /GX /Z7 /Od /Gf /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fr"$(INTDIR)/" /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\Winmem.obj" : $(SOURCE) $(DEP_CPP_WINME) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\Winmem.sbr" : $(SOURCE) $(DEP_CPP_WINME) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

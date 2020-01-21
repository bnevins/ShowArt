# Microsoft Developer Studio Generated NMAKE File, Format Version 4.10
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=Tiff - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Tiff - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Tiff - Win32 Release" && "$(CFG)" != "Tiff - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tiff32.mak" CFG="Tiff - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tiff - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Tiff - Win32 Debug" (based on "Win32 (x86) Static Library")
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
# PROP Target_Last_Scanned "Tiff - Win32 Debug"
CPP=cl.exe

!IF  "$(CFG)" == "Tiff - Win32 Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "WinRel"
OUTDIR=.
INTDIR=.\WinRel

ALL : "..\..\LIB\TIFF.lib"

CLEAN : 
	-@erase "$(INTDIR)\MYTIFF.OBJ"
	-@erase "$(INTDIR)\stdtif.obj"
	-@erase "$(INTDIR)\TIF_CCIT.OBJ"
	-@erase "$(INTDIR)\TIF_CLOS.OBJ"
	-@erase "$(INTDIR)\TIF_COMP.OBJ"
	-@erase "$(INTDIR)\TIF_COMR.OBJ"
	-@erase "$(INTDIR)\TIF_DIR.OBJ"
	-@erase "$(INTDIR)\TIF_DUMP.OBJ"
	-@erase "$(INTDIR)\TIF_ERRO.OBJ"
	-@erase "$(INTDIR)\TIF_FAX3.OBJ"
	-@erase "$(INTDIR)\TIF_FAX4.OBJ"
	-@erase "$(INTDIR)\TIF_FLUS.OBJ"
	-@erase "$(INTDIR)\TIF_LZW.OBJ"
	-@erase "$(INTDIR)\TIF_OPEN.OBJ"
	-@erase "$(INTDIR)\TIF_PACK.OBJ"
	-@erase "$(INTDIR)\TIF_READ.OBJ"
	-@erase "$(INTDIR)\TIF_SWAB.OBJ"
	-@erase "$(INTDIR)\TIF_WARN.OBJ"
	-@erase "$(INTDIR)\TIF_WRIT.OBJ"
	-@erase "..\..\LIB\TIFF.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

$(OUTDIR)/TIFF.bsc : $(OUTDIR)  $(BSC32_SBRS)
# ADD BASE CPP /nologo /G3 /MT /W4 /GX /Ox /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /GX /Ox /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /c
# SUBTRACT CPP /Fr /Yu
CPP_PROJ=/nologo /ML /GX /Ox /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinRel/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Tiff32.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"C:\SRC\WIN\LIB\TIFF.lib"
LIB32_FLAGS=/nologo /out:"C:\SRC\WIN\LIB\TIFF.lib" 
LIB32_OBJS= \
	"$(INTDIR)\MYTIFF.OBJ" \
	"$(INTDIR)\stdtif.obj" \
	"$(INTDIR)\TIF_CCIT.OBJ" \
	"$(INTDIR)\TIF_CLOS.OBJ" \
	"$(INTDIR)\TIF_COMP.OBJ" \
	"$(INTDIR)\TIF_COMR.OBJ" \
	"$(INTDIR)\TIF_DIR.OBJ" \
	"$(INTDIR)\TIF_DUMP.OBJ" \
	"$(INTDIR)\TIF_ERRO.OBJ" \
	"$(INTDIR)\TIF_FAX3.OBJ" \
	"$(INTDIR)\TIF_FAX4.OBJ" \
	"$(INTDIR)\TIF_FLUS.OBJ" \
	"$(INTDIR)\TIF_LZW.OBJ" \
	"$(INTDIR)\TIF_OPEN.OBJ" \
	"$(INTDIR)\TIF_PACK.OBJ" \
	"$(INTDIR)\TIF_READ.OBJ" \
	"$(INTDIR)\TIF_SWAB.OBJ" \
	"$(INTDIR)\TIF_WARN.OBJ" \
	"$(INTDIR)\TIF_WRIT.OBJ"

"..\..\LIB\TIFF.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Tiff - Win32 Debug"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.
INTDIR=.\WinDebug

ALL : "..\..\LIB\TIFFD.lib"

CLEAN : 
	-@erase "$(INTDIR)\MYTIFF.OBJ"
	-@erase "$(INTDIR)\stdtif.obj"
	-@erase "$(INTDIR)\TIF_CCIT.OBJ"
	-@erase "$(INTDIR)\TIF_CLOS.OBJ"
	-@erase "$(INTDIR)\TIF_COMP.OBJ"
	-@erase "$(INTDIR)\TIF_COMR.OBJ"
	-@erase "$(INTDIR)\TIF_DIR.OBJ"
	-@erase "$(INTDIR)\TIF_DUMP.OBJ"
	-@erase "$(INTDIR)\TIF_ERRO.OBJ"
	-@erase "$(INTDIR)\TIF_FAX3.OBJ"
	-@erase "$(INTDIR)\TIF_FAX4.OBJ"
	-@erase "$(INTDIR)\TIF_FLUS.OBJ"
	-@erase "$(INTDIR)\TIF_LZW.OBJ"
	-@erase "$(INTDIR)\TIF_OPEN.OBJ"
	-@erase "$(INTDIR)\TIF_PACK.OBJ"
	-@erase "$(INTDIR)\TIF_READ.OBJ"
	-@erase "$(INTDIR)\TIF_SWAB.OBJ"
	-@erase "$(INTDIR)\TIF_WARN.OBJ"
	-@erase "$(INTDIR)\TIF_WRIT.OBJ"
	-@erase "..\..\LIB\TIFFD.lib"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

$(OUTDIR)/TIFF.bsc : $(OUTDIR)  $(BSC32_SBRS)
# ADD BASE CPP /nologo /MT /W4 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /c
# ADD CPP /nologo /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /c
# SUBTRACT CPP /Fr /Yu
CPP_PROJ=/nologo /MLd /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\WinDebug/
CPP_SBRS=.\.
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Tiff32.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"C:\SRC\WIN\LIB\TIFFD.lib"
LIB32_FLAGS=/nologo /out:"C:\SRC\WIN\LIB\TIFFD.lib" 
LIB32_OBJS= \
	"$(INTDIR)\MYTIFF.OBJ" \
	"$(INTDIR)\stdtif.obj" \
	"$(INTDIR)\TIF_CCIT.OBJ" \
	"$(INTDIR)\TIF_CLOS.OBJ" \
	"$(INTDIR)\TIF_COMP.OBJ" \
	"$(INTDIR)\TIF_COMR.OBJ" \
	"$(INTDIR)\TIF_DIR.OBJ" \
	"$(INTDIR)\TIF_DUMP.OBJ" \
	"$(INTDIR)\TIF_ERRO.OBJ" \
	"$(INTDIR)\TIF_FAX3.OBJ" \
	"$(INTDIR)\TIF_FAX4.OBJ" \
	"$(INTDIR)\TIF_FLUS.OBJ" \
	"$(INTDIR)\TIF_LZW.OBJ" \
	"$(INTDIR)\TIF_OPEN.OBJ" \
	"$(INTDIR)\TIF_PACK.OBJ" \
	"$(INTDIR)\TIF_READ.OBJ" \
	"$(INTDIR)\TIF_SWAB.OBJ" \
	"$(INTDIR)\TIF_WARN.OBJ" \
	"$(INTDIR)\TIF_WRIT.OBJ"

"..\..\LIB\TIFFD.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
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

# Name "Tiff - Win32 Release"
# Name "Tiff - Win32 Debug"

!IF  "$(CFG)" == "Tiff - Win32 Release"

!ELSEIF  "$(CFG)" == "Tiff - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\MYTIFF.C
DEP_CPP_MYTIF=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "Tiff - Win32 Release"


"$(INTDIR)\MYTIFF.OBJ" : $(SOURCE) $(DEP_CPP_MYTIF) "$(INTDIR)"
   $(CPP) /nologo /ML /GX /Ox /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Tiff - Win32 Debug"

# ADD CPP /W3

"$(INTDIR)\MYTIFF.OBJ" : $(SOURCE) $(DEP_CPP_MYTIF) "$(INTDIR)"
   $(CPP) /nologo /MLd /W3 /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_CCIT.C
DEP_CPP_TIF_C=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_CCIT.OBJ" : $(SOURCE) $(DEP_CPP_TIF_C) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_CLOS.C
DEP_CPP_TIF_CL=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_CLOS.OBJ" : $(SOURCE) $(DEP_CPP_TIF_CL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_COMP.C
DEP_CPP_TIF_CO=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_COMP.OBJ" : $(SOURCE) $(DEP_CPP_TIF_CO) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_COMR.C
DEP_CPP_TIF_COM=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_COMR.OBJ" : $(SOURCE) $(DEP_CPP_TIF_COM) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_DIR.C
DEP_CPP_TIF_D=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_DIR.OBJ" : $(SOURCE) $(DEP_CPP_TIF_D) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_DUMP.C
DEP_CPP_TIF_DU=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_DUMP.OBJ" : $(SOURCE) $(DEP_CPP_TIF_DU) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_ERRO.C
DEP_CPP_TIF_E=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_ERRO.OBJ" : $(SOURCE) $(DEP_CPP_TIF_E) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_FAX3.C
DEP_CPP_TIF_F=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_FAX3.OBJ" : $(SOURCE) $(DEP_CPP_TIF_F) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_FAX4.C
DEP_CPP_TIF_FA=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_FAX4.OBJ" : $(SOURCE) $(DEP_CPP_TIF_FA) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_FLUS.C
DEP_CPP_TIF_FL=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_FLUS.OBJ" : $(SOURCE) $(DEP_CPP_TIF_FL) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_LZW.C
DEP_CPP_TIF_L=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_LZW.OBJ" : $(SOURCE) $(DEP_CPP_TIF_L) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_OPEN.C
DEP_CPP_TIF_O=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_OPEN.OBJ" : $(SOURCE) $(DEP_CPP_TIF_O) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_PACK.C
DEP_CPP_TIF_P=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_PACK.OBJ" : $(SOURCE) $(DEP_CPP_TIF_P) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_READ.C
DEP_CPP_TIF_R=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_READ.OBJ" : $(SOURCE) $(DEP_CPP_TIF_R) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_SWAB.C
DEP_CPP_TIF_S=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_SWAB.OBJ" : $(SOURCE) $(DEP_CPP_TIF_S) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_WARN.C
DEP_CPP_TIF_W=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_WARN.OBJ" : $(SOURCE) $(DEP_CPP_TIF_W) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\TIF_WRIT.C
DEP_CPP_TIF_WR=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

"$(INTDIR)\TIF_WRIT.OBJ" : $(SOURCE) $(DEP_CPP_TIF_WR) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdtif.c
DEP_CPP_STDTI=\
	".\stdtif.h"\
	".\TIFF.H"\
	".\TIFFCOMP.H"\
	".\TIFFDEFS.H"\
	".\TIFFIO.H"\
	{$(INCLUDE)}"\sys\stat.h"\
	{$(INCLUDE)}"\sys\types.h"\
	

!IF  "$(CFG)" == "Tiff - Win32 Release"


"$(INTDIR)\stdtif.obj" : $(SOURCE) $(DEP_CPP_STDTI) "$(INTDIR)"
   $(CPP) /nologo /ML /GX /Ox /Gf /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ELSEIF  "$(CFG)" == "Tiff - Win32 Debug"

# SUBTRACT CPP /Yc

"$(INTDIR)\stdtif.obj" : $(SOURCE) $(DEP_CPP_STDTI) "$(INTDIR)"
   $(CPP) /nologo /MLd /GX /Z7 /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /Fo"$(INTDIR)/" /c $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################

# Microsoft Developer Studio Project File - Name="3DAlignment" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=3DAlignment - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "3DAlignment.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "3DAlignment.mak" CFG="3DAlignment - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "3DAlignment - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "3DAlignment - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "3DAlignment - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x404 /d "NDEBUG"
# ADD RSC /l 0x404 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "3DAlignment - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x404 /d "_DEBUG"
# ADD RSC /l 0x404 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "3DAlignment - Win32 Release"
# Name "3DAlignment - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "fftw_c"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fftw\config.c
# End Source File
# Begin Source File

SOURCE=.\fftw\executor.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_1.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_10.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_11.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_12.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_128.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_13.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_14.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_15.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_16.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_2.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_3.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_32.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_4.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_5.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_6.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_64.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_7.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_8.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fcr_9.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fftwf77.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fftwnd.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_10.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_16.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_2.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_3.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_32.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_4.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_5.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_6.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_7.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_8.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhb_9.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_10.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_16.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_2.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_3.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_32.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_4.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_5.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_6.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_7.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_8.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fhf_9.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_1.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_10.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_11.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_12.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_13.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_14.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_15.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_16.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_2.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_3.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_32.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_4.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_5.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_6.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_64.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_7.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_8.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fn_9.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_1.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_10.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_11.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_12.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_13.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_14.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_15.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_16.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_2.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_3.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_32.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_4.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_5.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_6.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_64.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_7.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_8.c
# End Source File
# Begin Source File

SOURCE=.\fftw\fni_9.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_1.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_10.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_11.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_12.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_128.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_13.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_14.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_15.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_16.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_2.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_3.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_32.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_4.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_5.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_6.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_64.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_7.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_8.c
# End Source File
# Begin Source File

SOURCE=.\fftw\frc_9.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_10.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_16.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_2.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_3.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_32.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_4.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_5.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_6.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_64.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_7.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_8.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftw_9.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_10.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_16.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_2.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_3.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_32.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_4.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_5.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_6.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_64.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_7.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_8.c
# End Source File
# Begin Source File

SOURCE=.\fftw\ftwi_9.c
# End Source File
# Begin Source File

SOURCE=.\fftw\generic.c
# End Source File
# Begin Source File

SOURCE=.\fftw\malloc.c
# End Source File
# Begin Source File

SOURCE=.\fftw\planner.c
# End Source File
# Begin Source File

SOURCE=.\fftw\putils.c
# End Source File
# Begin Source File

SOURCE=.\fftw\rader.c
# End Source File
# Begin Source File

SOURCE=.\fftw\rconfig.c
# End Source File
# Begin Source File

SOURCE=.\fftw\rexec.c
# End Source File
# Begin Source File

SOURCE=.\fftw\rexec2.c
# End Source File
# Begin Source File

SOURCE=.\fftw\rfftwf77.c
# End Source File
# Begin Source File

SOURCE=.\fftw\rfftwnd.c
# End Source File
# Begin Source File

SOURCE=.\fftw\rgeneric.c
# End Source File
# Begin Source File

SOURCE=.\fftw\rplanner.c
# End Source File
# Begin Source File

SOURCE=.\fftw\timer.c
# End Source File
# Begin Source File

SOURCE=.\fftw\twiddle.c
# End Source File
# Begin Source File

SOURCE=.\fftw\wisdom.c
# End Source File
# Begin Source File

SOURCE=.\fftw\wisdomio.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\Bitmap.c
# End Source File
# Begin Source File

SOURCE=.\Circularity.c
# End Source File
# Begin Source File

SOURCE=.\ColorDescriptor.c
# End Source File
# Begin Source File

SOURCE=.\Convert.c
# End Source File
# Begin Source File

SOURCE=.\Eccentricity.c
# End Source File
# Begin Source File

SOURCE=.\edge.c
# End Source File
# Begin Source File

SOURCE=.\FourierDescriptor.c
# End Source File
# Begin Source File

SOURCE=.\Main.c
# End Source File
# Begin Source File

SOURCE=.\MORPHOLOGY.C
# End Source File
# Begin Source File

SOURCE=.\RecovAffine.c
# End Source File
# Begin Source File

SOURCE=.\Refine.c
# End Source File
# Begin Source File

SOURCE=.\RegionShape.c
# End Source File
# Begin Source File

SOURCE=.\Rotate.c
# End Source File
# Begin Source File

SOURCE=.\RWObj.c
# End Source File
# Begin Source File

SOURCE=.\thin.c
# End Source File
# Begin Source File

SOURCE=.\TraceContour.c
# End Source File
# Begin Source File

SOURCE=.\TranslateScale.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "fftw_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fftw\config.h
# End Source File
# Begin Source File

SOURCE=.\fftw\f77_func.h
# End Source File
# Begin Source File

SOURCE=".\fftw\fftw-int.h"
# End Source File
# Begin Source File

SOURCE=.\fftw\fftw.h
# End Source File
# Begin Source File

SOURCE=.\fftw\rfftw.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BITMAP.H
# End Source File
# Begin Source File

SOURCE=.\Circularity.h
# End Source File
# Begin Source File

SOURCE=.\ColorDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\convert.h
# End Source File
# Begin Source File

SOURCE=.\ds.h
# End Source File
# Begin Source File

SOURCE=.\Eccentricity.h
# End Source File
# Begin Source File

SOURCE=.\Edge.h
# End Source File
# Begin Source File

SOURCE=.\FourierDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\glut.h
# End Source File
# Begin Source File

SOURCE=.\MORPHOLOGY.H
# End Source File
# Begin Source File

SOURCE=.\RecovAffine.h
# End Source File
# Begin Source File

SOURCE=.\Refine.h
# End Source File
# Begin Source File

SOURCE=.\RegionShape.h
# End Source File
# Begin Source File

SOURCE=.\Rotate.h
# End Source File
# Begin Source File

SOURCE=.\RWObj.h
# End Source File
# Begin Source File

SOURCE=.\thin.h
# End Source File
# Begin Source File

SOURCE=.\TraceContour.h
# End Source File
# Begin Source File

SOURCE=.\TranslateScale.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

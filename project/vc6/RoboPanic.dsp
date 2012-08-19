# Microsoft Developer Studio Project File - Name="RoboPanic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RoboPanic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RoboPanic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RoboPanic.mak" CFG="RoboPanic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RoboPanic - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RoboPanic - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=xicl6.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RoboPanic - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dinput.lib ddraw.lib dsound.lib winmm.lib user32.lib gdi32.lib /nologo /subsystem:windows /machine:I386
# Begin Special Build Tool
TargetPath=.\Release\RoboPanic.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Copying .exe to root dir....	copy $(TargetPath)  ..\..
# End Special Build Tool

!ELSEIF  "$(CFG)" == "RoboPanic - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dinput.lib ddraw.lib dsound.lib winmm.lib user32.lib gdi32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# Begin Special Build Tool
TargetPath=.\Debug\RoboPanic.exe
SOURCE="$(InputPath)"
PostBuild_Cmds=echo Copying .exe to root dir....	copy $(TargetPath)  ..\..
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "RoboPanic - Win32 Release"
# Name "RoboPanic - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\Action.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ActionMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\BmpLoader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\BoundingBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Bullet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Character.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\ddutil.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\debug.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Door.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\DXGame.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Enemy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Entity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\HDoor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Interface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Inventory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\InvisibleEntity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\RoboPanicGame.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene2.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene3.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene5.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene6.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene7.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\SceneCoordinate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Sound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Tile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TileMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\Timer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\TrashCan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\VDoor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\World.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\src\Action.h
# End Source File
# Begin Source File

SOURCE=..\..\src\ActionMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\src\BmpLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\src\BoundingBox.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Bullet.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Character.h
# End Source File
# Begin Source File

SOURCE=..\..\src\debug.h
# End Source File
# Begin Source File

SOURCE=..\..\src\debugdec.h
# End Source File
# Begin Source File

SOURCE=..\..\src\debugset.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Defines.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Door.h
# End Source File
# Begin Source File

SOURCE=..\..\src\DXGame.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Enemy.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Entity.h
# End Source File
# Begin Source File

SOURCE=..\..\src\files.h
# End Source File
# Begin Source File

SOURCE=..\..\src\HDoor.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Interface.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Inventory.h
# End Source File
# Begin Source File

SOURCE=..\..\src\InvisibleEntity.h
# End Source File
# Begin Source File

SOURCE=..\..\src\RoboPanicGame.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene1.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene2.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene3.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene4.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene5.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene6.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Scene7.h
# End Source File
# Begin Source File

SOURCE=..\..\src\SceneCoordinate.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Sndlist.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Sound.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Sprite.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Tile.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TileMap.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Timer.h
# End Source File
# Begin Source File

SOURCE=..\..\src\TrashCan.h
# End Source File
# Begin Source File

SOURCE=..\..\src\VDoor.h
# End Source File
# Begin Source File

SOURCE=..\..\src\World.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="hl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=hl - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "hl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hl.mak" CFG="hl - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "hl - Win32 Profile" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../build/release/hl/"
# PROP BASE Intermediate_Dir "../build/release/hl/"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\build\release\server"
# PROP Intermediate_Dir "..\build\release\server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /MT /W3 /GX /Zi /O2 /Ob2 /I ".\\" /I "..\\" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\game_shared" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /D "CLIENT_WEAPONS" /D "MUSIC" /Fr /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 user32.lib advapi32.lib /nologo /subsystem:windows /dll /incremental:no /machine:I386 /def:".\hl.def" /out:"..\build\wizardwars.dll"
# SUBTRACT LINK32 /profile /map /debug
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=cd ..\	cmd.exe /c update.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "hl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../build/debug/hl/"
# PROP BASE Intermediate_Dir "../build/debug/hl/"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\build\debug\server"
# PROP Intermediate_Dir "..\build\debug\server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W4 /Gm /Zi /Od /I ".\\" /I "..\\" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\game_shared" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "CLIENT_WEAPONS" /D "HLSDK_3_2_OLD_EIFACE" /D "STRICT" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\engine" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 user32.lib advapi32.lib /nologo /subsystem:windows /dll /incremental:no /map /debug /machine:I386 /def:".\hl.def" /out:"..\build\wizardwars.dll" /implib:".\hl.lib"
# SUBTRACT LINK32 /profile
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=cd ..\	cmd.exe /c update.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "hl - Win32 Profile"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../build/profile/hl/"
# PROP BASE Intermediate_Dir "../build/profile/hl/"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\build\profile\server"
# PROP Intermediate_Dir "..\build\profile\server"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MT /W3 /GX /Zi /O2 /I "..\engine" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /YX /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /G5 /MT /W3 /Zi /O2 /I ".\\" /I "..\\" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\game_shared" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /D "CLIENT_WEAPONS" /D "MUSIC" /Fr /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:I386 /def:".\hl.def"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 user32.lib advapi32.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /def:".\hl.def" /out:"..\build\wizardwars.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=cd ..\	cmd.exe /c update.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "hl - Win32 Release"
# Name "hl - Win32 Debug"
# Name "hl - Win32 Profile"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Group "ww"

# PROP Default_Filter ""
# Begin Group "ents"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ww_emitter.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_goblin.cpp.dead
# End Source File
# Begin Source File

SOURCE=.\ww_sentrycrystal.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_sentrycrystal.h
# End Source File
# Begin Source File

SOURCE=.\ww_thornbush.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_thornbush.h
# End Source File
# Begin Source File

SOURCE=.\ww_villager.cpp.dead
# End Source File
# End Group
# Begin Group "satchels"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ww_archsatchel.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_deathsatchel.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_dragonsatchel.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_earthsatchel.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_firesatchel.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_icesatchel.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_lifesatchel.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_lightningsatchel.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_naturesatchel.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_satchels.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_satchels.h
# End Source File
# Begin Source File

SOURCE=.\ww_windsatchel.cpp
# End Source File
# End Group
# Begin Group "seals"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ww_deathseal.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_dragonseal.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_earthseal.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_fireseal.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_iceseal.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_lifeseal.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_lightningseal.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_natureseal.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_seals.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_seals.h
# End Source File
# Begin Source File

SOURCE=.\ww_windseal.cpp
# End Source File
# End Group
# Begin Group "weapons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ww_shared\ww_beanstalk.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_beanstalk.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_bearbite.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_bearbite.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_bird.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_bird.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_deathray.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_deathray.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_doublemissile.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_doublemissile.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_dragonbreath.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_dragonbreath.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_fireball.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_fireball.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_flamelick.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_flamelick.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_forceblast.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_forceblast.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_freezeray.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_freezeray.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_icepoke.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_icepoke.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_levitate.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_levitate.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_lightningbolt.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_lightningbolt.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_missile.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_missile.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_pebbleblast.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_pebbleblast.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_rollingstone.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_rollingstone.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_seal.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_seal.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_shield.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_shield.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_skull.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_skull.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_spotbolt.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_spotbolt.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_staff.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_staff.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_thornblast.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_thornblast.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_updraft.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_updraft.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_whiteray.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_whiteray.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_wyvern.cpp
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_wyvern.h
# End Source File
# End Group
# Begin Group "wizards"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ww_archmage.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_archmage.h
# End Source File
# Begin Source File

SOURCE=.\ww_deathwizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_deathwizard.h
# End Source File
# Begin Source File

SOURCE=.\ww_dragonwizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_dragonwizard.h
# End Source File
# Begin Source File

SOURCE=.\ww_earthwizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_earthwizard.h
# End Source File
# Begin Source File

SOURCE=.\ww_firewizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_firewizard.h
# End Source File
# Begin Source File

SOURCE=.\ww_icewizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_icewizard.h
# End Source File
# Begin Source File

SOURCE=.\ww_lifewizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_lifewizard.h
# End Source File
# Begin Source File

SOURCE=.\ww_lightningwizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_lightningwizard.h
# End Source File
# Begin Source File

SOURCE=.\ww_naturewizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_naturewizard.h
# End Source File
# Begin Source File

SOURCE=.\ww_windwizard.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_windwizard.h
# End Source File
# Begin Source File

SOURCE=.\ww_wizards.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_wizards.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\ww_shared\ww_build.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_defs.h
# End Source File
# Begin Source File

SOURCE=.\ww_gamerules.cpp
# End Source File
# Begin Source File

SOURCE=.\ww_gamerules.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_weapons.h
# End Source File
# End Group
# Begin Group "tfents"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\tf_baseents.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_ent.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_ent.h
# End Source File
# Begin Source File

SOURCE=.\tf_globalinfo.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_globalinfo.h
# End Source File
# Begin Source File

SOURCE=.\tf_globals.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_globals.h
# End Source File
# Begin Source File

SOURCE=.\tf_info_detect.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_info_detect.h
# End Source File
# Begin Source File

SOURCE=.\tf_info_goal.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_info_goal.h
# End Source File
# Begin Source File

SOURCE=.\tf_info_teamspawn.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_info_teamspawn.h
# End Source File
# Begin Source File

SOURCE=.\tf_info_timer.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_info_timer.h
# End Source File
# Begin Source File

SOURCE=.\tf_item_goal.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_item_goal.h
# End Source File
# Begin Source File

SOURCE=.\tf_teamcheck.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_teamcheck.h
# End Source File
# Begin Source File

SOURCE=.\tf_teamset.cpp
# End Source File
# Begin Source File

SOURCE=.\tf_teamset.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AI_BaseNPC_Schedule.cpp
# End Source File
# Begin Source File

SOURCE=.\animating.cpp
# End Source File
# Begin Source File

SOURCE=.\animation.cpp
# End Source File
# Begin Source File

SOURCE=.\bmodels.cpp
# End Source File
# Begin Source File

SOURCE=.\buttons.cpp
# End Source File
# Begin Source File

SOURCE=.\cbase.cpp
# End Source File
# Begin Source File

SOURCE=.\client.cpp
# End Source File
# Begin Source File

SOURCE=.\combat.cpp
# End Source File
# Begin Source File

SOURCE=.\defaultai.cpp
# End Source File
# Begin Source File

SOURCE=.\doors.cpp
# End Source File
# Begin Source File

SOURCE=.\effects.cpp
# End Source File
# Begin Source File

SOURCE=.\explode.cpp
# End Source File
# Begin Source File

SOURCE=.\flyingmonster.cpp
# End Source File
# Begin Source File

SOURCE=.\func_break.cpp
# End Source File
# Begin Source File

SOURCE=.\game.cpp
# End Source File
# Begin Source File

SOURCE=.\gamerules.cpp
# End Source File
# Begin Source File

SOURCE=.\genericmonster.cpp
# End Source File
# Begin Source File

SOURCE=.\ggrenade.cpp
# End Source File
# Begin Source File

SOURCE=.\globals.cpp
# End Source File
# Begin Source File

SOURCE=.\h_ai.cpp
# End Source File
# Begin Source File

SOURCE=.\h_battery.cpp
# End Source File
# Begin Source File

SOURCE=.\h_cycler.cpp
# End Source File
# Begin Source File

SOURCE=.\h_export.cpp
# End Source File
# Begin Source File

SOURCE=.\healthkit.cpp
# End Source File
# Begin Source File

SOURCE=.\items.cpp
# End Source File
# Begin Source File

SOURCE=.\lights.cpp
# End Source File
# Begin Source File

SOURCE=.\maprules.cpp
# End Source File
# Begin Source File

SOURCE=.\monstermaker.cpp
# End Source File
# Begin Source File

SOURCE=.\monsters.cpp
# End Source File
# Begin Source File

SOURCE=.\monsterstate.cpp
# End Source File
# Begin Source File

SOURCE=.\multiplay_gamerules.cpp
# End Source File
# Begin Source File

SOURCE=.\nodes.cpp
# End Source File
# Begin Source File

SOURCE=.\observer.cpp
# End Source File
# Begin Source File

SOURCE=.\pathcorner.cpp
# End Source File
# Begin Source File

SOURCE=.\plane.cpp
# End Source File
# Begin Source File

SOURCE=.\plats.cpp
# End Source File
# Begin Source File

SOURCE=.\player.cpp
# End Source File
# Begin Source File

SOURCE=.\playermonster.cpp
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_debug.c
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_math.c
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_shared.c
# End Source File
# Begin Source File

SOURCE=.\scripted.cpp
# End Source File
# Begin Source File

SOURCE=.\skill.cpp
# End Source File
# Begin Source File

SOURCE=.\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\soundent.cpp
# End Source File
# Begin Source File

SOURCE=.\spectator.cpp
# End Source File
# Begin Source File

SOURCE=.\squadmonster.cpp
# End Source File
# Begin Source File

SOURCE=.\subs.cpp
# End Source File
# Begin Source File

SOURCE=.\talkmonster.cpp
# End Source File
# Begin Source File

SOURCE=.\triggers.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_gamemgr.cpp
# End Source File
# Begin Source File

SOURCE=.\weapons.cpp
# End Source File
# Begin Source File

SOURCE=.\world.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\activity.h
# End Source File
# Begin Source File

SOURCE=.\activitymap.h
# End Source File
# Begin Source File

SOURCE=.\animation.h
# End Source File
# Begin Source File

SOURCE=.\basemonster.h
# End Source File
# Begin Source File

SOURCE=.\cbase.h
# End Source File
# Begin Source File

SOURCE=.\cdll_dll.h
# End Source File
# Begin Source File

SOURCE=.\client.h
# End Source File
# Begin Source File

SOURCE=.\decals.h
# End Source File
# Begin Source File

SOURCE=.\defaultai.h
# End Source File
# Begin Source File

SOURCE=.\doors.h
# End Source File
# Begin Source File

SOURCE=.\effects.h
# End Source File
# Begin Source File

SOURCE=..\engine\eiface.h
# End Source File
# Begin Source File

SOURCE=.\enginecallback.h
# End Source File
# Begin Source File

SOURCE=.\explode.h
# End Source File
# Begin Source File

SOURCE=.\extdll.h
# End Source File
# Begin Source File

SOURCE=.\flyingmonster.h
# End Source File
# Begin Source File

SOURCE=.\func_break.h
# End Source File
# Begin Source File

SOURCE=.\func_tank.h
# End Source File
# Begin Source File

SOURCE=.\gamerules.h
# End Source File
# Begin Source File

SOURCE=.\h_battery.h
# End Source File
# Begin Source File

SOURCE=.\healthkit.h
# End Source File
# Begin Source File

SOURCE=.\items.h
# End Source File
# Begin Source File

SOURCE=.\monsterevent.h
# End Source File
# Begin Source File

SOURCE=.\monsters.h
# End Source File
# Begin Source File

SOURCE=.\movewith.h
# End Source File
# Begin Source File

SOURCE=.\nodes.h
# End Source File
# Begin Source File

SOURCE=.\plane.h
# End Source File
# Begin Source File

SOURCE=.\plats.h
# End Source File
# Begin Source File

SOURCE=.\player.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_debug.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_defs.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_info.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_materials.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_movevars.h
# End Source File
# Begin Source File

SOURCE=..\pm_shared\pm_shared.h
# End Source File
# Begin Source File

SOURCE=.\saverestore.h
# End Source File
# Begin Source File

SOURCE=.\schedule.h
# End Source File
# Begin Source File

SOURCE=.\scripted.h
# End Source File
# Begin Source File

SOURCE=.\scriptevent.h
# End Source File
# Begin Source File

SOURCE=.\skill.h
# End Source File
# Begin Source File

SOURCE=.\soundent.h
# End Source File
# Begin Source File

SOURCE=.\spectator.h
# End Source File
# Begin Source File

SOURCE=.\squadmonster.h
# End Source File
# Begin Source File

SOURCE=.\swarm.h
# End Source File
# Begin Source File

SOURCE=.\talkmonster.h
# End Source File
# Begin Source File

SOURCE=.\teamplay_gamerules.h
# End Source File
# Begin Source File

SOURCE=.\trains.h
# End Source File
# Begin Source File

SOURCE=.\triggers.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# Begin Source File

SOURCE=.\weapons.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

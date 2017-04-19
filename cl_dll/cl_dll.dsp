# Microsoft Developer Studio Project File - Name="cl_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cl_dll - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cl_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cl_dll.mak" CFG="cl_dll - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cl_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cl_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cl_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../build/release/client/"
# PROP BASE Intermediate_Dir "../build/release/client/"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\build\release\client"
# PROP Intermediate_Dir "..\build\release\client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I ".\\" /I "..\\" /I "..\dlls" /I "..\common" /I "..\pm_shared" /I "..\engine" /I "..\utils\vgui\include" /I "..\game_shared" /D "NDEBUG" /D "MUSIC" /D "WIN32" /D "_WINDOWS" /D "CLIENT_DLL" /D "CLIENT_WEAPONS" /D "HLSDK_3_2_OLD_EIFACE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /x /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib winmm.lib ../utils/vgui/lib/win32_vc6/vgui.lib wsock32.lib /nologo /subsystem:windows /dll /map /machine:I386 /out:"..\build\client.dll"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=cd ..\	cmd.exe /c update.bat
# End Special Build Tool

!ELSEIF  "$(CFG)" == "cl_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../build/debug/client/"
# PROP BASE Intermediate_Dir "../build/debug/client/"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\build\debug\client"
# PROP Intermediate_Dir "..\build\debug\client"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /Zi /Od /I ".\\" /I "..\\" /I "..\dlls" /I "..\common" /I "..\pm_shared" /I "..\engine" /I "..\utils\vgui\include" /I "..\game_shared" /D "_MBCS" /D "CLIENT_DLL" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "CLIENT_WEAPONS" /D "HLSDK_3_2_OLD_EIFACE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 wsock32.lib kernel32.lib user32.lib winmm.lib ../utils/vgui/lib/win32_vc6/vgui.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"..\build\client.dll"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=cd ..\	cmd.exe /c update.bat
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "cl_dll - Win32 Release"
# Name "cl_dll - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;for;f90"
# Begin Group "hl"

# PROP Default_Filter "*.CPP"
# Begin Source File

SOURCE=.\ev_hldm.cpp
# End Source File
# Begin Source File

SOURCE=.\hl\hl_baseentity.cpp
# End Source File
# Begin Source File

SOURCE=.\hl\hl_events.cpp
# End Source File
# Begin Source File

SOURCE=.\hl\hl_objects.cpp
# End Source File
# Begin Source File

SOURCE=.\hl\hl_weapons.cpp
# End Source File
# Begin Source File

SOURCE=..\common\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_KickMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_scrollbar2.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_slider2.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_WorldMap.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_banmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_status.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_vgui_tweakdlg.cpp
# End Source File
# End Group
# Begin Group "ww"

# PROP Default_Filter ""
# Begin Group "events"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ww\events\ww_deathevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_dragonevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_earthevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_events.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_fireevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_iceevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_lifeevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_lightningevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_natureevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_plantevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_satchelevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_sealevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_weaponevents.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\events\ww_windevents.cpp
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
# Begin Source File

SOURCE=..\ww_shared\ww_build.h
# End Source File
# Begin Source File

SOURCE=.\ww\ww_customhud.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\ww_customhud.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_defs.h
# End Source File
# Begin Source File

SOURCE=.\ww\ww_emitter.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\ww_emitter.h
# End Source File
# Begin Source File

SOURCE=.\ww\ww_emitterloader.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\ww_emittermanger.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\ww_emittermanger.h
# End Source File
# Begin Source File

SOURCE=.\ww\ww_particle.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\ww_particle.h
# End Source File
# Begin Source File

SOURCE=.\ww\ww_particlemanger.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\ww_particlemanger.h
# End Source File
# Begin Source File

SOURCE=..\ww_shared\ww_weapons.h
# End Source File
# Begin Source File

SOURCE=.\ww\ww_weather.cpp
# End Source File
# Begin Source File

SOURCE=.\ww\ww_weather.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ammo.cpp
# End Source File
# Begin Source File

SOURCE=.\ammo_secondary.cpp
# End Source File
# Begin Source File

SOURCE=.\ammohistory.cpp
# End Source File
# Begin Source File

SOURCE=.\battery.cpp
# End Source File
# Begin Source File

SOURCE=.\cdll_int.cpp
# End Source File
# Begin Source File

SOURCE=.\com_weapons.cpp
# End Source File
# Begin Source File

SOURCE=.\death.cpp
# End Source File
# Begin Source File

SOURCE=.\demo.cpp
# End Source File
# Begin Source File

SOURCE=.\entity.cpp
# End Source File
# Begin Source File

SOURCE=.\ev_common.cpp
# End Source File
# Begin Source File

SOURCE=.\events.cpp
# End Source File
# Begin Source File

SOURCE=.\flashlight.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStudioModelRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\health.cpp
# End Source File
# Begin Source File

SOURCE=.\hud.cpp
# End Source File
# Begin Source File

SOURCE=.\hud_msg.cpp
# End Source File
# Begin Source File

SOURCE=.\hud_redraw.cpp
# End Source File
# Begin Source File

SOURCE=.\hud_servers.cpp
# End Source File
# Begin Source File

SOURCE=.\hud_spectator.cpp
# End Source File
# Begin Source File

SOURCE=.\hud_update.cpp
# End Source File
# Begin Source File

SOURCE=.\in_camera.cpp
# End Source File
# Begin Source File

SOURCE=.\input.cpp
# End Source File
# Begin Source File

SOURCE=.\inputw32.cpp
# End Source File
# Begin Source File

SOURCE=.\menu.cpp
# End Source File
# Begin Source File

SOURCE=.\message.cpp
# End Source File
# Begin Source File

SOURCE=.\overview.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\parsemsg.cpp
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

SOURCE=.\saytext.cpp
# End Source File
# Begin Source File

SOURCE=.\status_icons.cpp
# End Source File
# Begin Source File

SOURCE=.\statusbar.cpp
# End Source File
# Begin Source File

SOURCE=.\studio_util.cpp
# End Source File
# Begin Source File

SOURCE=.\StudioModelRenderer.cpp
# End Source File
# Begin Source File

SOURCE=.\text_message.cpp
# End Source File
# Begin Source File

SOURCE=.\train.cpp
# End Source File
# Begin Source File

SOURCE=.\tri.cpp
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_checkbutton2.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ClassMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ConsolePanel.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ControlConfigPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_CustomObjects.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_grid.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_helpers.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_int.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_listbox.cpp
# End Source File
# Begin Source File

SOURCE=..\game_shared\vgui_loadtga.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_MOTDWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_SchemeManager.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ScorePanel.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_ServerBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_SpectatorPanel.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_TeamFortressViewport.cpp
# End Source File
# Begin Source File

SOURCE=.\vgui_teammenu.cpp
# End Source File
# Begin Source File

SOURCE=.\view.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\ammo.h
# End Source File
# Begin Source File

SOURCE=.\ammohistory.h
# End Source File
# Begin Source File

SOURCE=.\camera.h
# End Source File
# Begin Source File

SOURCE=.\cl_dll.h
# End Source File
# Begin Source File

SOURCE=.\com_weapons.h
# End Source File
# Begin Source File

SOURCE=.\demo.h
# End Source File
# Begin Source File

SOURCE=.\ev_hldm.h
# End Source File
# Begin Source File

SOURCE=.\eventscripts.h
# End Source File
# Begin Source File

SOURCE=.\GameStudioModelRenderer.h
# End Source File
# Begin Source File

SOURCE=.\health.h
# End Source File
# Begin Source File

SOURCE=.\hud.h
# End Source File
# Begin Source File

SOURCE=.\hud_iface.h
# End Source File
# Begin Source File

SOURCE=.\hud_servers.h
# End Source File
# Begin Source File

SOURCE=.\hud_servers_priv.h
# End Source File
# Begin Source File

SOURCE=.\hud_spectator.h
# End Source File
# Begin Source File

SOURCE=.\in_defs.h
# End Source File
# Begin Source File

SOURCE=..\common\itrackeruser.h
# End Source File
# Begin Source File

SOURCE=.\kbutton.h
# End Source File
# Begin Source File

SOURCE=.\overview.h
# End Source File
# Begin Source File

SOURCE=.\parsemsg.h
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

SOURCE=..\common\r_studioint.h
# End Source File
# Begin Source File

SOURCE=.\studio_util.h
# End Source File
# Begin Source File

SOURCE=.\StudioModelRenderer.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\util_vector.h
# End Source File
# Begin Source File

SOURCE=.\vgui_ConsolePanel.h
# End Source File
# Begin Source File

SOURCE=.\vgui_ControlConfigPanel.h
# End Source File
# Begin Source File

SOURCE=.\vgui_int.h
# End Source File
# Begin Source File

SOURCE=.\vgui_SchemeManager.h
# End Source File
# Begin Source File

SOURCE=.\vgui_ScorePanel.h
# End Source File
# Begin Source File

SOURCE=.\vgui_ServerBrowser.h
# End Source File
# Begin Source File

SOURCE=.\vgui_SpectatorPanel.h
# End Source File
# Begin Source File

SOURCE=.\vgui_TeamFortressViewport.h
# End Source File
# Begin Source File

SOURCE=.\view.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_banmgr.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_status.h
# End Source File
# Begin Source File

SOURCE=..\game_shared\voice_vgui_tweakdlg.h
# End Source File
# Begin Source File

SOURCE=.\wrect.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

# Microsoft Developer Studio Generated NMAKE File, Based on hl.dsp
!IF "$(CFG)" == ""
CFG=hl - Win32 Release
!MESSAGE No configuration specified. Defaulting to hl - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "hl - Win32 Release" && "$(CFG)" != "hl - Win32 Debug" && "$(CFG)" != "hl - Win32 Profile"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hl - Win32 Release"

OUTDIR=.\..\build\release\server
INTDIR=.\..\build\release\server
# Begin Custom Macros
OutDir=.\..\build\release\server
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\build\hl.dll" "$(OUTDIR)\hl.bsc"

!ELSE 

ALL : "cl_dll - Win32 Release" "..\build\hl.dll" "$(OUTDIR)\hl.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cl_dll - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AI_BaseNPC_Schedule.obj"
	-@erase "$(INTDIR)\AI_BaseNPC_Schedule.sbr"
	-@erase "$(INTDIR)\alias.obj"
	-@erase "$(INTDIR)\alias.sbr"
	-@erase "$(INTDIR)\animating.obj"
	-@erase "$(INTDIR)\animating.sbr"
	-@erase "$(INTDIR)\animation.obj"
	-@erase "$(INTDIR)\animation.sbr"
	-@erase "$(INTDIR)\bmodels.obj"
	-@erase "$(INTDIR)\bmodels.sbr"
	-@erase "$(INTDIR)\buttons.obj"
	-@erase "$(INTDIR)\buttons.sbr"
	-@erase "$(INTDIR)\cbase.obj"
	-@erase "$(INTDIR)\cbase.sbr"
	-@erase "$(INTDIR)\client.obj"
	-@erase "$(INTDIR)\client.sbr"
	-@erase "$(INTDIR)\combat.obj"
	-@erase "$(INTDIR)\combat.sbr"
	-@erase "$(INTDIR)\controller.obj"
	-@erase "$(INTDIR)\controller.sbr"
	-@erase "$(INTDIR)\defaultai.obj"
	-@erase "$(INTDIR)\defaultai.sbr"
	-@erase "$(INTDIR)\doors.obj"
	-@erase "$(INTDIR)\doors.sbr"
	-@erase "$(INTDIR)\effects.obj"
	-@erase "$(INTDIR)\effects.sbr"
	-@erase "$(INTDIR)\explode.obj"
	-@erase "$(INTDIR)\explode.sbr"
	-@erase "$(INTDIR)\flyingmonster.obj"
	-@erase "$(INTDIR)\flyingmonster.sbr"
	-@erase "$(INTDIR)\func_break.obj"
	-@erase "$(INTDIR)\func_break.sbr"
	-@erase "$(INTDIR)\game.obj"
	-@erase "$(INTDIR)\game.sbr"
	-@erase "$(INTDIR)\gamerules.obj"
	-@erase "$(INTDIR)\gamerules.sbr"
	-@erase "$(INTDIR)\genericmonster.obj"
	-@erase "$(INTDIR)\genericmonster.sbr"
	-@erase "$(INTDIR)\ggrenade.obj"
	-@erase "$(INTDIR)\ggrenade.sbr"
	-@erase "$(INTDIR)\globals.obj"
	-@erase "$(INTDIR)\globals.sbr"
	-@erase "$(INTDIR)\h_ai.obj"
	-@erase "$(INTDIR)\h_ai.sbr"
	-@erase "$(INTDIR)\h_battery.obj"
	-@erase "$(INTDIR)\h_battery.sbr"
	-@erase "$(INTDIR)\h_cine.obj"
	-@erase "$(INTDIR)\h_cine.sbr"
	-@erase "$(INTDIR)\h_cycler.obj"
	-@erase "$(INTDIR)\h_cycler.sbr"
	-@erase "$(INTDIR)\h_export.obj"
	-@erase "$(INTDIR)\h_export.sbr"
	-@erase "$(INTDIR)\healthkit.obj"
	-@erase "$(INTDIR)\healthkit.sbr"
	-@erase "$(INTDIR)\items.obj"
	-@erase "$(INTDIR)\items.sbr"
	-@erase "$(INTDIR)\lights.obj"
	-@erase "$(INTDIR)\lights.sbr"
	-@erase "$(INTDIR)\locus.obj"
	-@erase "$(INTDIR)\locus.sbr"
	-@erase "$(INTDIR)\maprules.obj"
	-@erase "$(INTDIR)\maprules.sbr"
	-@erase "$(INTDIR)\monstermaker.obj"
	-@erase "$(INTDIR)\monstermaker.sbr"
	-@erase "$(INTDIR)\monsters.obj"
	-@erase "$(INTDIR)\monsters.sbr"
	-@erase "$(INTDIR)\monsterstate.obj"
	-@erase "$(INTDIR)\monsterstate.sbr"
	-@erase "$(INTDIR)\movewith.obj"
	-@erase "$(INTDIR)\movewith.sbr"
	-@erase "$(INTDIR)\multiplay_gamerules.obj"
	-@erase "$(INTDIR)\multiplay_gamerules.sbr"
	-@erase "$(INTDIR)\nodes.obj"
	-@erase "$(INTDIR)\nodes.sbr"
	-@erase "$(INTDIR)\observer.obj"
	-@erase "$(INTDIR)\observer.sbr"
	-@erase "$(INTDIR)\pathcorner.obj"
	-@erase "$(INTDIR)\pathcorner.sbr"
	-@erase "$(INTDIR)\plane.obj"
	-@erase "$(INTDIR)\plane.sbr"
	-@erase "$(INTDIR)\plats.obj"
	-@erase "$(INTDIR)\plats.sbr"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\player.sbr"
	-@erase "$(INTDIR)\playermonster.obj"
	-@erase "$(INTDIR)\playermonster.sbr"
	-@erase "$(INTDIR)\pm_debug.obj"
	-@erase "$(INTDIR)\pm_debug.sbr"
	-@erase "$(INTDIR)\pm_math.obj"
	-@erase "$(INTDIR)\pm_math.sbr"
	-@erase "$(INTDIR)\pm_shared.obj"
	-@erase "$(INTDIR)\pm_shared.sbr"
	-@erase "$(INTDIR)\scripted.obj"
	-@erase "$(INTDIR)\scripted.sbr"
	-@erase "$(INTDIR)\singleplay_gamerules.obj"
	-@erase "$(INTDIR)\singleplay_gamerules.sbr"
	-@erase "$(INTDIR)\skill.obj"
	-@erase "$(INTDIR)\skill.sbr"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\sound.sbr"
	-@erase "$(INTDIR)\soundent.obj"
	-@erase "$(INTDIR)\soundent.sbr"
	-@erase "$(INTDIR)\spectator.obj"
	-@erase "$(INTDIR)\spectator.sbr"
	-@erase "$(INTDIR)\squadmonster.obj"
	-@erase "$(INTDIR)\squadmonster.sbr"
	-@erase "$(INTDIR)\stats.obj"
	-@erase "$(INTDIR)\stats.sbr"
	-@erase "$(INTDIR)\subs.obj"
	-@erase "$(INTDIR)\subs.sbr"
	-@erase "$(INTDIR)\talkmonster.obj"
	-@erase "$(INTDIR)\talkmonster.sbr"
	-@erase "$(INTDIR)\teamplay_gamerules.obj"
	-@erase "$(INTDIR)\teamplay_gamerules.sbr"
	-@erase "$(INTDIR)\tempmonster.obj"
	-@erase "$(INTDIR)\tempmonster.sbr"
	-@erase "$(INTDIR)\tf_baseents.obj"
	-@erase "$(INTDIR)\tf_baseents.sbr"
	-@erase "$(INTDIR)\tf_ent.obj"
	-@erase "$(INTDIR)\tf_ent.sbr"
	-@erase "$(INTDIR)\tf_globalinfo.obj"
	-@erase "$(INTDIR)\tf_globalinfo.sbr"
	-@erase "$(INTDIR)\tf_globals.obj"
	-@erase "$(INTDIR)\tf_globals.sbr"
	-@erase "$(INTDIR)\tf_info_detect.obj"
	-@erase "$(INTDIR)\tf_info_detect.sbr"
	-@erase "$(INTDIR)\tf_info_goal.obj"
	-@erase "$(INTDIR)\tf_info_goal.sbr"
	-@erase "$(INTDIR)\tf_info_teamspawn.obj"
	-@erase "$(INTDIR)\tf_info_teamspawn.sbr"
	-@erase "$(INTDIR)\tf_info_timer.obj"
	-@erase "$(INTDIR)\tf_info_timer.sbr"
	-@erase "$(INTDIR)\tf_item_goal.obj"
	-@erase "$(INTDIR)\tf_item_goal.sbr"
	-@erase "$(INTDIR)\tf_teamcheck.obj"
	-@erase "$(INTDIR)\tf_teamcheck.sbr"
	-@erase "$(INTDIR)\tf_teamset.obj"
	-@erase "$(INTDIR)\tf_teamset.sbr"
	-@erase "$(INTDIR)\triggers.obj"
	-@erase "$(INTDIR)\triggers.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\voice_gamemgr.obj"
	-@erase "$(INTDIR)\voice_gamemgr.sbr"
	-@erase "$(INTDIR)\weapons.obj"
	-@erase "$(INTDIR)\weapons.sbr"
	-@erase "$(INTDIR)\world.obj"
	-@erase "$(INTDIR)\world.sbr"
	-@erase "$(INTDIR)\ww_archmage.obj"
	-@erase "$(INTDIR)\ww_archmage.sbr"
	-@erase "$(INTDIR)\ww_bearbite.obj"
	-@erase "$(INTDIR)\ww_bearbite.sbr"
	-@erase "$(INTDIR)\ww_bird.obj"
	-@erase "$(INTDIR)\ww_bird.sbr"
	-@erase "$(INTDIR)\ww_deathray.obj"
	-@erase "$(INTDIR)\ww_deathray.sbr"
	-@erase "$(INTDIR)\ww_deathseal.obj"
	-@erase "$(INTDIR)\ww_deathseal.sbr"
	-@erase "$(INTDIR)\ww_deathwizard.obj"
	-@erase "$(INTDIR)\ww_deathwizard.sbr"
	-@erase "$(INTDIR)\ww_doublemissile.obj"
	-@erase "$(INTDIR)\ww_doublemissile.sbr"
	-@erase "$(INTDIR)\ww_dragonseal.obj"
	-@erase "$(INTDIR)\ww_dragonseal.sbr"
	-@erase "$(INTDIR)\ww_dragonwizard.obj"
	-@erase "$(INTDIR)\ww_dragonwizard.sbr"
	-@erase "$(INTDIR)\ww_earthseal.obj"
	-@erase "$(INTDIR)\ww_earthseal.sbr"
	-@erase "$(INTDIR)\ww_earthwizard.obj"
	-@erase "$(INTDIR)\ww_earthwizard.sbr"
	-@erase "$(INTDIR)\ww_emitter.obj"
	-@erase "$(INTDIR)\ww_emitter.sbr"
	-@erase "$(INTDIR)\ww_fireball.obj"
	-@erase "$(INTDIR)\ww_fireball.sbr"
	-@erase "$(INTDIR)\ww_fireseal.obj"
	-@erase "$(INTDIR)\ww_fireseal.sbr"
	-@erase "$(INTDIR)\ww_firewizard.obj"
	-@erase "$(INTDIR)\ww_firewizard.sbr"
	-@erase "$(INTDIR)\ww_flamelick.obj"
	-@erase "$(INTDIR)\ww_flamelick.sbr"
	-@erase "$(INTDIR)\ww_forceblast.obj"
	-@erase "$(INTDIR)\ww_forceblast.sbr"
	-@erase "$(INTDIR)\ww_gamerules.obj"
	-@erase "$(INTDIR)\ww_gamerules.sbr"
	-@erase "$(INTDIR)\ww_iceseal.obj"
	-@erase "$(INTDIR)\ww_iceseal.sbr"
	-@erase "$(INTDIR)\ww_icewizard.obj"
	-@erase "$(INTDIR)\ww_icewizard.sbr"
	-@erase "$(INTDIR)\ww_levitate.obj"
	-@erase "$(INTDIR)\ww_levitate.sbr"
	-@erase "$(INTDIR)\ww_lifeseal.obj"
	-@erase "$(INTDIR)\ww_lifeseal.sbr"
	-@erase "$(INTDIR)\ww_lifewizard.obj"
	-@erase "$(INTDIR)\ww_lifewizard.sbr"
	-@erase "$(INTDIR)\ww_lightningbolt.obj"
	-@erase "$(INTDIR)\ww_lightningbolt.sbr"
	-@erase "$(INTDIR)\ww_lightningseal.obj"
	-@erase "$(INTDIR)\ww_lightningseal.sbr"
	-@erase "$(INTDIR)\ww_lightningwizard.obj"
	-@erase "$(INTDIR)\ww_lightningwizard.sbr"
	-@erase "$(INTDIR)\ww_missile.obj"
	-@erase "$(INTDIR)\ww_missile.sbr"
	-@erase "$(INTDIR)\ww_natureseal.obj"
	-@erase "$(INTDIR)\ww_natureseal.sbr"
	-@erase "$(INTDIR)\ww_naturewizard.obj"
	-@erase "$(INTDIR)\ww_naturewizard.sbr"
	-@erase "$(INTDIR)\ww_seal.obj"
	-@erase "$(INTDIR)\ww_seal.sbr"
	-@erase "$(INTDIR)\ww_seals.obj"
	-@erase "$(INTDIR)\ww_seals.sbr"
	-@erase "$(INTDIR)\ww_sentrycrystal.obj"
	-@erase "$(INTDIR)\ww_sentrycrystal.sbr"
	-@erase "$(INTDIR)\ww_skull.obj"
	-@erase "$(INTDIR)\ww_skull.sbr"
	-@erase "$(INTDIR)\ww_spotbolt.obj"
	-@erase "$(INTDIR)\ww_spotbolt.sbr"
	-@erase "$(INTDIR)\ww_staff.obj"
	-@erase "$(INTDIR)\ww_staff.sbr"
	-@erase "$(INTDIR)\ww_updraft.obj"
	-@erase "$(INTDIR)\ww_updraft.sbr"
	-@erase "$(INTDIR)\ww_whiteray.obj"
	-@erase "$(INTDIR)\ww_whiteray.sbr"
	-@erase "$(INTDIR)\ww_windseal.obj"
	-@erase "$(INTDIR)\ww_windseal.sbr"
	-@erase "$(INTDIR)\ww_windwizard.obj"
	-@erase "$(INTDIR)\ww_windwizard.sbr"
	-@erase "$(INTDIR)\ww_wizards.obj"
	-@erase "$(INTDIR)\ww_wizards.sbr"
	-@erase "$(INTDIR)\xen.obj"
	-@erase "$(INTDIR)\xen.sbr"
	-@erase "$(OUTDIR)\hl.bsc"
	-@erase "$(OUTDIR)\hl.exp"
	-@erase "$(OUTDIR)\hl.lib"
	-@erase "..\build\hl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /MT /W3 /GX /Zi /O2 /Ob2 /I ".\\" /I "..\\" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\game_shared" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /D "CLIENT_WEAPONS" /D "MUSIC" /Fr"$(INTDIR)\\" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hl.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ww_archmage.sbr" \
	"$(INTDIR)\ww_deathwizard.sbr" \
	"$(INTDIR)\ww_dragonwizard.sbr" \
	"$(INTDIR)\ww_earthwizard.sbr" \
	"$(INTDIR)\ww_firewizard.sbr" \
	"$(INTDIR)\ww_icewizard.sbr" \
	"$(INTDIR)\ww_lifewizard.sbr" \
	"$(INTDIR)\ww_lightningwizard.sbr" \
	"$(INTDIR)\ww_naturewizard.sbr" \
	"$(INTDIR)\ww_windwizard.sbr" \
	"$(INTDIR)\ww_wizards.sbr" \
	"$(INTDIR)\ww_deathseal.sbr" \
	"$(INTDIR)\ww_dragonseal.sbr" \
	"$(INTDIR)\ww_earthseal.sbr" \
	"$(INTDIR)\ww_fireseal.sbr" \
	"$(INTDIR)\ww_iceseal.sbr" \
	"$(INTDIR)\ww_lifeseal.sbr" \
	"$(INTDIR)\ww_lightningseal.sbr" \
	"$(INTDIR)\ww_natureseal.sbr" \
	"$(INTDIR)\ww_seals.sbr" \
	"$(INTDIR)\ww_windseal.sbr" \
	"$(INTDIR)\ww_deathray.sbr" \
	"$(INTDIR)\ww_doublemissile.sbr" \
	"$(INTDIR)\ww_fireball.sbr" \
	"$(INTDIR)\ww_flamelick.sbr" \
	"$(INTDIR)\ww_levitate.sbr" \
	"$(INTDIR)\ww_lightningbolt.sbr" \
	"$(INTDIR)\ww_missile.sbr" \
	"$(INTDIR)\ww_seal.sbr" \
	"$(INTDIR)\ww_spotbolt.sbr" \
	"$(INTDIR)\ww_staff.sbr" \
	"$(INTDIR)\ww_updraft.sbr" \
	"$(INTDIR)\ww_emitter.sbr" \
	"$(INTDIR)\ww_sentrycrystal.sbr" \
	"$(INTDIR)\ww_gamerules.sbr" \
	"$(INTDIR)\tf_baseents.sbr" \
	"$(INTDIR)\tf_ent.sbr" \
	"$(INTDIR)\tf_globalinfo.sbr" \
	"$(INTDIR)\tf_globals.sbr" \
	"$(INTDIR)\tf_info_detect.sbr" \
	"$(INTDIR)\tf_info_goal.sbr" \
	"$(INTDIR)\tf_info_teamspawn.sbr" \
	"$(INTDIR)\tf_info_timer.sbr" \
	"$(INTDIR)\tf_item_goal.sbr" \
	"$(INTDIR)\tf_teamcheck.sbr" \
	"$(INTDIR)\tf_teamset.sbr" \
	"$(INTDIR)\AI_BaseNPC_Schedule.sbr" \
	"$(INTDIR)\alias.sbr" \
	"$(INTDIR)\animating.sbr" \
	"$(INTDIR)\animation.sbr" \
	"$(INTDIR)\bmodels.sbr" \
	"$(INTDIR)\buttons.sbr" \
	"$(INTDIR)\cbase.sbr" \
	"$(INTDIR)\client.sbr" \
	"$(INTDIR)\combat.sbr" \
	"$(INTDIR)\controller.sbr" \
	"$(INTDIR)\defaultai.sbr" \
	"$(INTDIR)\doors.sbr" \
	"$(INTDIR)\effects.sbr" \
	"$(INTDIR)\explode.sbr" \
	"$(INTDIR)\flyingmonster.sbr" \
	"$(INTDIR)\func_break.sbr" \
	"$(INTDIR)\game.sbr" \
	"$(INTDIR)\gamerules.sbr" \
	"$(INTDIR)\genericmonster.sbr" \
	"$(INTDIR)\ggrenade.sbr" \
	"$(INTDIR)\globals.sbr" \
	"$(INTDIR)\h_ai.sbr" \
	"$(INTDIR)\h_battery.sbr" \
	"$(INTDIR)\h_cine.sbr" \
	"$(INTDIR)\h_cycler.sbr" \
	"$(INTDIR)\h_export.sbr" \
	"$(INTDIR)\healthkit.sbr" \
	"$(INTDIR)\items.sbr" \
	"$(INTDIR)\lights.sbr" \
	"$(INTDIR)\locus.sbr" \
	"$(INTDIR)\maprules.sbr" \
	"$(INTDIR)\monstermaker.sbr" \
	"$(INTDIR)\monsters.sbr" \
	"$(INTDIR)\monsterstate.sbr" \
	"$(INTDIR)\movewith.sbr" \
	"$(INTDIR)\multiplay_gamerules.sbr" \
	"$(INTDIR)\nodes.sbr" \
	"$(INTDIR)\observer.sbr" \
	"$(INTDIR)\pathcorner.sbr" \
	"$(INTDIR)\plane.sbr" \
	"$(INTDIR)\plats.sbr" \
	"$(INTDIR)\player.sbr" \
	"$(INTDIR)\playermonster.sbr" \
	"$(INTDIR)\pm_debug.sbr" \
	"$(INTDIR)\pm_math.sbr" \
	"$(INTDIR)\pm_shared.sbr" \
	"$(INTDIR)\scripted.sbr" \
	"$(INTDIR)\singleplay_gamerules.sbr" \
	"$(INTDIR)\skill.sbr" \
	"$(INTDIR)\sound.sbr" \
	"$(INTDIR)\soundent.sbr" \
	"$(INTDIR)\spectator.sbr" \
	"$(INTDIR)\squadmonster.sbr" \
	"$(INTDIR)\stats.sbr" \
	"$(INTDIR)\subs.sbr" \
	"$(INTDIR)\talkmonster.sbr" \
	"$(INTDIR)\teamplay_gamerules.sbr" \
	"$(INTDIR)\tempmonster.sbr" \
	"$(INTDIR)\triggers.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\voice_gamemgr.sbr" \
	"$(INTDIR)\weapons.sbr" \
	"$(INTDIR)\world.sbr" \
	"$(INTDIR)\xen.sbr" \
	"$(INTDIR)\ww_whiteray.sbr" \
	"$(INTDIR)\ww_forceblast.sbr" \
	"$(INTDIR)\ww_bearbite.sbr" \
	"$(INTDIR)\ww_skull.sbr" \
	"$(INTDIR)\ww_bird.sbr"

"$(OUTDIR)\hl.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib advapi32.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"$(OUTDIR)\hl.pdb" /machine:I386 /def:".\hl.def" /out:"..\build\hl.dll" /implib:"$(OUTDIR)\hl.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ww_archmage.obj" \
	"$(INTDIR)\ww_deathwizard.obj" \
	"$(INTDIR)\ww_dragonwizard.obj" \
	"$(INTDIR)\ww_earthwizard.obj" \
	"$(INTDIR)\ww_firewizard.obj" \
	"$(INTDIR)\ww_icewizard.obj" \
	"$(INTDIR)\ww_lifewizard.obj" \
	"$(INTDIR)\ww_lightningwizard.obj" \
	"$(INTDIR)\ww_naturewizard.obj" \
	"$(INTDIR)\ww_windwizard.obj" \
	"$(INTDIR)\ww_wizards.obj" \
	"$(INTDIR)\ww_deathseal.obj" \
	"$(INTDIR)\ww_dragonseal.obj" \
	"$(INTDIR)\ww_earthseal.obj" \
	"$(INTDIR)\ww_fireseal.obj" \
	"$(INTDIR)\ww_iceseal.obj" \
	"$(INTDIR)\ww_lifeseal.obj" \
	"$(INTDIR)\ww_lightningseal.obj" \
	"$(INTDIR)\ww_natureseal.obj" \
	"$(INTDIR)\ww_seals.obj" \
	"$(INTDIR)\ww_windseal.obj" \
	"$(INTDIR)\ww_deathray.obj" \
	"$(INTDIR)\ww_doublemissile.obj" \
	"$(INTDIR)\ww_fireball.obj" \
	"$(INTDIR)\ww_flamelick.obj" \
	"$(INTDIR)\ww_levitate.obj" \
	"$(INTDIR)\ww_lightningbolt.obj" \
	"$(INTDIR)\ww_missile.obj" \
	"$(INTDIR)\ww_seal.obj" \
	"$(INTDIR)\ww_spotbolt.obj" \
	"$(INTDIR)\ww_staff.obj" \
	"$(INTDIR)\ww_updraft.obj" \
	"$(INTDIR)\ww_emitter.obj" \
	"$(INTDIR)\ww_sentrycrystal.obj" \
	"$(INTDIR)\ww_gamerules.obj" \
	"$(INTDIR)\tf_baseents.obj" \
	"$(INTDIR)\tf_ent.obj" \
	"$(INTDIR)\tf_globalinfo.obj" \
	"$(INTDIR)\tf_globals.obj" \
	"$(INTDIR)\tf_info_detect.obj" \
	"$(INTDIR)\tf_info_goal.obj" \
	"$(INTDIR)\tf_info_teamspawn.obj" \
	"$(INTDIR)\tf_info_timer.obj" \
	"$(INTDIR)\tf_item_goal.obj" \
	"$(INTDIR)\tf_teamcheck.obj" \
	"$(INTDIR)\tf_teamset.obj" \
	"$(INTDIR)\AI_BaseNPC_Schedule.obj" \
	"$(INTDIR)\alias.obj" \
	"$(INTDIR)\animating.obj" \
	"$(INTDIR)\animation.obj" \
	"$(INTDIR)\bmodels.obj" \
	"$(INTDIR)\buttons.obj" \
	"$(INTDIR)\cbase.obj" \
	"$(INTDIR)\client.obj" \
	"$(INTDIR)\combat.obj" \
	"$(INTDIR)\controller.obj" \
	"$(INTDIR)\defaultai.obj" \
	"$(INTDIR)\doors.obj" \
	"$(INTDIR)\effects.obj" \
	"$(INTDIR)\explode.obj" \
	"$(INTDIR)\flyingmonster.obj" \
	"$(INTDIR)\func_break.obj" \
	"$(INTDIR)\game.obj" \
	"$(INTDIR)\gamerules.obj" \
	"$(INTDIR)\genericmonster.obj" \
	"$(INTDIR)\ggrenade.obj" \
	"$(INTDIR)\globals.obj" \
	"$(INTDIR)\h_ai.obj" \
	"$(INTDIR)\h_battery.obj" \
	"$(INTDIR)\h_cine.obj" \
	"$(INTDIR)\h_cycler.obj" \
	"$(INTDIR)\h_export.obj" \
	"$(INTDIR)\healthkit.obj" \
	"$(INTDIR)\items.obj" \
	"$(INTDIR)\lights.obj" \
	"$(INTDIR)\locus.obj" \
	"$(INTDIR)\maprules.obj" \
	"$(INTDIR)\monstermaker.obj" \
	"$(INTDIR)\monsters.obj" \
	"$(INTDIR)\monsterstate.obj" \
	"$(INTDIR)\movewith.obj" \
	"$(INTDIR)\multiplay_gamerules.obj" \
	"$(INTDIR)\nodes.obj" \
	"$(INTDIR)\observer.obj" \
	"$(INTDIR)\pathcorner.obj" \
	"$(INTDIR)\plane.obj" \
	"$(INTDIR)\plats.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playermonster.obj" \
	"$(INTDIR)\pm_debug.obj" \
	"$(INTDIR)\pm_math.obj" \
	"$(INTDIR)\pm_shared.obj" \
	"$(INTDIR)\scripted.obj" \
	"$(INTDIR)\singleplay_gamerules.obj" \
	"$(INTDIR)\skill.obj" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\soundent.obj" \
	"$(INTDIR)\spectator.obj" \
	"$(INTDIR)\squadmonster.obj" \
	"$(INTDIR)\stats.obj" \
	"$(INTDIR)\subs.obj" \
	"$(INTDIR)\talkmonster.obj" \
	"$(INTDIR)\teamplay_gamerules.obj" \
	"$(INTDIR)\tempmonster.obj" \
	"$(INTDIR)\triggers.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\voice_gamemgr.obj" \
	"$(INTDIR)\weapons.obj" \
	"$(INTDIR)\world.obj" \
	"$(INTDIR)\xen.obj" \
	"$(INTDIR)\ww_whiteray.obj" \
	"$(INTDIR)\ww_forceblast.obj" \
	"$(INTDIR)\ww_bearbite.obj" \
	"$(INTDIR)\ww_skull.obj" \
	"$(INTDIR)\ww_bird.obj" \
	"..\build\release\client\client.lib"

"..\build\hl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\..\build\release\server
# End Custom Macros

$(DS_POSTBUILD_DEP) : "cl_dll - Win32 Release" "..\build\hl.dll" "$(OUTDIR)\hl.bsc"
   cd ..\
	cmd.exe /c update.bat
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "hl - Win32 Debug"

OUTDIR=.\..\build\debug\server
INTDIR=.\..\build\debug\server
# Begin Custom Macros
OutDir=.\..\build\debug\server
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\build\hl.dll" "$(OUTDIR)\hl.bsc"

!ELSE 

ALL : "cl_dll - Win32 Debug" "..\build\hl.dll" "$(OUTDIR)\hl.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"cl_dll - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AI_BaseNPC_Schedule.obj"
	-@erase "$(INTDIR)\AI_BaseNPC_Schedule.sbr"
	-@erase "$(INTDIR)\alias.obj"
	-@erase "$(INTDIR)\alias.sbr"
	-@erase "$(INTDIR)\animating.obj"
	-@erase "$(INTDIR)\animating.sbr"
	-@erase "$(INTDIR)\animation.obj"
	-@erase "$(INTDIR)\animation.sbr"
	-@erase "$(INTDIR)\bmodels.obj"
	-@erase "$(INTDIR)\bmodels.sbr"
	-@erase "$(INTDIR)\buttons.obj"
	-@erase "$(INTDIR)\buttons.sbr"
	-@erase "$(INTDIR)\cbase.obj"
	-@erase "$(INTDIR)\cbase.sbr"
	-@erase "$(INTDIR)\client.obj"
	-@erase "$(INTDIR)\client.sbr"
	-@erase "$(INTDIR)\combat.obj"
	-@erase "$(INTDIR)\combat.sbr"
	-@erase "$(INTDIR)\controller.obj"
	-@erase "$(INTDIR)\controller.sbr"
	-@erase "$(INTDIR)\defaultai.obj"
	-@erase "$(INTDIR)\defaultai.sbr"
	-@erase "$(INTDIR)\doors.obj"
	-@erase "$(INTDIR)\doors.sbr"
	-@erase "$(INTDIR)\effects.obj"
	-@erase "$(INTDIR)\effects.sbr"
	-@erase "$(INTDIR)\explode.obj"
	-@erase "$(INTDIR)\explode.sbr"
	-@erase "$(INTDIR)\flyingmonster.obj"
	-@erase "$(INTDIR)\flyingmonster.sbr"
	-@erase "$(INTDIR)\func_break.obj"
	-@erase "$(INTDIR)\func_break.sbr"
	-@erase "$(INTDIR)\game.obj"
	-@erase "$(INTDIR)\game.sbr"
	-@erase "$(INTDIR)\gamerules.obj"
	-@erase "$(INTDIR)\gamerules.sbr"
	-@erase "$(INTDIR)\genericmonster.obj"
	-@erase "$(INTDIR)\genericmonster.sbr"
	-@erase "$(INTDIR)\ggrenade.obj"
	-@erase "$(INTDIR)\ggrenade.sbr"
	-@erase "$(INTDIR)\globals.obj"
	-@erase "$(INTDIR)\globals.sbr"
	-@erase "$(INTDIR)\h_ai.obj"
	-@erase "$(INTDIR)\h_ai.sbr"
	-@erase "$(INTDIR)\h_battery.obj"
	-@erase "$(INTDIR)\h_battery.sbr"
	-@erase "$(INTDIR)\h_cine.obj"
	-@erase "$(INTDIR)\h_cine.sbr"
	-@erase "$(INTDIR)\h_cycler.obj"
	-@erase "$(INTDIR)\h_cycler.sbr"
	-@erase "$(INTDIR)\h_export.obj"
	-@erase "$(INTDIR)\h_export.sbr"
	-@erase "$(INTDIR)\healthkit.obj"
	-@erase "$(INTDIR)\healthkit.sbr"
	-@erase "$(INTDIR)\items.obj"
	-@erase "$(INTDIR)\items.sbr"
	-@erase "$(INTDIR)\lights.obj"
	-@erase "$(INTDIR)\lights.sbr"
	-@erase "$(INTDIR)\locus.obj"
	-@erase "$(INTDIR)\locus.sbr"
	-@erase "$(INTDIR)\maprules.obj"
	-@erase "$(INTDIR)\maprules.sbr"
	-@erase "$(INTDIR)\monstermaker.obj"
	-@erase "$(INTDIR)\monstermaker.sbr"
	-@erase "$(INTDIR)\monsters.obj"
	-@erase "$(INTDIR)\monsters.sbr"
	-@erase "$(INTDIR)\monsterstate.obj"
	-@erase "$(INTDIR)\monsterstate.sbr"
	-@erase "$(INTDIR)\movewith.obj"
	-@erase "$(INTDIR)\movewith.sbr"
	-@erase "$(INTDIR)\multiplay_gamerules.obj"
	-@erase "$(INTDIR)\multiplay_gamerules.sbr"
	-@erase "$(INTDIR)\nodes.obj"
	-@erase "$(INTDIR)\nodes.sbr"
	-@erase "$(INTDIR)\observer.obj"
	-@erase "$(INTDIR)\observer.sbr"
	-@erase "$(INTDIR)\pathcorner.obj"
	-@erase "$(INTDIR)\pathcorner.sbr"
	-@erase "$(INTDIR)\plane.obj"
	-@erase "$(INTDIR)\plane.sbr"
	-@erase "$(INTDIR)\plats.obj"
	-@erase "$(INTDIR)\plats.sbr"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\player.sbr"
	-@erase "$(INTDIR)\playermonster.obj"
	-@erase "$(INTDIR)\playermonster.sbr"
	-@erase "$(INTDIR)\pm_debug.obj"
	-@erase "$(INTDIR)\pm_debug.sbr"
	-@erase "$(INTDIR)\pm_math.obj"
	-@erase "$(INTDIR)\pm_math.sbr"
	-@erase "$(INTDIR)\pm_shared.obj"
	-@erase "$(INTDIR)\pm_shared.sbr"
	-@erase "$(INTDIR)\scripted.obj"
	-@erase "$(INTDIR)\scripted.sbr"
	-@erase "$(INTDIR)\singleplay_gamerules.obj"
	-@erase "$(INTDIR)\singleplay_gamerules.sbr"
	-@erase "$(INTDIR)\skill.obj"
	-@erase "$(INTDIR)\skill.sbr"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\sound.sbr"
	-@erase "$(INTDIR)\soundent.obj"
	-@erase "$(INTDIR)\soundent.sbr"
	-@erase "$(INTDIR)\spectator.obj"
	-@erase "$(INTDIR)\spectator.sbr"
	-@erase "$(INTDIR)\squadmonster.obj"
	-@erase "$(INTDIR)\squadmonster.sbr"
	-@erase "$(INTDIR)\stats.obj"
	-@erase "$(INTDIR)\stats.sbr"
	-@erase "$(INTDIR)\subs.obj"
	-@erase "$(INTDIR)\subs.sbr"
	-@erase "$(INTDIR)\talkmonster.obj"
	-@erase "$(INTDIR)\talkmonster.sbr"
	-@erase "$(INTDIR)\teamplay_gamerules.obj"
	-@erase "$(INTDIR)\teamplay_gamerules.sbr"
	-@erase "$(INTDIR)\tempmonster.obj"
	-@erase "$(INTDIR)\tempmonster.sbr"
	-@erase "$(INTDIR)\tf_baseents.obj"
	-@erase "$(INTDIR)\tf_baseents.sbr"
	-@erase "$(INTDIR)\tf_ent.obj"
	-@erase "$(INTDIR)\tf_ent.sbr"
	-@erase "$(INTDIR)\tf_globalinfo.obj"
	-@erase "$(INTDIR)\tf_globalinfo.sbr"
	-@erase "$(INTDIR)\tf_globals.obj"
	-@erase "$(INTDIR)\tf_globals.sbr"
	-@erase "$(INTDIR)\tf_info_detect.obj"
	-@erase "$(INTDIR)\tf_info_detect.sbr"
	-@erase "$(INTDIR)\tf_info_goal.obj"
	-@erase "$(INTDIR)\tf_info_goal.sbr"
	-@erase "$(INTDIR)\tf_info_teamspawn.obj"
	-@erase "$(INTDIR)\tf_info_teamspawn.sbr"
	-@erase "$(INTDIR)\tf_info_timer.obj"
	-@erase "$(INTDIR)\tf_info_timer.sbr"
	-@erase "$(INTDIR)\tf_item_goal.obj"
	-@erase "$(INTDIR)\tf_item_goal.sbr"
	-@erase "$(INTDIR)\tf_teamcheck.obj"
	-@erase "$(INTDIR)\tf_teamcheck.sbr"
	-@erase "$(INTDIR)\tf_teamset.obj"
	-@erase "$(INTDIR)\tf_teamset.sbr"
	-@erase "$(INTDIR)\triggers.obj"
	-@erase "$(INTDIR)\triggers.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\voice_gamemgr.obj"
	-@erase "$(INTDIR)\voice_gamemgr.sbr"
	-@erase "$(INTDIR)\weapons.obj"
	-@erase "$(INTDIR)\weapons.sbr"
	-@erase "$(INTDIR)\world.obj"
	-@erase "$(INTDIR)\world.sbr"
	-@erase "$(INTDIR)\ww_archmage.obj"
	-@erase "$(INTDIR)\ww_archmage.sbr"
	-@erase "$(INTDIR)\ww_bearbite.obj"
	-@erase "$(INTDIR)\ww_bearbite.sbr"
	-@erase "$(INTDIR)\ww_bird.obj"
	-@erase "$(INTDIR)\ww_bird.sbr"
	-@erase "$(INTDIR)\ww_deathray.obj"
	-@erase "$(INTDIR)\ww_deathray.sbr"
	-@erase "$(INTDIR)\ww_deathseal.obj"
	-@erase "$(INTDIR)\ww_deathseal.sbr"
	-@erase "$(INTDIR)\ww_deathwizard.obj"
	-@erase "$(INTDIR)\ww_deathwizard.sbr"
	-@erase "$(INTDIR)\ww_doublemissile.obj"
	-@erase "$(INTDIR)\ww_doublemissile.sbr"
	-@erase "$(INTDIR)\ww_dragonseal.obj"
	-@erase "$(INTDIR)\ww_dragonseal.sbr"
	-@erase "$(INTDIR)\ww_dragonwizard.obj"
	-@erase "$(INTDIR)\ww_dragonwizard.sbr"
	-@erase "$(INTDIR)\ww_earthseal.obj"
	-@erase "$(INTDIR)\ww_earthseal.sbr"
	-@erase "$(INTDIR)\ww_earthwizard.obj"
	-@erase "$(INTDIR)\ww_earthwizard.sbr"
	-@erase "$(INTDIR)\ww_emitter.obj"
	-@erase "$(INTDIR)\ww_emitter.sbr"
	-@erase "$(INTDIR)\ww_fireball.obj"
	-@erase "$(INTDIR)\ww_fireball.sbr"
	-@erase "$(INTDIR)\ww_fireseal.obj"
	-@erase "$(INTDIR)\ww_fireseal.sbr"
	-@erase "$(INTDIR)\ww_firewizard.obj"
	-@erase "$(INTDIR)\ww_firewizard.sbr"
	-@erase "$(INTDIR)\ww_flamelick.obj"
	-@erase "$(INTDIR)\ww_flamelick.sbr"
	-@erase "$(INTDIR)\ww_forceblast.obj"
	-@erase "$(INTDIR)\ww_forceblast.sbr"
	-@erase "$(INTDIR)\ww_gamerules.obj"
	-@erase "$(INTDIR)\ww_gamerules.sbr"
	-@erase "$(INTDIR)\ww_iceseal.obj"
	-@erase "$(INTDIR)\ww_iceseal.sbr"
	-@erase "$(INTDIR)\ww_icewizard.obj"
	-@erase "$(INTDIR)\ww_icewizard.sbr"
	-@erase "$(INTDIR)\ww_levitate.obj"
	-@erase "$(INTDIR)\ww_levitate.sbr"
	-@erase "$(INTDIR)\ww_lifeseal.obj"
	-@erase "$(INTDIR)\ww_lifeseal.sbr"
	-@erase "$(INTDIR)\ww_lifewizard.obj"
	-@erase "$(INTDIR)\ww_lifewizard.sbr"
	-@erase "$(INTDIR)\ww_lightningbolt.obj"
	-@erase "$(INTDIR)\ww_lightningbolt.sbr"
	-@erase "$(INTDIR)\ww_lightningseal.obj"
	-@erase "$(INTDIR)\ww_lightningseal.sbr"
	-@erase "$(INTDIR)\ww_lightningwizard.obj"
	-@erase "$(INTDIR)\ww_lightningwizard.sbr"
	-@erase "$(INTDIR)\ww_missile.obj"
	-@erase "$(INTDIR)\ww_missile.sbr"
	-@erase "$(INTDIR)\ww_natureseal.obj"
	-@erase "$(INTDIR)\ww_natureseal.sbr"
	-@erase "$(INTDIR)\ww_naturewizard.obj"
	-@erase "$(INTDIR)\ww_naturewizard.sbr"
	-@erase "$(INTDIR)\ww_seal.obj"
	-@erase "$(INTDIR)\ww_seal.sbr"
	-@erase "$(INTDIR)\ww_seals.obj"
	-@erase "$(INTDIR)\ww_seals.sbr"
	-@erase "$(INTDIR)\ww_sentrycrystal.obj"
	-@erase "$(INTDIR)\ww_sentrycrystal.sbr"
	-@erase "$(INTDIR)\ww_skull.obj"
	-@erase "$(INTDIR)\ww_skull.sbr"
	-@erase "$(INTDIR)\ww_spotbolt.obj"
	-@erase "$(INTDIR)\ww_spotbolt.sbr"
	-@erase "$(INTDIR)\ww_staff.obj"
	-@erase "$(INTDIR)\ww_staff.sbr"
	-@erase "$(INTDIR)\ww_updraft.obj"
	-@erase "$(INTDIR)\ww_updraft.sbr"
	-@erase "$(INTDIR)\ww_whiteray.obj"
	-@erase "$(INTDIR)\ww_whiteray.sbr"
	-@erase "$(INTDIR)\ww_windseal.obj"
	-@erase "$(INTDIR)\ww_windseal.sbr"
	-@erase "$(INTDIR)\ww_windwizard.obj"
	-@erase "$(INTDIR)\ww_windwizard.sbr"
	-@erase "$(INTDIR)\ww_wizards.obj"
	-@erase "$(INTDIR)\ww_wizards.sbr"
	-@erase "$(INTDIR)\xen.obj"
	-@erase "$(INTDIR)\xen.sbr"
	-@erase "$(OUTDIR)\hl.bsc"
	-@erase "$(OUTDIR)\hl.map"
	-@erase "$(OUTDIR)\hl.pdb"
	-@erase "..\build\hl.dll"
	-@erase "..\build\hl.ilk"
	-@erase ".\hl.exp"
	-@erase ".\hl.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /ZI /Od /I ".\\" /I "..\\" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\game_shared" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /D "CLIENT_WEAPONS" /D "MUSIC" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\hl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hl.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ww_archmage.sbr" \
	"$(INTDIR)\ww_deathwizard.sbr" \
	"$(INTDIR)\ww_dragonwizard.sbr" \
	"$(INTDIR)\ww_earthwizard.sbr" \
	"$(INTDIR)\ww_firewizard.sbr" \
	"$(INTDIR)\ww_icewizard.sbr" \
	"$(INTDIR)\ww_lifewizard.sbr" \
	"$(INTDIR)\ww_lightningwizard.sbr" \
	"$(INTDIR)\ww_naturewizard.sbr" \
	"$(INTDIR)\ww_windwizard.sbr" \
	"$(INTDIR)\ww_wizards.sbr" \
	"$(INTDIR)\ww_deathseal.sbr" \
	"$(INTDIR)\ww_dragonseal.sbr" \
	"$(INTDIR)\ww_earthseal.sbr" \
	"$(INTDIR)\ww_fireseal.sbr" \
	"$(INTDIR)\ww_iceseal.sbr" \
	"$(INTDIR)\ww_lifeseal.sbr" \
	"$(INTDIR)\ww_lightningseal.sbr" \
	"$(INTDIR)\ww_natureseal.sbr" \
	"$(INTDIR)\ww_seals.sbr" \
	"$(INTDIR)\ww_windseal.sbr" \
	"$(INTDIR)\ww_deathray.sbr" \
	"$(INTDIR)\ww_doublemissile.sbr" \
	"$(INTDIR)\ww_fireball.sbr" \
	"$(INTDIR)\ww_flamelick.sbr" \
	"$(INTDIR)\ww_levitate.sbr" \
	"$(INTDIR)\ww_lightningbolt.sbr" \
	"$(INTDIR)\ww_missile.sbr" \
	"$(INTDIR)\ww_seal.sbr" \
	"$(INTDIR)\ww_spotbolt.sbr" \
	"$(INTDIR)\ww_staff.sbr" \
	"$(INTDIR)\ww_updraft.sbr" \
	"$(INTDIR)\ww_emitter.sbr" \
	"$(INTDIR)\ww_sentrycrystal.sbr" \
	"$(INTDIR)\ww_gamerules.sbr" \
	"$(INTDIR)\tf_baseents.sbr" \
	"$(INTDIR)\tf_ent.sbr" \
	"$(INTDIR)\tf_globalinfo.sbr" \
	"$(INTDIR)\tf_globals.sbr" \
	"$(INTDIR)\tf_info_detect.sbr" \
	"$(INTDIR)\tf_info_goal.sbr" \
	"$(INTDIR)\tf_info_teamspawn.sbr" \
	"$(INTDIR)\tf_info_timer.sbr" \
	"$(INTDIR)\tf_item_goal.sbr" \
	"$(INTDIR)\tf_teamcheck.sbr" \
	"$(INTDIR)\tf_teamset.sbr" \
	"$(INTDIR)\AI_BaseNPC_Schedule.sbr" \
	"$(INTDIR)\alias.sbr" \
	"$(INTDIR)\animating.sbr" \
	"$(INTDIR)\animation.sbr" \
	"$(INTDIR)\bmodels.sbr" \
	"$(INTDIR)\buttons.sbr" \
	"$(INTDIR)\cbase.sbr" \
	"$(INTDIR)\client.sbr" \
	"$(INTDIR)\combat.sbr" \
	"$(INTDIR)\controller.sbr" \
	"$(INTDIR)\defaultai.sbr" \
	"$(INTDIR)\doors.sbr" \
	"$(INTDIR)\effects.sbr" \
	"$(INTDIR)\explode.sbr" \
	"$(INTDIR)\flyingmonster.sbr" \
	"$(INTDIR)\func_break.sbr" \
	"$(INTDIR)\game.sbr" \
	"$(INTDIR)\gamerules.sbr" \
	"$(INTDIR)\genericmonster.sbr" \
	"$(INTDIR)\ggrenade.sbr" \
	"$(INTDIR)\globals.sbr" \
	"$(INTDIR)\h_ai.sbr" \
	"$(INTDIR)\h_battery.sbr" \
	"$(INTDIR)\h_cine.sbr" \
	"$(INTDIR)\h_cycler.sbr" \
	"$(INTDIR)\h_export.sbr" \
	"$(INTDIR)\healthkit.sbr" \
	"$(INTDIR)\items.sbr" \
	"$(INTDIR)\lights.sbr" \
	"$(INTDIR)\locus.sbr" \
	"$(INTDIR)\maprules.sbr" \
	"$(INTDIR)\monstermaker.sbr" \
	"$(INTDIR)\monsters.sbr" \
	"$(INTDIR)\monsterstate.sbr" \
	"$(INTDIR)\movewith.sbr" \
	"$(INTDIR)\multiplay_gamerules.sbr" \
	"$(INTDIR)\nodes.sbr" \
	"$(INTDIR)\observer.sbr" \
	"$(INTDIR)\pathcorner.sbr" \
	"$(INTDIR)\plane.sbr" \
	"$(INTDIR)\plats.sbr" \
	"$(INTDIR)\player.sbr" \
	"$(INTDIR)\playermonster.sbr" \
	"$(INTDIR)\pm_debug.sbr" \
	"$(INTDIR)\pm_math.sbr" \
	"$(INTDIR)\pm_shared.sbr" \
	"$(INTDIR)\scripted.sbr" \
	"$(INTDIR)\singleplay_gamerules.sbr" \
	"$(INTDIR)\skill.sbr" \
	"$(INTDIR)\sound.sbr" \
	"$(INTDIR)\soundent.sbr" \
	"$(INTDIR)\spectator.sbr" \
	"$(INTDIR)\squadmonster.sbr" \
	"$(INTDIR)\stats.sbr" \
	"$(INTDIR)\subs.sbr" \
	"$(INTDIR)\talkmonster.sbr" \
	"$(INTDIR)\teamplay_gamerules.sbr" \
	"$(INTDIR)\tempmonster.sbr" \
	"$(INTDIR)\triggers.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\voice_gamemgr.sbr" \
	"$(INTDIR)\weapons.sbr" \
	"$(INTDIR)\world.sbr" \
	"$(INTDIR)\xen.sbr" \
	"$(INTDIR)\ww_whiteray.sbr" \
	"$(INTDIR)\ww_forceblast.sbr" \
	"$(INTDIR)\ww_bearbite.sbr" \
	"$(INTDIR)\ww_skull.sbr" \
	"$(INTDIR)\ww_bird.sbr"

"$(OUTDIR)\hl.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib advapi32.lib /nologo /subsystem:windows /dll /incremental:yes /pdb:"$(OUTDIR)\hl.pdb" /map:"$(INTDIR)\hl.map" /debug /machine:I386 /def:".\hl.def" /out:"..\build\hl.dll" /implib:".\hl.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ww_archmage.obj" \
	"$(INTDIR)\ww_deathwizard.obj" \
	"$(INTDIR)\ww_dragonwizard.obj" \
	"$(INTDIR)\ww_earthwizard.obj" \
	"$(INTDIR)\ww_firewizard.obj" \
	"$(INTDIR)\ww_icewizard.obj" \
	"$(INTDIR)\ww_lifewizard.obj" \
	"$(INTDIR)\ww_lightningwizard.obj" \
	"$(INTDIR)\ww_naturewizard.obj" \
	"$(INTDIR)\ww_windwizard.obj" \
	"$(INTDIR)\ww_wizards.obj" \
	"$(INTDIR)\ww_deathseal.obj" \
	"$(INTDIR)\ww_dragonseal.obj" \
	"$(INTDIR)\ww_earthseal.obj" \
	"$(INTDIR)\ww_fireseal.obj" \
	"$(INTDIR)\ww_iceseal.obj" \
	"$(INTDIR)\ww_lifeseal.obj" \
	"$(INTDIR)\ww_lightningseal.obj" \
	"$(INTDIR)\ww_natureseal.obj" \
	"$(INTDIR)\ww_seals.obj" \
	"$(INTDIR)\ww_windseal.obj" \
	"$(INTDIR)\ww_deathray.obj" \
	"$(INTDIR)\ww_doublemissile.obj" \
	"$(INTDIR)\ww_fireball.obj" \
	"$(INTDIR)\ww_flamelick.obj" \
	"$(INTDIR)\ww_levitate.obj" \
	"$(INTDIR)\ww_lightningbolt.obj" \
	"$(INTDIR)\ww_missile.obj" \
	"$(INTDIR)\ww_seal.obj" \
	"$(INTDIR)\ww_spotbolt.obj" \
	"$(INTDIR)\ww_staff.obj" \
	"$(INTDIR)\ww_updraft.obj" \
	"$(INTDIR)\ww_emitter.obj" \
	"$(INTDIR)\ww_sentrycrystal.obj" \
	"$(INTDIR)\ww_gamerules.obj" \
	"$(INTDIR)\tf_baseents.obj" \
	"$(INTDIR)\tf_ent.obj" \
	"$(INTDIR)\tf_globalinfo.obj" \
	"$(INTDIR)\tf_globals.obj" \
	"$(INTDIR)\tf_info_detect.obj" \
	"$(INTDIR)\tf_info_goal.obj" \
	"$(INTDIR)\tf_info_teamspawn.obj" \
	"$(INTDIR)\tf_info_timer.obj" \
	"$(INTDIR)\tf_item_goal.obj" \
	"$(INTDIR)\tf_teamcheck.obj" \
	"$(INTDIR)\tf_teamset.obj" \
	"$(INTDIR)\AI_BaseNPC_Schedule.obj" \
	"$(INTDIR)\alias.obj" \
	"$(INTDIR)\animating.obj" \
	"$(INTDIR)\animation.obj" \
	"$(INTDIR)\bmodels.obj" \
	"$(INTDIR)\buttons.obj" \
	"$(INTDIR)\cbase.obj" \
	"$(INTDIR)\client.obj" \
	"$(INTDIR)\combat.obj" \
	"$(INTDIR)\controller.obj" \
	"$(INTDIR)\defaultai.obj" \
	"$(INTDIR)\doors.obj" \
	"$(INTDIR)\effects.obj" \
	"$(INTDIR)\explode.obj" \
	"$(INTDIR)\flyingmonster.obj" \
	"$(INTDIR)\func_break.obj" \
	"$(INTDIR)\game.obj" \
	"$(INTDIR)\gamerules.obj" \
	"$(INTDIR)\genericmonster.obj" \
	"$(INTDIR)\ggrenade.obj" \
	"$(INTDIR)\globals.obj" \
	"$(INTDIR)\h_ai.obj" \
	"$(INTDIR)\h_battery.obj" \
	"$(INTDIR)\h_cine.obj" \
	"$(INTDIR)\h_cycler.obj" \
	"$(INTDIR)\h_export.obj" \
	"$(INTDIR)\healthkit.obj" \
	"$(INTDIR)\items.obj" \
	"$(INTDIR)\lights.obj" \
	"$(INTDIR)\locus.obj" \
	"$(INTDIR)\maprules.obj" \
	"$(INTDIR)\monstermaker.obj" \
	"$(INTDIR)\monsters.obj" \
	"$(INTDIR)\monsterstate.obj" \
	"$(INTDIR)\movewith.obj" \
	"$(INTDIR)\multiplay_gamerules.obj" \
	"$(INTDIR)\nodes.obj" \
	"$(INTDIR)\observer.obj" \
	"$(INTDIR)\pathcorner.obj" \
	"$(INTDIR)\plane.obj" \
	"$(INTDIR)\plats.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playermonster.obj" \
	"$(INTDIR)\pm_debug.obj" \
	"$(INTDIR)\pm_math.obj" \
	"$(INTDIR)\pm_shared.obj" \
	"$(INTDIR)\scripted.obj" \
	"$(INTDIR)\singleplay_gamerules.obj" \
	"$(INTDIR)\skill.obj" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\soundent.obj" \
	"$(INTDIR)\spectator.obj" \
	"$(INTDIR)\squadmonster.obj" \
	"$(INTDIR)\stats.obj" \
	"$(INTDIR)\subs.obj" \
	"$(INTDIR)\talkmonster.obj" \
	"$(INTDIR)\teamplay_gamerules.obj" \
	"$(INTDIR)\tempmonster.obj" \
	"$(INTDIR)\triggers.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\voice_gamemgr.obj" \
	"$(INTDIR)\weapons.obj" \
	"$(INTDIR)\world.obj" \
	"$(INTDIR)\xen.obj" \
	"$(INTDIR)\ww_whiteray.obj" \
	"$(INTDIR)\ww_forceblast.obj" \
	"$(INTDIR)\ww_bearbite.obj" \
	"$(INTDIR)\ww_skull.obj" \
	"$(INTDIR)\ww_bird.obj" \
	"..\build\debug\client\client.lib"

"..\build\hl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\..\build\debug\server
# End Custom Macros

$(DS_POSTBUILD_DEP) : "cl_dll - Win32 Debug" "..\build\hl.dll" "$(OUTDIR)\hl.bsc"
   cd ..\
	cmd.exe /c update.bat
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ELSEIF  "$(CFG)" == "hl - Win32 Profile"

OUTDIR=.\..\build\profile\server
INTDIR=.\..\build\profile\server
# Begin Custom Macros
OutDir=.\..\build\profile\server
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\build\hl.dll" "$(OUTDIR)\hl.bsc"

!ELSE 

ALL : "..\build\hl.dll" "$(OUTDIR)\hl.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\AI_BaseNPC_Schedule.obj"
	-@erase "$(INTDIR)\AI_BaseNPC_Schedule.sbr"
	-@erase "$(INTDIR)\alias.obj"
	-@erase "$(INTDIR)\alias.sbr"
	-@erase "$(INTDIR)\animating.obj"
	-@erase "$(INTDIR)\animating.sbr"
	-@erase "$(INTDIR)\animation.obj"
	-@erase "$(INTDIR)\animation.sbr"
	-@erase "$(INTDIR)\bmodels.obj"
	-@erase "$(INTDIR)\bmodels.sbr"
	-@erase "$(INTDIR)\buttons.obj"
	-@erase "$(INTDIR)\buttons.sbr"
	-@erase "$(INTDIR)\cbase.obj"
	-@erase "$(INTDIR)\cbase.sbr"
	-@erase "$(INTDIR)\client.obj"
	-@erase "$(INTDIR)\client.sbr"
	-@erase "$(INTDIR)\combat.obj"
	-@erase "$(INTDIR)\combat.sbr"
	-@erase "$(INTDIR)\controller.obj"
	-@erase "$(INTDIR)\controller.sbr"
	-@erase "$(INTDIR)\defaultai.obj"
	-@erase "$(INTDIR)\defaultai.sbr"
	-@erase "$(INTDIR)\doors.obj"
	-@erase "$(INTDIR)\doors.sbr"
	-@erase "$(INTDIR)\effects.obj"
	-@erase "$(INTDIR)\effects.sbr"
	-@erase "$(INTDIR)\explode.obj"
	-@erase "$(INTDIR)\explode.sbr"
	-@erase "$(INTDIR)\flyingmonster.obj"
	-@erase "$(INTDIR)\flyingmonster.sbr"
	-@erase "$(INTDIR)\func_break.obj"
	-@erase "$(INTDIR)\func_break.sbr"
	-@erase "$(INTDIR)\game.obj"
	-@erase "$(INTDIR)\game.sbr"
	-@erase "$(INTDIR)\gamerules.obj"
	-@erase "$(INTDIR)\gamerules.sbr"
	-@erase "$(INTDIR)\genericmonster.obj"
	-@erase "$(INTDIR)\genericmonster.sbr"
	-@erase "$(INTDIR)\ggrenade.obj"
	-@erase "$(INTDIR)\ggrenade.sbr"
	-@erase "$(INTDIR)\globals.obj"
	-@erase "$(INTDIR)\globals.sbr"
	-@erase "$(INTDIR)\h_ai.obj"
	-@erase "$(INTDIR)\h_ai.sbr"
	-@erase "$(INTDIR)\h_battery.obj"
	-@erase "$(INTDIR)\h_battery.sbr"
	-@erase "$(INTDIR)\h_cine.obj"
	-@erase "$(INTDIR)\h_cine.sbr"
	-@erase "$(INTDIR)\h_cycler.obj"
	-@erase "$(INTDIR)\h_cycler.sbr"
	-@erase "$(INTDIR)\h_export.obj"
	-@erase "$(INTDIR)\h_export.sbr"
	-@erase "$(INTDIR)\healthkit.obj"
	-@erase "$(INTDIR)\healthkit.sbr"
	-@erase "$(INTDIR)\items.obj"
	-@erase "$(INTDIR)\items.sbr"
	-@erase "$(INTDIR)\lights.obj"
	-@erase "$(INTDIR)\lights.sbr"
	-@erase "$(INTDIR)\locus.obj"
	-@erase "$(INTDIR)\locus.sbr"
	-@erase "$(INTDIR)\maprules.obj"
	-@erase "$(INTDIR)\maprules.sbr"
	-@erase "$(INTDIR)\monstermaker.obj"
	-@erase "$(INTDIR)\monstermaker.sbr"
	-@erase "$(INTDIR)\monsters.obj"
	-@erase "$(INTDIR)\monsters.sbr"
	-@erase "$(INTDIR)\monsterstate.obj"
	-@erase "$(INTDIR)\monsterstate.sbr"
	-@erase "$(INTDIR)\movewith.obj"
	-@erase "$(INTDIR)\movewith.sbr"
	-@erase "$(INTDIR)\multiplay_gamerules.obj"
	-@erase "$(INTDIR)\multiplay_gamerules.sbr"
	-@erase "$(INTDIR)\nodes.obj"
	-@erase "$(INTDIR)\nodes.sbr"
	-@erase "$(INTDIR)\observer.obj"
	-@erase "$(INTDIR)\observer.sbr"
	-@erase "$(INTDIR)\pathcorner.obj"
	-@erase "$(INTDIR)\pathcorner.sbr"
	-@erase "$(INTDIR)\plane.obj"
	-@erase "$(INTDIR)\plane.sbr"
	-@erase "$(INTDIR)\plats.obj"
	-@erase "$(INTDIR)\plats.sbr"
	-@erase "$(INTDIR)\player.obj"
	-@erase "$(INTDIR)\player.sbr"
	-@erase "$(INTDIR)\playermonster.obj"
	-@erase "$(INTDIR)\playermonster.sbr"
	-@erase "$(INTDIR)\pm_debug.obj"
	-@erase "$(INTDIR)\pm_debug.sbr"
	-@erase "$(INTDIR)\pm_math.obj"
	-@erase "$(INTDIR)\pm_math.sbr"
	-@erase "$(INTDIR)\pm_shared.obj"
	-@erase "$(INTDIR)\pm_shared.sbr"
	-@erase "$(INTDIR)\scripted.obj"
	-@erase "$(INTDIR)\scripted.sbr"
	-@erase "$(INTDIR)\singleplay_gamerules.obj"
	-@erase "$(INTDIR)\singleplay_gamerules.sbr"
	-@erase "$(INTDIR)\skill.obj"
	-@erase "$(INTDIR)\skill.sbr"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\sound.sbr"
	-@erase "$(INTDIR)\soundent.obj"
	-@erase "$(INTDIR)\soundent.sbr"
	-@erase "$(INTDIR)\spectator.obj"
	-@erase "$(INTDIR)\spectator.sbr"
	-@erase "$(INTDIR)\squadmonster.obj"
	-@erase "$(INTDIR)\squadmonster.sbr"
	-@erase "$(INTDIR)\stats.obj"
	-@erase "$(INTDIR)\stats.sbr"
	-@erase "$(INTDIR)\subs.obj"
	-@erase "$(INTDIR)\subs.sbr"
	-@erase "$(INTDIR)\talkmonster.obj"
	-@erase "$(INTDIR)\talkmonster.sbr"
	-@erase "$(INTDIR)\teamplay_gamerules.obj"
	-@erase "$(INTDIR)\teamplay_gamerules.sbr"
	-@erase "$(INTDIR)\tempmonster.obj"
	-@erase "$(INTDIR)\tempmonster.sbr"
	-@erase "$(INTDIR)\tf_baseents.obj"
	-@erase "$(INTDIR)\tf_baseents.sbr"
	-@erase "$(INTDIR)\tf_ent.obj"
	-@erase "$(INTDIR)\tf_ent.sbr"
	-@erase "$(INTDIR)\tf_globalinfo.obj"
	-@erase "$(INTDIR)\tf_globalinfo.sbr"
	-@erase "$(INTDIR)\tf_globals.obj"
	-@erase "$(INTDIR)\tf_globals.sbr"
	-@erase "$(INTDIR)\tf_info_detect.obj"
	-@erase "$(INTDIR)\tf_info_detect.sbr"
	-@erase "$(INTDIR)\tf_info_goal.obj"
	-@erase "$(INTDIR)\tf_info_goal.sbr"
	-@erase "$(INTDIR)\tf_info_teamspawn.obj"
	-@erase "$(INTDIR)\tf_info_teamspawn.sbr"
	-@erase "$(INTDIR)\tf_info_timer.obj"
	-@erase "$(INTDIR)\tf_info_timer.sbr"
	-@erase "$(INTDIR)\tf_item_goal.obj"
	-@erase "$(INTDIR)\tf_item_goal.sbr"
	-@erase "$(INTDIR)\tf_teamcheck.obj"
	-@erase "$(INTDIR)\tf_teamcheck.sbr"
	-@erase "$(INTDIR)\tf_teamset.obj"
	-@erase "$(INTDIR)\tf_teamset.sbr"
	-@erase "$(INTDIR)\triggers.obj"
	-@erase "$(INTDIR)\triggers.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\voice_gamemgr.obj"
	-@erase "$(INTDIR)\voice_gamemgr.sbr"
	-@erase "$(INTDIR)\weapons.obj"
	-@erase "$(INTDIR)\weapons.sbr"
	-@erase "$(INTDIR)\world.obj"
	-@erase "$(INTDIR)\world.sbr"
	-@erase "$(INTDIR)\ww_archmage.obj"
	-@erase "$(INTDIR)\ww_archmage.sbr"
	-@erase "$(INTDIR)\ww_bearbite.obj"
	-@erase "$(INTDIR)\ww_bearbite.sbr"
	-@erase "$(INTDIR)\ww_bird.obj"
	-@erase "$(INTDIR)\ww_bird.sbr"
	-@erase "$(INTDIR)\ww_deathray.obj"
	-@erase "$(INTDIR)\ww_deathray.sbr"
	-@erase "$(INTDIR)\ww_deathseal.obj"
	-@erase "$(INTDIR)\ww_deathseal.sbr"
	-@erase "$(INTDIR)\ww_deathwizard.obj"
	-@erase "$(INTDIR)\ww_deathwizard.sbr"
	-@erase "$(INTDIR)\ww_doublemissile.obj"
	-@erase "$(INTDIR)\ww_doublemissile.sbr"
	-@erase "$(INTDIR)\ww_dragonseal.obj"
	-@erase "$(INTDIR)\ww_dragonseal.sbr"
	-@erase "$(INTDIR)\ww_dragonwizard.obj"
	-@erase "$(INTDIR)\ww_dragonwizard.sbr"
	-@erase "$(INTDIR)\ww_earthseal.obj"
	-@erase "$(INTDIR)\ww_earthseal.sbr"
	-@erase "$(INTDIR)\ww_earthwizard.obj"
	-@erase "$(INTDIR)\ww_earthwizard.sbr"
	-@erase "$(INTDIR)\ww_emitter.obj"
	-@erase "$(INTDIR)\ww_emitter.sbr"
	-@erase "$(INTDIR)\ww_fireball.obj"
	-@erase "$(INTDIR)\ww_fireball.sbr"
	-@erase "$(INTDIR)\ww_fireseal.obj"
	-@erase "$(INTDIR)\ww_fireseal.sbr"
	-@erase "$(INTDIR)\ww_firewizard.obj"
	-@erase "$(INTDIR)\ww_firewizard.sbr"
	-@erase "$(INTDIR)\ww_flamelick.obj"
	-@erase "$(INTDIR)\ww_flamelick.sbr"
	-@erase "$(INTDIR)\ww_forceblast.obj"
	-@erase "$(INTDIR)\ww_forceblast.sbr"
	-@erase "$(INTDIR)\ww_gamerules.obj"
	-@erase "$(INTDIR)\ww_gamerules.sbr"
	-@erase "$(INTDIR)\ww_iceseal.obj"
	-@erase "$(INTDIR)\ww_iceseal.sbr"
	-@erase "$(INTDIR)\ww_icewizard.obj"
	-@erase "$(INTDIR)\ww_icewizard.sbr"
	-@erase "$(INTDIR)\ww_levitate.obj"
	-@erase "$(INTDIR)\ww_levitate.sbr"
	-@erase "$(INTDIR)\ww_lifeseal.obj"
	-@erase "$(INTDIR)\ww_lifeseal.sbr"
	-@erase "$(INTDIR)\ww_lifewizard.obj"
	-@erase "$(INTDIR)\ww_lifewizard.sbr"
	-@erase "$(INTDIR)\ww_lightningbolt.obj"
	-@erase "$(INTDIR)\ww_lightningbolt.sbr"
	-@erase "$(INTDIR)\ww_lightningseal.obj"
	-@erase "$(INTDIR)\ww_lightningseal.sbr"
	-@erase "$(INTDIR)\ww_lightningwizard.obj"
	-@erase "$(INTDIR)\ww_lightningwizard.sbr"
	-@erase "$(INTDIR)\ww_missile.obj"
	-@erase "$(INTDIR)\ww_missile.sbr"
	-@erase "$(INTDIR)\ww_natureseal.obj"
	-@erase "$(INTDIR)\ww_natureseal.sbr"
	-@erase "$(INTDIR)\ww_naturewizard.obj"
	-@erase "$(INTDIR)\ww_naturewizard.sbr"
	-@erase "$(INTDIR)\ww_seal.obj"
	-@erase "$(INTDIR)\ww_seal.sbr"
	-@erase "$(INTDIR)\ww_seals.obj"
	-@erase "$(INTDIR)\ww_seals.sbr"
	-@erase "$(INTDIR)\ww_sentrycrystal.obj"
	-@erase "$(INTDIR)\ww_sentrycrystal.sbr"
	-@erase "$(INTDIR)\ww_skull.obj"
	-@erase "$(INTDIR)\ww_skull.sbr"
	-@erase "$(INTDIR)\ww_spotbolt.obj"
	-@erase "$(INTDIR)\ww_spotbolt.sbr"
	-@erase "$(INTDIR)\ww_staff.obj"
	-@erase "$(INTDIR)\ww_staff.sbr"
	-@erase "$(INTDIR)\ww_updraft.obj"
	-@erase "$(INTDIR)\ww_updraft.sbr"
	-@erase "$(INTDIR)\ww_whiteray.obj"
	-@erase "$(INTDIR)\ww_whiteray.sbr"
	-@erase "$(INTDIR)\ww_windseal.obj"
	-@erase "$(INTDIR)\ww_windseal.sbr"
	-@erase "$(INTDIR)\ww_windwizard.obj"
	-@erase "$(INTDIR)\ww_windwizard.sbr"
	-@erase "$(INTDIR)\ww_wizards.obj"
	-@erase "$(INTDIR)\ww_wizards.sbr"
	-@erase "$(INTDIR)\xen.obj"
	-@erase "$(INTDIR)\xen.sbr"
	-@erase "$(OUTDIR)\hl.bsc"
	-@erase "$(OUTDIR)\hl.exp"
	-@erase "$(OUTDIR)\hl.lib"
	-@erase "..\build\hl.dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /MT /W3 /Zi /O2 /I ".\\" /I "..\\" /I "..\dlls" /I "..\engine" /I "..\common" /I "..\pm_shared" /I "..\game_shared" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "QUIVER" /D "VOXEL" /D "QUAKE2" /D "VALVE_DLL" /D "CLIENT_WEAPONS" /D "MUSIC" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\hl.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hl.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ww_archmage.sbr" \
	"$(INTDIR)\ww_deathwizard.sbr" \
	"$(INTDIR)\ww_dragonwizard.sbr" \
	"$(INTDIR)\ww_earthwizard.sbr" \
	"$(INTDIR)\ww_firewizard.sbr" \
	"$(INTDIR)\ww_icewizard.sbr" \
	"$(INTDIR)\ww_lifewizard.sbr" \
	"$(INTDIR)\ww_lightningwizard.sbr" \
	"$(INTDIR)\ww_naturewizard.sbr" \
	"$(INTDIR)\ww_windwizard.sbr" \
	"$(INTDIR)\ww_wizards.sbr" \
	"$(INTDIR)\ww_deathseal.sbr" \
	"$(INTDIR)\ww_dragonseal.sbr" \
	"$(INTDIR)\ww_earthseal.sbr" \
	"$(INTDIR)\ww_fireseal.sbr" \
	"$(INTDIR)\ww_iceseal.sbr" \
	"$(INTDIR)\ww_lifeseal.sbr" \
	"$(INTDIR)\ww_lightningseal.sbr" \
	"$(INTDIR)\ww_natureseal.sbr" \
	"$(INTDIR)\ww_seals.sbr" \
	"$(INTDIR)\ww_windseal.sbr" \
	"$(INTDIR)\ww_deathray.sbr" \
	"$(INTDIR)\ww_doublemissile.sbr" \
	"$(INTDIR)\ww_fireball.sbr" \
	"$(INTDIR)\ww_flamelick.sbr" \
	"$(INTDIR)\ww_levitate.sbr" \
	"$(INTDIR)\ww_lightningbolt.sbr" \
	"$(INTDIR)\ww_missile.sbr" \
	"$(INTDIR)\ww_seal.sbr" \
	"$(INTDIR)\ww_spotbolt.sbr" \
	"$(INTDIR)\ww_staff.sbr" \
	"$(INTDIR)\ww_updraft.sbr" \
	"$(INTDIR)\ww_emitter.sbr" \
	"$(INTDIR)\ww_sentrycrystal.sbr" \
	"$(INTDIR)\ww_gamerules.sbr" \
	"$(INTDIR)\tf_baseents.sbr" \
	"$(INTDIR)\tf_ent.sbr" \
	"$(INTDIR)\tf_globalinfo.sbr" \
	"$(INTDIR)\tf_globals.sbr" \
	"$(INTDIR)\tf_info_detect.sbr" \
	"$(INTDIR)\tf_info_goal.sbr" \
	"$(INTDIR)\tf_info_teamspawn.sbr" \
	"$(INTDIR)\tf_info_timer.sbr" \
	"$(INTDIR)\tf_item_goal.sbr" \
	"$(INTDIR)\tf_teamcheck.sbr" \
	"$(INTDIR)\tf_teamset.sbr" \
	"$(INTDIR)\AI_BaseNPC_Schedule.sbr" \
	"$(INTDIR)\alias.sbr" \
	"$(INTDIR)\animating.sbr" \
	"$(INTDIR)\animation.sbr" \
	"$(INTDIR)\bmodels.sbr" \
	"$(INTDIR)\buttons.sbr" \
	"$(INTDIR)\cbase.sbr" \
	"$(INTDIR)\client.sbr" \
	"$(INTDIR)\combat.sbr" \
	"$(INTDIR)\controller.sbr" \
	"$(INTDIR)\defaultai.sbr" \
	"$(INTDIR)\doors.sbr" \
	"$(INTDIR)\effects.sbr" \
	"$(INTDIR)\explode.sbr" \
	"$(INTDIR)\flyingmonster.sbr" \
	"$(INTDIR)\func_break.sbr" \
	"$(INTDIR)\game.sbr" \
	"$(INTDIR)\gamerules.sbr" \
	"$(INTDIR)\genericmonster.sbr" \
	"$(INTDIR)\ggrenade.sbr" \
	"$(INTDIR)\globals.sbr" \
	"$(INTDIR)\h_ai.sbr" \
	"$(INTDIR)\h_battery.sbr" \
	"$(INTDIR)\h_cine.sbr" \
	"$(INTDIR)\h_cycler.sbr" \
	"$(INTDIR)\h_export.sbr" \
	"$(INTDIR)\healthkit.sbr" \
	"$(INTDIR)\items.sbr" \
	"$(INTDIR)\lights.sbr" \
	"$(INTDIR)\locus.sbr" \
	"$(INTDIR)\maprules.sbr" \
	"$(INTDIR)\monstermaker.sbr" \
	"$(INTDIR)\monsters.sbr" \
	"$(INTDIR)\monsterstate.sbr" \
	"$(INTDIR)\movewith.sbr" \
	"$(INTDIR)\multiplay_gamerules.sbr" \
	"$(INTDIR)\nodes.sbr" \
	"$(INTDIR)\observer.sbr" \
	"$(INTDIR)\pathcorner.sbr" \
	"$(INTDIR)\plane.sbr" \
	"$(INTDIR)\plats.sbr" \
	"$(INTDIR)\player.sbr" \
	"$(INTDIR)\playermonster.sbr" \
	"$(INTDIR)\pm_debug.sbr" \
	"$(INTDIR)\pm_math.sbr" \
	"$(INTDIR)\pm_shared.sbr" \
	"$(INTDIR)\scripted.sbr" \
	"$(INTDIR)\singleplay_gamerules.sbr" \
	"$(INTDIR)\skill.sbr" \
	"$(INTDIR)\sound.sbr" \
	"$(INTDIR)\soundent.sbr" \
	"$(INTDIR)\spectator.sbr" \
	"$(INTDIR)\squadmonster.sbr" \
	"$(INTDIR)\stats.sbr" \
	"$(INTDIR)\subs.sbr" \
	"$(INTDIR)\talkmonster.sbr" \
	"$(INTDIR)\teamplay_gamerules.sbr" \
	"$(INTDIR)\tempmonster.sbr" \
	"$(INTDIR)\triggers.sbr" \
	"$(INTDIR)\util.sbr" \
	"$(INTDIR)\voice_gamemgr.sbr" \
	"$(INTDIR)\weapons.sbr" \
	"$(INTDIR)\world.sbr" \
	"$(INTDIR)\xen.sbr" \
	"$(INTDIR)\ww_whiteray.sbr" \
	"$(INTDIR)\ww_forceblast.sbr" \
	"$(INTDIR)\ww_bearbite.sbr" \
	"$(INTDIR)\ww_skull.sbr" \
	"$(INTDIR)\ww_bird.sbr"

"$(OUTDIR)\hl.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=user32.lib advapi32.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /def:".\hl.def" /out:"..\build\hl.dll" /implib:"$(OUTDIR)\hl.lib" 
LINK32_OBJS= \
	"$(INTDIR)\ww_archmage.obj" \
	"$(INTDIR)\ww_deathwizard.obj" \
	"$(INTDIR)\ww_dragonwizard.obj" \
	"$(INTDIR)\ww_earthwizard.obj" \
	"$(INTDIR)\ww_firewizard.obj" \
	"$(INTDIR)\ww_icewizard.obj" \
	"$(INTDIR)\ww_lifewizard.obj" \
	"$(INTDIR)\ww_lightningwizard.obj" \
	"$(INTDIR)\ww_naturewizard.obj" \
	"$(INTDIR)\ww_windwizard.obj" \
	"$(INTDIR)\ww_wizards.obj" \
	"$(INTDIR)\ww_deathseal.obj" \
	"$(INTDIR)\ww_dragonseal.obj" \
	"$(INTDIR)\ww_earthseal.obj" \
	"$(INTDIR)\ww_fireseal.obj" \
	"$(INTDIR)\ww_iceseal.obj" \
	"$(INTDIR)\ww_lifeseal.obj" \
	"$(INTDIR)\ww_lightningseal.obj" \
	"$(INTDIR)\ww_natureseal.obj" \
	"$(INTDIR)\ww_seals.obj" \
	"$(INTDIR)\ww_windseal.obj" \
	"$(INTDIR)\ww_deathray.obj" \
	"$(INTDIR)\ww_doublemissile.obj" \
	"$(INTDIR)\ww_fireball.obj" \
	"$(INTDIR)\ww_flamelick.obj" \
	"$(INTDIR)\ww_levitate.obj" \
	"$(INTDIR)\ww_lightningbolt.obj" \
	"$(INTDIR)\ww_missile.obj" \
	"$(INTDIR)\ww_seal.obj" \
	"$(INTDIR)\ww_spotbolt.obj" \
	"$(INTDIR)\ww_staff.obj" \
	"$(INTDIR)\ww_updraft.obj" \
	"$(INTDIR)\ww_emitter.obj" \
	"$(INTDIR)\ww_sentrycrystal.obj" \
	"$(INTDIR)\ww_gamerules.obj" \
	"$(INTDIR)\tf_baseents.obj" \
	"$(INTDIR)\tf_ent.obj" \
	"$(INTDIR)\tf_globalinfo.obj" \
	"$(INTDIR)\tf_globals.obj" \
	"$(INTDIR)\tf_info_detect.obj" \
	"$(INTDIR)\tf_info_goal.obj" \
	"$(INTDIR)\tf_info_teamspawn.obj" \
	"$(INTDIR)\tf_info_timer.obj" \
	"$(INTDIR)\tf_item_goal.obj" \
	"$(INTDIR)\tf_teamcheck.obj" \
	"$(INTDIR)\tf_teamset.obj" \
	"$(INTDIR)\AI_BaseNPC_Schedule.obj" \
	"$(INTDIR)\alias.obj" \
	"$(INTDIR)\animating.obj" \
	"$(INTDIR)\animation.obj" \
	"$(INTDIR)\bmodels.obj" \
	"$(INTDIR)\buttons.obj" \
	"$(INTDIR)\cbase.obj" \
	"$(INTDIR)\client.obj" \
	"$(INTDIR)\combat.obj" \
	"$(INTDIR)\controller.obj" \
	"$(INTDIR)\defaultai.obj" \
	"$(INTDIR)\doors.obj" \
	"$(INTDIR)\effects.obj" \
	"$(INTDIR)\explode.obj" \
	"$(INTDIR)\flyingmonster.obj" \
	"$(INTDIR)\func_break.obj" \
	"$(INTDIR)\game.obj" \
	"$(INTDIR)\gamerules.obj" \
	"$(INTDIR)\genericmonster.obj" \
	"$(INTDIR)\ggrenade.obj" \
	"$(INTDIR)\globals.obj" \
	"$(INTDIR)\h_ai.obj" \
	"$(INTDIR)\h_battery.obj" \
	"$(INTDIR)\h_cine.obj" \
	"$(INTDIR)\h_cycler.obj" \
	"$(INTDIR)\h_export.obj" \
	"$(INTDIR)\healthkit.obj" \
	"$(INTDIR)\items.obj" \
	"$(INTDIR)\lights.obj" \
	"$(INTDIR)\locus.obj" \
	"$(INTDIR)\maprules.obj" \
	"$(INTDIR)\monstermaker.obj" \
	"$(INTDIR)\monsters.obj" \
	"$(INTDIR)\monsterstate.obj" \
	"$(INTDIR)\movewith.obj" \
	"$(INTDIR)\multiplay_gamerules.obj" \
	"$(INTDIR)\nodes.obj" \
	"$(INTDIR)\observer.obj" \
	"$(INTDIR)\pathcorner.obj" \
	"$(INTDIR)\plane.obj" \
	"$(INTDIR)\plats.obj" \
	"$(INTDIR)\player.obj" \
	"$(INTDIR)\playermonster.obj" \
	"$(INTDIR)\pm_debug.obj" \
	"$(INTDIR)\pm_math.obj" \
	"$(INTDIR)\pm_shared.obj" \
	"$(INTDIR)\scripted.obj" \
	"$(INTDIR)\singleplay_gamerules.obj" \
	"$(INTDIR)\skill.obj" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\soundent.obj" \
	"$(INTDIR)\spectator.obj" \
	"$(INTDIR)\squadmonster.obj" \
	"$(INTDIR)\stats.obj" \
	"$(INTDIR)\subs.obj" \
	"$(INTDIR)\talkmonster.obj" \
	"$(INTDIR)\teamplay_gamerules.obj" \
	"$(INTDIR)\tempmonster.obj" \
	"$(INTDIR)\triggers.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\voice_gamemgr.obj" \
	"$(INTDIR)\weapons.obj" \
	"$(INTDIR)\world.obj" \
	"$(INTDIR)\xen.obj" \
	"$(INTDIR)\ww_whiteray.obj" \
	"$(INTDIR)\ww_forceblast.obj" \
	"$(INTDIR)\ww_bearbite.obj" \
	"$(INTDIR)\ww_skull.obj" \
	"$(INTDIR)\ww_bird.obj"

"..\build\hl.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

SOURCE="$(InputPath)"
DS_POSTBUILD_DEP=$(INTDIR)\postbld.dep

ALL : $(DS_POSTBUILD_DEP)

# Begin Custom Macros
OutDir=.\..\build\profile\server
# End Custom Macros

$(DS_POSTBUILD_DEP) : "..\build\hl.dll" "$(OUTDIR)\hl.bsc"
   cd ..\
	cmd.exe /c update.bat
	echo Helper for Post-build step > "$(DS_POSTBUILD_DEP)"

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("hl.dep")
!INCLUDE "hl.dep"
!ELSE 
!MESSAGE Warning: cannot find "hl.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "hl - Win32 Release" || "$(CFG)" == "hl - Win32 Debug" || "$(CFG)" == "hl - Win32 Profile"
SOURCE=.\ww\wizards\ww_archmage.cpp

"$(INTDIR)\ww_archmage.obj"	"$(INTDIR)\ww_archmage.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_deathwizard.cpp

"$(INTDIR)\ww_deathwizard.obj"	"$(INTDIR)\ww_deathwizard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_dragonwizard.cpp

"$(INTDIR)\ww_dragonwizard.obj"	"$(INTDIR)\ww_dragonwizard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_earthwizard.cpp

"$(INTDIR)\ww_earthwizard.obj"	"$(INTDIR)\ww_earthwizard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_firewizard.cpp

"$(INTDIR)\ww_firewizard.obj"	"$(INTDIR)\ww_firewizard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_icewizard.cpp

"$(INTDIR)\ww_icewizard.obj"	"$(INTDIR)\ww_icewizard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_lifewizard.cpp

"$(INTDIR)\ww_lifewizard.obj"	"$(INTDIR)\ww_lifewizard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_lightningwizard.cpp

"$(INTDIR)\ww_lightningwizard.obj"	"$(INTDIR)\ww_lightningwizard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_naturewizard.cpp

"$(INTDIR)\ww_naturewizard.obj"	"$(INTDIR)\ww_naturewizard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_windwizard.cpp

"$(INTDIR)\ww_windwizard.obj"	"$(INTDIR)\ww_windwizard.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\wizards\ww_wizards.cpp

"$(INTDIR)\ww_wizards.obj"	"$(INTDIR)\ww_wizards.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_deathseal.cpp

"$(INTDIR)\ww_deathseal.obj"	"$(INTDIR)\ww_deathseal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_dragonseal.cpp

"$(INTDIR)\ww_dragonseal.obj"	"$(INTDIR)\ww_dragonseal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_earthseal.cpp

"$(INTDIR)\ww_earthseal.obj"	"$(INTDIR)\ww_earthseal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_fireseal.cpp

"$(INTDIR)\ww_fireseal.obj"	"$(INTDIR)\ww_fireseal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_iceseal.cpp

"$(INTDIR)\ww_iceseal.obj"	"$(INTDIR)\ww_iceseal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_lifeseal.cpp

"$(INTDIR)\ww_lifeseal.obj"	"$(INTDIR)\ww_lifeseal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_lightningseal.cpp

"$(INTDIR)\ww_lightningseal.obj"	"$(INTDIR)\ww_lightningseal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_natureseal.cpp

"$(INTDIR)\ww_natureseal.obj"	"$(INTDIR)\ww_natureseal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_seals.cpp

"$(INTDIR)\ww_seals.obj"	"$(INTDIR)\ww_seals.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\seals\ww_windseal.cpp

"$(INTDIR)\ww_windseal.obj"	"$(INTDIR)\ww_windseal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_bearbite.cpp

"$(INTDIR)\ww_bearbite.obj"	"$(INTDIR)\ww_bearbite.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_bird.cpp

"$(INTDIR)\ww_bird.obj"	"$(INTDIR)\ww_bird.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_deathray.cpp

"$(INTDIR)\ww_deathray.obj"	"$(INTDIR)\ww_deathray.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_doublemissile.cpp

"$(INTDIR)\ww_doublemissile.obj"	"$(INTDIR)\ww_doublemissile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_fireball.cpp

"$(INTDIR)\ww_fireball.obj"	"$(INTDIR)\ww_fireball.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_flamelick.cpp

"$(INTDIR)\ww_flamelick.obj"	"$(INTDIR)\ww_flamelick.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_forceblast.cpp

"$(INTDIR)\ww_forceblast.obj"	"$(INTDIR)\ww_forceblast.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_levitate.cpp

"$(INTDIR)\ww_levitate.obj"	"$(INTDIR)\ww_levitate.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_lightningbolt.cpp

"$(INTDIR)\ww_lightningbolt.obj"	"$(INTDIR)\ww_lightningbolt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_missile.cpp

"$(INTDIR)\ww_missile.obj"	"$(INTDIR)\ww_missile.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_seal.cpp

"$(INTDIR)\ww_seal.obj"	"$(INTDIR)\ww_seal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_skull.cpp

"$(INTDIR)\ww_skull.obj"	"$(INTDIR)\ww_skull.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_spotbolt.cpp

"$(INTDIR)\ww_spotbolt.obj"	"$(INTDIR)\ww_spotbolt.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_staff.cpp

"$(INTDIR)\ww_staff.obj"	"$(INTDIR)\ww_staff.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_updraft.cpp

"$(INTDIR)\ww_updraft.obj"	"$(INTDIR)\ww_updraft.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\ww_shared\weapons\ww_whiteray.cpp

"$(INTDIR)\ww_whiteray.obj"	"$(INTDIR)\ww_whiteray.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\ents\ww_emitter.cpp

"$(INTDIR)\ww_emitter.obj"	"$(INTDIR)\ww_emitter.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\ents\ww_goblin.cpp
SOURCE=.\ww\ents\ww_sentrycrystal.cpp

"$(INTDIR)\ww_sentrycrystal.obj"	"$(INTDIR)\ww_sentrycrystal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\ww\ents\ww_villager.cpp
SOURCE=.\ww\ww_gamerules.cpp

"$(INTDIR)\ww_gamerules.obj"	"$(INTDIR)\ww_gamerules.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_baseents.cpp

"$(INTDIR)\tf_baseents.obj"	"$(INTDIR)\tf_baseents.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_ent.cpp

"$(INTDIR)\tf_ent.obj"	"$(INTDIR)\tf_ent.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_globalinfo.cpp

"$(INTDIR)\tf_globalinfo.obj"	"$(INTDIR)\tf_globalinfo.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_globals.cpp

"$(INTDIR)\tf_globals.obj"	"$(INTDIR)\tf_globals.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_info_detect.cpp

"$(INTDIR)\tf_info_detect.obj"	"$(INTDIR)\tf_info_detect.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_info_goal.cpp

"$(INTDIR)\tf_info_goal.obj"	"$(INTDIR)\tf_info_goal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_info_teamspawn.cpp

"$(INTDIR)\tf_info_teamspawn.obj"	"$(INTDIR)\tf_info_teamspawn.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_info_timer.cpp

"$(INTDIR)\tf_info_timer.obj"	"$(INTDIR)\tf_info_timer.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_item_goal.cpp

"$(INTDIR)\tf_item_goal.obj"	"$(INTDIR)\tf_item_goal.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_teamcheck.cpp

"$(INTDIR)\tf_teamcheck.obj"	"$(INTDIR)\tf_teamcheck.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\tfents\tf_teamset.cpp

"$(INTDIR)\tf_teamset.obj"	"$(INTDIR)\tf_teamset.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\AI_BaseNPC_Schedule.cpp

"$(INTDIR)\AI_BaseNPC_Schedule.obj"	"$(INTDIR)\AI_BaseNPC_Schedule.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\alias.cpp

"$(INTDIR)\alias.obj"	"$(INTDIR)\alias.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\animating.cpp

"$(INTDIR)\animating.obj"	"$(INTDIR)\animating.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\animation.cpp

"$(INTDIR)\animation.obj"	"$(INTDIR)\animation.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\bmodels.cpp

"$(INTDIR)\bmodels.obj"	"$(INTDIR)\bmodels.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\buttons.cpp

"$(INTDIR)\buttons.obj"	"$(INTDIR)\buttons.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\cbase.cpp

"$(INTDIR)\cbase.obj"	"$(INTDIR)\cbase.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\client.cpp

"$(INTDIR)\client.obj"	"$(INTDIR)\client.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\combat.cpp

"$(INTDIR)\combat.obj"	"$(INTDIR)\combat.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\controller.cpp

"$(INTDIR)\controller.obj"	"$(INTDIR)\controller.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\defaultai.cpp

"$(INTDIR)\defaultai.obj"	"$(INTDIR)\defaultai.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\doors.cpp

"$(INTDIR)\doors.obj"	"$(INTDIR)\doors.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\effects.cpp

"$(INTDIR)\effects.obj"	"$(INTDIR)\effects.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\explode.cpp

"$(INTDIR)\explode.obj"	"$(INTDIR)\explode.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\flyingmonster.cpp

"$(INTDIR)\flyingmonster.obj"	"$(INTDIR)\flyingmonster.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\func_break.cpp

"$(INTDIR)\func_break.obj"	"$(INTDIR)\func_break.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\game.cpp

"$(INTDIR)\game.obj"	"$(INTDIR)\game.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\gamerules.cpp

"$(INTDIR)\gamerules.obj"	"$(INTDIR)\gamerules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\genericmonster.cpp

"$(INTDIR)\genericmonster.obj"	"$(INTDIR)\genericmonster.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ggrenade.cpp

"$(INTDIR)\ggrenade.obj"	"$(INTDIR)\ggrenade.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\globals.cpp

"$(INTDIR)\globals.obj"	"$(INTDIR)\globals.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\h_ai.cpp

"$(INTDIR)\h_ai.obj"	"$(INTDIR)\h_ai.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\h_battery.cpp

"$(INTDIR)\h_battery.obj"	"$(INTDIR)\h_battery.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\h_cine.cpp

"$(INTDIR)\h_cine.obj"	"$(INTDIR)\h_cine.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\h_cycler.cpp

"$(INTDIR)\h_cycler.obj"	"$(INTDIR)\h_cycler.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\h_export.cpp

"$(INTDIR)\h_export.obj"	"$(INTDIR)\h_export.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\healthkit.cpp

"$(INTDIR)\healthkit.obj"	"$(INTDIR)\healthkit.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\items.cpp

"$(INTDIR)\items.obj"	"$(INTDIR)\items.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\lights.cpp

"$(INTDIR)\lights.obj"	"$(INTDIR)\lights.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\locus.cpp

"$(INTDIR)\locus.obj"	"$(INTDIR)\locus.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\maprules.cpp

"$(INTDIR)\maprules.obj"	"$(INTDIR)\maprules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\monstermaker.cpp

"$(INTDIR)\monstermaker.obj"	"$(INTDIR)\monstermaker.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\monsters.cpp

"$(INTDIR)\monsters.obj"	"$(INTDIR)\monsters.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\monsterstate.cpp

"$(INTDIR)\monsterstate.obj"	"$(INTDIR)\monsterstate.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\movewith.cpp

"$(INTDIR)\movewith.obj"	"$(INTDIR)\movewith.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\multiplay_gamerules.cpp

"$(INTDIR)\multiplay_gamerules.obj"	"$(INTDIR)\multiplay_gamerules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\nodes.cpp

"$(INTDIR)\nodes.obj"	"$(INTDIR)\nodes.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\observer.cpp

"$(INTDIR)\observer.obj"	"$(INTDIR)\observer.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\pathcorner.cpp

"$(INTDIR)\pathcorner.obj"	"$(INTDIR)\pathcorner.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\plane.cpp

"$(INTDIR)\plane.obj"	"$(INTDIR)\plane.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\plats.cpp

"$(INTDIR)\plats.obj"	"$(INTDIR)\plats.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\player.cpp

"$(INTDIR)\player.obj"	"$(INTDIR)\player.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\playermonster.cpp

"$(INTDIR)\playermonster.obj"	"$(INTDIR)\playermonster.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\pm_shared\pm_debug.c

"$(INTDIR)\pm_debug.obj"	"$(INTDIR)\pm_debug.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\pm_shared\pm_math.c

"$(INTDIR)\pm_math.obj"	"$(INTDIR)\pm_math.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\pm_shared\pm_shared.c

"$(INTDIR)\pm_shared.obj"	"$(INTDIR)\pm_shared.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\scripted.cpp

"$(INTDIR)\scripted.obj"	"$(INTDIR)\scripted.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\singleplay_gamerules.cpp

"$(INTDIR)\singleplay_gamerules.obj"	"$(INTDIR)\singleplay_gamerules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\skill.cpp

"$(INTDIR)\skill.obj"	"$(INTDIR)\skill.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sound.cpp

"$(INTDIR)\sound.obj"	"$(INTDIR)\sound.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\soundent.cpp

"$(INTDIR)\soundent.obj"	"$(INTDIR)\soundent.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\spectator.cpp

"$(INTDIR)\spectator.obj"	"$(INTDIR)\spectator.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\squadmonster.cpp

"$(INTDIR)\squadmonster.obj"	"$(INTDIR)\squadmonster.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\stats.cpp

"$(INTDIR)\stats.obj"	"$(INTDIR)\stats.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\subs.cpp

"$(INTDIR)\subs.obj"	"$(INTDIR)\subs.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\talkmonster.cpp

"$(INTDIR)\talkmonster.obj"	"$(INTDIR)\talkmonster.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\teamplay_gamerules.cpp

"$(INTDIR)\teamplay_gamerules.obj"	"$(INTDIR)\teamplay_gamerules.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\tempmonster.cpp

"$(INTDIR)\tempmonster.obj"	"$(INTDIR)\tempmonster.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\triggers.cpp

"$(INTDIR)\triggers.obj"	"$(INTDIR)\triggers.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\util.cpp

"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=..\game_shared\voice_gamemgr.cpp

"$(INTDIR)\voice_gamemgr.obj"	"$(INTDIR)\voice_gamemgr.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\weapons.cpp

"$(INTDIR)\weapons.obj"	"$(INTDIR)\weapons.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\world.cpp

"$(INTDIR)\world.obj"	"$(INTDIR)\world.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\xen.cpp

"$(INTDIR)\xen.obj"	"$(INTDIR)\xen.sbr" : $(SOURCE) "$(INTDIR)"


!IF  "$(CFG)" == "hl - Win32 Release"

"cl_dll - Win32 Release" : 
   cd "\Documents\Projects\Esper\source\cl_dll"
   $(MAKE) /$(MAKEFLAGS) /F .\cl_dll.mak CFG="cl_dll - Win32 Release" 
   cd "..\dlls"

"cl_dll - Win32 ReleaseCLEAN" : 
   cd "\Documents\Projects\Esper\source\cl_dll"
   $(MAKE) /$(MAKEFLAGS) /F .\cl_dll.mak CFG="cl_dll - Win32 Release" RECURSE=1 CLEAN 
   cd "..\dlls"

!ELSEIF  "$(CFG)" == "hl - Win32 Debug"

"cl_dll - Win32 Debug" : 
   cd "\Documents\Projects\Esper\source\cl_dll"
   $(MAKE) /$(MAKEFLAGS) /F .\cl_dll.mak CFG="cl_dll - Win32 Debug" 
   cd "..\dlls"

"cl_dll - Win32 DebugCLEAN" : 
   cd "\Documents\Projects\Esper\source\cl_dll"
   $(MAKE) /$(MAKEFLAGS) /F .\cl_dll.mak CFG="cl_dll - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\dlls"

!ELSEIF  "$(CFG)" == "hl - Win32 Profile"

!ENDIF 


!ENDIF 


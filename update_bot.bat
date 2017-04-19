@echo off
call settings.bat

echo Updating Bot for %WW_BUILD%...

set SRC=%WW_SOURCEPATH%\build
set DEST=%WW_STEAMPATH%\SteamApps\%WW_STEAMUSER%\half-life\%WW_DIR%

IF EXIST "%SRC%\HPB_bot_mm.dll" (
	copy "%SRC%\HPB_bot_mm.dll" "%DEST%\addons\HPB_bot_mm\dlls\HPB_bot_mm.dll" /y >nul
	echo copied "%SRC%\HPB_bot_mm.dll" to "%DEST%\addons\HPB_bot_mm\dlls\HPB_bot_mm.dll"
) ELSE (
	echo couldn't copy addons\HPB_bot_mm\dlls\HPB_bot_mm.dl
)
echo.
echo Done.
if %WW_PAUSE%==1 pause

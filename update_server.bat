@echo off
call settings.bat

echo Updating Server for %WW_BUILD%...

set SRC=%WW_SOURCEPATH%\build
set DEST=%WW_STEAMPATH%\SteamApps\%WW_STEAMUSER%\half-life\%WW_DIR%

IF EXIST "%SRC%\hl.dll" (
	copy "%SRC%\hl.dll" "%DEST%\dlls\wizardwars.dll" /y >nul
	echo copied "%SRC%\hl.dll" to "%DEST%\dlls\wizardwars.dll"
) ELSE (
	IF EXIST "%SRC%\wizardwars.dll" (
	copy "%SRC%\wizardwars.dll" "%DEST%\dlls\wizardwars.dll" /y >nul
	echo copied "%SRC%\wizardwars.dll" to "%DEST%\dlls\wizardwars.dll"
	) ELSE (
		IF EXIST "%SRC%\wizwars.dll" (
			copy "%SRC%\wizwars.dll" "%DEST%\dlls\wizardwars.dll" /y >nul
			echo copied "%SRC%\wizwars.dll" to "%DEST%\dlls\wizardwars.dll"
		) ELSE (
			echo couldn't copy dlls\wizardwars.dll
		)
	)
)
echo.
echo Done.
if %WW_PAUSE%==1 pause

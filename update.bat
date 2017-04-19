@echo off
call settings.bat

echo Updating %WW_BUILD%...

set SRC=%WW_SOURCEPATH%\build
set DEST=%WW_STEAMPATH%\SteamApps\%WW_STEAMUSER%\half-life\%WW_DIR%

IF EXIST "%SRC%\client.dll" (
copy "%SRC%\client.dll" "%DEST%\cl_dlls\client.dll" /y >nul
echo copied "%SRC%\client.dll" to "%DEST%\cl_dlls\client.dll"
) ELSE (
echo couldn't copy cl_dlls\client.dll
)

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

@echo off
call settings.bat

echo Updating Cleint for %WW_BUILD%...

set SRC=%WW_SOURCEPATH%\build
set DEST=%WW_STEAMPATH%\SteamApps\%WW_STEAMUSER%\half-life\%WW_DIR%

IF EXIST "%SRC%\client.dll" (
copy "%SRC%\client.dll" "%DEST%\cl_dlls\client.dll" /y >nul
echo copied "%SRC%\client.dll" to "%DEST%\cl_dlls\client.dll"
) ELSE (
echo couldn't copy cl_dlls\client.dll
)


echo.
echo Done.
if %WW_PAUSE%==1 pause

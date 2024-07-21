set config=%1
set platform=%2
call .\scripts\win-init.bat
make config=%config%_%platform%
echo F | xcopy /y .\daedalus-core\src\build\daedalus-core\bin\%platform%\%config%\Daedalus-Core.dll .\build\daedalus-entropia\bin\%platform%\%config%\Daedalus-Core.dll
if %platform%==run .\build\daedalus-entropia\bin\run\%config%\Daedalus-Entropia.exe .\main.ent

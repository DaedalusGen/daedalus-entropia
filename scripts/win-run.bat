call .\scripts\win-init.bat
make
echo F | xcopy /y .\daedalus-core\src\build\daedalus-core\bin\run\Daedalus-Core.dll .\build\daedalus-entropia\bin\run\Daedalus-Core.dll
.\build\daedalus-entropia\bin\run\Daedalus-Entropia.exe .\main.t

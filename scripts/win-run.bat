call .\scripts\win-init.bat
make
echo F | xcopy /y .\daedalus-core\src\build\daedalus-core\bin\run\Daedalus-Core.dll .\build\daedalus-tlang\bin\run\Daedalus-Core.dll
.\build\daedalus-tlang\bin\run\Daedalus-Tlang.exe
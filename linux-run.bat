premake5 --file=.\build-tlang.lua gmake2
make
cp .\daedalus-core\src\build\daedalus-core\bin\run\Daedalus-Core.dll .\build\daedalus-tlang\bin\run\Daedalus-Core.dll
.\build\daedalus-tlang\bin\run\Daedalus-Tlang.exe
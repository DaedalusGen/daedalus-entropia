sh ./scripts/linux-init.sh
make
cp ./daedalus-core/src/build/daedalus-core/bin/run/Daedalus-Core.dll ./build/daedalus-tlang/bin/run/Daedalus-Core.dll
./build/daedalus-tlang/bin/run/Daedalus-Tlang.exe ./main.t
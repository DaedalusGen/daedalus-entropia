sh ./scripts/linux-init.sh
make $1_$2
cp ./daedalus-core/src/build/daedalus-core/bin/$2/$(echo $1 | tr 'a-z' 'A-Z')/Daedalus-Core.dll ./build/daedalus-entropia/bin/$2/$(echo $1 | tr 'a-z' 'A-Z')/Daedalus-Core.dll
if [ "$2"=="run" ] then
    ./build/daedalus-entropia/bin/run/$(echo $1 | tr 'a-z' 'A-Z')/Daedalus-Entropia.exe ./main.t
fi

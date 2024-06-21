# Daedalus Tlang

Daedalus Tlang is a test language for the Daedalus core.

To use it, follow these steps :

## Run with main.cpp

If you wish to run Tlang with the given main file, you can run either the `win-run.bat` or `linux-run.bat` depending on your OS.

It will automatically build and run the main.cpp file and its dependancies.

## Build as static library

To build the T language as a static library, you first need to run `win-init.bat` or `linux-init.sh` depending on your OS.

Once this is done, you only need to run the following make command :

```sh
make config=static-build
```

This will generate the file `Daedalus-Tlang.lib` in `build\daedalus-tlang\bin\static-build\`.

## Build as dynamic library

To build the T language as a static library, you first need to run `win-init.bat` or `linux-init.sh` depending on your OS.

Once this is done, you only need to run the following make command :

```sh
make config=dynamic-build
```

This will generate the files `Daedalus-Tlang.dll` and `Daedalus-Tlang.lib` in `build\daedalus-tlang\bin\dynamic-build\`.
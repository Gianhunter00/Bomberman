@echo off
set BASE_PATH=%~dp0
set BIN_PATH=%BASE_PATH%bin
set MAIN_PATH=%BASE_PATH%main

clang.exe -o "%BIN_PATH%\bomberman.exe" -I .\SDL2-2.0.20\include\ -L .\SDL2-2.0.20\lib\x64\ "%MAIN_PATH%\src\main.c" "%MAIN_PATH%\*.c" -Xlinker /subsystem:console -lSDL2main -lSDL2 -lshell32 -lws2_32 && "%BIN_PATH%\bomberman.exe"

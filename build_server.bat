@echo off
set BASE_PATH=%~dp0
set BIN_PATH=%BASE_PATH%bin
set MAIN_PATH=%BASE_PATH%main
set INC_PATH=%BASE_PATH%Include


rem RMDIR /s /q %BIN_PATH%
rem MD "%BIN_PATH%"

clang.exe -o "%BIN_PATH%\server.exe" ^
    -I "%INC_PATH%" ^
    "%MAIN_PATH%\src\main_server.c" "%MAIN_PATH%\server.c" ^
    -Xlinker /subsystem:console -lshell32 -lws2_32

IF %ERRORLEVEL% EQU 0 (
    "%BIN_PATH%\server.exe"
)

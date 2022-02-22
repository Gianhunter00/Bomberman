@echo off
set BASE_PATH=%~dp0
set BIN_PATH=%BASE_PATH%bin
set MAIN_PATH=%BASE_PATH%main
set INC_PATH=%BASE_PATH%Include
set SDL_DLL_PATH=%BASE_PATH%SDL2-2.0.20\lib\x64


RMDIR /s /q %BIN_PATH%
MD "%BIN_PATH%"

clang.exe -o "%BIN_PATH%\bomberman.exe" ^
    -I .\SDL2-2.0.20\include\ ^
    -I "%INC_PATH%" ^
    -L .\SDL2-2.0.20\lib\x64\ ^
    "%MAIN_PATH%\src\main.c" "%MAIN_PATH%\*.c" ^
    -Xlinker /subsystem:console -lSDL2main -lSDL2 -lshell32 -lws2_32

IF %ERRORLEVEL% EQU 0 (
    COPY /Y "%SDL_DLL_PATH%\*.dll" "%BIN_PATH%"
    XCOPY /E /Q /Y "%BASE_PATH%\resources\" "%BIN_PATH%\resources\"
    "%BIN_PATH%\bomberman.exe"
)

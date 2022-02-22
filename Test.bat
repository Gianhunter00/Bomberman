@echo off
set BASE_PATH=%~dp0
set BIN_PATH=%BASE_PATH%bin
set MAIN_PATH=%BASE_PATH%tests
set INC_FILE_C_PATH=%BASE_PATH%main
set INC_PATH=%BASE_PATH%Include
set INC_PATH_TEST=%MAIN_PATH%\Include



RMDIR /s /q %BIN_PATH%
MD "%BIN_PATH%"

clang.exe -o "%BIN_PATH%\tests.exe" ^
    -I .\SDL2-2.0.20\include\ ^
    -I "%INC_PATH%" ^
    -I "%INC_PATH_TEST%" ^
    -L .\SDL2-2.0.20\lib\x64\ ^
    "%MAIN_PATH%\main.c" "%INC_FILE_C_PATH%\*.c" ^
    -Xlinker /subsystem:console -lSDL2main -lSDL2 -lshell32 -lws2_32

IF %ERRORLEVEL% EQU 0 (
    COPY /Y "%SDL_DLL_PATH%\*.dll" "%BIN_PATH%"
    XCOPY /E /Q /Y "%BASE_PATH%\resources\" "%BIN_PATH%\resources\"
    "%BIN_PATH%\tests.exe"
)